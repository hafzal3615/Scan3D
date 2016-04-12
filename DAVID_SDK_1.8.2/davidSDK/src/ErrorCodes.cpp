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

//=============================================================================
// This file was generated automatically.
// Do not modify this file.
//=============================================================================

#include "davidSDK/ErrorCodes.h"

namespace david {

//=============================================================================
// Global functions
//=============================================================================

std::string GetDefaultErrorText(ErrorCode errorCode)
{
	if (errorCode == OK) return "All ok.";
	if (errorCode == Error_InvalidArgument) return "One or more arguments of function call are not valid.";
	if (errorCode == Error_FormatNotAvailable) return "Format is not available.";
	if (errorCode == Error_InvalidFormat) return "Invalid format.";
	if (errorCode == Error_NoAccess) return "General access denied error.";
	if (errorCode == Error_MissingObject) return "Important object is missing.";
	if (errorCode == Error_OutOfMemory) return "Failed to allocate necessary memory.";
	if (errorCode == Error_Abort) return "Operation aborted.";
	if (errorCode == Error_Fail) return "Unspecified failure.";
	if (errorCode == Error_NotLicensed) return "Software license does not support this request.";
	if (errorCode == Error_NotImplemented) return "The requested function is not implemented.";
	if (errorCode == Error_InvalidPath) return "Invalid path syntax.";
	if (errorCode == Error_FileNotFound) return "Could not find the specified file.";
	if (errorCode == Error_DirectoryNotFound) return "Could not find the specified directory.";
	if (errorCode == Error_MissingFilename) return "The specified path does not contain a file name.";
	if (errorCode == Error_InconsistentBuffer) return "Buffer size differs from expected buffer size.";
	if (errorCode == Error_InvalidVersion) return "Invalid or unsupported version.";
	if (errorCode == Error_DeviceNotAvailable) return "Device is not available / not found.";
	if (errorCode == Error_NoLiveImage) return "Can not get live image from camera.";
	if (errorCode == Error_ActuatorStalled) return "Actuator stalled. Required torque might be too high.";
	if (errorCode == Error_CameraCalibration) return "Camera calibration failed.";
	if (errorCode == Error_ProjectorCalibration) return "Projector calibration failed.";
	if (errorCode == Error_ScanFailed) return "Scan failed.";
	if (errorCode == Error_NotCalibrated) return "Scanner is not calibrated.";
	if (errorCode == Error_AlignFailed) return "Alignment failed.";
	if (errorCode == Error_InvalidMeshId) return "One or more mesh IDs are invalid.";
	if (errorCode == ConnectionErrors) return "Errors related to the RPC connection between client and server. All errors below this value are connection errors.";
	if (errorCode == Error_ConnectionFailed) return "Connection to server failed.";
	if (errorCode == Error_ConnectionLost) return "Lost connection between client and server.";
	if (errorCode == ConnectionBinaryErrors) return "Special connection errors related to binary transfer of data via binary channel.";
	if (errorCode == Error_BinarySetup) return "Setup of binary channel failed.";
	if (errorCode == Error_BinaryRead) return "Reading binary data from server failed.";
	if (errorCode == Error_BinaryWrite) return "Writing binary data to server failed.";
	if (errorCode == ConnectionProtocolErrors) return "Special connection errors related to remote procedure calls. Range [-32099, -32000] reserved for implementation-defined server protocol errors.";
	if (errorCode == Error_RpcClientVersion) return "Invalid client version that is not supported by the server.";
	if (errorCode == Error_RpcInvalidRequest) return "The JSON sent is not a valid Request object.";
	if (errorCode == Error_RpcMethodNotFound) return "The method does not exist / is not available.";
	if (errorCode == Error_RpcInvalidParams) return "Invalid method parameter(s).";
	if (errorCode == Error_RpcInternal) return "Internal JSON-RPC error.";
	if (errorCode == Error_RpcParse) return "Invalid JSON was received by the server. An error occurred on the server while parsing the JSON text.";
	if (errorCode == Error_DavidInternal) return "Internal DAVID error.";
	return "Unknown error code.";
}


} // namespace

