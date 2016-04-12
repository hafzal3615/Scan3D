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

#ifndef DAVID_SDK_JSON_RPC_FUNCTIONS_H
#define DAVID_SDK_JSON_RPC_FUNCTIONS_H

namespace david { 

//*****************************************************************************
/// @addtogroup LowLevelGroup
/// @{
/// @defgroup JsonRpcGroup JsonRPC protocol specification
/// Specification of communication protocol between a DAVID client and a DAVID Enterprise Server
/// using JsonRPC remote procedure calls.
/// @{
//*****************************************************************************

/// Specifications for DAVID remote procedure calls in JSON RPC 2.0.
///
/// @warning Only for advanced users who want to implement the protocol for a different platform.
///
/// A list of supported JSON RPC 2.0 functions in DAVID is given in the documentation of the following enumerators:
/// - david::jsonrpc::JsonRpcFunctions
/// - david::jsonrpc::JsonRpcFunctions_sls
/// - david::jsonrpc::JsonRpcFunctions_turntable
/// - david::jsonrpc::JsonRpcFunctions_shapefusion
/// - david::jsonrpc::JsonRpcFunctions_measure
///
/// Only "params" and "result" of the transmitted JSON RPC 2.0 content is documented in the enumerators. Complete example:
///
/// @code
/// --> {"jsonrpc": "2.0", "method": "david::shapefusion::DeleteMesh", "params": {"meshID" : 5}, "id": 1}
/// <-- {"jsonrpc": "2.0", "result": null, "id": 1}
/// @endcode
///
/// Or in case of an error:
///
/// @code
/// --> {"jsonrpc": "2.0", "method": "david::shapefusion::DeleteMesh", "params": {"meshID" : 5}, "id": 1}
/// <-- {"jsonrpc": "2.0", "error": {"code" : -1, "message" : "Invalid mesh ID."}, "id": 1}
/// @endcode
///
/// @note In the individual <b>Error Codes</b> sections of the following documentation 
/// the possible error codes are defined. In case of an error this error is returned
/// in "code" of "error" (see example above).
///
/// @see http://www.jsonrpc.org/specification
///
namespace jsonrpc {


/// A list of supported JSON RPC 2.0 functions in namespace david.
///
enum JsonRpcFunctions
{	
	/// Initializes connection between client and server.
	///
	/// @code
	/// "method": "david::Connect"
	///
	/// --> "params" : {"clientVersion" : double}
	/// <-- "result" : {"serverVersion" : string, "licenseOk" : bool}
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>clientVersion</td>  <td>Version of the client software represented as a double (like 1.1).</td> </tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>serverVersion</td> <td>Version of the server software.</td>  </tr>
	///   <tr> <td>licenseOk</td>     <td>Is license of server ok?</td>  </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_RpcClientVersion</td> <td>Client version not compatible.</td> </tr>
	/// </table>
	///
	david_Connect = 1,
	
	/// Informs the server that the client is going to disconnect.
	///
	/// @code
	/// "method": "david::Disconnect"
	///
	/// --> "params" : null
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	david_Disconnect,

	/// Requests IP address and port for setting up a binary channel over TCP/IP.
	///
	/// @code
	/// "method": "david::RequestBinaryChannel"
	///
	/// --> "params" : null
	/// <-- "result" : {"port" : uint16_t}
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>port</td>    <td>Server port for the connection of the binary channel.</td>  </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_BinarySetup</td> <td>Binary channel not supported.</td> </tr>
	/// </table>
	///
	david_RequestBinaryChannel,

	/// Shows the main window of DAVID Server.
	///
	/// @code
	/// "method": "david::ShowMainWindow"
	///
	/// --> "params" : null
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	david_ShowMainWindow,

	/// Hides the main window of DAVID Server.
	///
	/// @code
	/// "method": "david::HideMainWindow"
	///
	/// --> "params" : null
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	david_HideMainWindow,

	/// Shows the GUI menus in DAVID Server.
	///
	/// @code
	/// "method": "david::ShowMenus"
	///
	/// --> "params" : null
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	david_ShowMenus,

	/// Hides the GUI menus in DAVID Server.
	///
	/// @code
	/// "method": "david::HideMenus"
	///
	/// --> "params" : null
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	david_HideMenus,

	/// Stops the DAVID SDK Server.
	///
	/// @code
	/// "method": "david::StopServer"
	///
	/// --> "params" : null
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	david_StopServer,
};


/// A list of supported JSON RPC 2.0 functions in namespace david::sls.
///
enum JsonRpcFunctions_sls
{
	/// Sets the new screen ID.
	///
	/// @code
	/// "method": "david::sls::SetScreenID"
	///
	/// --> "params" : {"screenID" : int}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>screenID</td>  <td>ID of the screen used for displaying structured light patterns, or 0 for OFF.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidArgument</td>    <td>Invalid screen ID.</td> </tr>
	/// </table>
	///
	david_sls_SetScreenID = 300,

	/// Gets a list of available cameras.
	///
	/// @code
	/// "method": "david::sls::GetAvailableCameraNames"
	///
	/// --> "params" : null
	/// <-- "result" : {"names" : string[]} 
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>names</td>   <td>Array of strings, containing the names of the cameras.</td> </tr>
	/// </table>
	///
	david_sls_GetAvailableCameraNames,

