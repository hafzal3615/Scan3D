//=============================================================================
//
// BSD 3-Clause License
//
// Copyright © 2013-2015, DAVID 3D Solutions GbR
// All rights reserved.
//
// Redistribution and use in source and binary forms, 
// with or without modification, are permitted provided that 
// the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// - Neither the name of DAVID 3D Solutions GbR nor the names of its contributors 
//   may be used to endorse or promote products derived from this software 
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//=============================================================================

#include "davidSDK/StructuredLightScannerD4Lib.h"
#include "davidSDK/ShapeFusionD4Lib.h"
#include <sstream>
#include <fstream>

namespace david { 

//=============================================================================
// Global functions
//=============================================================================

//=============================================================================
// StructuredLightScannerD4Lib::ResultFilteringParamsEx
//=============================================================================

StructuredLightScannerD4Lib::ResultFilteringParamsEx::ResultFilteringParamsEx()
	: qualityCheck(0.5)
	, backgroundRemovalEnabled(false)
	, backgroundRemovalEpsilon(2.f)
	, minContrast(10)
	, minDepth(0.f)
	, maxDepth(std::numeric_limits<float>::max())
	, depthThres(10.f)
	, outlierRemoval(0)
{
}


//=============================================================================
// StructuredLightScannerD4Lib::AcquisitionParams
//=============================================================================

StructuredLightScannerD4Lib::AcquisitionParams::AcquisitionParams()
	: minContentChange(20)
	, skipCount(1)
	, maxWaitTime(1.0)
{
}


//=============================================================================
// StructuredLightScannerD4Lib
//=============================================================================

StructuredLightScannerD4Lib::StructuredLightScannerD4Lib(IShapeFusion* shapeFusion)
	: m_cam(d4cam_newDirectShowCamera())
	, m_camCalib(d4calib_newCameraCalibration())
	, m_projector(d4projector_newExtendedDisplayProjector())
	, m_projectorCalib(d4calib_newProjectorCalibration())
	, m_live(d4img_newImage())
	, m_sls(d4sls_newStructuredLightScanner())
	, m_scale(240.0)
	, m_noiseReduction(0)
	, m_shapeFusion(shapeFusion)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::StructuredLightScannerD4Lib");
	
	m_whiteBalance[0] = 1.0;
	m_whiteBalance[1] = 1.0;
	m_whiteBalance[2] = 1.0;

	AcquisitionParams acq;
	acq.skipCount = 2;
	SetAcquisitionParams(acq);

	SetFrequencies(8);
}


StructuredLightScannerD4Lib::~StructuredLightScannerD4Lib()
{
	
}


void StructuredLightScannerD4Lib::SetResultFilteringParamsEx(const ResultFilteringParamsEx& params)
{
	Guard guard(m_mutex);
	m_resultFilteringParams = params;
}


void StructuredLightScannerD4Lib::SetAcquisitionParams(const AcquisitionParams& params)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::SetAcquisitionParams");
	
	m_acqParams = params;
	eec = d4cam_setSkipCount(m_cam, params.skipCount);
}


void StructuredLightScannerD4Lib::SetFrequencies(int frequencies)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::SetFrequencies");

	d4sls_CodedLightPhaseShiftParams params;
	eec = d4sls_getCodedLightPhaseShiftParams(m_sls, &params);
	params.frequencies = frequencies;
	eec = d4sls_setCodedLightPhaseShiftParams(m_sls, &params);
	DisplaySetupImage();
}


void StructuredLightScannerD4Lib::SetScreenID(int screenID)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::SetScreenID");
	if (screenID >= 0)
	{
		eec = d4projector_openByScreenID(m_projector, screenID);
		DisplaySetupImage();
	}
	else 
	{
		if (m_projector) eec = d4projector_close(m_projector);
	}
}


