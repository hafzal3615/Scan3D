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

#include "davidSDK/StructuredLightScannerJsonRpc.h"

namespace david { 

//=============================================================================
// Global functions
//=============================================================================

std::string ConvertPixelFormat(enum PixelFormat pixelFormat)
{
	if      (pixelFormat == Y800)                  return "Y800";
	else if (pixelFormat == RGB24)                 return "RGB24";
	else if (pixelFormat == YUY2)                  return "YUY2";
	else if (pixelFormat == BY8)                   return "BY8";
	else if (pixelFormat == UnknownPixelFormat)    return "Default";
	else return "";
}


std::string ConvertOrientation(enum Orientation orientation)
{
	if      (orientation == HorizontalOrientation)    return "horizontal";
	else if (orientation == VerticalOrientation)      return "vertical";
	else if (orientation == BothOrientations)         return "both";
	else return "";
}


enum Orientation ConvertOrientation(const std::string& orientation, const std::string& source)
{
	if      (orientation == "horizontal")    return HorizontalOrientation;
	else if (orientation == "vertical")      return VerticalOrientation;
	else if (orientation == "both")          return BothOrientations;
	else 
	{
		ThrowException(david::Error_RpcInvalidParams, "Could not convert orientation.", source);
		return Orientation();
	}
}


std::string ConvertColor(enum PatternColor color)
{
	if      (color == PatternColor::White)    return "white";
	else if (color == PatternColor::Red)      return "red";
	else if (color == PatternColor::Green)    return "green";
	else if (color == PatternColor::Blue)     return "blue";
	else return "";
}


enum PatternColor ConvertColor(const std::string& color, const std::string& source)
{
	if      (color == "white")    return PatternColor::White;
	else if (color == "red")      return PatternColor::Red;
	else if (color == "green")    return PatternColor::Green;
	else if (color == "blue")     return PatternColor::Blue;
	else 
	{
		ThrowException(david::Error_RpcInvalidParams, "Could not convert color.", source);
		return PatternColor();
	}
}


std::string ConvertCamPropsUsage(enum CamPropsUsage camPropsUsage)
{
	if      (camPropsUsage == CamPropsForScanning)  return "Scanning";
	else if (camPropsUsage == CamPropsForTexturing) return "Texturing";
	else return "";
}


//=============================================================================
// StructuredLightScannerJsonRpc
//=============================================================================

StructuredLightScannerJsonRpc::StructuredLightScannerJsonRpc(ClientJsonRpc* client)
	: m_client(client)
{

}


StructuredLightScannerJsonRpc::~StructuredLightScannerJsonRpc()
{

}


void StructuredLightScannerJsonRpc::SetScreenID(int screenID)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::SetScreenID");
	rpc["params"]["screenID"] = screenID;
		
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


std::vector<std::string> StructuredLightScannerJsonRpc::GetAvailableCameraNames()
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::GetAvailableCameraNames");

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
	Json::Value result = response["result"];

	const Json::Value& av = result["names"];
	if (av.type() == Json::nullValue)  throw david::ProtocolException(david::Error_RpcInvalidParams, "Missing parameter 'names'", "StructuredLightScannerJsonRpc::GetAvailableCameraNames");
	if (av.type() != Json::arrayValue) throw david::ProtocolException(david::Error_RpcInvalidParams, "Invalid type of parameter 'names'", "StructuredLightScannerJsonRpc::GetAvailableCameraNames");

	std::vector<std::string> names;
	for (Json::UInt i=0; i<av.size(); ++i)
	{
		names.push_back(av[i].asString());
	}

	return names;
}


void StructuredLightScannerJsonRpc::SelectCamera(const std::string& cameraName, const ImageFormat& imageFormat)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::SelectCamera");
	rpc["params"]["cameraName"] = cameraName;
	if (imageFormat.width > 0 || imageFormat.height > 0 || imageFormat.fps > 0.0 || imageFormat.pixelFormat != UnknownPixelFormat)
	{
		rpc["params"]["imageFormat"]["width"] = imageFormat.width;
		rpc["params"]["imageFormat"]["height"] = imageFormat.height;
		rpc["params"]["imageFormat"]["fps"] = imageFormat.fps;
		rpc["params"]["imageFormat"]["pixelFormat"] = ConvertPixelFormat(imageFormat.pixelFormat);
	}
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


bool StructuredLightScannerJsonRpc::CheckCamera()
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::CheckCamera");
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	return m_client->ExtractJsonParam<bool>(response, "result");
}


