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

#ifndef DAVID_SDK_COMMON_H
#define DAVID_SDK_COMMON_H

#include <stdint.h>
#include "davidSDK/Exceptions.h"

namespace david { 

//*****************************************************************************
/// @defgroup ManagerGroup Management
/// Important high level management functions and types.
//*****************************************************************************

//*****************************************************************************
/// @defgroup InterfaceGroup High Level Interfaces
/// High level Interface classes are listed here.
//*****************************************************************************

//*****************************************************************************
/// @defgroup LowLevelGroup Low Level Interfaces
/// Low level stuff is listet here.
//*****************************************************************************


//=============================================================================
// Global variables
//=============================================================================

/// Defines the version of the client.
/// The server checks this client version against its minimum required client version.
const double DAVID_SDK_ClientVersion = 1.0;

/// Defines DAVID default port.
const uint16_t DAVID_SDK_DefaultPort = 10500;


//=============================================================================
// PixelFormat
//=============================================================================

/// Describes different pixel formats.
enum PixelFormat
{
	UnknownPixelFormat = 0,   ///< Unknown pixel format
	Y800,                     ///< Standard 8bit grayscale format. 
	RGB24,                    ///< Red, green, and blue 8bit color components for each pixel. Byte 0=8bit blue; Byte 1=8bit green; Byte 2=8bit red.
	YUY2,                     ///< 16bit color format. Byte 0=8-bit Y'0; Byte 1=8-bit Cb; Byte 2=8-bit Y'1; Byte 3=8-bit Cr.
	BY8,                      ///< 8bit bayer color format.
};

/// Get bits per pixel for given pixel format.
/// @param[in] pixelFormat Pixel format, see #PixelFormat.
/// @return Bits per pixel or zero.
int GetBitsPerPixel(enum PixelFormat pixelFormat);

/// Get image size in bytes.
/// @param[in] width Width of the image in [px]
/// @param[in] height Height of the image in [px]
/// @param[in] pixelFormat See #PixelFormat. 
/// @return Image size in bytes or zero.
size_t GetImageSize(int width, int height, enum PixelFormat pixelFormat);


//=============================================================================
// Optional
//=============================================================================

/// The class template david::Optional manages an optional contained value, i.e. a value that semantically may not be present.
///
/// The value inside an optional object may be in either an initialized or uninitialized state. 
/// An optional object with a value in initialized state is called engaged, 
/// whereas if the value is in uninitialized state, the object is called disengaged. 
///
/// Interface is similar to std::optional defined in C++14:
/// http://en.cppreference.com/w/cpp/utility/optional
///
template <class T>
class Optional
{
public:
	/// Standard constructor.
	/// Value is not initialized.
	Optional() : m_engaged(false) {}
	
	/// Constructor that initializes the value. 
	/// @param[in] value Value to assign to the contained value.
	Optional(const T& value) : m_value(value), m_engaged(true) {}

	/// Get value.
	/// @return Current value. Might be anything, if not initialized.
	/// @exception david::Error_MissingObject (david::Exception) Value is not initialized.
	const T& value() const 
	{
		if (!m_engaged) throw david::Exception(david::Error_MissingObject, "Value is not initialized.", "david::Optional::value");
		return m_value;
	}

	/// Checks whether *this is in engaged state, i.e. whether the contained value is initialized. 
	/// @return true if *this is in engaged state, false otherwise. 
	operator bool() const {return m_engaged;}

private:
	T      m_value;         ///< Stores the value.
	bool   m_engaged;       ///< Is value initialized?
};


} // namespace

#endif // DAVID_SDK_COMMON_H
