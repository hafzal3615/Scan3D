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

#ifndef DAVID_SDK_D4LIB_H
#define DAVID_SDK_D4LIB_H

#include <stddef.h>

#ifdef __cplusplus
	#ifndef NULL
	#define NULL 0
	#endif
#else
	#ifndef NULL
	#define NULL (void *)0
	#endif
#endif 

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
	#ifdef BUILD_D4LIB
		#ifndef D4LIB_API
		#define D4LIB_API __declspec( dllexport )
		#endif
	#else
		#ifndef D4LIB_API
		#define D4LIB_API __declspec( dllimport )
		#endif
	#endif
#else
	#ifndef D4LIB_API
	#define D4LIB_API __attribute__((visibility("default")))
	#endif
#endif

//*****************************************************************************
/// @addtogroup LowLevelGroup
/// @{
/// @defgroup d4lib DAVID Low Level Library (d4lib)
/// Provides a low level C interface to 'Scanning' and 'Shape Fusion' functions.
/// @{
//*****************************************************************************

//=============================================================================
/// @name Library initialization and basic types
/// @note The DAVID Low Level Library has to be initialized once with your license key.
///
/// @code
/// int result = d4lib_init("xxx");
/// @endcode
///
/// When you are done, call #d4lib_release.
/// @{
//=============================================================================

/// Basic size type.
typedef size_t d4size;

/// Image pointer.
typedef struct d4img_* d4img_ptr;

/// Float image pointer.
typedef struct d4img_* d4fimg_ptr;

/// Camera pointer.
typedef struct d4cam_* d4cam_ptr;

/// Projector pointer.
typedef struct d4projector_* d4projector_ptr;

/// Calibration pointer.
typedef struct d4calib_* d4calib_ptr;

/// Structured light scanning pointer.
typedef struct d4sls_* d4sls_ptr;

/// Mesh pointer.
typedef struct d4mesh_* d4mesh_ptr;

/// Initialize DAVID Low Level Library.
/// @param[in] licenseKey License key.
/// @return d4ok, d4error_NotLicensed
D4LIB_API int d4lib_init(const char* licenseKey);

/// Release DAVID Low Level Library.
/// @return d4ok
D4LIB_API int d4lib_release();

/// Get version of library.
/// @param[out] version Valid pointer to string that gets the version.
/// @param[in] maxLength Maximum string length inlucing null terminator that fits into 'version'.
D4LIB_API int d4lib_getVersion(char* version, d4size maxLength);

// BEGIN_D4LIB_ERROR_CODES

/// All DAVID Low Level Library functions return an error code.
enum d4error_ErrorCode
{
	d4ok = 0,                                                       ///< All ok.
	d4error_InvalidArgument = -100,                           ///< One or more arguments of function call are not valid.
	d4error_FormatNotAvailable = -101,                        ///< Format is not available.
	d4error_InvalidFormat = -102,                             ///< Invalid format.
	d4error_NoAccess = -103,                                  ///< General access denied error.
	d4error_MissingObject = -104,                             ///< Important object is missing.
	d4error_OutOfMemory = -105,                               ///< Failed to allocate necessary memory.
	d4error_Abort = -106,                                     ///< Operation aborted.
	d4error_Fail = -107,                                      ///< Unspecified failure.
	d4error_NotLicensed = -108,                               ///< Software license does not support this request.
	d4error_NotImplemented = -109,                            ///< The requested function is not implemented.
	d4error_InvalidPath = -200,                               ///< Invalid path syntax.
	d4error_FileNotFound = -201,                              ///< Could not find the specified file.
	d4error_DirectoryNotFound = -202,                         ///< Could not find the specified directory.
	d4error_MissingFilename = -203,                           ///< The specified path does not contain a file name.
	d4error_InconsistentBuffer = -204,                        ///< Buffer size differs from expected buffer size.
	d4error_InvalidVersion = -205,                            ///< Invalid or unsupported version.
	d4error_DeviceNotAvailable = -300,                        ///< Device is not available / not found.
	d4error_NoLiveImage = -301,                               ///< Can not get live image from camera.
	d4error_ActuatorStalled = -302,                           ///< Actuator stalled. Required torque might be too high.
	d4error_CameraCalibration = -400,                         ///< Camera calibration failed.
	d4error_ProjectorCalibration = -401,                      ///< Projector calibration failed.
	d4error_ScanFailed = -500,                                ///< Scan failed.
	d4error_NotCalibrated = -501,                             ///< Scanner is not calibrated.
	d4error_AlignFailed = -600,                               ///< Alignment failed.
	d4error_InvalidMeshId = -601,                             ///< One or more mesh IDs are invalid.
};

// END_D4LIB_ERROR_CODES

/// @}


//=============================================================================
/// @name Memory management
/// Objects are created and deleted by the library. In order to create new objects
/// use the d4xxx_newXxx functions. Life time of an object is managed by reference counting:
/// Use #d4mem_decRefCount to release an object.
/// @{
//=============================================================================

/// Decrement the reference count for the object identified by 'ptr'.
/// @param[in,out] ptr Valid pointer to the object.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mem_decRefCount(void* ptr);

/// Increment the reference count for the object identified by 'ptr'.
/// The object is deleted when the reference count reaches zero.
/// @param[in,out] ptr Valid pointer to the object.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mem_incRefCount(void* ptr);

/// @}


//=============================================================================
/// @name Images
/// Functions and types related to images.
/// Notes:
/// - Image pointer is #d4img_ptr
/// - New images are created via #d4img_newImage
/// - Different pixel formats are supported. See #d4img_PixelFormat
/// - Access to image format is via #d4img_setImageFormat and #d4img_getImageFormat
/// - Access to image data is via #d4img_setImageData and #d4img_getImageData
/// @{
//=============================================================================

/// Supported pixel formats.
enum d4img_PixelFormat
{
	d4img_UnknownPixelFormat = 0,   ///< Pixel format is unknown.
	d4img_Y800,                     ///< 8bit grayscale images.
	d4img_RGB24,                    ///< 24bit RGB images. Byte 0=8bit blue; Byte 1=8bit green; Byte 2=8bit red.
	d4img_YUY2,                     ///< 16bit color format. Byte 0=8bit Y'0; Byte 1=8bit Cb; Byte 2=8bit Y'1; Byte 3=8bit Cr.
	d4img_BY8,                      ///< 8bit bayer color format.
};

/// Describes the format of an image.
struct d4img_ImageFormat
{
	int   width;       ///< Width of the image.
	int   height;      ///< Height of the image.
	int   pixelFormat; ///< Pixel format of the image. See #d4img_PixelFormat.
};

/// Creates a new image.
/// @return Pointer to an image or NULL in case of an error.
D4LIB_API d4img_ptr d4img_newImage();

/// Get format of the image.
/// @param[in] img Valid pointer to an image.
/// @param[out] width Pointer to int for width of image. May be NULL.
/// @param[out] height Pointer to int for height of image. May be NULL.
/// @param[out] pixelFormat Pointer to int for #d4img_PixelFormat. May be NULL.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4img_getImageFormat(d4img_ptr img, int* width, int* height, int* pixelFormat);

/// Set format of the image.
/// @param[out] img Valid pointer to an image.
/// @param[in] width Width of the image or zero.
/// @param[in] height Height of the image or zero.
/// @param[in] pixelFormat See #d4img_PixelFormat. #d4img_UnknownPixelFormat is allowed.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4img_setImageFormat(d4img_ptr img, int width, int height, int pixelFormat);

/// Get image data. Image data is converted, if 'pixelFormat' does not fit.
/// @param[in] img Valid pointer to an image.
/// @param[out] data Destination buffer for image data.
/// @param[in] size Size of destination buffer in bytes.
/// @param[in] pixelFormat Desired pixel format. See #d4img_PixelFormat. 
///                        When set to #d4img_UnknownPixelFormat, pixel format of 'img' is used.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4img_getImageData(d4img_ptr img, void* data, d4size size, int pixelFormat);

/// Set the image data. Image data is converted, if 'pixelFormat' does not fit.
/// @param[out] img Valid pointer to an image.
/// @param[in] data Valid pointer it image data array.
/// @param[in] size Size of image data array in bytes.
/// @param[in] pixelFormat Pixel format of data array. See #d4img_PixelFormat. 
///                        When set to #d4img_UnknownPixelFormat, pixel format of 'img' is used.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4img_setImageData(d4img_ptr img, const void* data, d4size size, int pixelFormat);

