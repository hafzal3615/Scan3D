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

#ifndef DAVID_SDK_I_MODULES_H
#define DAVID_SDK_I_MODULES_H

#include "davidSDK/Common.h"
#include "davidSDK/IStructuredLightScanner.h"
#include "davidSDK/IShapeFusion.h"
#include "davidSDK/IMeasure.h"
#include "davidSDK/ITurntable.h"
#include "davidSDK/IMainWindow.h"

namespace david {

//****************************************************************************
/// @addtogroup InterfaceGroup
/// @{
/// @defgroup ModulesGroup Modules access
/// Access functions to all supported module interfaces.
/// @{
//****************************************************************************

//=============================================================================
// IModules
//=============================================================================

/// Gives access to module interfaces.
class IModules
{
public:
	/// Destructor.
	virtual ~IModules() {}

	/// Returns an interface to the 'Structured Light Scanning' module.
	/// @return Interface class for 'Structured Light Scanning' functions.
	virtual IStructuredLightScanner& sls() = 0;

	/// Returns an interface to the 'Shape Fusion' module.
	/// @return Interface class for 'Shape Fusion' functions.
	virtual IShapeFusion& fusion() = 0;

	/// Returns an interface to the 'Turntable' module.
	/// @return Interface class for 'Turntable' functions.
	virtual ITurntable& turntable() = 0;

	/// Returns an interface to the 'Measurement' module.
	/// @return Interface class for 'Measurement' functions.
	virtual IMeasure& measure() = 0;

	/// Returns an interface to the 'Main Window' module.
	/// @return Interface class for 'Main Window' functions.
	virtual IMainWindow& mainWindow() = 0;
};

/// @} InterfaceGroup
/// @} ModulesGroup

} // namespace

#endif // DAVID_SDK_I_MODULES_H