	/// Selects a new camera.
	/// Note: For using pre-recorded image files, please see david::sls::SelectImageSequence.
	///
	/// @code
	/// "method": "david::sls::SelectCamera"
	///
	/// --> "params" : {"cameraName" : string, "format" : {"width" : uint32_t, "height" : uint32_t, "fps" : double, "pixelFormat" : string}}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>cameraName</td>     <td></td>            <td></td>        <td>Name of the camera.</td> </tr>
	///   <tr> <td>[imageFormat]</td>  <td></td>            <td></td>        <td>Desired camera format [optional].</td> </tr>
	///   <tr> <td></td>               <td>width</td>       <td></td>        <td>Desired width. Set 0 for default.</td> </tr>
	///   <tr> <td></td>               <td>height</td>      <td></td>        <td>Desired height. Set 0 for default.</td> </tr>
	///   <tr> <td></td>               <td>fps</td>         <td></td>        <td>Desired frames per second. Set 0 for default.</td> </tr>
	///   <tr> <td></td>               <td>pixelFormat</td> <td></td>        <td>Pixel format type:</td> </tr>
	///   <tr> <td></td>               <td></td>            <td>Default</td> <td>Use default pixel format.</td> </tr>
	///   <tr> <td></td>               <td></td>            <td>Y800</td>    <td>Standard 8bit grayscale format.</td> </tr>
	///   <tr> <td></td>               <td></td>            <td>BY8</td>     <td>Standard 8bit Bayer format.</td> </tr>
	///   <tr> <td></td>               <td></td>            <td>RGB24</td>   <td>Red, green, and blue 8bit color components for each pixel.</td> </tr>
	///   <tr> <td></td>               <td></td>            <td>YUY2</td>    <td>16bit color format. Byte 0=8-bit Y'0; Byte 1=8-bit Cb; Byte 2=8-bit Y'1; Byte 3=8-bit Cr.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_DeviceNotAvailable</td> <td>Selected camera is not available.</td> </tr>
	///   <tr> <td>david::Error_FormatNotAvailable</td> <td>Selected camera image format is invalid.</td> </tr>
	/// </table>
	///
	david_sls_SelectCamera,

	/// Checks the state of the camera.
	///
	/// @code
	/// "method": "david::sls::CheckCamera"
	///
	/// --> "params" : null
	/// <-- "result" : bool
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>true</td>   <td>Camera is connected and is working correctly.</td> </tr>
	///   <tr> <td>false</td>  <td>Camera is not working correctly or camera is not connected.</td> </tr>
	/// </table>
	///
	david_sls_CheckCamera,

	/// Sets the camera property 'propertyName'.
	///
	/// @code
	/// "method": "david::sls::SetCameraProperty"
	///
	/// --> "params" : {"propertyName" : string, "value" : double, "usage" : string}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>propertyName</td>   <td>Name of camera property.</td>    </tr>
	///   <tr> <td>value</td>          <td>New value of the property.</td>  </tr>
	///   <tr> <td>[usage]</td>        <td>Usage for camera property. If usage is omitted, property value is set for all cases.</td> </tr>
	///   <tr> <td></td>               <td>Scanning</td>    <td>Property should be used for scanning only.</td> </tr>
	///   <tr> <td></td>               <td>Texturing</td>   <td>Property should be used for texturing only.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_DeviceNotAvailable</td> <td>Selected camera is not available.</td> </tr>
	///   <tr> <td>david::Error_InvalidArgument</td> <td>Invalid property or property is not available.</td> </tr>
	/// </table>
	///
	david_sls_SetCameraProperty,

	/// Gets the camera property 'propertyName'.
	///
	/// @code
	/// "method": "david::sls::GetCameraProperty"
	///
	/// --> "params" : {"propertyName" : string, "usage" : string}
	/// <-- "result" : {"value" : double}
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>propertyName</td>   <td>Name of camera property.</td>    </tr>
	///   <tr> <td>usage</td>          <td>Usage for camera property. If usage is omitted, property value is set for all cases.</td> </tr>
	///   <tr> <td></td>               <td>Scanning</td>    <td>Property should be used for scanning only.</td> </tr>
	///   <tr> <td></td>               <td>Texturing</td>   <td>Property should be used for texturing only.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_DeviceNotAvailable</td> <td>Selected camera is not available.</td> </tr>
	///   <tr> <td>david::Error_InvalidArgument</td> <td>Invalid property or property is not available.</td> </tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>value</td>   <td>Value of the property.</td> </tr>
	/// </table>
	///
	david_sls_GetCameraProperty,

	/// Gets a list of camera property names. This list may depend on the used camera.
	///
	/// @code
	/// "method": "david::sls::GetCameraPropertyNames"
	///
	/// --> "params" : {"onlyAvailable" : bool}
	/// <-- "result" : {"names" : string[]} 
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>names</td>   <td>Array of strings, containing the names of camera properties.</td> </tr>
	/// </table>
	///
	david_sls_GetCameraPropertyNames,

	/// Selects a sequence of image files as "virtual camera". All images must be located in the same directory.
	/// The image file names are chosen automatically: 
	/// <table borders="0">
	///   <tr> <td>Image for camera calibration</td>   <td>File name must contain "cam_calib"</td> </tr>
	///   <tr> <td>Pattern images</td>                 <td>File names must contain "image" and be sorted alphabetically.</td> </tr>
	///   <tr> <td>Texture image</td>                  <td>File name must contain "texture"</td> </tr>
	/// </table>
	///
	/// @code
	/// "method": "david::sls::SelectImageSequence"
	///
	/// --> "params" : {"directory" : string}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>directory</td>        <td>Path to the image files.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidPath</td>        <td>Path invalid.</td> </tr>
	///   <tr> <td>david::Error_DirectoryNotFound</td>  <td>Directory does not exist.</td> </tr>
	///   <tr> <td>david::Error_DeviceNotAvailable</td> <td>Could not read image sequence.</td> </tr>
	/// </table>
	///
	david_sls_SelectImageSequence,

	/// Gets the current grayscale live image from the camera.
	///
	/// @code
	/// "method": "david::sls::GetLiveImage"
	///
	/// --> "params" : null
	/// <-- "result" : {"width" : int, "height" : int, "size" : uint32_t} 
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>width</td>   <td>Width of the camera image.</td> </tr>
	///   <tr> <td>height</td>  <td>Height of the camera image.</td> </tr>
	///   <tr> <td>size</td>    <td>Size of the pixel buffer in bytes to be transmitted over binary channel.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_NoLiveImage</td>  <td>Live image is not available.</td> </tr>
	/// </table>
	///
	david_sls_GetLiveImage,
	