void StructuredLightScannerJsonRpc::SetCameraProperty(const std::string& propertyName, double value, enum CamPropsUsage camPropsUsage)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::SetCameraProperty");
	rpc["params"]["propertyName"] = propertyName;
	rpc["params"]["value"] = value;
	if (camPropsUsage != CamPropsForAll) rpc["params"]["usage"] = ConvertCamPropsUsage(camPropsUsage);
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


double StructuredLightScannerJsonRpc::GetCameraProperty(const std::string& propertyName, enum CamPropsUsage camPropsUsage)
{
	if (camPropsUsage == CamPropsForAll)  throw david::FunctionException(david::Error_InvalidArgument, "CamPropsForAll is not allowed here.", "StructuredLightScannerJsonRpc::SetCameraProperty");
	
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::GetCameraProperty");
	rpc["params"]["propertyName"] = propertyName;
	rpc["params"]["usage"] = ConvertCamPropsUsage(camPropsUsage);
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
	Json::Value result = response["result"];
	return m_client->ExtractJsonParam<double>(result, "value");
}


std::vector<std::string> StructuredLightScannerJsonRpc::GetCameraPropertyNames(bool onlyAvailable)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::GetCameraPropertyNames");
	rpc["params"]["onlyAvailable"] = onlyAvailable;

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
	Json::Value result = response["result"];

	const Json::Value& av = result["names"];
	if (av.type() == Json::nullValue)  throw david::ProtocolException(david::Error_RpcInvalidParams, "Missing parameter 'names'", "StructuredLightScannerJsonRpc::GetCameraPropertyNames");
	if (av.type() != Json::arrayValue) throw david::ProtocolException(david::Error_RpcInvalidParams, "Invalid type of parameter 'names'", "StructuredLightScannerJsonRpc::GetCameraPropertyNames");

	std::vector<std::string> names;
	for (Json::UInt i=0; i<av.size(); ++i)
	{
		names.push_back(av[i].asString());
	}

	return names;
}


void StructuredLightScannerJsonRpc::SelectImageSequence(const std::string& directory)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::SelectImageSequence");
	rpc["params"]["directory"] = directory;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


void StructuredLightScannerJsonRpc::GetLiveImage(std::vector<uint8_t>& pixelValues, int& width, int& height)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::GetLiveImage");
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	Json::Value result = response["result"];
	const uint32_t size = m_client->ExtractJsonParam<uint32_t>(result, "size");
	width = m_client->ExtractJsonParam<int>(result, "width");
	height = m_client->ExtractJsonParam<int>(result, "height");
		
	pixelValues.resize(size);
	if (size > 0)
	{
		m_client->ReadBinaryData(&pixelValues[0], size);
	}
}


void StructuredLightScannerJsonRpc::SetCodedLightPhaseShiftMode(const CodedLightPhaseShiftParams& params)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::SetCodedLightPhaseShiftMode");
	if (params.frequencies) rpc["params"]["frequencies"] = params.frequencies.value();
	if (params.inverse) rpc["params"]["inverse"] = params.inverse.value();
	if (params.shifts) rpc["params"]["shifts"] = params.shifts.value();
	if (params.orientation) rpc["params"]["orientation"] = ConvertOrientation(params.orientation.value());
	if (params.color) rpc["params"]["color"] = ConvertColor(params.color.value());
	if (params.brightness) rpc["params"]["brightness"] = params.brightness.value();
	if (params.noiseReduction) rpc["params"]["noiseReduction"] = params.noiseReduction.value();

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


CodedLightPhaseShiftParams StructuredLightScannerJsonRpc::GetCodedLightPhaseShiftParams()
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::GetCodedLightPhaseShiftParams");

	CodedLightPhaseShiftParams params;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	Json::Value result = response["result"];

	m_client->ExtractJsonParam(params.frequencies, result, "frequencies");
	m_client->ExtractJsonParam(params.inverse, result, "inverse");
	m_client->ExtractJsonParam(params.shifts, result, "shifts");
	Optional<std::string> color;
	m_client->ExtractJsonParam(color, result, "color");
	if (color)
	{
		params.color = ConvertColor(color.value(), "StructuredLightScannerJsonRpc::GetCodedLightPhaseShiftParams");
	}
	Optional<std::string> orientation;
	m_client->ExtractJsonParam(orientation, result, "orientation");
	if (orientation)
	{
		params.orientation = ConvertOrientation(orientation.value(), "StructuredLightScannerJsonRpc::GetCodedLightPhaseShiftParams");
	}
	m_client->ExtractJsonParam(params.brightness, result, "brightness");
	m_client->ExtractJsonParam(params.noiseReduction, result, "noiseReduction");

	return params;
}


