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

#ifndef DAVID_SDK_CLIENT_D4LIB_H
#define DAVID_SDK_CLIENT_D4LIB_H

#include "davidSDK/IModules.h"

namespace david {

//*****************************************************************************
/// @addtogroup ManagerGroup
/// @{
/// @defgroup ClientD4LibGroup Client for DAVID Low Level Library
/// IModules implementation for binding of DAVID Low Level Library (d4lib).
/// @{
//*****************************************************************************

//=============================================================================
// LibraryInfo
//=============================================================================

/// Informat about the library.
struct LibraryInfo
{
	bool            licenseOk;        ///< Is license ok?
	std::string     libraryVersion;   ///< Describes the version of the library.

	/// Constructor.
	LibraryInfo() : licenseOk(false) {}
};


//=============================================================================
// ClientD4Lib
//=============================================================================

/// Implementation of IModules and client using DAVID Low Level Library (d4lib).
class ClientD4Lib : public IModules
{
public:
	/// Constructor.
	ClientD4Lib();
	
	/// Destructor.
	virtual ~ClientD4Lib();

	/// Init 'DAVID Low Level Library' (d4lib).
	/// @warning You need a valid shared library file ('d4lib.dll' on Windows) and a valid 'licenseKey'.
	///          Both are only available on request.
	/// @param[in] licenseKey Valid license key.
	/// @return Information about the library, see david::LibraryInfo.
	virtual LibraryInfo InitLibrary(const std::string& licenseKey);

	/// Release library.
	virtual void ReleaseLibrary();

	// See IModules
	virtual IStructuredLightScanner& sls();

	// See IModules
	virtual IShapeFusion& fusion();

	// See IModules
	virtual ITurntable& turntable();

	// See IModules
	virtual IMeasure& measure();

	// See IModules
	virtual IMainWindow& mainWindow();

protected:
	/// Create module instances.
	virtual void CreateModules();

	/// Create IStructuredLightScanner module.
	/// @param[in,out] fusion Valid pointer to IShapeFusion implementation.
	/// @return Pointer to IStructuredLightScanner implementation.
	virtual IStructuredLightScanner* Create_sls(IShapeFusion* fusion);

	/// Create IShapeFusion module.
	/// @return Pointer to IShapeFusion implementation.
	virtual IShapeFusion* Create_fusion();

protected:
	IStructuredLightScanner*    m_sls;          ///< IStructuredLightScanner implementation.
	IShapeFusion*               m_fusion;       ///< IShapeFusion implementation.
	IMeasure*                   m_measure;      ///< IMeasure implementation.
	ITurntable*                 m_turntable;    ///< ITurntable implementation.
	IMainWindow*                m_mainWindow;   ///< IMainWindow implementation.
};

/// @} ClientD4LibGroup
/// @} ManagerGroup

} // namespace

#endif // DAVID_SDK_CLIENT_D4LIB_H