	/// Selects scan mode Coded Light + Phase Shift (standard DAVID4 mode) and sets the parameters.
	/// Missing parameters will remain unchanged.
	///
	/// @code
	/// "method": "david::sls::SetCodedLightPhaseShiftMode"
	///
	/// --> "params" : {"inverse" : bool, "shifts" : int, "orientation" : string, "brightness" : int, "noiseReduction" : int}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>[frequencies]</td>    <td>Number of pattern frequencies. Default is 0 == auto.</td> </tr>
	///   <tr> <td>[inverse]</td>        <td>Use inverse Coded Light patterns [true|false]. Doubles the amount of Coded Light patterns.</td> </tr>
	///   <tr> <td>[shifts]</td>         <td>Number of sine patterns (Phase Shift shifts) [3-16].</td> </tr>
	///   <tr> <td>[orientation]</td>    <td></td>                  <td>Orientation of SL patterns</td> </tr>
	///   <tr> <td></td>                   <td>horizontal</td>    <td>Patterns for horizontal setups only (projector and camera are next to each other).</td> </tr>
	///   <tr> <td></td>                   <td>vertical</td>      <td>Patterns for vertical setups only (projector and camera are above each other).</td> </tr>
	///   <tr> <td></td>                   <td>both</td>          <td>Both pattern orientations (projector and camera are located diagonally).</td> </tr>
	///   <tr> <td>[brightness]</td>     <td>Brightness of the projected patterns [0-255]. Default is [255].</td> </tr>
	///   <tr> <td>[color]</td>            <td></td>              <td>Color of SL patterns. Default is white.</td> </tr>
	///   <tr> <td></td>                   <td>white</td>         <td>White (default).</td> </tr>
	///   <tr> <td></td>                   <td>red</td>           <td>Red.</td> </tr>
	///   <tr> <td></td>                   <td>green</td>         <td>Green.</td> </tr>
	///   <tr> <td></td>                   <td>blue</td>          <td>Blue.</td> </tr>
	///   <tr> <td>[noiseReduction]</td> <td>Amount of noise reduction [0-5].</td> </tr>
	/// </table>
	///
	david_sls_SetCodedLightPhaseShiftMode,
	
	/// Gets current SLS params for Coded Light and Phase Shift.
	///
	/// @code
	/// "method": "david::sls::GetCodedLightPhaseShiftParams"
	///
	/// --> "params" : null
	/// <-- "result" : {"inverse" : bool, "shifts" : int, "orientation" : string, "brightness" : int, "noiseReduction" : int}
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>inverse</td>        <td>Use inverse Coded Light patterns [true|false]. Doubles the amount of Coded Light patterns.</td> </tr>
	///   <tr> <td>shifts</td>         <td>Number of sine patterns (Phase Shift shifts) [3-16].</td> </tr>
	///   <tr> <td>orientation</td>    <td></td>                  <td>Orientation of SL patterns</td> </tr>
	///   <tr> <td></td>                   <td>horizontal</td>    <td>Patterns for horizontal setups only (projector and camera are next to each other).</td> </tr>
	///   <tr> <td></td>                   <td>vertical</td>      <td>Patterns for vertical setups only (projector and camera are above each other).</td> </tr>
	///   <tr> <td></td>                   <td>both</td>          <td>Both pattern orientations (projector and camera are located diagonally).</td> </tr>
	///   <tr> <td>brightness</td>     <td>Brightness of the projected patterns [0-255].</td> </tr>
	///   <tr> <td>noiseReduction</td> <td>Amount of noise reduction [0-5].</td> </tr>
	/// </table>
	///s
	david_sls_GetCodedLightPhaseShiftParams,
	
	/// Sets the result filtering parameters for SL scans.
	///
	/// @code
	/// "method": "david::sls::SetResultFiltering"
	///
	/// --> "params" : {"qualityCheck" : double, "backgroundRemovalEnabled" : bool, "backgroundRemovalEpsilon" : double}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>qualityCheck</td>              <td>Quality threshold for 3D data in [0,1] [optional; default: keep current setting].</td> </tr>
	///   <tr> <td>backgroundRemovalEnabled</td>  <td>Enable background removal true/false [optional; default: keep current setting].</td> </tr>
	///   <tr> <td>backgroundRemovalEpsilon</td>  <td>Background removal epsilon in [mm] [optional; default: keep current setting].</td> </tr>
	///   <tr> <td>outlierRemoval</td>            <td>Outlier filter [0,1]. E.g. 0.1=remove all fragments that are smaller than 10% of the largest fragment. [optional; default: keep current setting].</td> </tr>
	/// </table>
	///
	david_sls_SetResultFiltering,
	
	/// Gets the result filtering parameters for SL scans.
	///
	/// @code
	/// "method": "david::sls::GetResultFiltering"
	///
	/// --> "params" : null
	/// <-- "result" : {"qualityCheck" : double, "backgroundRemovalEnabled" : bool, "backgroundRemovalEpsilon" : double}
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>qualityCheck</td>              <td>Quality threshold for 3D data [0,1].</td> </tr>
	///   <tr> <td>backgroundRemovalEnabled</td>  <td>Enable background removal true/false.</td> </tr>
	///   <tr> <td>backgroundRemovalEpsilon</td>  <td>Background removal epsilon in [mm]</td> </tr>
	///   <tr> <td>outlierRemoval</td>            <td>Outlier filter value in [0,1].</td> </tr>
	/// </table>
	///
	david_sls_GetResultFiltering,
	
	/// Imports calibration parameters of camera and projector from an XML files.
	///
	/// @warning File paths like "c:\\david\\camera.cal" are interpreted as files located on the server.
	/// For files located on the client you have to use a shared network path like "\\\\mycomputer\\david\\camera.cal".
	///
	/// @code
	/// "method": "david::sls::ImportCalibration"
	///
	/// --> "params" : {"cameraCalibrationFilename" : string, "projectorCalibrationFilename" : string}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>cameraCalibrationFilename</td>     <td>Filename for importing the camera calibration.</td> </tr>
	///   <tr> <td>projectorCalibrationFilename</td>  <td>Filename for importing the projector calibration.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_CameraCalibration</td>     <td>Could not read camera calibration file.</td> </tr>
	///   <tr> <td>david::Error_ProjectorCalibration</td>  <td>Could not read projector calibration file.</td> </tr>
	/// </table>
	///
	david_sls_ImportCalibration,