std::vector<std::string> StructuredLightScannerD4Lib::GetAvailableCameraNames()
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::GetAvailableCameraNames");

	int camCount = 0;
	eec = d4cam_getAvailableCamerasCount(m_cam, &camCount);
	std::vector<std::string> names;
	for (int i=0; i < camCount; ++i)
	{
		char name[256];
		eec = d4cam_getAvailableCamerasName(m_cam, name, sizeof(name), i);
		names.push_back(name);
	}
	
	return names;
}


void StructuredLightScannerD4Lib::SelectCamera(const std::string& cameraName, const ImageFormat& imageFormat)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::SelectCamera");

	if (cameraName.empty())
	{
		// close camera
		eec = d4cam_close(m_cam);
	}
	else
	{
		// open desired camera
		eec = d4cam_open(m_cam, cameraName.c_str(), imageFormat.width, imageFormat.height, imageFormat.pixelFormat, imageFormat.fps);
	}

	m_scan.Reset();
	m_texture.Reset();
	m_live.Reset();
	m_sequence.clear();
}


bool StructuredLightScannerD4Lib::CheckCamera()
{
	Guard guard(m_mutex);
	int result = d4cam_check(m_cam);
	return result == d4ok;
}


void StructuredLightScannerD4Lib::SetCameraProperty(const std::string& propertyName, double value, enum CamPropsUsage camPropsUsage)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::SetCameraProperty");

	if (camPropsUsage == CamPropsForAll)
	{
		// delete all entries for 'propertyName'.
		m_camPropsForScanning.erase(propertyName);
		m_camPropsForTexturing.erase(propertyName);
	}
	else if (camPropsUsage == CamPropsForScanning)
	{
		m_camPropsForScanning[propertyName] = value;
	}
	else if (camPropsUsage == CamPropsForTexturing)
	{
		m_camPropsForTexturing[propertyName] = value;
	}

	eec = d4cam_setProperty(m_cam, value, propertyName.c_str());
}


double StructuredLightScannerD4Lib::GetCameraProperty(const std::string& propertyName, enum CamPropsUsage camPropsUsage)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::GetCameraProperty");
	
	if (camPropsUsage == CamPropsForAll)
	{
		ThrowException(david::Error_InvalidArgument, "CamPropsForAll is not allowed.", "StructuredLightScannerD4Lib::GetCameraProperty");
	}
	
	if (camPropsUsage == CamPropsForTexturing)
	{
		if (m_camPropsForTexturing.find(propertyName) != m_camPropsForTexturing.end())
		{
			return m_camPropsForTexturing[propertyName];
		}
	}
	else
	{
		if (m_camPropsForScanning.find(propertyName) != m_camPropsForScanning.end())
		{
			return m_camPropsForScanning[propertyName];
		}
	}

	double value = 0.0;
	
	eec = d4cam_getProperty(m_cam, &value, propertyName.c_str());
	return value;
}


std::vector<std::string> StructuredLightScannerD4Lib::GetCameraPropertyNames(bool onlyAvailable)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::GetCameraPropertyNames");

	int count = 0;
	eec = d4cam_getPropertyCount(m_cam, &count, onlyAvailable);
	std::vector<std::string> names;
	for (int i=0; i < count; ++i)
	{
		char name[256];
		eec = d4cam_getPropertyName(m_cam, name, sizeof(name), i, onlyAvailable);
		names.push_back(name);
	}

	return names;
}


void StructuredLightScannerD4Lib::SelectImageSequence(const std::string& directory)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::SelectImageSequence");
	
	// TODO
	eec = d4error_NotImplemented;
}


void StructuredLightScannerD4Lib::GetLiveImage(std::vector<uint8_t>& pixelValues, int& width, int& height)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::GetLiveImage");

	eec = d4cam_getImageFormat(m_cam, &width, &height, NULL, NULL);
	pixelValues.resize(width*height);
	if (pixelValues.size() > 0)
	{
		eec = d4cam_captureImageAsync(m_cam, 0, 1.0, 0);
		eec = d4cam_getCapturedImage(m_cam, m_live, d4img_Y800);
		eec = d4img_getImageData(m_live, &pixelValues[0], pixelValues.size(), d4img_Y800);
	}
}