/// Set the image data. Image data is converted, if 'pixelFormat' does not fit.
/// @param[out] destImg Valid pointer to destination image.
/// @param[in] pixelFormat Pixel format of data array. See #d4img_PixelFormat. 
///                        When set to #d4img_UnknownPixelFormat, image data is just copied.
/// @param[in] srcImg Valid pointer to source image.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4img_convert(d4img_ptr destImg, int pixelFormat, d4img_ptr srcImg);

/// Import image from file.
/// @note Supported file formats: .bmp
/// @note Supported image formats: #d4img_RGB24
/// @param[out] img Valid pointer to destination image.
/// @param[in] filename Filename of image.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_InvalidPath, #d4error_FileNotFound, #d4error_NoAccess, #d4error_FormatNotAvailable, #d4error_Fail
D4LIB_API int d4img_import(d4img_ptr img, const char* filename);

/// Export image to file.
/// @note Supported file formats: .bmp, .png
/// @note Supported image formats: #d4img_RGB24
/// @param[in] img Valid pointer to destination image.
/// @param[in] filename Filename of image.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_InvalidPath, #d4error_NoAccess, #d4error_FormatNotAvailable, #d4error_Fail
D4LIB_API int d4img_export(d4img_ptr img, const char* filename);

/// Compute white balance parameters. The color image has to be captured with a white background.
/// @note See #d4img_convert for supported image formats.
/// @param[in] img Valid pointer to input to a color image.
/// @param[out] redFactor Valid pointer to double that gets the scaling factor for red color component.
/// @param[out] greenFactor Valid pointer to double that gets the scaling factor for green color component.
/// @param[out] blueFactor Valid pointer to double that gets the scaling factor for blue color component.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_FormatNotAvailable
D4LIB_API int d4img_computeWhiteBalance(d4img_ptr img, double* redFactor, double* greenFactor, double* blueFactor);

/// Apply white balance correction to 'img'.
/// @note Supported image formats: d4img_RGB24
/// @param[in,out] img Valid pointer to image that should be corrected.
/// @param[in] redFactor Red correction factor > 0.
/// @param[in] greenFactor Green correction factor > 0.
/// @param[in] blueFactor Blue correction factor > 0.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_FormatNotAvailable
D4LIB_API int d4img_applyWhiteBalance(d4img_ptr img, double redFactor, double greenFactor, double blueFactor);

/// @}


//=============================================================================
/// @name Float images
/// Special functions and types related to scalar float images. 
/// Each pixel has a 32bit scalar float value with no special bounds on ranges.
/// Notes:
/// - Image pointer is #d4fimg_ptr
/// - New images are created via #d4fimg_newImage
/// - Access to image format si via #d4fimg_setImageFormat and #d4fimg_getImageFormat
/// - Access to image data is via #d4fimg_setImageData and #d4fimg_getImageData
/// @{
//=============================================================================

/// Creates a new 32bit float image.
/// @return Pointer to an image or d4null in case of an error.
D4LIB_API d4fimg_ptr d4fimg_newImage();

/// Get format of the float image.
/// @param[in] img Valid pointer to a float image.
/// @param[out] width Pointer to an int that gets the width of the image. May be NULL.
/// @param[out] height Pointer to an int that gets the height of the image. May be NULL.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4fimg_getImageFormat(d4fimg_ptr img, int* width, int* height);

/// Set format of the float image.
/// @param[in] img Valid pointer to a float image.
/// @param[out] width Width of the image or zero.
/// @param[out] height Height of the image or zero.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4fimg_setImageFormat(d4fimg_ptr img, int width, int height);

/// Get image data. 
/// @param[in] img Valid pointer to an image.
/// @param[out] data Destination buffer for image data.
/// @param[in] size Size of 'data' in bytes.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4fimg_getImageData(d4fimg_ptr img, float* data, d4size size);

/// Set image data.
/// @param[out] img Valid pointer to an image.
/// @param[in] data Source buffer for image data.
/// @param[in] size Size of 'data' in bytes.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4fimg_setImageData(d4fimg_ptr img, const float* data, d4size size);

/// Supported replace operations.
enum d4fimg_ReplaceOp
{
	d4fimg_ReplaceIfLess,      ///< Replace value if it less than a comparison value.
	d4fimg_ReplaceIfGreater    ///< Replace value if it greater than a comparison value.
};

/// Replace pixel value by 'replaceValue', if condition is valid. Otherwise source value is used.
/// @code
/// d4fimg_ReplaceIfLess:    img[i] = compare[i] < threshold ? replaceValue : img[i];
/// d4fimg_ReplaceIfGreater: img[i] = compare[i] > threshold ? replaceValue : img[i];
/// @endcode
///
/// @param[in,out] img Valid source and target image.
/// @param[in] replaceValue Value for replacement.
/// @param[in] compare Valid image that stores mask values for each pixel.
/// @param[in] threshold Threshold value that is used for comparison.
/// @param[in] replaceOp Desired replace operation.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4fimg_replace(d4fimg_ptr img, float replaceValue, d4fimg_ptr compare, float threshold, int replaceOp);

/// Sets all pixel values to given float value.
/// @param[in,out] img Valid image.
/// @param[in] value Value to be set.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4fimg_setFloat(d4fimg_ptr img, float value);

/// Export float image to file.
/// @note Supported file formats: .pfm
/// @param[in] img Valid pointer to float image.
/// @param[in] filename Filename of image.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_InvalidPath, #d4error_NoAccess, #d4error_FormatNotAvailable, #d4error_Fail
D4LIB_API int d4fimg_export(d4fimg_ptr img, const char* filename);

/// Import float image from file.
/// @note Supported file formats: .pfm
/// @param[out] img Valid pointer to destination float image.
/// @param[in] filename Filename of image.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_InvalidPath, #d4error_FileNotFound, #d4error_NoAccess, #d4error_FormatNotAvailable, #d4error_Fail
D4LIB_API int d4fimg_import(d4fimg_ptr img, const char* filename);


/// @}

//=============================================================================
/// @name Camera
/// Functions and types related to camera interface.
/// @note At the moment, only DirectShow cameras are supported.
/// @{
//=============================================================================

/*
// Definition of supported camera modes.
enum d4cam_Mode
{
	d4cam_StandardMode = 0,     ///< Standard camera mode. Primary use: Preview and scanning.
	d4cam_HighresMode = 1,      ///< Some cameras might support a highres mode. Primary use: Texturing and highres scanning.
};
*/

/// Creates a new DirectShow camera instance.
/// @return Pointer to camera object.
D4LIB_API d4cam_ptr d4cam_newDirectShowCamera();

/// Get the number of available cameras.
/// Can be used in combination with #d4cam_getAvailableCamerasName.
/// @param[in] cam Valid pointer to camera interface.
/// @param[out] count Valid pointer to int that gets the number of available cameras.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_getAvailableCamerasCount(d4cam_ptr cam, int* count);

/// Get the name of an available camera.
/// Can be used in combination with #d4cam_getAvailableCamerasCount.
/// @param[in] cam Valid pointer to camera interface.
/// @param[out] name Valid pointer to string that gets the camera name.
/// @param[in] maxLength Maximum length of 'name'.
/// @param[in] index Zero based index of available index.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_getAvailableCamerasName(d4cam_ptr cam, char* name, d4size maxLength, int index);

/// Open camera.
///
/// Example:
/// @code
/// d4cam_open(cam, "DAVID-CAM-3-M (8F6DEE1E)", 0, 0, d4cam_Y800, 0.0);
/// d4cam_captureImageAsync(cam, img, 0.0, 1.0, 0);
/// d4cam_waitForCapture(cam, img);
/// @endcode
///
/// @param[in,out] cam Valid pointer to camera.
/// @param[in] name Name of the camera.
/// @param[in] width Desired with of the camera forat. Set zero to ignore.
/// @param[in] height Desired height of the camera format. Set zero to ignore.
/// @param[in] pixelFormat Desired pixel format. Set to d4cam_UnknownPixelFormat to ignore.
/// @param[in] fps Desired frames per second of the camera format. Set zero to ignore.
/// @return #d4ok All ok.
/// @return #d4error_InvalidArgument One of the arguments is invalid.
/// @return #d4error_DeviceNotAvailable Selected camera is not available.
/// @return #d4error_FormatNotAvailable Selected camera image format is invalid.
D4LIB_API int d4cam_open(d4cam_ptr cam, const char* name, int width, int height, int pixelFormat, double fps);

/// Check if camera is open and ok.
/// @param[in] cam Pointer to camera.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_check(d4cam_ptr cam);