	/// Exports the current calibration parameters of camera and projector to XML files.
	///
	/// @warning File paths like "c:\\david\\camera.cal" are interpreted as files located on the server.
	/// For files located on the client you have to use a shared network path like "\\\\mycomputer\\david\\camera.cal".
	///
	/// @code
	/// "method": "david::sls::ExportCalibration"
	///
	/// --> "params" : {"cameraCalibrationFilename" : string, "projectorCalibrationFilename" : string}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>cameraCalibrationFilename</td>     <td>Filename for the exported camera calibration.</td> </tr>
	///   <tr> <td>projectorCalibrationFilename</td>  <td>Filename for the exported projector calibration.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_NotCalibrated</td>    <td>Scanner is not calibrated.</td> </tr>
	/// </table>
	///
	david_sls_ExportCalibration,

	/// Triggers automatic calibration of the Structured Light Scanner. The function returns 
	/// when calibration is finished or an error occurred.
	///
	/// @code
	/// "method": "david::sls::Calibrate"
	///
	/// --> "params" : {"calibScale" : double}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>calibScale</td>  <td>Scale of the calibration pattern. Typically in mm.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_CameraCalibration</td>     <td>Camera calibration failed.</td> </tr>
	///   <tr> <td>david::Error_ProjectorCalibration</td>  <td>Projector calibration failed.</td> </tr>
	///   <tr> <td>david::Error_NoLiveImage</td>           <td>Camera/Grabber is not running.</td> </tr>
	/// </table>
	///
	david_sls_Calibrate,
	
	/// Triggers a Structured Light Scan. The function returns when 
	/// scanning is finished or an error occurred.
	///
	/// @code
	/// "method": "david::sls::Scan"
	///
	/// --> "params" : {"grabTexture" : bool, "setAsBackground" : bool}
	/// <-- "result" : int
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>[grabTexture]</td>      <td>Grab a texture with the scan? [optional, default false]</td> </tr>
	///   <tr> <td>[setAsBackground]</td>  <td>Define the new scan as background, for Background Removal (see SetResultFiltering) [optional, default false]</td> </tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>Number of triangulated 3D points.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_NotCalibrated</td>       <td>Cannot scan because scanner is not calibrated.</td> </tr>
	///   <tr> <td>david::Error_NoLiveImage</td>         <td>Camera/Grabber is not running.</td> </tr>
	/// </table>
	///
	david_sls_Scan,

	/// Grabs a new texture.
	///
	/// @code
	/// "method": "david::sls::GrabTexture"
	///
	/// --> "params" : null
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_NoLiveImage</td>         <td>Camera/Grabber is not running.</td> </tr>
	/// </table>
	///
	david_sls_GrabTexture,

	/// Executes an auto white balancing procedure.
	///
	/// @code
	/// "method": "david::sls::AutoWhiteBalance"
	///
	/// --> "params" : null
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_NoLiveImage</td>         <td>Camera/Grabber is not running.</td> </tr>
	/// </table>
	///
	david_sls_AutoWhiteBalance,
	
	/// Adds the current scan to 'Shape Fusion' module.
	///
	/// @code
	/// "method": "david::sls::AddScanToShapeFusion"
	///
	/// --> "params" : null
	/// <-- "result" : int
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>ID of the mesh.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_MissingObject</td>    <td>No scan data at hand.</td> </tr>
	/// </table>
	///
	david_sls_AddScanToShapeFusion,

	/// Exports the scan as a mesh. See documentation of DAVID software for supported file formats.
	/// Note: If SDK Server is not licensed, the resulting OBJ file will have reduced mesh resolution (like Free Edition).
	///
	/// @warning File paths like "c:\\meshes\\myobject.obj" are interpreted as files located on the server.
	/// For files located on the client you have to use a shared network path like "\\\\mycomputer\\meshes\\myobject.obj".
	///
	/// @code
	/// "method": "david::sls::ExportMesh"
	///
	/// --> "params" : {"filename" : string}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>filename</td>  <td>Filename of the mesh to be exported.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidPath</td>       <td>Export path invalid.</td> </tr>
	///   <tr> <td>david::Error_DirectoryNotFound</td> <td>Export target directory does not exist.</td> </tr>
	///   <tr> <td>david::Error_MissingFilename</td>   <td>Export path misses file name.</td> </tr>
	///   <tr> <td>david::Error_MissingObject</td>     <td>No scan data at hand.</td> </tr>
	/// </table>
	///
	david_sls_ExportMesh,

	/// Exports the current background scan as depthmap (PFM file).
	/// Note: If SDK Server is not licensed, this function will not run.
	///
	/// @warning File paths like "c:\\meshes\\background.pfm" are interpreted as files located on the server.
	/// For files located on the client you have to use a shared network path like "\\\\mycomputer\\meshes\\background.pfm".
	///
	/// @code
	/// "method": "david::sls::ExportBackgroundDepthmap"
	///
	/// --> "params" : {"filename" : string}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>filename</td>  <td>Filename of the depthmap to be exported (must end with .pfm).</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidPath</td>       <td>Export path invalid.</td> </tr>
	///   <tr> <td>david::Error_DirectoryNotFound</td> <td>Export target directory does not exist.</td> </tr>
	///   <tr> <td>david::Error_MissingFilename</td>   <td>Export path misses file name.</td> </tr>
	///   <tr> <td>david::Error_MissingObject</td>     <td>No background data at hand.</td> </tr>
	///   <tr> <td>david::Error_NotLicensed</td>       <td>SDK Server is not licensed to save.</td> </tr>
	/// </table>
	///
	david_sls_ExportBackgroundDepthmap,