void StructuredLightScannerD4Lib::SetCodedLightPhaseShiftMode(const CodedLightPhaseShiftParams& params)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec;

	// Get.
	d4sls_CodedLightPhaseShiftParams slsParams;
	eec = d4sls_getCodedLightPhaseShiftParams(m_sls, &slsParams);
	
	// Set.
	if (params.frequencies && params.frequencies.value()!=0) slsParams.frequencies = params.frequencies.value();  // todo: AutoFrequencies
	if (params.color) slsParams.colorSelect = params.color.value();
	if (params.brightness) slsParams.brightness = params.brightness.value();
	if (params.inverse) slsParams.inverse = params.inverse.value();
	if (params.orientation) slsParams.orientation = params.orientation.value();
	if (params.noiseReduction) m_noiseReduction = params.noiseReduction.value();
	if (params.shifts) slsParams.shifts = params.shifts.value();

	eec = d4sls_setCodedLightPhaseShiftParams(m_sls, &slsParams);
}


CodedLightPhaseShiftParams StructuredLightScannerD4Lib::GetCodedLightPhaseShiftParams()
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::GetCodedLightPhaseShiftParams");

	// Get.
	d4sls_CodedLightPhaseShiftParams slsParams;
	eec = d4sls_getCodedLightPhaseShiftParams(m_sls, &slsParams);

	CodedLightPhaseShiftParams params;
	params.frequencies = slsParams.frequencies;
	params.color = (PatternColor) slsParams.colorSelect;
	params.brightness = slsParams.brightness;
	params.inverse = slsParams.inverse;
	params.noiseReduction = m_noiseReduction;
	params.orientation = (Orientation) slsParams.orientation;
	params.shifts = slsParams.shifts;
	return params;
}


void StructuredLightScannerD4Lib::SetResultFiltering(const ResultFilteringParams& params)
{
	Guard guard(m_mutex);
	if (params.backgroundRemovalEnabled) m_resultFilteringParams.backgroundRemovalEnabled = params.backgroundRemovalEnabled.value();
	if (params.backgroundRemovalEpsilon) m_resultFilteringParams.backgroundRemovalEpsilon = (float)params.backgroundRemovalEpsilon.value();
	if (params.qualityCheck) m_resultFilteringParams.qualityCheck = (float)params.qualityCheck.value();
	if (params.outlierRemoval) m_resultFilteringParams.outlierRemoval = params.outlierRemoval.value();
}


ResultFilteringParams StructuredLightScannerD4Lib::GetResultFiltering()
{
	Guard guard(m_mutex);
	ResultFilteringParams params;
	params.backgroundRemovalEnabled = m_resultFilteringParams.backgroundRemovalEnabled;
	params.backgroundRemovalEpsilon = m_resultFilteringParams.backgroundRemovalEpsilon;
	params.qualityCheck = m_resultFilteringParams.qualityCheck;
	return params;
}


void StructuredLightScannerD4Lib::ImportCalibration(const std::string& camCalibFilename, const std::string& projectorCalibFilename)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::ImportCalibration");
	
	eec = d4calib_importCalibration(m_projectorCalib, projectorCalibFilename.c_str());
	eec = d4calib_importCalibration(m_camCalib, camCalibFilename.c_str());
}


void StructuredLightScannerD4Lib::ExportCalibration(const std::string& camCalibFilename, const std::string& projectorCalibFilename)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::ExportCalibration");

	eec = d4calib_exportCalibration(m_projectorCalib, projectorCalibFilename.c_str());
	eec = d4calib_exportCalibration(m_camCalib, camCalibFilename.c_str());
}


void StructuredLightScannerD4Lib::ChangeCameraCalibrationResolution(int newWidth, int newHeight)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::ChangeCameraCalibrationResolution");

	eec = d4calib_changeCalibrationResolution(m_camCalib, newWidth, newHeight);
}