/// Close camera.
/// @param[in,out] cam Valid pointer to camera.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4cam_close(d4cam_ptr cam);

/// Set running camera into a paused state.
/// Camera is automatically resumed when calling any capture function.
/// Resuming a camera from paused state takes some time depeding on the camera driver.
/// @param[in,out] cam Valid pointer to a camera. 
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_pause(d4cam_ptr cam);

/*
// Set the camera mode.
// @param[in,out] Valid pointer to a camera. 
// @param[in] mode Desired mode. See #d4cam_Mode.
// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_setMode(d4cam_ptr cam, int mode);

// Get current camera mode.
// @param[in] Valid pointer to a camera.
// @param[out] Valid pointer to an int that gets the camera mode (see #d4cam_Mode).
// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_getMode(d4cam_ptr cam, int* mode);
*/

/// Start a new asynchronous image capture. The function returns immediately.
/// Use #d4cam_getCapturedImage to wait for the capturing to be finished.
/// Example:
/// @code
/// d4img_ptr img = d4img_newImage();
/// d4cam_captureImageAsync(cam, 0, 1.0, 0);
/// d4cam_getCapturedImage(cam, img, d4img_Y800);
/// // ...
/// d4mem_decRefCount(img);
/// img = NULL;
/// @endcode
/// @param[in,out] cam Valid pointer to a camera.
/// @param[in] minContentChange How much has the content of the new image to be different from the previous image? Range: [0,100]%.
/// @param[in] maxWaitTime Maximum wait time [seconds] until an image has to be captured.
///                        'minContentChange' will be ignored when 'maxWaitTime' has been exceeded.
/// @param[in] temporalSmoothCount Temporal smoothing: Final image is average of '1+temporalSmoothCount' camera images. 
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_captureImageAsync(d4cam_ptr cam, int minContentChange, double maxWaitTime, int temporalSmoothCount);

/// Set the number of images that are skipped after a content change was detected.
/// This value will be used when #d4cam_captureImageAsync is called with 'minContentChange' > 0.
/// Only set this value when there are problems (e.g. when using rolling shutter camera or image tearing).
/// @param[in,out] cam Valid pointer to a camera.
/// @param[in] skipCount Number of images to be skipped. Default value is 1.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_setSkipCount(d4cam_ptr cam, int skipCount);

/// Get the captured image. If a capturing is still in progress (started via #d4cam_captureImageAsync), the function will wait until capturing is finished. 
/// @param[in,out] cam Valid pointer to a camera.
/// @param[out] img Valid pointer to an image that gets the captured image.
/// @param[in] pixelFormat Desired pixel format (see #d4img_PixelFormat). In case of #d4img_UnknownPixelFormat, camera pixel format will be used.
/// @return #d4ok, #d4error_InvalidArgument
/// @return #d4error_DeviceNotAvailable Camera is not open or does not deliver any images.
/// @return #d4error_Fail #d4cam_captureImageAsync was not called before.
D4LIB_API int d4cam_getCapturedImage(d4cam_ptr cam, d4img_ptr img, int pixelFormat);

/// Get the image format of the camera.
/// @param[in] cam Valid pointer to camera.
/// @param[out] width Optional pointer to int for width of image. May be NULL.
/// @param[out] height Optioal pointer to int for height of image. May be NULL.
/// @param[out] pixelFormat Optional pointer to int for #d4img_PixelFormat. May be NULL.
/// @param[out] fps Optional pointer to double for frames per second. May be NULL.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_getImageFormat(d4cam_ptr cam, int* width, int* height, int* pixelFormat, double* fps);

/// Get the timestamp of the last captured image.
/// @param[in,out] cam Valid pointer to a camera.
/// @param[out] timestamp Valid pointer to a double that gets the timestamp [seconds].
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_getTimestamp(d4cam_ptr cam, double* timestamp);

/// Set a camera property.
/// @param[in,out] cam Valid pointer to an open camera.
/// @param[in] value Value to be set.
/// @param[in] name Null terminated string of camera name.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_setProperty(d4cam_ptr cam, double value, const char* name);

/// Get a camera property.
/// @param[in,out] cam Valid pointer to an open camera.
/// @param[out] value Valid pointer to double that gets the value.
/// @param[in] name Valid string to name of property.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable, #d4error_Fail
D4LIB_API int d4cam_getProperty(d4cam_ptr cam, double* value, const char* name);

/// Get a camera property range.
/// @note Not all values (like 'deltaValue') might be available by every camera.
/// @param[in,out] cam Valid pointer to an open camera.
/// @param[out] minValue Valid pointer to double that gets the minimum value.
/// @param[out] maxValue Valid pointer to double that gets the maximum value.
/// @param[out] defValue Valid pointer to double that gets the default value.
/// @param[out] deltaValue Valid pointer to double that gets the delta value.
/// @param[in] name Valid string to name of property.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable, #d4error_Fail
D4LIB_API int d4cam_getPropertyRange(d4cam_ptr cam, double* minValue, double* maxValue, double* defValue, double* deltaValue, const char* name);

/// Get number of all camera property names or 'onlyAvailable' camera property names.
/// @param[in,out] cam Valid pointer to open camera.
/// @param[out] count Valid pointer to int that gets the count of camera property names.
/// @param[in] onlyAvailable List only available camera properties?
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_getPropertyCount(d4cam_ptr cam, int* count, bool onlyAvailable);

/// Get a camera property name. 
/// @warning Function has be used in combination with #d4cam_getPropertyCount and 'onlyAvailable' identical.
/// @param[in,out] cam Valid pointer to open camera.
/// @param[out] name Valid pointer to string that gets the property name.
/// @param[in] maxLength Maximum length of 'name'.
/// @param[in] propertyNameIndex Zero based index of a property name. Has to be in range [0, #d4cam_getPropertyCount).
/// @param[in] onlyAvailable List only available camera properties?
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4cam_getPropertyName(d4cam_ptr cam, char* name, d4size maxLength, int propertyNameIndex, bool onlyAvailable);

/// @}


//=============================================================================
/// @name Projector
/// Interface functions for controlling projectors.
/// Notes:
/// - Projector connected as extended display is created via #d4projector_newExtendedDisplayProjector
/// - Establish connection via #d4projector_openByScreenID, #d4projector_getScreenCount, and #d4projector_getScreenFormat
/// - Necessary image format for displaying images is available via #d4projector_getImageFormat
/// - Show images: #d4projector_displayImage and #d4projector_displayUniformColor
/// - Call #d4projector_close or release object when done
/// @{
//=============================================================================

/// Creates a new projector using extended display.
/// @return Pointer to projector object or NULL in case of an error.
D4LIB_API d4projector_ptr d4projector_newExtendedDisplayProjector();

/// Closes the connection to the projector.
/// @param[in,out] projector Valid pointer to the projector.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4projector_close(d4projector_ptr projector);

/// Get image format for displaying images.
/// @param[in,out] projector Valid pointer to the projector.
/// @param[out] width Optional pointer to int that gets width of image [px].
/// @param[out] height Optional pointer to int that gets height of image [px].
/// @param[out] pixelFormat Optional pointer to int that gets pixel format of image (see #d4img_PixelFormat).
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4projector_getImageFormat(d4projector_ptr projector, int* width, int* height, int* pixelFormat);

/// Display an image.
/// @warning 'img' has to be of the format given by #d4projector_getImageFormat.
/// @param[in,out] projector Valid pointer to the projector.
/// @param[in] img Image to be displayed. Must have same format as #d4projector_getImageFormat.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4projector_displayImage(d4projector_ptr projector, d4img_ptr img);

/// Display an image with a uniform color.
/// @param[in,out] projector Valid pointer to the projector.
/// @param[in] red Red color component [0,255].
/// @param[in] green Green color component [0,255].
/// @param[in] blue Blue color component [0,255].
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4projector_displayUniformColor(d4projector_ptr projector, unsigned char red, unsigned char green, unsigned char blue);

/// Establish the connection to a projector by given 'screenID'.
/// @param[in,out] projector Valid pointer to the projector.
/// @param[in] screenID Zero based valid ID of a screen. Use in combination with #d4projector_getScreenCount.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4projector_openByScreenID(d4projector_ptr projector, int screenID);

/// Check if projector is open and ok.
/// @param[in] projector Pointer to projector.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4projector_check(d4projector_ptr projector);