	/// Imports a background scan from a depthmap (PFM file).
	///
	/// @warning File paths like "c:\\meshes\\background.pfm" are interpreted as files located on the server.
	/// For files located on the client you have to use a shared network path like "\\\\mycomputer\\meshes\\background.pfm".
	///
	/// @code
	/// "method": "david::sls::ImportBackgroundDepthmap"
	///
	/// --> "params" : {"filename" : string}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>filename</td>  <td>Filename of the depthmap to be imported (must end with .pfm).</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidPath</td>       <td>Export path invalid.</td> </tr>
	///   <tr> <td>david::Error_DirectoryNotFound</td> <td>Export target directory does not exist.</td> </tr>
	///   <tr> <td>david::Error_MissingFilename</td>   <td>Export path misses file name.</td> </tr>
	/// </table>
	///
	david_sls_ImportBackgroundDepthmap,
};

/// A list of supported JSON RPC 2.0 functions in namespace david::turntable.
///
enum JsonRpcFunctions_turntable
{
	/// Connects or disconnects a turntable.
	///
	/// @code
	/// "method": "david::turntable::Setup"
	///
	/// --> "params" : {"enable" : bool}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>enable</td>  <td>True for connect, false for disconnect.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_DeviceNotAvailable</td>   <td>Turntable not available</td> </tr>
	/// </table>
	///
	david_turntable_Setup = 400,

	/// Rotates the turntable. The turntable rotates counterclockwise ('degrees' is postive) or clockwise ('degrees' is negative).
	///
	/// @code
	/// "method": "david::turntable::Rotate"
	///
	/// --> "params" : {"degrees" : double}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>degrees</td>  <td>Amount of rotation in degrees.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_DeviceNotAvailable</td>   <td>Turntable not available</td> </tr>
	///   <tr> <td>david::Error_Fail</td>                 <td>Rotation of turntable failed.</td> </tr>
	/// </table>
	///
	david_turntable_Rotate,
};

/// A list of supported JSON RPC 2.0 functions in namespace david::shapefusion.
///
enum JsonRpcFunctions_shapefusion
{
	/// Imports a mesh into the mesh list from a file given by 'filename'. See documentation of DAVID software for supported file formats.
	///
	/// @warning File paths like "c:\\meshes\\myobject.obj" are interpreted as files located on the server.
	/// For files located on the client you have to use a shared network path like "\\\\mycomputer\\meshes\\myobject.obj".
	///
	/// @code
	/// "method": "david::shapefusion::ImportMesh"
	///
	/// --> "params" : {"filename" : string}
	/// <-- "result" : int
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>filename</td>  <td>Filename of the mesh to be imported.</td> </tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>ID of the mesh.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_NoAccess</td>           <td>No access to path.</td> </tr>
	///   <tr> <td>david::Error_InvalidPath</td>        <td>Specified path is invalid.</td> </tr>
	///   <tr> <td>david::Error_MissingFilename</td>    <td>Specified path misses file name.</td> </tr>
	///   <tr> <td>david::Error_DirectoryNotFound</td>  <td>Specified directory does not exist.</td> </tr>
	///   <tr> <td>david::Error_FileNotFound</td>       <td>Specified file does not exist.</td> </tr>
	/// </table>
	///
	david_shapefusion_ImportMesh = 500,

	/// Exports a mesh. This mesh can be an item of the scan list or the fusion result. Just provide a valid 'meshID'.
	/// See documentation of DAVID software for supported file formats.
	/// Note: If SDK Server is not licensed, saving will not be possible, and david::Error_NotLicensed will be returned.
	///
	/// @warning File paths like "c:\\meshes\\myobject.obj" are interpreted as files located on the server.
	/// For files located on the client you have to use a shared network path like "\\\\mycomputer\\meshes\\myobject.obj".
	///
	/// @code
	/// "method": "david::shapefusion::ExportMesh"
	///
	/// --> "params" : {"meshID" : int, "filename" : string}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID</td>    <td>ID of the mesh to be exported.</td> </tr>
	///   <tr> <td>filename</td>  <td>Filename of the mesh to be exported.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_NoAccess</td>           <td>No access to path.</td> </tr>
	///   <tr> <td>david::Error_InvalidPath</td>        <td>Specified path is invalid.</td> </tr>
	///   <tr> <td>david::Error_MissingFilename</td>    <td>Specified path misses file name.</td> </tr>
	///   <tr> <td>david::Error_DirectoryNotFound</td>  <td>Specified directory does not exist.</td> </tr>
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	///   <tr> <td>david::Error_NotLicensed</td>        <td>SDK Server is not licensed to save.</td> </tr>
	/// </table>
	///
	david_shapefusion_ExportMesh,

	/// Duplicate mesh with mesh ID 'sourceMeshID'.
	///
	/// @code
	/// "method": "david::shapefusion::DuplicateMesh"
	///
	/// --> "params" : {"sourceMeshID" : int}
	/// <-- "result" : int
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>sourceMeshID</td>  <td>ID of the mesh to be duplicated.</td> </tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>ID of the newly created mesh.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	/// </table>
	///
	david_shapefusion_DuplicateMesh,

	/// Deletes all meshes in 'Shape Fusion'.
	///
	/// @code
	/// "method": "david::shapefusion::DeleteAllMeshes"
	///
	/// --> "params" : null
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	david_shapefusion_DeleteAllMeshes,

	/// Deletes a specific mesh of the mesh list. 
	///
	/// @code
	/// "method": "david::shapefusion::DeleteMesh"
	///
	/// --> "params" : {"meshID" : int}
	/// <-- "result" : null 
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID</td>    <td>ID of the mesh.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	/// </table>
	///
	david_shapefusion_DeleteMesh,
	
