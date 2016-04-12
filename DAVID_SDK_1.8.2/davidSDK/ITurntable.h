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

#ifndef DAVID_SDK_I_TURNTABLE_H
#define DAVID_SDK_I_TURNTABLE_H

#include "davidSDK/Common.h"

namespace david {

//****************************************************************************
/// @addtogroup InterfaceGroup
/// @{
///
/// @defgroup TurntableGroup Turntable
/// Functions and types related to 'Turntable' access.
/// @{
//****************************************************************************

//=============================================================================
// Turntable
//=============================================================================

/// Interface class for turntable control.
/// All calls are delegated to a JSON-RPC 2.0 client.
/// Example:
/// @include SimpleTurntable.cpp
class ITurntable
{
public:
	/// Destructor.
	virtual ~ITurntable() {}

	/// Set up the turntable (connect/disconnect).
	/// @param[in] enable True for connect, false for disconnect.
	/// @exception david::Error_DeviceNotAvailable (david::FunctionException) Connection to turntable failed. Turntable not available?
	virtual void Setup(bool enable) = 0;

	/// Rotate the turntable.
	/// The turntable rotates counterclockwise ('degrees' is postive) or clockwise ('degrees' is negative).
	/// @param[in] degrees Amount of rotation in degrees. 
	/// @exception david::Error_DeviceNotAvailable (david::FunctionException) Rotation of turntable failed. Turntable not available?
	virtual void Rotate(double degrees) = 0;
};

/// @} // TurntableGroup
/// @} // InterfaceGroup

} // namespace

#endif // DAVID_SDK_I_TURNTABLE_H