/// Get the number of available screens.
/// Should be used in combination with #d4projector_getScreenFormat.
/// @param[in,out] projector Valid pointer to the projector.
/// @param[out] screenCount Valid pointer to int that gets the number of available screens.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4projector_getScreenCount(d4projector_ptr projector, int* screenCount);

/// Get screen dimensions and position for requested screen.
/// Should be used in combination with #d4projector_getScreenCount.
/// @param[in,out] projector Valid pointer to the projector.
/// @param[out] x Optional pointer to int that gets x position of screen [px].
/// @param[out] y Optional pointer to int that gets x position of screen [px].
/// @param[out] width Optional pointer to int that gets width of screen [px].
/// @param[out] height Optional pointer to int that gets height of screen [px].
/// @param[in] screenID Zero based valid ID of a screen. Use in combination with #d4projector_getScreenCount.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_DeviceNotAvailable
D4LIB_API int d4projector_getScreenFormat(d4projector_ptr projector, int* x, int* y, int* width, int* height, int screenID);

/// @}


//=============================================================================
/// @name Calibration
/// Interface functions related to camera and projector calibration.
/// Notes:
/// - New calibration objects are created via #d4calib_newCameraCalibration or #d4calib_newProjectorCalibration.
/// - Calibration state is checked via #d4calib_isCalibrated.
/// - Import and export of calibration data: #d4calib_importCalibration and #d4calib_exportCalibration.
/// - Calibration: #d4calib_calibrate
/// @{
//=============================================================================

/// Point correspondence used for calibration.
/// 'worldRefPos3d' is projected into camera image at 'projectedPos2d'.
/// The error between 'projectedPos2d' and measured marker point 'measuredPos2d' should be minimal.
struct d4calib_CalibPoint
{
	double    worldRefPos3d[3];     ///< Reference 3d point (x,y,z) with respect to world coordinates.
	double    measuredPos2d[2];     ///< Measured 2d image point with respect to image coordinates.
	double    projectedPos2d[2];    ///< Image coordinates of 'worldRefPos3d' projected into image using calibrated model parameters.
};

/// More detailed camera calibration error.
enum d4calib_CamCalibError
{
	d4calib_CamCalibError_ok =                              0,  ///< No error.
	d4calib_CamCalibError_MarkerDetection =              -100,  ///< Marker detection failed.
	d4calib_CamCalibError_MarkerMapping =                -101,  ///< Left and right calibration planes do not intersect.
	d4calib_CamCalibError_PlaneIntersection =            -102,  ///< Left and right calibration planes do not intersect.
	d4calib_CamCalibError_LeftPlane =                    -103,  ///< Calibration of left plane failed.
	d4calib_CamCalibError_RightPlane =                   -104,  ///< Calibration of right plane failed.
	d4calib_CamCalibError_ModelFitting =                 -105,  ///< Could not find valid mapping between measured points and model parameters.
	d4calib_CamCalibError_ErrorValuesTooHigh =           -106,  ///< The resulting calibration error values are too high.
	d4calib_CamCalibError_Unknown =                      -107,  ///< Unknown calibration error.
};

/// More detailed projector calibration error.
enum d4calib_ProjectorCalibError
{
	d4calib_ProjectorCalibError_ok =                        0,  ///< No error.
	d4calib_ProjectorCalibError_PhaseImage =             -200,  ///< Could not compute phase image from input sequence.
	d4calib_ProjectorCalibError_ModelFitting =           -201,  ///< Could not find valid mapping between measured points and model parameters.
	d4calib_ProjectorCalibError_Unknown =                -202,  ///< Unknown calibration error.
};

/// Create a new calibration object for cameras.
/// @return Pointer to camera calibration object or NULL in case of an error.
D4LIB_API d4calib_ptr d4calib_newCameraCalibration();

/// Create a new calibration object for projectors.
/// @return Pointer to projector calibration object or NULL in case of an error.
D4LIB_API d4calib_ptr d4calib_newProjectorCalibration();

/// Is calibration valid?
/// @param[in,out] calib Valid pointer to calibration object.
/// @param[out] calibrated Valid pointer to bool that gets calibration state: True, if calibrated.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4calib_isCalibrated(d4calib_ptr calib, bool* calibrated);

/// Import calibration data from file.
/// @param[in,out] calib Valid pointer to calibration object.
/// @param[in] filename Valid pointer to filename string.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_FileNotFound, #d4error_InvalidFormat
D4LIB_API int d4calib_importCalibration(d4calib_ptr calib, const char* filename);

/// Export calibration data to file.
/// @param[in,out] calib Valid pointer to calibration object.
/// @param[in] filename Valid pointer to filename string.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4calib_exportCalibration(d4calib_ptr calib, const char* filename);

/// Change resolution of image format used for calibration. Intrinsics are adapted to this new resolution.
/// @note Only available for camera calibration. Projector model uses normalized coordinates.
/// @warning The aspect ratio is not allowed to change. 
/// @param[in,out] calib Valid pointer to calibration object.
/// @param[in] newWidth New camera image width.
/// @param[in] newHeight New camera image height.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_NotCalibrated, #d4error_InvalidFormat
D4LIB_API int d4calib_changeCalibrationResolution(d4calib_ptr calib, int newWidth, int newHeight);

/// Get the resolution of a calibration.
/// @note Only available for camera calibration. Projector model uses normalized coordinates.
/// @param[in] calib Valid pointer to calibration object.
/// @param[out] width The image width.
/// @param[out] height The image height.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_NotCalibrated
D4LIB_API int d4calib_getCalibrationResolution(d4calib_ptr calib, int *width, int *height);

/// Standard calibration procedure for one camera and one projector using the corner calibration panels.
/// @warning Only #d4img_Y800 pixel format is supported for 'imgCamCalib' and 'capturedImages'.
/// @param[out] camCalib Valid pointer to camera calibration object.
/// @param[in] imgCamCalib Valid pointer to camera image of calibration corner.
/// @param[out] projectorCalib Valid pointer to projector calibration object.
/// @param[in,out] sls Valid pointer to structured light scanning object.
/// @param[in] capturedImages Valid pointer to an array of image pointers. Every pointer has to be valid and of pixel format #d4img_Y800.
/// @param[in] imageCount Number of image pointers in 'capturedImages'.
/// @param[in] scale Calibration scale from panel.
/// @param[in] compensatePatternOffsets  Use bundle adjustment to determine offsets of left and right patterns within their respective planes. Set to false, if calibration corner is perfect.
/// @param[out] calibError Optional pointer to int that gets additional information about calibration error (#d4calib_CamCalibError, #d4calib_ProjectorCalibError, or #d4ok).
/// @param[out] calibPoints Optional pointer to array of calibration points that is filled with correspondences used for calibration. See #d4calib_CalibPoint.
/// @param[in,out] calibPointCount Optional pointer to int. Input: Maximum number of calibration points that fit into 'calibPoints'. Output: Number of calibration points written to 'calibPoints'.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_FormatNotAvailable, #d4error_CameraCalibration, #d4error_ProjectorCalibration
D4LIB_API int d4calib_calibrate(d4calib_ptr camCalib, d4img_ptr imgCamCalib, 
                                d4calib_ptr projectorCalib, d4sls_ptr sls, d4img_ptr* capturedImages, int imageCount,
                                double scale, bool compensatePatternOffsets, int* calibError, 
                                d4calib_CalibPoint* calibPoints, int* calibPointCount);

/// @}


//=============================================================================
/// @name Structured Light Scanner
/// Types and functions related to Structured Light Scanning (SLS).
/// Notes:
/// - Create new SLS object: #d4sls_newStructuredLightScanner
/// - Access to parameters: #d4sls_getCodedLightPhaseShiftParams and #d4sls_setCodedLightPhaseShiftParams
/// - Project pattern images: #d4sls_getPatternCount, #d4sls_getPatternImage
/// - Compute scan result as depth image: #d4sls_computeDepthImage
/// @{ 
//=============================================================================

/// Selects the color used for all generated patterns.
enum d4sls_ColorSelect
{
	d4sls_White,    ///< All color channels are used by same magnitude.
	d4sls_Red,      ///< Only red color channel is used.
	d4sls_Green,    ///< Only green color channel is used.
	d4sls_Blue      ///< Only blue color channel is used.
};

/// Defines orientations for hardware setup.
enum d4sls_Orientation
{
	d4sls_HorizontalOrientation,  ///< Horizontal setup. Projector and camera are next to each other.
	d4sls_VerticalOrientation,    ///< Vertical setup. Projector and camera are above each other.
	d4sls_BothOrientations        ///< Diagonal setup. Projector and camera are located diagonally.
};