	/// Gets a selected data (array buffer like vertex positions) from a specified mesh.
	/// Note: If SDK Server is not licensed, part of the vertex coordinates / normals (sent over binary channel) will be ZEROES only.
	///
	/// @code
	/// "method": "david::shapefusion::GetMeshBuffer"
	///
	/// --> "params" : {"meshID" : int, "bufferType" : string}
	/// <-- "result" : {"vertexCount": uint32_t, "triangleCount:" uint32_t, "size" : uint32_t}
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID</td>              <td></td>                   <td>ID of the mesh that owns the requested buffer.</td> </tr>
	///   <tr> <td>bufferType</td>          <td></td>                   <td>Type of the buffer to be transmitted over binary channel:</td> </tr>
	///   <tr> <td></td>                    <td>VertexPositions</td>    <td>Positions of all vertices as float triples (x,y,z).</td> </tr>
	///   <tr> <td></td>                    <td>VertexNormals</td>      <td>Normals of all vertices as float triples (nx,ny,nz).</td> </tr>
	///   <tr> <td></td>                    <td>VertexTexCoords</td>    <td>Texture coordinates of all vertices as float pairs (u,v).</td> </tr>
	///   <tr> <td></td>                    <td>Triangles</td>          <td>Triangle indices (indexed face set) as int32_t triples (id1,id2,id3).</td> </tr>
	///   <tr> <td>[coordinateSystem]</td>  <td></td>                   <td>Coordinates should be relative to which coordinate system. World is assumed by default.</td></tr>
	///   <tr> <td></td>                    <td>WorldCoordinates</td>   <td>Coordinates are given with respect to world (global coordinates).</td></tr>
	///   <tr> <td></td>                    <td>LocalCoordinates</td>   <td>Coordinates are given with respect to local object coordinate system.</td></tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>vertexCount</td>    <td>Number of vertices for requested mesh.</td> </tr>
	///   <tr> <td>triangleCount</td>  <td>Number of triangles for requested mesh.</td> </tr>
	///   <tr> <td>size</td>           <td>Size of the buffer in bytes to be transmitted over binary channel.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	///   <tr> <td>david::Error_InvalidArgument</td>    <td>Invalid buffer type.</td> </tr>
	/// </table>
	///
	david_shapefusion_GetMeshBuffer,

	/// Gets the texture image of a mesh as binary data. Each pixel is represented by 3 bytes in BGR order. The pixels are ordered row by row, from top to bottom, each row from left to right. The data size is (3*width*height) bytes.
	///
	/// @code
	/// "method": "david::shapefusion::GetTextureImage"
	///
	/// --> "params" : {"meshID" : int, "sendData" : bool}
	/// <-- "result" : {"width": uint32_t, "height:" uint32_t, "size" : uint32_t}
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID</td>         <td></td>                   <td>ID of the mesh that owns the requested buffer.</td> </tr>
	///   <tr> <td>sendData</td>       <td></td>                   <td>Send image data over binary channel?</td> </tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>width</td>          <td>Width of the texture image in [px].</td> </tr>
	///   <tr> <td>height</td>         <td>Height of the texture image in [px].</td> </tr>
	///   <tr> <td>size</td>           <td>Size of the buffer in bytes to be transmitted over binary channel.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	///   <tr> <td>david::Error_MissingObject</td>      <td>Specified mesh has no texture.</td> </tr>
	/// </table>
	///
	david_shapefusion_GetTextureImage,

	/// Gets the current transformation (position+rotation) of a mesh. Initially, an imported mesh has no transformation. The transformation depends on previous calls of the Align... functions, or the Rotate or Translate functions.
	/// The position is returned in vector (px, py, pz). The rotation is returned in three different representations which are equivalent: A rotation matrix (n,o,a), roll-pitch-yaw angles, and a quaternion.
	/// Note: If SDK Server is not licensed, this function will return the error code david::Error_NotLicensed.
	///
	/// @code
	/// "method": "david::shapefusion::GetPose"
	///
	/// --> "params" : {"meshID" : int}
	/// <-- "result" : {"px": double, "py": double, "pz": double, 
	///                 "nx": double, "ny": double, "nz": double, 
	///                 "ox": double, "oy": double, "oz": double, 
	///                 "ax": double, "ay": double, "az": double, 
	///                 "roll": double, "pitch": double, "yaw": double, 
	///                 "quat0": double, "quat1": double, "quat2": double, "quat3": double}
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID</td>         <td></td>                   <td>ID of the mesh whose pose to retrieve.</td> </tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>px, py, pz</td>                  <td>position vector of the current mesh pose.</td> </tr>
	///   <tr> <td>nx, ny, nz</td>                  <td>1st column of the rotation matrix.</td> </tr>
	///   <tr> <td>ox, oy, oz</td>                  <td>2nd column of the rotation matrix.</td> </tr>
	///   <tr> <td>ax, ay, az</td>                  <td>3rd column of the rotation matrix.</td> </tr>
	///   <tr> <td>roll, pitch, yaw</td>            <td>roll, pitch, yaw angles in [RAD], assuming this multiplication order: RotZ(yaw)*RotY(pitch)*RotX(roll).</td> </tr>
	///   <tr> <td>quat0, quat1, quat2, quat3</td>  <td>Quaternion represention of the mesh rotation (quat0 + i*quat1 + j*quat2 + k*quat3).</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	///   <tr> <td>david::Error_NotLicensed</td>        <td>SDK Server is not licensed.</td> </tr>
	/// </table>
	///
	david_shapefusion_GetPose,

	/// Sets the current transformation (position+rotation) of a mesh. Initially, an imported mesh has no transformation. The transformation depends on previous calls of the Align... functions, or the Rotate or Translate functions.
	/// The position is returned in vector (px, py, pz). The rotation is returned in three different representations which are equivalent: A rotation matrix (n,o,a), roll-pitch-yaw angles, and a quaternion.
	/// Note: If SDK Server is not licensed, this function will return the error code david::Error_NotLicensed.
	///
	/// @code
	/// "method": "david::shapefusion::SetPose"
	///
	/// --> "params" : {"meshID" : int,
	///                 "px": double, "py": double, "pz": double, 
	///                 "nx": double, "ny": double, "nz": double, 
	///                 "ox": double, "oy": double, "oz": double, 
	///                 "ax": double, "ay": double, "az": double}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID</td>                     <td>ID of the mesh whose pose to retrieve.</td> </tr>
	///   <tr> <td>px, py, pz</td>                  <td>position vector of the current mesh pose.</td> </tr>
	///   <tr> <td>nx, ny, nz</td>                  <td>1st column of the rotation matrix.</td> </tr>
	///   <tr> <td>ox, oy, oz</td>                  <td>2nd column of the rotation matrix.</td> </tr>
	///   <tr> <td>ax, ay, az</td>                  <td>3rd column of the rotation matrix.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	///   <tr> <td>david::Error_NotLicensed</td>        <td>SDK Server is not licensed.</td> </tr>
	/// </table>
	///
	david_shapefusion_SetPose,