std::vector<CalibPoint> StructuredLightScannerD4Lib::Calibrate(double scale)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::Calibrate");
	
	m_scale = scale;

	// Save old params for later reset.
	d4sls_CodedLightPhaseShiftParams oldParams;
	eec = d4sls_getCodedLightPhaseShiftParams(m_sls, &oldParams);

	eec("capture image for camera calibration");
	D4Ptr<d4img_ptr> calibImg(d4img_newImage());
	{
		eec = d4cam_captureImageAsync(m_cam, m_acqParams.minContentChange, 1.0, 10);
		uint8_t brightness = uint8_t((oldParams.brightness*3)/4);
		eec = d4projector_displayUniformColor(m_projector, brightness, brightness, brightness);
		eec = d4cam_getCapturedImage(m_cam, calibImg, d4img_Y800);
	}

	// We use high quality for SLS calibration.
	// Important: Ensure that old parameters are restored.
	eec("capture sequence for projector calibration");
	d4sls_CodedLightPhaseShiftParams params = oldParams;
	params.inverse = true;
	params.shifts = 15;
	params.orientation = d4sls_BothOrientations;
	eec = d4sls_setCodedLightPhaseShiftParams(m_sls, &params);
	std::vector<d4calib_CalibPoint> calibPoints;
	try
	{
		CaptureSequence();
		std::vector<d4img_ptr> capturedImages;
		{
			// Convert captured images to depth image.
			for (size_t i=0; i < m_sequence.size(); ++i)
			{
				capturedImages.push_back(m_sequence[i]);
			}
		}
		if (capturedImages.size() < 1) eec = d4error_Fail;

		// calibrate
		eec("calibrate");
		int calibError;
		bool compensatePatternOffsets = true;
		int calibPointCount = 1000;
		calibPoints.resize(calibPointCount);
		eec = d4calib_calibrate(m_camCalib, calibImg, m_projectorCalib, m_sls, &capturedImages[0], (int)capturedImages.size(), m_scale, compensatePatternOffsets, 
			&calibError, &calibPoints[0], &calibPointCount);
		calibPoints.resize(calibPointCount);
	}
	catch (david::Exception&)
	{
		d4sls_setCodedLightPhaseShiftParams(m_sls, &oldParams);
		throw;
	}

	// Restore old pattern parameters.
	eec("restore");
	eec = d4sls_setCodedLightPhaseShiftParams(m_sls, &oldParams);

	// Display chessboard or setup image.
	if (!DisplayChessboardImage()) DisplaySetupImage();

	// Notification about successful calibration.
	OnCalibrationDone(calibPoints);

	// Return calibration error.
	std::vector<CalibPoint> _calibPoints(calibPoints.size());
	for (size_t c=0; c < calibPoints.size(); ++c)
	{
		for (int j=0; j < 3; ++j) _calibPoints[c].worldRefPos3d[j] = calibPoints[c].worldRefPos3d[j];
		for (int j=0; j < 2; ++j)
		{
			_calibPoints[c].projectedPos2d[j] = calibPoints[c].projectedPos2d[j];
			_calibPoints[c].measuredPos2d[j] = calibPoints[c].measuredPos2d[j];
		}
	}
	return _calibPoints;
}