/// Defines parameters used in structured light mode 'coded light + phase shift' ('cl+ps').
struct d4sls_CodedLightPhaseShiftParams
{
	int        colorSelect;           ///< Select color to channel for projection. See d4sls_ColorSelect.
	int        orientation;           ///< Orientation of setup. See d4sls_Orientation.
	int        brightness;            ///< Maximum brightness of pattern colors in range [0,255].
	int        shifts;                ///< Number of phase shifts. 
	int        frequencies;           ///< Number of frequencies (affects number of patterns). Optimal setting depends on camera/projector resolution and setup.
	bool       inverse;               ///< Use inverted patterns for coded light? 
	bool       randomizeSequence;     ///< Randomize pattern sequence? Changes the order of pattern in sequence. Can improve pattern change detection based on image content.
};

/// Defines a plane in 3D. Used for d4sls_planesToDepthImage.
struct d4sls_Plane
{
	double pos[3];            ///< One point on the plane (X,Y,Z)
	double normal[3];         ///< Normal vector of the plane (X,Y,Z)
};

/// Create new SLS object.
/// @return Pointer to SLS object or NULL.
D4LIB_API d4sls_ptr d4sls_newStructuredLightScanner();

/// Get parameters for 'cl+ps' mode.
/// @param[in,out] sls Valid pointer to a SLS instance.
/// @param[out] params Valid pointer to parameters.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4sls_getCodedLightPhaseShiftParams(d4sls_ptr sls, d4sls_CodedLightPhaseShiftParams* params);

/// Set parameters for 'cl+ps' mode.
/// @note By setting parameters the 'cl+ps' will be activated automatically. 'cl+ps' is active by default.
/// @param[in,out] sls Valid pointer to a SLS instance.
/// @param[out] params Valid pointer to parameters.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4sls_setCodedLightPhaseShiftParams(d4sls_ptr sls, const d4sls_CodedLightPhaseShiftParams* params);

/// Get number of pattern in a scanning sequence.
/// @param[in,out] sls Valid pointer to a SLS instance.
/// @param[out] patternCount Valid pointer to int that gets the pattern count.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4sls_getPatternCount(d4sls_ptr sls, int* patternCount);

/// Get pattern image at 'patternIndex'.
/// @param[in,out] sls Valid pointer to Structured Light Scanning instance.
/// @param[out] img Valid pointer to image for pattern.
/// @param[in] width Desired image width [px].
/// @param[in] height Desired image height [px].
/// @param[in] pixelFormat Desired pixel format, see #d4img_PixelFormat. Supported formats are:
///                        - d4img_RGB24
/// @param[in] patternIndex Valid zero based index of pattern. @see #d4sls_getPatternCount.
/// @return #d4ok, #d4error_InvalidArgument
/// @return #d4error_InvalidFormat Selected pixel format is not supported.
D4LIB_API int d4sls_getPatternImage(d4sls_ptr sls, d4img_ptr img, int width, int height, int pixelFormat, int patternIndex);

/// Get special image for setup.
/// @param[in,out] sls Valid pointer to Structured Light Scanning instance.
/// @param[out] img Valid pointer to image for pattern.
/// @param[in] width Desired image width [px].
/// @param[in] height Desired image height [px].
/// @param[in] pixelFormat Desired pixel format, see #d4img_PixelFormat. Supported formats are:
///                        - d4img_RGB24
/// @return #d4ok, #d4error_InvalidArgument
/// @return #d4error_InvalidFormat Selected pixel format is not supported.
D4LIB_API int d4sls_getSetupImage(d4sls_ptr sls, d4img_ptr img, int width, int height, int pixelFormat);

/// Get special chessboard image that can be used to validate a calibration performed by #d4calib_calibrate.
/// @param[in,out] sls Valid pointer to Structured Light Scanning instance.
/// @param[out] img Valid pointer to image for pattern.
/// @param[in] width Desired image width [px].
/// @param[in] height Desired image height [px].
/// @param[in] pixelFormat Desired pixel format, see #d4img_PixelFormat. Supported formats are:
///                        - d4img_RGB24
/// @param[in] projectorCalib Valid pointer to projector calibration.
/// @param[in] scale Scale used for calibration.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_NotCalibrated
/// @return #d4error_InvalidFormat Selected pixel format is not supported.
D4LIB_API int d4sls_getChessboardImage(d4sls_ptr sls, d4img_ptr img, int width, int height, int pixelFormat, d4calib_ptr projectorCalib, double scale);

/// Compute depth image that represents the current scan.
/// @warning At the moment, only #d4img_Y800 is supported for captured images.
/// @param[in,out] sls Valid pointer to Structured Light Scanning instance.
/// @param[out] depthImage Valid pointer to float image that gets depth values for each pixel of the camera.
/// @param[out] qualityImage Optional pointer to float image that gets quality values for each depth value.
/// @param[in] capturedImages Valid pointer to an array of image pointers. Every pointer has to be valid and of pixel format #d4img_Y800.
/// @param[in] imageCount Number of image pointers in 'capturedImages'.
/// @param[in] camCalib Valid pointer to camera calibration object.
/// @param[in] projectorCalib Valid pointer to projector calibration object.
/// @param[in] minContrast Minimum required contrast (magnitude of amplitude signal). Set to <= 0 in order to use default value (10).
D4LIB_API int d4sls_computeDepthImage(d4sls_ptr sls, d4fimg_ptr depthImage, d4fimg_ptr qualityImage, 
                                      d4img_ptr* capturedImages, int imageCount, d4calib_ptr camCalib, d4calib_ptr projectorCalib,
                                      int minContrast);

/// Computes a depth image from a list of planes. 
/// @param[in] sls Valid pointer to Structured Light Scanning instance.
/// @param[out] depthImage Valid pointer to float image that gets depth values for each pixel of the camera.
/// @param[in] camCalib Valid pointer to camera calibration object.
/// @param[in] numPlanes Number of planes given by parameter "planes".
/// @param[in] planes Array of d4sls_Plane structs.
D4LIB_API int d4sls_planesToDepthImage(d4sls_ptr sls, d4fimg_ptr depthImage, d4calib_ptr camCalib, int numPlanes, const d4sls_Plane* planes);

/// @}


//=============================================================================
/// @name Scanning
/// Generic scanning functions.
/// Notes:
/// - Triangulation of depth images: #d4scan_convertDepthImageToMesh
/// - Background removal: #d4scan_removeBackground
/// @{
//=============================================================================

/// Compute mesh from a depth image.
/// For every pixel of the depth image with a depth value in range [minDepth, maxDepth] a vertex is generated.
/// Neighbouring vertices are triangulated. Vertices that are not referenced by any triangle are removed.
/// @param[out] mesh Valid pointer to a mesh.
/// @param[in] depthImage Valid pointer to a depth image.
/// @param[in] qualityImage Optional pointer to a quality image with values in range [0,1]=[bad,good].
/// @param[in] calib Valid (camera) calibration.
/// @param[in] minDepth Minimum depth value in [mm]. Has to be > 0.
/// @param[in] maxDepth Maximum depth value in [mm]. Has to be > 'minDepth'.
/// @param[in] depthThres Neighbor pixels to a triangle will not be connected, if their difference in depth is larger than this value [mm].
/// @param[in] minQuality Minimum quality value. Only used if 'qualityImage' is a valid pointer.
/// @param[in] outlierRemoval Outlier filter, removing smallest fragments. 0=off. 1=remove all except the largest fragment. e.g. 0.1=remove all fragments that are smaller than 10% of the largest fragment.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4scan_convertDepthImageToMesh(d4mesh_ptr mesh, d4fimg_ptr depthImage, d4fimg_ptr qualityImage, d4calib_ptr calib, float minDepth, float maxDepth, float depthThres, float minQuality, double outlierRemoval);

/// Remove background scan from current scan.
/// All depth values that are farther away the the background, are set to -1:
/// @code
/// depthImage[i] = (depthImage[i] < backgroundImage[i] - epsilon) ? depthImage[i] : -1.f;
/// @endcode
///
/// @param[in,out] depthImage Valid pointer to depth image that represents the current scan.
/// @param[in] backgroundImage Valid pointer to depth image that represents the background scan.
/// @param[in] epsilon Expand background scan by this amount into the direction of the cameras origin.
D4LIB_API int d4scan_removeBackground(d4fimg_ptr depthImage, d4fimg_ptr backgroundImage, float epsilon);

/// @}