	/// Rotates a mesh around specified axis by given degrees. 
	///
	/// @code
	/// "method": "david::shapefusion::Rotate"
	///
	/// --> "params" : {"meshID" : int, "transformType" : string, "degrees" : double }
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	/// 
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID</td>           <td></td>                      <td>ID of mesh to be transformed.</td> </tr>
	///   <tr> <td>transformType</td>    <td></td>                      <td>Selects axis and coordinate system of rotation:</td> </tr>
	///   <tr> <td></td>                 <td>TransformGlobalX</td>      <td>Rotate around global (world) x axis.</td> </tr>
	///   <tr> <td></td>                 <td>TransformGlobalY</td>      <td>Rotate around global (world) y axis.</td> </tr>
	///   <tr> <td></td>                 <td>TransformGlobalZ</td>      <td>Rotate around global (world) z axis.</td> </tr>
	///   <tr> <td></td>                 <td>TransformMeshX</td>        <td>Rotate around mesh's x axis.</td> </tr>
	///   <tr> <td></td>                 <td>TransformMeshY</td>        <td>Rotate around mesh's y axis.</td> </tr>
	///   <tr> <td></td>                 <td>TransformMeshZ</td>        <td>Rotate around mesh's z axis.</td> </tr>
	///   <tr> <td>degrees</td>          <td></td>                      <td>Amount of rotation in degrees</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	///   <tr> <td>david::Error_InvalidArgument</td>    <td>Invalid transform type.</td> </tr>
	/// </table>
	///
	david_shapefusion_Rotate,

	/// Translates a mesh along specified axis by given degrees.
	///
	/// @code
	/// "method": "david::shapefusion::Translate"
	///
	/// --> "params" : {"meshID" : int, "transformType" : string, "amount" : double }
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	/// 
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID</td>           <td></td>                      <td>ID of mesh to be transformed.</td> </tr>
	///   <tr> <td>transformType</td>    <td></td>                      <td>Selects axis and coordinate system of translation:</td> </tr>
	///   <tr> <td></td>                 <td>TransformGlobalX</td>      <td>Translate along global (world) x axis.</td> </tr>
	///   <tr> <td></td>                 <td>TransformGlobalY</td>      <td>Translate along global (world) y axis.</td> </tr>
	///   <tr> <td></td>                 <td>TransformGlobalZ</td>      <td>Translate along global (world) z axis.</td> </tr>
	///   <tr> <td></td>                 <td>TransformMeshX</td>        <td>Translate along mesh's x axis.</td> </tr>
	///   <tr> <td></td>                 <td>TransformMeshY</td>        <td>Translate along mesh's y axis.</td> </tr>
	///   <tr> <td></td>                 <td>TransformMeshZ</td>        <td>Translate along mesh's z axis.</td> </tr>
	///   <tr> <td>amount</td>           <td></td>                      <td>Amount of translation. Typically in mm.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	///   <tr> <td>david::Error_RpcInvalidParams</td>   <td>Invalid transform type.</td> </tr>
	/// </table>
	///
	david_shapefusion_Translate,

	/// Reduces the density of a mesh.
	///
	/// @code
	/// "method": "david::shapefusion::ReduceMeshDensity"
	///
	/// --> "params" : {"meshID" : int, "factor" : float }
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	/// 
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID</td>           <td></td>                      <td>ID of mesh to be reduced.</td> </tr>
	///   <tr> <td>factor</td>           <td></td>                      <td>Factor of reduction, between 0 and 1.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	///   <tr> <td>david::Error_InvalidArgument</td>    <td>Invalid factor value.</td> </tr>
	/// </table>
	///
	david_shapefusion_ReduceMeshDensity,

	/// Combines several meshes into one group.
	///
	/// @code
	/// "method": "david::shapefusion::CombineMeshes"
	///
	/// --> "params" : {"meshIDs" : int[]}
	/// <-- "result" : int
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	/// 
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshIDs</td>          <td></td>                      <td>IDs of the meshes to be combined (must contain at least 2 IDs).</td> </tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>Mesh ID of the combined mesh.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	///   <tr> <td>david::Error_InvalidArgument</td>    <td>Invalid number of IDs.</td> </tr>
	/// </table>
	///
	david_shapefusion_CombineMeshes,

	/// Uncombines a groups of meshes into seperate meshes.
	///
	/// @code
	/// "method": "david::shapefusion::UncombineMeshes"
	///
	/// --> "params" : {"meshID" : int}
	/// <-- "result" : int[]
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	/// 
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID</td>          <td></td>                      <td>ID of the mesh group to be uncombined.</td> </tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>Array of Mesh IDs of the separated meshes.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist, or is no group.</td> </tr>
	/// </table>
	///
	david_shapefusion_UncombineMeshes,

	/// Coarse alignment of a pair of meshes.
	///
	/// @code
	/// "method": "david::shapefusion::AlignPairCoarse"
	///
	/// --> "params" : {"meshID1" : int, 
	///                 "meshID2" : int, 
	///                 "texturePercentage" : int, 
	///                 "motionInfo" : {"angleKnown" : bool, 
	///                                 "axisDirKnown" : bool, 
	///                                 "angle" : double, 
	///                                 "angleTolerance" : double, 
	///                                 "axisDir" : [double,double,double]},
	///                 "qualityFactor" : float }
	/// <-- "result" : double
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID1</td>                    <td></td>               <td>ID of the mesh that is to be aligned.</td> </tr>
	///   <tr> <td>meshID2</td>                    <td></td>               <td>ID of the other mesh.</td> </tr>
	///   <tr> <td>[texturePercentage]</td>        <td></td>               <td>Percentage of texture influence on alignment (0-99). Only used if value is higher than 0 [optional, default 0].</td> </tr>
	///   <tr> <td>[motionInfo]</td>               <td></td>               <td>MotionInfo: Additional information about motion between two meshes [optional].</td> </tr>
	///   <tr> <td></td>                           <td>angleKnown</td>     <td>Is rotation 'angle' around motion axis known?</td>  </tr>
	///   <tr> <td></td>                           <td>axisDirKnown</td>   <td>Is direction vector 'axisDir' of motion axis is known?</td> </tr>
	///   <tr> <td></td>                           <td>angle</td>          <td>Angle of rotation in degrees.</td> </tr>
	///   <tr> <td></td>                           <td>angleTolerance</td> <td>Tolerance of angle in degrees.</td> </tr>
	///   <tr> <td></td>                           <td>axisDir</td>        <td>3D direction vector (x,y,z) of motion axis w.r.t. world coordinates.</td> </tr>
	///   <tr> <td>[qualityFactor]</td>            <td></td>               <td>Quality parameter for alignment. 1 is recommended. Larger values like 5 or 10 may create better results for symmetric objects, but increase computation time. Lower values like 0.01 to 0.1 decrease computation time, but may result in wrong alignment results [optional, default 1].</td> </tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>Quality of alignment result as percentage of contact, in range [0,100]=[bad,good].</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	/// </table>
	///
	david_shapefusion_AlignPairCoarse,

