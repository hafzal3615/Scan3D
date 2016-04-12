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

#ifndef DAVID_SDK_I_STRUCTURED_LIGHT_SCANNER_H
#define DAVID_SDK_I_STRUCTURED_LIGHT_SCANNER_H

#include "davidSDK/Common.h"
#include <vector>

namespace david {

//*****************************************************************************
/// @addtogroup InterfaceGroup
/// @{
/// @defgroup StructuredLightScannerGroup Structured Light Scanning
/// Functions and types related to 'Structured Light Scanning'.
/// @{
//*****************************************************************************

//=============================================================================
// ImageFormat
//=============================================================================

/// Describes the image format.
struct ImageFormat
{
	int           width;        ///< Width of the image.
	int           height;       ///< Height of the image.
	PixelFormat   pixelFormat;  ///< Pixel format.
	double        fps;          ///< Frames per second.
	
	/// Constructor.
	/// 
	/// @param[in] width Sets the image width.
	/// @param[in] height Sets the image height.
	/// @param[in] fps Sets the frames per second.
	/// @param[in] pixelFormat Sets the pixel format.
	ImageFormat(int width=0, int height=0, PixelFormat pixelFormat = UnknownPixelFormat, double fps=0);
};


//=============================================================================
// CameraPropertyTarget
//=============================================================================

/// Defines different types of usage for camera properties.
/// Camera properties are distinguished by their name and their usage (e.g. usage in scanning or texturing). 
enum CamPropsUsage
{
	CamPropsForAll,              ///< Camera property should be used for all algorithms.
	CamPropsForScanning,         ///< Camera property should be used for scanning only.
	CamPropsForTexturing,        ///< Camera property should be used for texturing only.
};


//=============================================================================
// Orientation
//=============================================================================

/// Defines orientations for hardware setup.
enum Orientation
{
	HorizontalOrientation,       ///< Horizontal setup. Projector and camera are next to each other.
	VerticalOrientation,         ///< Vertical setup. Projector and camera are above each other.
	BothOrientations,            ///< Diagonal setup. Projector and camera are located diagonally.
};


//=============================================================================
// PatternColor
//=============================================================================

/// Defines pattern colors for SL.
enum PatternColor
{
	White,
	Red,
	Green,
	Blue,
};


//=============================================================================
// CodedLightPhaseShiftParams
//=============================================================================

/// Defines parameters used in structured light mode 'coded light + phase shift' ('cl+ps').
struct CodedLightPhaseShiftParams
{
	Optional<int>             frequencies;       ///< Number of frequencies. 0==auto (but not available in DLL mode).
	Optional<bool>            inverse;           ///< Use inverse Coded Light patterns. Doubles the amount of Coded Light patterns.
	Optional<int>             shifts;            ///< Number of sine patterns (Phase Shift shifts).
	Optional<Orientation>     orientation;       ///< Defines orientation of hardware setup.
	Optional<PatternColor>    color;             ///< Color of the projected patterns.
	Optional<int>             brightness;        ///< Brightness of the projected patterns [0-255]. Default is 255.
	Optional<int>             noiseReduction;    ///< Amount of noise reduction.
};


//=============================================================================
// ResultFilteringParams
//=============================================================================

/// Defines parameters used for result filtering.
struct ResultFilteringParams
{
	Optional<double>          qualityCheck;                  ///< Quality threshold for 3D data [0,1].
	Optional<bool>            backgroundRemovalEnabled;      ///< Background Removal enabled (true/false).
	Optional<double>          backgroundRemovalEpsilon;      ///< Epsilon for Background Removal.
	Optional<double>          outlierRemoval;                ///< Outlier filter [0,1]
};


//=============================================================================
// CalibPoint
//=============================================================================

/// Point correspondence used for calibration.
/// 'worldRefPos3d' is projected into camera image at 'projectedPos2d'.
/// The error between 'projectedPos2d' and measured marker point 'measuredPos2d' should be minimal.
struct CalibPoint
{
	double    worldRefPos3d[3];     ///< Reference 3d point (x,y,z) with respect to world coordinates.
	double    measuredPos2d[2];     ///< Measured 2d image point with respect to image coordinates.
	double    projectedPos2d[2];    ///< Image coordinates of 'worldRefPos3d' projected into image using calibrated model parameters.
};


/// Defines a plane in 3D.
struct Plane
{
	double pos[3];            ///< One point on the plane (X,Y,Z)
	double normal[3];         ///< Normal vector of the plane (X,Y,Z)
};

//=============================================================================
// IStructuredLightScanner
//=============================================================================

/// Interface class for 'Structured Light Scanning'.
///
/// Example:
/// @include SimpleStructuredLightScanner.cpp
class IStructuredLightScanner
{
public:
	/// Destructor.
	virtual ~IStructuredLightScanner() {}