//=============================================================================
/// @name Mesh
/// A triangulated mesh represents the surface of a 3D object.
/// More information about triangulated meshes:
/// - http://en.wikipedia.org/wiki/Triangle_mesh
/// - http://en.wikipedia.org/wiki/Polygon_mesh#Face-vertex_meshes
/// 
/// Notes:
/// - Create new mesh: #d4mesh_newMesh
/// - Number of vertices and triangles: #d4mesh_getVertexCount, #d4mesh_getTriangleCount
/// - Access to buffers: #d4mesh_getBuffer, #d4mesh_setBuffer, #d4mesh_BufferType
/// - Access to texture: #d4mesh_getTexture, #d4mesh_setTexture
/// - Position and orientation of mesh: #d4mesh_getPose, #d4mesh_setPose
/// - Grouping / submeshes: #d4mesh_getSubmeshCount, #d4mesh_combine, #d4mesh_uncombine
///
/// @{
//=============================================================================

/// Buffer selection.
enum d4mesh_BufferType
{
	/// The vertex position buffer stores the vertex positions as an array of tuples.
	/// Each vertex position is described by a tuple (x,y,z) of three 32bit floats .
	/// This buffer is always present.
	/// @note Coordinates are always with respect to the local coordinate system of the mesh.
	d4mesh_VertexPositionBuffer,

	/// The vertex normal buffer stores the vertex normals as an array of tuples.
	/// Each vertex normal is described by a tuple (nx,ny,nz) of three 32bit floats.
	/// This buffer is always present.
	/// @note Coordinates are always with respect to the local coordinate system of the mesh.
	d4mesh_VertexNormalBuffer,
	
	/// The texture coordinates buffer stores texture coordinates as an array of tuples.
	/// Each vertex texture coordinate is described a tuple (u,v) of two 32bit floats.
	/// This buffer is only optional.
	/// @note Coordinates should be in range [0,1].
	d4mesh_VertexTexCoordBuffer,

	/// The texture color buffer stores vertex colors as an array of tuples.
	/// Each vertex texture coordinate is described a RGBA tuple (red, green, blue, alpha) of four 8bit unsigned chars.
	/// This buffer is only optional.
	/// @note Coordinates should be in range [0,1].
	d4mesh_VertexColorBuffer,

	/// The triangle index buffer stores triangles as an array of vertex indices.
	/// Each triangle is described by tuple (v0, v1, v2) of three 32bit integer vertex indices that are zero based.
	/// This buffer is always present.
	d4mesh_TriangleIndexBuffer,

	/// The vertex quality buffer stores a quality value for each vertex.
	/// This buffer is only optional.
	/// @note Quality values should be in range [0,1].
	d4mesh_VertexQualityBuffer
};


/// Create new mesh object.
/// @return Pointer to mesh object or NULL.
D4LIB_API d4mesh_ptr d4mesh_newMesh();

/// Clear all data of mesh object.
/// @param[in,out] mesh Valid pointer to mesh object.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_clear(d4mesh_ptr mesh);

/// Copy mesh data from 'sourceMesh' to 'targetMesh'. 
/// Old content of 'targetMesh' is deleted.
/// @param[out] targetMesh Valid pointer to target mesh object.
/// @param[in] sourceMesh Valid pointer to source mesh object.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_copy(d4mesh_ptr targetMesh, d4mesh_ptr sourceMesh);

/// Combine multiple meshes into submeshes of a single mesh.
/// @param[out] meshGroup Valid pointer to a mesh object. Must not be part of 'meshesToCombine'.
/// @param[in] meshesToCombine Pointer to array of valid pointers to mesh objects that have to be combined.
/// @param[in] meshCount Number of meshes referenced by 'meshesToCombine'.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_combine(d4mesh_ptr meshGroup, d4mesh_ptr* meshesToCombine, int meshCount);

/// Uncombine submeshes of 'meshGroup' into independent meshes.
/// 'meshCount' has to be equal to the submesh count of 'mesh' (see #d4mesh_getSubmeshCount).
/// @param[in] meshGroup Valid pointer to a mesh object that contains combined meshes. Must not be part of 'uncombinedMeshes'.
/// @param[out] uncombinedMeshes Pointer to array of valid pointers to mesh objects that get the uncombined meshes.
/// @param[in] meshCount Number of meshes referenced by 'uncombinedMeshes'.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_uncombine(d4mesh_ptr meshGroup, d4mesh_ptr* uncombinedMeshes, int meshCount);

/// Get the number of submeshes (combined meshes) in 'mesh'.
/// A mesh has at least one submesh. This is also the standard case unless #d4mesh_combine or #d4mesh_setSubmeshCount was used.
/// @param[in] mesh Valid pointer to a mesh object.
/// @param[out] count Valid pointer to int that gets number of combined meshes in 'mesh'.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_getSubmeshCount(d4mesh_ptr mesh, int* count);

/// Set the number of submeshes (combined meshes) in 'mesh'.
/// @param[in] mesh Valid pointer to a mesh object.
/// @param[in] count New number of submeshes (>0).
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_setSubmeshCount(d4mesh_ptr mesh, int count);

/// Copy requested buffer ('bufferType') content into 'data'.
/// You can use #d4mesh_getVertexCount, #d4mesh_getTriangleCount, and #d4mesh_getBufferSize to
/// compute the correct size of 'data'.
/// @param[in] mesh Valid pointer to a mesh object.
/// @param[out] data Valid pointer to data buffer.
/// @param[in] size Size of 'data' buffer in bytes.
/// @param[in] bufferType Desired data buffer, see #d4mesh_BufferType.
/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #d4mesh_getSubmeshCount.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_getBuffer(d4mesh_ptr mesh, void* data, d4size size, int bufferType, int submeshIndex);

/// Copy 'data' into the desired buffer ('bufferType').
/// You can use #d4mesh_getVertexCount, #d4mesh_getTriangleCount, and #d4mesh_getBufferSize to
/// compute the correct size of 'data'.
/// @param[out] mesh Valid pointer to a mesh object.
/// @param[in] data Valid pointer to data buffer.
/// @param[in] size Size of 'data' buffer in bytes.
/// @param[in] bufferType Desired data buffer, see #d4mesh_BufferType.
/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #d4mesh_getSubmeshCount.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_setBuffer(d4mesh_ptr mesh, const void* data, d4size size, int bufferType, int submeshIndex);

/// Is the buffer 'bufferType' available?
/// See #d4mesh_BufferType for information about which buffers are optional.
/// @param[in] mesh Valid pointer to a mesh object.
/// @param[out] hasBuffer Valid pointer to bool that gets answer.
/// @param[in] bufferType Desired data buffer, see #d4mesh_BufferType.
/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #d4mesh_getSubmeshCount.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_hasBuffer(d4mesh_ptr mesh, bool* hasBuffer, int bufferType, int submeshIndex);

/// Get size of buffer 'bufferType' in bytes (for all submeshes).
/// @param[in] mesh Valid pointer to a mesh object.
/// @param[out] size Valid pointer to value that gets the size of the buffer in bytes. 
///                  Size will be set to zero if buffer is not available.
/// @param[in] bufferType Desired data buffer, see #d4mesh_BufferType.
/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #d4mesh_getSubmeshCount.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_getBufferSize(d4mesh_ptr mesh, d4size* size, int bufferType, int submeshIndex);

/// Get the number of vertices for a given submesh of a mesh.
/// @param[in] mesh Valid pointer to a mesh object.
/// @param[out] vertexCount Valid pointer to int that gets the number of vertices.
/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #d4mesh_getSubmeshCount.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_getVertexCount(d4mesh_ptr mesh, int* vertexCount, int submeshIndex);

/// Set the number of vertices for a given submesh of a mesh.
/// @param[in] mesh Valid pointer to a mesh object.
/// @param[in] vertexCount New vertex count.
/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #d4mesh_getSubmeshCount.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_setVertexCount(d4mesh_ptr mesh, int vertexCount, int submeshIndex);

/// Get the number of triangles (total count for all submeshes).
/// @param[in] mesh Valid pointer to a mesh object.
/// @param[out] triangleCount Valid pointer to int that gets the number of triangles.
/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #d4mesh_getSubmeshCount.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_getTriangleCount(d4mesh_ptr mesh, int* triangleCount, int submeshIndex);

/// Set the number of triangles for a given submesh of a mesh.
/// @param[in] mesh Valid pointer to a mesh object.
/// @param[in] triangleCount New triangle count.
/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #d4mesh_getSubmeshCount.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_setTriangleCount(d4mesh_ptr mesh, int triangleCount, int submeshIndex);