int StructuredLightScannerD4Lib::Scan(bool grabTexture, bool setAsBackground)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::Scan");

	// check if camera and projector are calibrated:
	eec = d4projector_check(m_projector);
	eec = d4cam_check(m_cam);
	bool calibrated = false;
	d4calib_isCalibrated(m_projectorCalib, &calibrated);
	if (!calibrated) eec = d4error_NotCalibrated;
	d4calib_isCalibrated(m_camCalib, &calibrated);
	if (!calibrated) eec = d4error_NotCalibrated;

	SwitchCameraProperties(CamPropsForScanning);
	
	eec("capture");
	CaptureSequence();
	if (grabTexture) CaptureTexture(true);

	// Sequence empty? --> Nothing to do
	if (m_sequence.empty()) return -1;

	eec("compute depth image");
	D4Ptr<d4fimg_ptr> depthImage(d4fimg_newImage());
	D4Ptr<d4fimg_ptr> qualityImage(d4fimg_newImage());
	{
		// Convert captured images to depth image.
		std::vector<d4img_ptr> capturedImages;
		for (size_t i=0; i < m_sequence.size(); ++i)
		{
			capturedImages.push_back(m_sequence[i]);
		}
		
		eec = d4sls_computeDepthImage(m_sls, depthImage, qualityImage, 
			&capturedImages[0], int(capturedImages.size()), m_camCalib, m_projectorCalib, m_resultFilteringParams.minContrast);
	}

	eec("remove background");
	// Set or remove background.
	if (setAsBackground)
	{
		m_background = depthImage;
	}
	else if (m_background && m_resultFilteringParams.backgroundRemovalEnabled)
	{
		d4scan_removeBackground(depthImage, m_background, float(m_resultFilteringParams.backgroundRemovalEpsilon));
	}

	eec("depth image to mesh");
	m_scan = D4Ptr<d4mesh_ptr>(d4mesh_newMesh());
	eec = d4scan_convertDepthImageToMesh(m_scan, depthImage, qualityImage, m_camCalib, 
		m_resultFilteringParams.minDepth, m_resultFilteringParams.maxDepth, m_resultFilteringParams.depthThres, m_resultFilteringParams.qualityCheck, m_resultFilteringParams.outlierRemoval);

	eec("set texture");
	if (grabTexture && m_texture)
	{
		eec = d4mesh_setTexture(m_scan, m_texture, 0);
	}
	else
	{
		m_texture.Reset();
	}

	int vertexCount = 0;
	eec = d4mesh_getVertexCount(m_scan, &vertexCount, -1);
	
	return vertexCount;
}


void StructuredLightScannerD4Lib::GrabTexture()
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::GrabTexture");
	
	CaptureTexture(true);

	// Set texture.
	if (m_scan && m_texture)
	{
		eec = d4mesh_setTexture(m_scan, m_texture, 0);
	}
}


void StructuredLightScannerD4Lib::AutoWhiteBalance()
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::AutoWhiteBalance");
	
	CaptureTexture(false);

	eec = d4img_computeWhiteBalance(m_texture, &m_whiteBalance[0], &m_whiteBalance[1], &m_whiteBalance[2]);
	eec = d4img_applyWhiteBalance(m_texture, m_whiteBalance[0], m_whiteBalance[1], m_whiteBalance[2]);
}


int StructuredLightScannerD4Lib::AddScanToShapeFusion()
{
	Guard guard(m_mutex);
	ShapeFusionD4Lib* shapeFusion = dynamic_cast<ShapeFusionD4Lib*>(m_shapeFusion);
	if (shapeFusion && m_scan)
	{
		return shapeFusion->AddToList(m_scan);
	}
	else return -1;
}


void StructuredLightScannerD4Lib::ExportMesh(const std::string& filename)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::ExportMesh");

	eec = d4mesh_export(m_scan, filename.c_str());
}


void StructuredLightScannerD4Lib::CaptureSequence()
{
	david::ErrorExceptionConverter eec("StructuredLightScannerD4Lib::CaptureSequence");

	// Ensure that sequence has correct size.
	eec("setup sequence");
	int patternCount = 0;
	eec = d4sls_getPatternCount(m_sls, &patternCount);
	if (patternCount != int(m_sequence.size()))
	{
		m_sequence.resize(patternCount);
		for (int i=0; i < patternCount; ++i)
		{
			m_sequence[i] = D4Ptr<d4img_ptr>(d4img_newImage());
		}
	}

	eec("capture");
	david::D4Ptr<d4img_ptr> patternImage(d4img_newImage());
	for (int i=0; i < patternCount; ++i)
	{
		int projectorWidth=0, projectorHeight=0, projectorPixelFormat=0;
		eec = d4projector_getImageFormat(m_projector, &projectorWidth, &projectorHeight, &projectorPixelFormat);
		eec = d4cam_captureImageAsync(m_cam, m_acqParams.minContentChange, m_acqParams.maxWaitTime, m_noiseReduction);
		eec = d4sls_getPatternImage(m_sls, patternImage, projectorWidth, projectorHeight, projectorPixelFormat, i); 
		eec = d4projector_displayImage(m_projector, patternImage);
		eec = d4cam_getCapturedImage(m_cam, m_sequence[i], d4img_Y800);
	}

	/*
	for (int i=0; i < patternCount; ++i)
	{
		david::D4Ptr<d4img_ptr> temp(d4img_newImage());
		eec = d4img_convert(temp, d4img_RGB24, m_sequence[i]);
		{
			std::stringstream ss;
			ss << "M:\\Temp\\003\\";
			if (i+1 < 10) ss << "0";
			ss << (i+1) << ".bmp";
			eec = d4img_export(temp, ss.str().c_str());
		}
	}
	*/

	DisplaySetupImage();
}