	/// Fine alignment of a pair of meshes.
	///
	/// @code
	/// "method": "david::shapefusion::AlignPairFine"
	///
	/// --> "params" : {"meshID1" : int, "meshID2" : int, "texturePercentage" : int, "maxNumIterations" : int}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID1</td>           <td></td>            <td></td>            <td>ID of the mesh that is to be aligned.</td> </tr>
	///   <tr> <td>meshID2</td>           <td></td>            <td></td>            <td>ID of the other mesh.</td> </tr>
	///   <tr> <td>[texturePercentage]</td> <td></td>          <td></td>            <td>Percentage of texture influence on alignment (0-99). Only used if value is higher than 0 [optional, default 0].</td> </tr>
	///   <tr> <td>[maxNumIterations]</td> <td></td>           <td></td>            <td>Maximum number of iterations to run [optional, default 100].</td> </tr>
	///   <tr> <td>[maxTranslation]</td>   <td></td>           <td></td>            <td>Maximum allowed drift[mm] of mesh center [optional, default = 0 = no limit]. Alignment will stop when this limit is reached.</td> </tr>
	///   <tr> <td>[maxRotation_deg]</td>  <td></td>           <td></td>            <td>Maximum allowed rotation of mesh in degree [optional, default = 0 = no limit]. Alignment will stop when this limit is reached.</td> </tr>
	/// </table>
	///
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>      <td>Specified meshID does not exist.</td> </tr>
	/// </table>
	///
	david_shapefusion_AlignPairFine,

	/// Global fine alignment of all visible meshes.
	///
	/// @code
	/// "method": "david::shapefusion::AlignGlobalFine"
	///
	/// --> "params" : {"texturePercentage" : int, "numIterations" : int}
	/// <-- "result" : null
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>[texturePercentage]</td> <td></td>          <td>Percentage of texture influence on alignment (only used if value is higher than 0 [optional, default 0].</td> </tr>
	///   <tr> <td>[numIterations]</td> <td></td>              <td>Number of iterations to run [optional, default 20].</td> </tr>
	///   <tr> <td>[maxTranslation]</td>   <td></td>           <td>Maximum allowed drift[mm] of any mesh center [optional, default = 0 = no limit]. Global Fine Alignment will not rotate any scan more than this.</td> </tr>
	///   <tr> <td>[maxRotation_deg]</td>  <td></td>           <td>Maximum allowed rotation[deg] of any mesh in degree [optional, default = 0 = no limit]. Global Fine Alignment will not rotate any scan more than this.</td> </tr>
	/// </table>
	///
	david_shapefusion_AlignGlobalFine,

	/// Fuses all visible meshes of the mesh list.
	///
	/// @code
	/// "method": "david::shapefusion::Fuse"
	///
	/// --> "params" : {"resolution" : int, "closeHoles" : bool, "sharpness" : int}
	/// <-- "result" : int
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>resolution</td>        <td>Limits the maximum resolution and thus the required memory usage.</td> </tr>
	///   <tr> <td>holeSizeThresRel</td>  <td>Holes, which have a smaller area than holeSizeThresRel * size of original data surface, will be closed. 0 = open all holes. 1 = close all holes.</td> </tr>
	///   <tr> <td>sharpness</td>         <td>Fusion sharpness in [-3,+5]. Negative values for smoothing. Default 1.</td> </tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>Mesh ID of the fusion result.</td> </tr>
	/// </table>
	///
	david_shapefusion_Fuse,
};


/// A list of supported JSON RPC 2.0 functions in namespace david::measure.
///
enum JsonRpcFunctions_measure
{
	/// Computes signed distance values for each vertex of 'MeasureTestObject' relative to 'MeasureReferenceObject'.
	/// Note: If SDK Server is not licensed, part of the distance values (sent over binary channel) will be ZEROES only.
	///
	/// @code
	/// "method": "david::measure::ComputeSurfaceDistances"
	///
	/// --> "params" : {"meshID1" : int, "meshID2" : int, "distThresh" : double}
	/// <-- "result" : {"vertexCount": uint32_t, "size" : uint32_t}
	/// <-- "error" : {"code" : int, "message" : string}
	/// @endcode
	///
	/// <b>Parameters</b>
	/// <table borders="0">
	///   <tr> <td>meshID1</td>      <td>ID of the mesh that is the test object.</td> </tr>
	///   <tr> <td>meshID2</td>      <td>ID of the other mesh that is the reference object.</td> </tr>
	///   <tr> <td>distThresh</td>   <td>Distance threshold: Distance values are limited to the range [-distThresh, distThresh].</td> </tr>
	/// </table>
	///
	/// <b>Return Values</b>
	/// <table borders="0">
	///   <tr> <td>vertexCount</td>   <td>Number of vertices for mesh</td> </tr>
	///   <tr> <td>size</td>          <td>Size of float array in bytes to be transmitted over binary channel.</td> </tr>
	/// </table>
	/// 
	/// <b>Error Codes</b>
	/// <table borders="0">
	///   <tr> <td>david::Error_InvalidMeshId</td>   <td>One or both of the mesh IDs was invalid.</td> </tr>
	/// </table>
	///
	david_measure_ComputeSurfaceDistances = 600,

};


/// @} // JsonRpcGroup
/// @} // LowLevelGroup

} // namespace
} // namespace

#endif // DAVID_SDK_JSON_RPC_FUNCTIONS_H