/// Get the name of a mesh.
/// @param[in] mesh Valid pointer to a mesh object.
/// @param[out] name Valid pointer to string that gets the name.
/// @param[in] maxLength Maximum length of a string that fits into 'name'.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_getName(d4mesh_ptr mesh, char* name, d4size maxLength);

/// Set the name of a mesh.
/// @param[in] mesh Valid pointer to a mesh object.
/// @param[out] name Valid pointer to a string.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_setName(d4mesh_ptr mesh, const char* name);

/// Get the texture of a mesh.
/// Textures are optional.
/// @param[in] mesh Valid pointer to a mesh object.
/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #d4mesh_getSubmeshCount.
/// @return Pointer to an image or NULL. Reference count is incremented.
D4LIB_API d4img_ptr d4mesh_getTexture(d4mesh_ptr mesh, int submeshIndex);

/// Set the texture of a mesh.
/// Textures are optional.
/// @param[in,out] mesh Valid pointer to a mesh object.
/// @param[in,out] image Valid pointer to an image. Reference count is incremented, no copy is made.
/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #d4mesh_getSubmeshCount.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_setTexture(d4mesh_ptr mesh, d4img_ptr image, int submeshIndex);

/// Has 'mesh' a texture?
/// @param[in] mesh Valid pointer to a mesh.
/// @param[out] hasTexture Valid pointer to a bool that gets the answer.
/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #d4mesh_getSubmeshCount.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_hasTexture(d4mesh_ptr mesh, bool* hasTexture, int submeshIndex);

/// Get the position and orientation (pose) of a mesh.
/// The pose is represented as a homogenenous transformation matrix:
/// @code
/// | nx ox ax px | = | d0  d4  d8  d12 |
/// | ny oy ay py | = | d1  d5  d9  d13 |
/// | nz oz az pz | = | d2  d6  d10 d14 |
/// |  0  0  0  1 | = | d3  d7  d11 d15 |
///
/// world_T_local = (d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15)
/// @endcode
///
/// Some notes:
/// - The 3D vector (nx, ny, nz) is the x axis of the local coordinate system with respect to world coordinates.
/// - The 3D vector (ox, oy, oz) is the y axis of the local coordinate system with respect to world coordinates.
/// - The 3D vector (ax, ay, az) is the z axis of the local coordinate system with respect to world coordinates.
/// - The 3D vector (px, py, pz) is the position of the mesh with respect to world coordinates.
///
/// This matrix transforms local coordinates into world coordinates:
/// @code
/// (wPx, wPy, wPz, 1)^T = world_T_local * (lPx, lPy, lPz, 1)^T
/// @endcode
///
/// @param[in] mesh Valid pointer to a mesh.
/// @param[out] world_T_local Valid pointer to an array of 16 double values.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_getPose(d4mesh_ptr mesh, double world_T_local[16]);

/// Set the position and orientation (pose) of a mesh.
/// The pose is represented as a homogenenous transformation matrix.
/// @see #d4mesh_getPose for a more detailed description.
/// @param[in,out] mesh Valid pointer to a mesh.
/// @param[in] world_T_local Valid pointer to an array of 16 double values describing the transformation matrix.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_setPose(d4mesh_ptr mesh, const double world_T_local[16]);

/// @}


//=============================================================================
/// @name Mesh: Filtering
/// Filtering of meshes.
///
/// Notes:
/// - Smoothing: #d4mesh_smoothAverage
/// - Mesh simplification: #d4mesh_reduceMeshDensity
///
/// @{
//=============================================================================

/// Index to define a vertex in a submesh
struct d4mesh_SubmeshVertexIndex
{
	int submeshIndex;         ///< which submesh contains the vertex
	int vertexIndex;          ///< vertex index in that submesh
};

/// Smooth mesh using an average filter.
/// @param[in,out] mesh Valid pointer to a mesh.
/// @param[in] smoothing Number of smoothing steps.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_smoothAverage(d4mesh_ptr mesh, int smoothing);

/// Reduce the vertex/triangle density of mesh (simplification).
/// @param[in,out] mesh Valid pointer to a mesh.
/// @param[in] factor 0.5f for retaining 50% of all triangles, 0.1f for 10% triangles...
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_reduceMeshDensity(d4mesh_ptr mesh, float factor);

/// Remove selected vertices. Referenced triangles are also removed.
/// A vertex i is selected if selected[i]==true.
/// @param[in,out] mesh Valid pointer to a mesh.
/// @param[in] selectedVertices Valid pointer to an array of indices to vertices which should be removed.
/// @param[in] selectedVerticesCount Number of indices in 'selectedVertices'.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_removeSelectedVertices(d4mesh_ptr mesh, const d4mesh_SubmeshVertexIndex* selectedVertices, int selectedVerticesCount);

/// Invert orientation of all triangles in given mesh.
/// @param[in,out] mesh Valid pointer to a mesh.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_invertTriangleOrientation(d4mesh_ptr mesh);

/// Remove any vertex i that when projected into a virtual camera view has a mask value of 'mask[i]==true'.
/// Vertices outside view or with a mask value fo 'mask[i]==false' are not removed.
/// If a vertex is removed, its referenced triangles are also removed.
/// The virtual camera is described by a perspective camera model: 
/// - Central z axis goes through center of image --> Center is at (width/2, height/2). 
/// - Image plane has a size in x-direction of 'width and a size of 'height' in y-direction.
/// - x-axis is from left to right and y-axis from top to down (with respect to image).
/// - Camera looks along the positive z-axis.
/// - Visible points are on positive side of z-axis.
/// - Focal length is given by 'f'.
/// - Pose of the camera with respect to world coordinates is given by 'world_T_cam'.
///
/// @param[in,out] meshes Valid pointer to an array of valid mesh pointers.
/// @param[in] meshCount Number of meshes referenced by 'meshes'.
/// @param[in] mask Valid pointer to an boolean array of size 'width*height'. True: Vertex should be removed. False: Keep vertex.
/// @param[in] width Width of the mask image [pixel]. Has to be > 0.
/// @param[in] height Height of the mask image [pixel]. Has to be > 0.
/// @param[in] f Focal length (distance of image plane to origin camera coordinate system).
/// @param[in] world_T_cam Position and orientation (pose) of the camera with respect to world coordinates. 
///                        The pose is represented as a homogenenous transformation matrix.
///                        See #d4mesh_getPose for more information.
/// @param[in] onlyVisible Remove only visible vertices? WARNING: At the moment, only 'onlyVisible==false' is supported.
/// @return #d4ok, #d4error_InvalidArgument
D4LIB_API int d4mesh_removeVerticesByVirtualCamMask(d4mesh_ptr* meshes, int meshCount, const unsigned char* mask, int width, int height, double f, const double world_T_cam[16], bool onlyVisible);

// Selects all vertices that are on the negative half space defined by a plane.
// The plane is given by a 3D point on the plane 'pointOnPlane' and 3D vector 'planeNormal' orthogonal to the plane.
// @param[in] mesh Valid pointer to a mesh.
// @param[in,out] selected Valid pointer to an array of bools with minimum size equal to 
// @param[in] clearSelection Clear previous selection?
// @param[in] pointOnPlane 3D point (x,y,z) w.r.t. to local coordinate system.
// @param[in] planeNormal 3D vector (x,y,z) orthgonal to the plane w.r.t. to local coordinate system.
//D4LIB_API int d4mesh_selectVerticesNegativeHalfSpace(d4mesh_ptr mesh, bool* selected, bool clearSelection, const float pointOnPlane[3], const float planeNormal[3]);

/// @}


//=============================================================================
/// @name Mesh: Alignment
/// Alignment (registration) of meshes.
///
/// - Pairwise alignment: #d4mesh_alignPairCoarse and #d4mesh_alignPairFine
/// - Global alignment: #d4mesh_alignGlobalFine
///
/// @{
//=============================================================================

/// Provides user knowledge about the motion between two scans.
/// In most cases the motion can be described by a simple rotation around an axis, 
/// where the pitch along the (motion=rotation) axis is zero.
struct d4mesh_MotionInfo
{
	bool     angleKnown;      ///< Is rotation 'angle' around motion axis known?.
	bool     axisDirKnown;    ///< Is direction vector 'axisDir' of motion axis is known?
	double   angleDeg;        ///< Angle of rotation in degrees.
	double   angleToleranceDeg; ///< Tolerance of angleDeg in degrees.
	double   axisDir[3];      ///< 3D direction vector (x,y,z) of motion axis w.r.t. world coordinates.
};