	/// Set the new screen ID. The value 0 switches the projection off.
	/// @param[in] screenID ID of the screen used for displaying structured light patterns, or 0 for OFF.
	virtual void SetScreenID(int screenID) = 0;

	/// Returns a list of available cameras.
	/// @return The list of names.
	virtual std::vector<std::string> GetAvailableCameraNames() = 0;

	/// Select a new camera.
	///
	/// Example:
	/// @code
	/// SelectCamera("DAVID-CAM-3-M (8F6DEE1E)");
	/// @endcode
	///
	/// @param[in] cameraName Name of the camera.
	/// @param[in] imageFormat Format of the camera image.
	/// @exception david::Error_DeviceNotAvailable (david::FunctionException) Selected camera is not available.
	/// @exception david::Error_FormatNotAvailable (david::FunctionException) Selected camera image format is invalid.
	virtual void SelectCamera(const std::string& cameraName, const ImageFormat& imageFormat = ImageFormat()) = 0;

	/// Check state of camera.
	/// @return False, if camera is not working correctly or camera is not connected.
	/// @return True, if camera is connected and is working correctly.
	virtual bool CheckCamera() = 0;

	/// Set the camera property 'propertyName'.
	///
	/// Here are some important property names:
	/// - exposure (should be 1/60 s, 1/30 s, or 1/15 s for most cases)
	/// - gain (should be as low as possible)
	/// - brightness
	/// - sharpness 
	/// - gamma
	/// - ....
	/// 
	/// @note Not all cameras support all properties. See also #GetCameraPropertyNames.
	///
	/// @param[in] propertyName Name of property.
	/// @param[in] value New value of the property.
	/// @param[in] camPropsUsage For which algorithm should the camera property be set? See david::CamPropsSelect.
	/// @exception david::Error_DeviceNotAvailable (david::FunctionException) Selected camera is not available.
	/// @exception david::Error_InvalidArgument (david::FunctionException) Invalid property or property is not available.
	virtual void SetCameraProperty(const std::string& propertyName, double value, enum CamPropsUsage camPropsUsage = CamPropsForAll) = 0;

	/// Get the camera property 'propertyName'.
	/// @param[in] propertyName Name of property.
	/// @param[in] camPropsUsage For which algorithm should the camera property be queried? See david::CamPropsSelect. CamPropsForAll is not allowed here.
	/// @return Value of the camera property.
	/// @exception david::Error_DeviceNotAvailable (david::FunctionException) Selected camera is not available.
	/// @exception david::Error_InvalidArgument (david::FunctionException) Invalid property or property is not available.
	virtual double GetCameraProperty(const std::string& propertyName, enum CamPropsUsage camPropsUsage = CamPropsForScanning) = 0;

	/// Get a list of camera property names.
	/// @param[in] onlyAvailable List only available camera properties?
	/// @return Array of camera property names.
	virtual std::vector<std::string> GetCameraPropertyNames(bool onlyAvailable) = 0;