void StructuredLightScannerJsonRpc::SetResultFiltering(const ResultFilteringParams& params)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::SetResultFiltering");
	if (params.qualityCheck)             rpc["params"]["qualityCheck"]             = params.qualityCheck.value();
	if (params.backgroundRemovalEnabled) rpc["params"]["backgroundRemovalEnabled"] = params.backgroundRemovalEnabled.value();
	if (params.backgroundRemovalEpsilon) rpc["params"]["backgroundRemovalEpsilon"] = params.backgroundRemovalEpsilon.value();
	if (params.outlierRemoval)           rpc["params"]["outlierRemoval"]           = params.outlierRemoval.value();

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


ResultFilteringParams StructuredLightScannerJsonRpc::GetResultFiltering()
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::GetResultFiltering");

	ResultFilteringParams params;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	Json::Value result = response["result"];
	m_client->ExtractJsonParam(params.qualityCheck,             result, "qualityCheck");
	m_client->ExtractJsonParam(params.backgroundRemovalEnabled, result, "backgroundRemovalEnabled");
	m_client->ExtractJsonParam(params.backgroundRemovalEpsilon, result, "backgroundRemovalEpsilon");
	m_client->ExtractJsonParam(params.outlierRemoval,           result, "outlierRemoval");
	
	return params;
}


void StructuredLightScannerJsonRpc::ImportCalibration(const std::string& camCalibFilename, const std::string& projectorCalibFilename)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::ImportCalibration");
	rpc["params"]["cameraCalibrationFilename"]    = camCalibFilename;
	rpc["params"]["projectorCalibrationFilename"] = projectorCalibFilename;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


void StructuredLightScannerJsonRpc::ExportCalibration(const std::string& camCalibFilename, const std::string& projectorCalibFilename)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::ExportCalibration");
	rpc["params"]["cameraCalibrationFilename"]    = camCalibFilename;
	rpc["params"]["projectorCalibrationFilename"] = projectorCalibFilename;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


void StructuredLightScannerJsonRpc::ChangeCameraCalibrationResolution(int newWidth, int newHeight)
{
	ThrowException(Error_NotImplemented, "StructuredLightScannerJsonRpc::ChangeCameraCalibrationResolution is not implemented yet", "StructuredLightScannerJsonRpc::ChangeCameraCalibrationResolution");
	// TODO
}


std::vector<CalibPoint> StructuredLightScannerJsonRpc::Calibrate(double scale)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::Calibrate");
	rpc["params"]["calibScale"] = scale;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	return std::vector<CalibPoint>(); // TODO
}


int StructuredLightScannerJsonRpc::Scan(bool grabTexture, bool setAsBackground)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::Scan");
	rpc["params"]["grabTexture"]     = grabTexture;
	rpc["params"]["setAsBackground"] = setAsBackground;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	return m_client->ExtractJsonParam<int>(response, "result");
}


void StructuredLightScannerJsonRpc::GrabTexture()
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::GrabTexture");
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


void StructuredLightScannerJsonRpc::AutoWhiteBalance()
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::AutoWhiteBalance");
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


int StructuredLightScannerJsonRpc::AddScanToShapeFusion()
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::AddScanToShapeFusion");
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	return m_client->ExtractJsonParam<int>(response, "result");
}


void StructuredLightScannerJsonRpc::ExportMesh(const std::string& filename)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::ExportMesh");
	rpc["params"]["filename"] = filename;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


void StructuredLightScannerJsonRpc::ExportBackgroundDepthmap(const std::string& filename)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::ExportBackgroundDepthmap");
	rpc["params"]["filename"] = filename;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


void StructuredLightScannerJsonRpc::ImportBackgroundDepthmap(const std::string& filename)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::sls::ImportBackgroundDepthmap");
	rpc["params"]["filename"] = filename;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}



void StructuredLightScannerJsonRpc::GetBackgroundDepthmap(std::vector<float>& depthValues, int &width, int &height)
{
	ThrowException(Error_NotImplemented, "StructuredLightScannerJsonRpc::GetBackgroundDepthmap is not implemented yet", "StructuredLightScannerJsonRpc::GetBackgroundDepthmap");
}


void StructuredLightScannerJsonRpc::SetBackgroundDepthmap(const std::vector<float>& depthValues, int width, int height)
{
	ThrowException(Error_NotImplemented, "StructuredLightScannerJsonRpc::SetBackgroundDepthmap is not implemented yet", "StructuredLightScannerJsonRpc::SetBackgroundDepthmap");
}


void StructuredLightScannerJsonRpc::SetBackgroundPlanes(const std::vector<Plane>& planes)
{
	ThrowException(Error_NotImplemented, "StructuredLightScannerJsonRpc::SetBackgroundPlane is not implemented yet", "StructuredLightScannerJsonRpc::SetBackgroundPlane");
}


} // namespace
