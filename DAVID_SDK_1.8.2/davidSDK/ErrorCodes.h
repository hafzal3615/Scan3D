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

#pragma once

#ifndef DAVID_SDK_ERROR_CODES_H
#define DAVID_SDK_ERROR_CODES_H

#include <string>

//=============================================================================
// ErrorCode
//=============================================================================

namespace david {

/// Defines error codes.
///
enum ErrorCode
{

	//-------------------------------------------------------------------------
	// Success values.
	//-------------------------------------------------------------------------

	OK = 0,                                                 ///< All ok.

	//-------------------------------------------------------------------------
	// General errors.
	//-------------------------------------------------------------------------

	Error_InvalidArgument = -100,                           ///< One or more arguments of function call are not valid.
	Error_FormatNotAvailable = -101,                        ///< Format is not available.
	Error_InvalidFormat = -102,                             ///< Invalid format.
	Error_NoAccess = -103,                                  ///< General access denied error.
	Error_MissingObject = -104,                             ///< Important object is missing.
	Error_OutOfMemory = -105,                               ///< Failed to allocate necessary memory.
	Error_Abort = -106,                                     ///< Operation aborted.
	Error_Fail = -107,                                      ///< Unspecified failure.
	Error_NotLicensed = -108,                               ///< Software license does not support this request.
	Error_NotImplemented = -109,                            ///< The requested function is not implemented.

	//-------------------------------------------------------------------------
	// Typical IO errors.
	//-------------------------------------------------------------------------

	Error_InvalidPath = -200,                               ///< Invalid path syntax.
	Error_FileNotFound = -201,                              ///< Could not find the specified file.
	Error_DirectoryNotFound = -202,                         ///< Could not find the specified directory.
	Error_MissingFilename = -203,                           ///< The specified path does not contain a file name.
	Error_InconsistentBuffer = -204,                        ///< Buffer size differs from expected buffer size.
	Error_InvalidVersion = -205,                            ///< Invalid or unsupported version.

	//-------------------------------------------------------------------------
	// Device related errors.
	//-------------------------------------------------------------------------

	Error_DeviceNotAvailable = -300,                        ///< Device is not available / not found.
	Error_NoLiveImage = -301,                               ///< Can not get live image from camera.
	Error_ActuatorStalled = -302,                           ///< Actuator stalled. Required torque might be too high.

	//-------------------------------------------------------------------------
	// Special calibration errors.
	//-------------------------------------------------------------------------

	Error_CameraCalibration = -400,                         ///< Camera calibration failed.
	Error_ProjectorCalibration = -401,                      ///< Projector calibration failed.

	//-------------------------------------------------------------------------
	// Scanning errors.
	//-------------------------------------------------------------------------

	Error_ScanFailed = -500,                                ///< Scan failed.
	Error_NotCalibrated = -501,                             ///< Scanner is not calibrated.

	//-------------------------------------------------------------------------
	// Special 'Shape Fusion' errors.
	//-------------------------------------------------------------------------

	Error_AlignFailed = -600,                               ///< Alignment failed.
	Error_InvalidMeshId = -601,                             ///< One or more mesh IDs are invalid.

	//-------------------------------------------------------------------------
	// Critical errors related to the connection between client and server.
	//-------------------------------------------------------------------------

	ConnectionErrors = -30000,                              ///< Errors related to the RPC connection between client and server. All errors below this value are connection errors.
	Error_ConnectionFailed = -30001,                        ///< Connection to server failed.
	Error_ConnectionLost = -30002,                          ///< Lost connection between client and server.

	ConnectionBinaryErrors = -31000,                        ///< Special connection errors related to binary transfer of data via binary channel.
	Error_BinarySetup = -31001,                             ///< Setup of binary channel failed.
	Error_BinaryRead = -31002,                              ///< Reading binary data from server failed.
	Error_BinaryWrite = -31003,                             ///< Writing binary data to server failed.

	ConnectionProtocolErrors = -32000,                      ///< Special connection errors related to remote procedure calls. Range [-32099, -32000] reserved for implementation-defined server protocol errors.
	Error_RpcClientVersion = -32001,                        ///< Invalid client version that is not supported by the server.
	Error_RpcInvalidRequest = -32600,                       ///< The JSON sent is not a valid Request object.
	Error_RpcMethodNotFound = -32601,                       ///< The method does not exist / is not available.
	Error_RpcInvalidParams = -32602,                        ///< Invalid method parameter(s).
	Error_RpcInternal = -32603,                             ///< Internal JSON-RPC error.
	Error_RpcParse = -32700,                                ///< Invalid JSON was received by the server. An error occurred on the server while parsing the JSON text.

	Error_DavidInternal = -40000,                           ///< Internal DAVID error.

};


//=============================================================================
// Global functions
//=============================================================================

/// Returns a default (standard) textual description for a given error code.
///
/// @param[in] errorCode Error code (see david::ErrorCode).
///
/// @return Textual description of error code.
///
std::string GetDefaultErrorText(ErrorCode errorCode);


} // namespace

#endif // DAVID_SDK_ERROR_CODES_H
