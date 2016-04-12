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

#ifndef DAVID_SDK_STRUCTURED_LIGHT_SCANNER_JSON_RPC_H
#define DAVID_SDK_STRUCTURED_LIGHT_SCANNER_JSON_RPC_H

#include "davidSDK/ClientJsonRpc.h"

namespace david {


//=============================================================================
// StructuredLightScannerJsonRpc
//=============================================================================

/// Implements interface functions of IStructuredLightScanner.
/// All calls are delegated to a JSON-RPC 2.0 client.
/// Example:
/// @include SimpleStructuredLightScanner.cpp
class StructuredLightScannerJsonRpc : public IStructuredLightScanner
{
public:
	/// Constructor.
	/// @param[in,out] client Valid pointer to client implementation.
	StructuredLightScannerJsonRpc(ClientJsonRpc* client);
	
	// See IStructuredLightScanner
	virtual ~StructuredLightScannerJsonRpc();

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

private:
	//-------------------------------------------------------------------------
	// Private member variables
	//-------------------------------------------------------------------------

	ClientJsonRpc*     m_client;     ///< Client implementation
};


} // namespace

#endif // DAVID_SDK_STRUCTURED_LIGHT_SCANNER_JSON_RPC_H