void StructuredLightScannerD4Lib::CaptureTexture(bool applyWhiteBalance)
{
	david::ErrorExceptionConverter eec("StructuredLightScannerD4Lib::CaptureTexture");

	SwitchCameraProperties(CamPropsForTexturing);

	eec("new pattern image");
	david::D4Ptr<d4img_ptr> patternImage(d4img_newImage());

	eec("new texture");
	m_texture = david::D4Ptr<d4img_ptr>(d4img_newImage());

	eec("capture texture");
	bool isColor = true;
	{
		int pixelFormat;
		eec = d4cam_getImageFormat(m_cam, NULL, NULL, &pixelFormat, NULL);
		if (pixelFormat == d4img_Y800) isColor = false;
	}
	//if (isColor) TODO
	{
		d4sls_CodedLightPhaseShiftParams params;
		eec = d4sls_getCodedLightPhaseShiftParams(m_sls, &params);
		unsigned char b = (unsigned char) params.brightness;
		eec = d4cam_captureImageAsync(m_cam, m_acqParams.minContentChange, m_acqParams.maxWaitTime, m_noiseReduction);
		eec = d4projector_displayUniformColor(m_projector, b, b, b); 
		eec = d4cam_getCapturedImage(m_cam, m_texture, d4img_RGB24);
	}

	if (applyWhiteBalance)
	{
		eec = d4img_applyWhiteBalance(m_texture, m_whiteBalance[0], m_whiteBalance[1], m_whiteBalance[2]);
	}
}


void StructuredLightScannerD4Lib::SwitchCameraProperties(CamPropsUsage usage)
{
	if (usage == CamPropsForScanning)
	{
		std::map<std::string, double>::iterator it = m_camPropsForScanning.begin();
		for (; it != m_camPropsForScanning.end(); ++it)
		{
			d4cam_setProperty(m_cam, it->second, it->first.c_str());
		}
	}
	else if (usage == CamPropsForTexturing)
	{
		std::map<std::string, double>::iterator it = m_camPropsForTexturing.begin();
		for (; it != m_camPropsForTexturing.end(); ++it)
		{
			d4cam_setProperty(m_cam, it->second, it->first.c_str());
		}
	}
}


void StructuredLightScannerD4Lib::DisplaySetupImage()
{
	david::ErrorExceptionConverter eec("StructuredLightScannerD4Lib::DisplaySetupImage");

	if (d4projector_check(m_projector) != d4ok) return;

	int width, height, pixelFormat;
	eec = d4projector_getImageFormat(m_projector, &width, &height, &pixelFormat);
	D4Ptr<d4img_ptr> img(d4img_newImage());
	eec = d4sls_getSetupImage(m_sls, img, width, height, pixelFormat);
	eec = d4projector_displayImage(m_projector, img);
}


