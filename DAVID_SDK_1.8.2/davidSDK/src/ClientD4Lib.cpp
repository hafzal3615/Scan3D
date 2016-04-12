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

#include "davidSDK/ClientD4Lib.h"
#include "davidSDK/ShapeFusionD4Lib.h"
#include "davidSDK/StructuredLightScannerD4Lib.h"

namespace david {

//=============================================================================
// ClientD4Lib
//=============================================================================

ClientD4Lib::ClientD4Lib()
	: m_sls(NULL)
	, m_fusion(NULL)
	, m_measure(NULL)
	, m_turntable(NULL)
	, m_mainWindow(NULL)
{

}


ClientD4Lib::~ClientD4Lib()
{
	ReleaseLibrary();
}


LibraryInfo ClientD4Lib::InitLibrary(const std::string& licenseKey)
{
	ErrorExceptionConverter eec;
	
	ReleaseLibrary();

	LibraryInfo li;
	int result = d4lib_init(licenseKey.c_str());
	if (result == d4ok)
	{
		li.licenseOk = true;
		char version[256];
		eec = d4lib_getVersion(version, sizeof(version));
		li.libraryVersion = version;

		this->CreateModules();
	}
	else if (result == d4error_NotLicensed)
	{
		li.licenseOk = false;
	}
	else eec = result;

	return li;
}


void ClientD4Lib::ReleaseLibrary()
{
	delete m_sls; m_sls = NULL;
	delete m_fusion; m_fusion = NULL;
	delete m_measure; m_measure = NULL;
	delete m_turntable; m_turntable = NULL;
	delete m_mainWindow; m_mainWindow = NULL;
	d4lib_release();
}


IStructuredLightScanner& ClientD4Lib::sls()
{
	if (m_sls == NULL)
	{
		ThrowException(Error_MissingObject, "Invalid IStructuredLightScanner interface. Not initialized?", "ClientD4Lib::sls");
	}

	return *m_sls;
}


IShapeFusion& ClientD4Lib::fusion()
{
	if (m_fusion == NULL)
	{
		ThrowException(Error_MissingObject, "Invalid IShapeFusion interface. Not initialized?", "ClientD4Lib::fusion");
	}
	
	return *m_fusion;
}


ITurntable& ClientD4Lib::turntable()
{
	if (m_turntable == NULL)
	{
		ThrowException(Error_NotImplemented, "Implementation missing.", "ClientD4Lib::turntable");
	}
	
	return *m_turntable;
}


IMeasure& ClientD4Lib::measure()
{
	if (m_measure == NULL)
	{
		ThrowException(Error_NotImplemented, "Implementation missing.", "ClientD4Lib::measure");
	}

	return *m_measure;
}


IMainWindow& ClientD4Lib::mainWindow()
{
	if (m_mainWindow == NULL)
	{
		ThrowException(Error_NotImplemented, "Implementation missing.", "ClientD4Lib::mainWindow");
	}
	
	return *m_mainWindow;
}


void ClientD4Lib::CreateModules()
{
	m_fusion = Create_fusion();
	m_sls = Create_sls(m_fusion);
}


IStructuredLightScanner* ClientD4Lib::Create_sls(IShapeFusion* fusion)
{
	return new StructuredLightScannerD4Lib(fusion);
}


IShapeFusion* ClientD4Lib::Create_fusion()
{
	return new ShapeFusionD4Lib();
}


} // namespace