/// A list of poses (4*4 matrices) plus tolerances for translation and rotation.
struct d4mesh_PosesWithTolerances
{
	int numPoses;             ///< Number of poses (each 4*4 doubles) specified in poses
	double *poses;            ///< List of poses (each 4*4 doubles) (nullptr for none)
	double toleranceTrans;    ///< Translational tolerance [mm] of the poses (0 to deactivate).
	double toleranceDeg;      ///< Rotational tolerance [degree] of the poses (0 to deactivate).
};


/// Parameter set for Coarse Alignment, see #d4mesh_alignPairCoarse.
struct d4mesh_CoarseAlignParams
{
	int texturePercentage;                  ///< Percentage of texture influence on alignment (0-99). Only used if value is higher than 0.
	d4mesh_MotionInfo* motionInfo;          ///< Valid pointer to additional information about motion between the two meshes. See #d4mesh_MotionInfo for more details.
	float qualityFactor;                    ///< 1 is recommended. Larger values like 5 or 10 may create better results for symmetric objects, but increase computation time. Lower values like 0.01 to 0.1 decrease computation time, but may result in wrong alignment results.

	// Contact Pairs:
	d4size numContactVertices1;             ///< Number of contact vertices on mesh1, specified in 'contactVertices1'
	int *contactVertices1;                  ///< Array of contact vertex indices on mesh1, or NULL
	d4size numContactVertices2;             ///< Number of contact vertices on mesh2, specified in 'contactVertices2'
	int *contactVertices2;                  ///< Array of contact vertex indices on mesh2, or NULL

	// Blacklisting/Whitelisting:
	d4mesh_PosesWithTolerances blacklist1;  ///< Blacklist of (rough) poses that mesh1 may not be moved to (can be empty)
	d4mesh_PosesWithTolerances whitelist1;  ///< Whitelist of (rough) poses that mesh1 must be moved to (can be empty)
};


/// Parameter set for Fine Alignment, see #d4mesh_alignPairFine.
struct d4mesh_FineAlignParams
{
	int texturePercentage;      ///< Percentage of texture influence on alignment. Only used, if value is higher than 0.
	int maxNumIterations;       ///< Number of iterations to run maximally (recommended: 100)
	double maxTranslation;      ///< Maximum allowed drift[mm] of mesh center (0 = no limit). Alignment will stop when this limit is reached.
	double maxRot_deg;          ///< Maximum allowed rotation[deg] of mesh in degree (0 = no limit). Alignment will stop when this limit is reached.
};


/// Parameter set for Global Fine Alignment, see #d4mesh_alignGlobalFine.
struct d4mesh_GlobalFineAlignParams
{
	int texturePercentage;      ///< Percentage of texture influence on alignment. Only used if value is higher than 0.
	int numIterations;          ///< Number of iterations to run (recommended: 20)
	double maxTranslation;      ///< Maximum allowed drift[mm] of any mesh center (0 = no limit). Global Fine Alignment will not move any scan more than this.
	double maxRot_deg;          ///< Maximum allowed rotation[deg] of any mesh in degree (0 = no limit). Global Fine Alignment will not rotate any scan more than this.
};

/// Align 'mesh1' to 'mesh2' coarsely.
/// @param[in,out] mesh1 Valid pointer to mesh that is moved (aligned).
/// @param[in] mesh2 Valid pointer to mesh.
/// @param[out] quality Optional pointer to double that gets quality value in range [-1,1]=[bad,good].
/// @param[in] params Valid pointer to alignment parameter set. See #d4mesh_CoarseAlignParams.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_Fail
D4LIB_API int d4mesh_alignPairCoarse(d4mesh_ptr mesh1, d4mesh_ptr mesh2, double* quality, const d4mesh_CoarseAlignParams* params);

/// Align 'mesh1' to 'mesh2' fine.
/// @param[in,out] mesh1 Valid pointer to mesh that is moved (aligned).
/// @param[in] mesh2 Valid pointer to mesh.
/// @param[out] quality Optional pointer to double that gets quality value in range [-1,1]=[bad,good].
/// @param[in] params Valid pointer to alignment parameter set. See #d4mesh_FineAlignParams.
/// @return #d4ok, #d4error_InvalidArgument,# d4error_Fail
D4LIB_API int d4mesh_alignPairFine(d4mesh_ptr mesh1, d4mesh_ptr mesh2, double* quality, const d4mesh_FineAlignParams* params);

/// Align meshes globally fine.
/// @param[in,out] meshes Valid pointer to an array of valid mesh pointers.
/// @param[in] meshCount Number of meshes referenced by 'meshes'.
/// @param[in] params Valid pointer to alignment parameter set. See #d4mesh_GlobalFineAlignParams.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_Fail
D4LIB_API int d4mesh_alignGlobalFine(d4mesh_ptr* meshes, int meshCount, const d4mesh_GlobalFineAlignParams* params);

/// @}


//=============================================================================
/// @name Mesh: Fusion
/// Fusion of meshes and textures into one mesh and one texture.
///
/// Notes:
/// - Geometric data is fused with #d4mesh_fuseGeometry
/// - Texture data is fused with #d4mesh_fuseTexture
///
/// @{
//=============================================================================

/// Fuse multiple meshes into one triangular mesh.
/// @param[out] fusedGeo Valid pointer to fusion result.
/// @param[in] meshes Valid pointer to an array of valid mesh pointers.
/// @param[in] meshCount Number of meshes referenced by 'meshes'.
/// @param[in] resolution Limits the maximum resolution and thus the required memory usage.
/// @param holeSizeThresRel Holes, which have a smaller area than holeSizeThresRel * size of original data surface, will be closed. 0 = open all holes. 1 = close all holes.
/// @param[in] sharpness Fusion sharpness in [-3,+5]. Negative values for smoothing. Default is 1.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_Fail
D4LIB_API int d4mesh_fuseGeometry(d4mesh_ptr fusedGeo, d4mesh_ptr* meshes, int meshCount, int resolution, float holeSizeThresRel, int sharpness);

/// Converts a metric resolution value into a resolution value required by #d4mesh_fuseGeometry.
/// It uses the maximum bounding box length of all 'meshes'.
/// @param[in] meshes Valid pointer to an array of valid mesh pointers.
/// @param[in] meshCount Number of meshes referenced by 'meshes'.
/// @param[out] resolution Valid pointer to resolution value that can be used in #d4mesh_fuseGeometry.
/// @param[in] metricResolution Smallest possible element size in units (e.g. 0.1 [mm]).
/// @return #d4ok, #d4error_InvalidArgument, #d4error_Fail
D4LIB_API int d4mesh_getFuseGeometryResolution(d4mesh_ptr* meshes, int meshCount, int* resolution, double metricResolution);

/// Fuse textures referenced by 'meshes' into one texture of 'fusedGeo'.
/// @param[in,out] fusedGeo Valid pointer to a mesh.
/// @param[in] meshes Valid pointer to an array of valid mesh pointers.
/// @param[in] meshCount Number of meshes referenced by 'meshes'.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_Fail
D4LIB_API int d4mesh_fuseTexture(d4mesh_ptr fusedGeo, d4mesh_ptr* meshes, int meshCount);

/// @}


//=============================================================================
/// @name Mesh: Import / Export
/// Import and export of meshes. See functions for supported formats.
///
/// Notes:
/// - Import: #d4mesh_import
/// - Export: #d4mesh_export
///
/// @{
//=============================================================================

/// Export 'mesh' to file.
/// Supported formats: OBJ, STL, PLY
/// @warning Texture is only exported, if there is only one submesh (see #d4mesh_getSubmeshCount).
/// @param[in] mesh Valid pointer to mesh.
/// @param[in] filename Valid pointer to string with filename.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_InvalidPath, #d4error_FileNotFound, #d4error_NoAccess
D4LIB_API int d4mesh_export(d4mesh_ptr mesh, const char* filename);

/// Import 'mesh' from file.
/// Supported formats: OBJ
/// @param[out] mesh Valid pointer to mesh.
/// @param[in] filename Valid pointer to string with filename.
/// @return #d4ok, #d4error_InvalidArgument, #d4error_InvalidPath, #d4error_FileNotFound, #d4error_NoAccess
D4LIB_API int d4mesh_import(d4mesh_ptr mesh, const char* filename);

/// @}

/// @} d4lib
/// @} LowLevelGroup


#ifdef __cplusplus
} // extern "C"
#endif


#endif // DAVID_SDK_D4LIB_H