bool StructuredLightScannerD4Lib::DisplayChessboardImage()
{
	david::ErrorExceptionConverter eec("StructuredLightScannerD4Lib::DisplayChessboardImage");

	// check if projector is calibrated
	if (d4projector_check(m_projector) != d4ok) return false;
	bool calibrated = false;
	if (d4calib_isCalibrated(m_projectorCalib, &calibrated) != d4ok || !calibrated) return false;

	// get image format of projector
	int width, height, pixelFormat;
	eec = d4projector_getImageFormat(m_projector, &width, &height, &pixelFormat);

	// get chessboard image and display it
	D4Ptr<d4img_ptr> img(d4img_newImage());
	int result = d4sls_getChessboardImage(m_sls, img, width, height, pixelFormat, m_projectorCalib, m_scale);
	if (result != d4ok) return false;
	eec = d4projector_displayImage(m_projector, img);
	return true;
}


void StructuredLightScannerD4Lib::OnCalibrationDone(const std::vector<d4calib_CalibPoint>& calibPoints)
{
	/*
	std::ofstream f("calibration_points.txt");
	for (size_t i=0; i < calibPoints.size(); ++i)
	{
		d4calib_CalibPoint cp = calibPoints[i];
		f << "index:            " << i << std::endl;
		f << "  worldRefPos3d:  " << cp.worldRefPos3d[0] << " " << cp.worldRefPos3d[1] << " " << cp.worldRefPos3d[2] << std::endl;
		f << "  measuredPos2d:  " << cp.measuredPos2d[0] << " " << cp.measuredPos2d[1] << std::endl;
		f << "  projectedPos2d: " << cp.projectedPos2d[0] << " " << cp.projectedPos2d[1] << std::endl;
	}
	*/
}


void StructuredLightScannerD4Lib::ExportBackgroundDepthmap(const std::string& filename)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::ExportBackgroundDepthmap");

	if (m_background == NULL) eec = d4error_MissingObject;
	eec = d4fimg_export(m_background, filename.c_str());
}


void StructuredLightScannerD4Lib::ImportBackgroundDepthmap(const std::string& filename)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::ImportBackgroundDepthmap");

	D4Ptr<d4fimg_ptr> backgrnd(d4fimg_newImage());
	eec = d4fimg_import(backgrnd, filename.c_str());
	m_background = backgrnd;
}


void StructuredLightScannerD4Lib::GetBackgroundDepthmap(std::vector<float>& depthValues, int &width, int &height)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::GetBackgroundDepthmap");

	if (m_background == NULL) eec = d4error_MissingObject;

	eec = d4fimg_getImageFormat(m_background, &width, &height);
	depthValues.resize(width*height);
	eec = d4fimg_getImageData(m_background, &depthValues[0], width*height*sizeof(float));
}


void StructuredLightScannerD4Lib::SetBackgroundDepthmap(const std::vector<float>& depthValues, int width, int height)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::SetBackgroundDepthmap");

	if (width*height != depthValues.size()) ThrowException(david::Error_InvalidArgument, "width and height do not fit", "StructuredLightScannerD4Lib::SetBackgroundDepthmap");

	D4Ptr<d4fimg_ptr> background(d4fimg_newImage());
	eec = d4fimg_setImageFormat(background, width, height);
	eec = d4fimg_setImageData(background, &depthValues[0], width*height*sizeof(float));

	m_background = background;
}


void StructuredLightScannerD4Lib::SetBackgroundPlanes(const std::vector<Plane>& planes)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("StructuredLightScannerD4Lib::SetBackgroundPlanes");

	// Get width and height:
	int camResX=0;
	int camResY=0;
	eec = d4calib_getCalibrationResolution(m_camCalib, &camResX, &camResY);
	if (camResX<=0 || camResY<=0) ThrowException(david::Error_FormatNotAvailable, "Could not determine camera resolution", "StructuredLightScannerD4Lib::SetBackgroundPlanes");

	// Prepare new depth map:
	D4Ptr<d4fimg_ptr> background(d4fimg_newImage());
	eec = d4fimg_setImageFormat(background, camResX, camResY);

	// Fill depth map:
	eec = d4sls_planesToDepthImage(m_sls, background, m_camCalib, (int)planes.size(), (const d4sls_Plane*)planes.data());

	// store to m_background:
	m_background = background;
}


} // namespace

