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

#pragma once

#ifndef DAVID_SDK_STRUCTURED_LIGHT_SCANNER_D4LIB_H
#define DAVID_SDK_STRUCTURED_LIGHT_SCANNER_D4LIB_H

#include "davidSDK/IModules.h"
#include "davidSDK/d4lib.h"
#include "davidSDK/D4Ptr.h"
#include <map>
#include <mutex>

namespace david {


//=============================================================================
// StructuredLightScannerD4Lib
//=============================================================================

/// Implements interface functions of IStructuredLightScanner using DAVID Low Level Library.
class StructuredLightScannerD4Lib : public IStructuredLightScanner
{
public:
	/// Extended parameters for result filtering.
	struct ResultFilteringParamsEx
	{
		float      qualityCheck;               ///< Quality threshold for 3D data [0,1].
		bool       backgroundRemovalEnabled;   ///< Background Removal enabled (true/false).
		float      backgroundRemovalEpsilon;   ///< Epsilon for Background Removal.
		int        minContrast;                ///< Minimum required contrast (magnitude of amplitude signal). Set to <= 0 in order to use default value (10).
		float      minDepth;                   ///< Minimum depth value in [mm], as distance from camera's focal point. Has to be >= 0.
		float      maxDepth;                   ///< Maximum depth value in [mm], as distance from camera's focal point. Has to be > 'minDepth'.
		float      depthThres;                 ///< Neighbor pixels to a triangle will not be connected, if their difference in depth is larger than this value [mm].
		double     outlierRemoval;             ///< Outlier filter [0,1]. 0=off. 1=remove all except the largest scan fragment. e.g. 0.1=remove all fragments that are smaller than 10% of the largest fragment.

		/// Constructor.
		ResultFilteringParamsEx();
	};

	/// Parameters for acquisition.
	struct AcquisitionParams
	{
		int        minContentChange;   ///< Minimum content change for automatic pattern change detection [%].
		int        skipCount;          ///< Set the number of images that are skipped after a content change was detected. Default value is 1.
		double     maxWaitTime;        ///< Maximum wait time to capture a new image [s].

		/// Constructor.
		AcquisitionParams();
	};

public:
	/// Constructor.
	/// @param[in,out] shapeFusion Optional pointer to 'Shape Fusion' module. Needed for #AddScanToShapeFusion.
	StructuredLightScannerD4Lib(IShapeFusion* shapeFusion);

	// See IStructuredLightScanner
	virtual ~StructuredLightScannerD4Lib();

	/// Set StructuredLightScannerD4Lib::ResultFilteringParamsEx
	/// @param[in] params See StructuredLightScannerD4Lib::ResultFilteringParamsEx.
	void SetResultFilteringParamsEx(const ResultFilteringParamsEx& params);

	/// Set StructuredLightScannerD4Lib::AcquisitionParams
	/// @param[in] params See StructuredLightScannerD4Lib::AcquisitionParams.
	void SetAcquisitionParams(const AcquisitionParams& params);

	/// Set number of frequencies (affects number of patterns). Optimal setting depends on camera/projector resolution and setup.
	/// @param[in] frequencies New value for frequencies.
	void SetFrequencies(int frequencies);

	// See IStructuredLightScanner
	virtual void SetScreenID(int screenID);

	// See IStructuredLightScanner
	virtual std::vector<std::string> GetAvailableCameraNames();

	// See IStructuredLightScanner
	virtual void SelectCamera(const std::string& cameraName, const ImageFormat& imageFormat = ImageFormat());

	// See IStructuredLightScanner
	virtual bool CheckCamera();

	// See IStructuredLightScanner
	virtual void SetCameraProperty(const std::string& propertyName, double value, enum CamPropsUsage camPropsUsage = CamPropsForAll);

	// See IStructuredLightScanner
	virtual double GetCameraProperty(const std::string& propertyName, enum CamPropsUsage camPropsUsage = CamPropsForScanning);

	// See IStructuredLightScanner
	virtual std::vector<std::string> GetCameraPropertyNames(bool onlyAvailable);

	// See IStructuredLightScanner
	virtual void SelectImageSequence(const std::string& directory);

	// See IStructuredLightScanner
	virtual void GetLiveImage(std::vector<uint8_t>& pixelValues, int& width, int& height);

	// See IStructuredLightScanner
	virtual void SetCodedLightPhaseShiftMode(const CodedLightPhaseShiftParams& params);

	// See IStructuredLightScanner
	virtual CodedLightPhaseShiftParams GetCodedLightPhaseShiftParams();

	// See IStructuredLightScanner
	virtual void SetResultFiltering(const ResultFilteringParams& params);

	// See IStructuredLightScanner
	virtual ResultFilteringParams GetResultFiltering();