	/// Selects a sequence of image files as "virtual camera". All images must be located in the same directory.
	/// The image file names are chosen automatically: 
	/// <table borders="0">
	///   <tr> <td>Image for camera calibration</td>   <td>File name must contain "cam_calib"</td> </tr>
	///   <tr> <td>Pattern images</td>                 <td>File names must contain "image" and be sorted alphabetically.</td> </tr>
	///   <tr> <td>Texture image</td>                  <td>File name must contain "texture"</td> </tr>
	/// </table>
	///
	/// @param[in] directory Path to the image files.
	/// @exception david::Error_InvalidPath (david::FunctionException) Path invalid.
	/// @exception david::Error_DirectoryNotFound (david::FunctionException) Directory does not exist.
	virtual void SelectImageSequence(const std::string& directory) = 0;

	/// Get the current grayscale live image from the camera.
	/// @param[out] pixelValues 8bit gray scale pixel values are writting into this buffer.
	/// @param[out] width Width of the image.
	/// @param[out] height Height of the image.
	/// @exception david::Error_NoLiveImage (david::FunctionException) Live image is not available.
	virtual void GetLiveImage(std::vector<uint8_t>& pixelValues, int& width, int& height) = 0;

	/// Select scan mode Coded Light + Phase Shift (standard DAVID4 mode) and sets the parameters.
	/// Missing parameters will remain unchanged.
	/// @param[in] params Parameters for this structured light mode.
	virtual void SetCodedLightPhaseShiftMode(const CodedLightPhaseShiftParams& params) = 0;

	/// Get current SLS params for Coded Light and Phase Shift.
	/// @return Parameters for this structured light mode. See david::CodedLightPhaseShiftParams.
	virtual CodedLightPhaseShiftParams GetCodedLightPhaseShiftParams() = 0;

	/// Set the result filtering parameters for SL scans.
	/// @param[in] params Parameters for result filtering.
	virtual void SetResultFiltering(const ResultFilteringParams& params) = 0;

	/// Get the result filtering parameters for SL scans.
	/// @return Current parameters for result filtering.
	virtual ResultFilteringParams GetResultFiltering() = 0;

	/// Import camera and projector calibration from XML files.
	/// @param[in] camCalibFilename Filename of the camera calibration to be imported from.
	/// @param[in] projectorCalibFilename Filename of the projector calibration to be imported from.
	/// @exception david::Error_CameraCalibration (david::FunctionException) Could not read camera calibration file.
	/// @exception david::Error_ProjectorCalibration (david::FunctionException) Could not read projector calibration file.
	virtual void ImportCalibration(const std::string& camCalibFilename, const std::string& projectorCalibFilename) = 0;

	/// Export camera and projector calibration to XML files.
	/// @param[in] camCalibFilename Filename of the camera calibration to be exported to.
	/// @param[in] projectorCalibFilename Filename of the projector calibration to be exported to.
	/// @exception david::Error_NotCalibrated (david::FunctionException) Scanner is not calibrated.
	virtual void ExportCalibration(const std::string& camCalibFilename, const std::string& projectorCalibFilename) = 0;

	/// Change resolution of image format used camera calibration.
	/// @warning The aspect ratio is not allowed to change. 
	/// @param[in] newWidth New camera image width.
	/// @param[in] newHeight New camera image height.
	/// @exception david::Error_NotCalibrated (david::FunctionException) Camera is not calibrated.
	/// @exception david::Error_InvalidFormat (david::FunctionException) Requested image resolution not supported.
	virtual void ChangeCameraCalibrationResolution(int newWidth, int newHeight) = 0;

	/// Trigger automatic calibration of the Structured Light Scanner.
	/// The function returns when calibration is finished or an error occurred.
	/// @param[in] scale Scale of the calibration patterns. Typically in mm.
	/// @return If calibration is successful: Array of correspondences used for calibration. Empty array otherwise.
	/// @exception david::Error_CameraCalibration (david::FunctionException) Camera calibration failed.
	/// @exception david::Error_ProjectorCalibration (david::FunctionException) Projector calibration failed.
	virtual std::vector<CalibPoint> Calibrate(double scale) = 0;