	// See IStructuredLightScanner
	virtual void ImportCalibration(const std::string& camCalibFilename, const std::string& projectorCalibFilename);

	// See IStructuredLightScanner
	virtual void ExportCalibration(const std::string& camCalibFilename, const std::string& projectorCalibFilename);

	// See IStructuredLightScanner
	virtual void ChangeCameraCalibrationResolution(int newWidth, int newHeight);

	// See IStructuredLightScanner
	virtual std::vector<CalibPoint> Calibrate(double scale);

	// See IStructuredLightScanner
	virtual int Scan(bool grabTexture=false, bool setAsBackground=false);

	// See IStructuredLightScanner
	virtual void GrabTexture();

	// See IStructuredLightScanner
	virtual void AutoWhiteBalance();

	// See IStructuredLightScanner
	virtual int AddScanToShapeFusion();

	// See IStructuredLightScanner
	virtual void ExportMesh(const std::string& filename);

	// See IStructuredLightScanner
	virtual void ExportBackgroundDepthmap(const std::string& filename);

	// See IStructuredLightScanner
	virtual void ImportBackgroundDepthmap(const std::string& filename);

	// See IStructuredLightScanner
	virtual void GetBackgroundDepthmap(std::vector<float>& depthValues, int &width, int &height);

	// See IStructuredLightScanner
	virtual void SetBackgroundDepthmap(const std::vector<float>& depthValues, int width, int height);

	// See IStructuredLightScanner
	virtual void SetBackgroundPlanes(const std::vector<Plane>& planes);

protected:
	/// Capture a SLS sequence using the current settings.
	/// The captured images are stored in 'm_sequence'.
	void CaptureSequence();

	/// Capture a texture using the current settings.
	/// @param[in] applyWhiteBalance Apply white balance to the captured texture image?
	void CaptureTexture(bool applyWhiteBalance);

	/// Switch camera properties.
	/// @param[in] usage Select camera property usage. See #CamPropsUsage.
	void SwitchCameraProperties(CamPropsUsage usage);

	/// Show setup image.
	void DisplaySetupImage();

	/// Display chessboard image for validation of calibration.
	/// @return False, if chessboard image can not be displayed.
	bool DisplayChessboardImage();

	/// Is called after successful calibration.
	/// @param[in] calibPoints Array of calibration points. See #d4calib_CalibPoint.
	virtual void OnCalibrationDone(const std::vector<d4calib_CalibPoint>& calibPoints);

protected:
	//-------------------------------------------------------------------------
	// Typedefs
	//-------------------------------------------------------------------------

	/// Used mutex.
	typedef std::recursive_mutex  Mutex;

	/// Guard for automatic unlocking.
	typedef std::lock_guard<Mutex>  Guard;

	//-------------------------------------------------------------------------
	// Protected member variables
	//-------------------------------------------------------------------------

	Mutex                           m_mutex;                     ///< Mutex used for synchronization.
	D4Ptr<d4cam_ptr>                m_cam;                       ///< Handle to camera.
	D4Ptr<d4calib_ptr>              m_camCalib;                  ///< Handle to camera calibration.
	D4Ptr<d4projector_ptr>          m_projector;                 ///< Handle to projector.
	D4Ptr<d4calib_ptr>              m_projectorCalib;            ///< Handle to projector calibration.
	D4Ptr<d4img_ptr>                m_live;                      ///< Handle to live image.
	D4Ptr<d4sls_ptr>                m_sls;                       ///< Handle to SLS instance.
	D4Ptr<d4fimg_ptr>               m_background;                ///< Handle to background depth image.
	D4Ptr<d4mesh_ptr>               m_scan;                      ///< Handle to current scan.
	D4Ptr<d4img_ptr>                m_texture;                   ///< Handle to current texture.
	std::vector<D4Ptr<d4img_ptr>>   m_sequence;                  ///< Array of handles to captured images.
	double                          m_scale;                     ///< Calibration scale.
	int                             m_noiseReduction;            ///< Temporal noise reduction.
	ResultFilteringParamsEx         m_resultFilteringParams;     ///< Parameters for result filtering.
	AcquisitionParams               m_acqParams;                 ///< Acquisition parameters.
	std::map<std::string, double>   m_camPropsForScanning;       ///< Camera properties for scanning mode.
	std::map<std::string, double>   m_camPropsForTexturing;      ///< Camera properties for texturing mode.
	double                          m_whiteBalance[3];           ///< White balance correction factors.
	IShapeFusion*                   m_shapeFusion;               ///< Interface to Shape Fusion module.
};


} // namespace

#endif // DAVID_SDK_STRUCTURED_LIGHT_SCANNER_D4LIB_H