	/// Helper function for computing the calibration error given an array of calibration correspondences.
	/// @param[out] maxDelta Maximum Euclidean distance [pixel] between any corresponding 'measuredPos2d' and 'projectedPos2d'.
	/// @param[out] rmsError Total root mean square error.
	/// @param[in] calibPoints Array of calibration points. See david::CalibPoint.
	static void GetCalibrationError(double& maxDelta, double& rmsError, const std::vector<CalibPoint>& calibPoints);

	/// Trigger a Structured Light Scan.
	/// The function returns when scanning is finished or an error occurred.
	/// @param[in] grabTexture     Also grab a texture with the scan (default: false)
	/// @param[in] setAsBackground Define this scan as background (for Background Removal in ResultFiltering) (default: false)
	/// @return Number of triangulated 3D points.
	/// @exception david::Error_NotCalibrated (david::FunctionException) Cannot scan because scanner is not calibrated.
	virtual int Scan(bool grabTexture=false, bool setAsBackground=false) = 0;

	/// Grab a new texture.
	virtual void GrabTexture() = 0;

	/// Execute an auto white balancing procedure.
	virtual void AutoWhiteBalance() = 0;

	/// Add the current scan to 'Shape Fusion' module.
	/// @return ID of the mesh.
	/// @exception david::Error_MissingObject (david::FunctionException) No scan data at hand.
	virtual int AddScanToShapeFusion() = 0;

	/// Export the current scan as a triangular mesh.
	/// See documentation of DAVID software for supported file formats.
	///
	/// @warning File paths like "c:\\meshes\\myobject.obj" are interpreted as files located on the server.
	/// For files located on the client you have to use a shared network path like "\\\\mycomputer\\meshes\\myobject.obj".
	/// 
	/// @param[in] filename Filename of the mesh to be exported.
	/// @exception david::Error_NoAccess (david::FunctionException) No access to path.</td> </tr>
	/// @exception david::Error_InvalidPath (david::FunctionException) Export path invalid.</td> </tr>
	/// @exception david::Error_DirectoryNotFound (david::FunctionException) Export target directory does not exist.</td> </tr>
	/// @exception david::Error_MissingFilename (david::FunctionException) Export path misses file name.</td> </tr>
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.</td> </tr>
	virtual void ExportMesh(const std::string& filename) = 0;

	/// Export the current background as a depth map file (.PFM).
	///
	/// @param[in] filename Filename of the depth map to be exported.
	virtual void ExportBackgroundDepthmap(const std::string& filename) = 0;

	/// Import background from a depth map file (.PFM).
	///
	/// @param[in] filename Filename of the depth map to be imported.
	virtual void ImportBackgroundDepthmap(const std::string& filename) = 0;

	/// Gets the currently used background scan as depth map.
	///
	/// @param[out] depthValues The depth values, row by row, from top to bottom, each row left to right.
	/// @param[out] width Number of columns
	/// @param[out] height Number of rows
	virtual void GetBackgroundDepthmap(std::vector<float>& depthValues, int &width, int &height) = 0;

	/// Sets the background scan, giving a depth map. The resolution should be identical to the camera resolution.
	///
	/// @param[in] depthValues The depth values, row by row, from top to bottom, each row left to right.
	/// @param[in] width Number of columns
	/// @param[in] height Number of rows
	virtual void SetBackgroundDepthmap(const std::vector<float>& depthValues, int width, int height) = 0;

	/// Sets the "background scan" from a given list of planes. Camera must be calibrated first!
	///
	/// @param[in] planes The list of planes that define the background.
	virtual void SetBackgroundPlanes(const std::vector<Plane>& planes) = 0;
};

/// @} StructuredLightScannerGroup
/// @} InterfaceGroup

} // namespace

#endif // DAVID_SDK_I_STRUCTURED_LIGHT_SCANNER_H
