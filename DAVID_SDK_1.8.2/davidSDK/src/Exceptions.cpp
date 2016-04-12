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

#include "davidSDK/Exceptions.h"
#include <iostream>
#include <sstream>

namespace david {

//=============================================================================
// Exception
//=============================================================================


Exception::Exception(int errorCode, const std::string& errorText, const std::string& errorSource)
	: std::runtime_error(errorSource + " - " + errorText)
	, m_errorCode(errorCode)
	, m_errorText(errorText)
	, m_errorSource(errorSource)
{
}

Exception::~Exception() throw()
{
}

int Exception::GetErrorCode() const
{
	return m_errorCode;
}
	

std::string Exception::GetErrorText() const
{
	return m_errorText;
}


std::string Exception::GetErrorSource() const
{
	return m_errorSource;
}


void Exception::PrintError() 
{
	std::cerr << "Exception in " << m_errorSource << ": " << m_errorText << " (" << m_errorCode << ")" << std::endl;
}


//=============================================================================
// ConnectionException
//=============================================================================

ConnectionException::ConnectionException(int errorCode, const std::string& errorText, const std::string& errorSource)
	: Exception(errorCode, errorText, errorSource)
{
}


//=============================================================================
// ProtocolException
//=============================================================================


ProtocolException::ProtocolException(int errorCode, const std::string& errorText, const std::string& errorSource)
	: ConnectionException(errorCode, errorText, errorSource)
{
}


//=============================================================================
// FunctionException
//=============================================================================

FunctionException::FunctionException(int errorCode, const std::string& errorText, const std::string& errorSource)
	: Exception(errorCode, errorText, errorSource)
{
}


//=============================================================================
// Global functions
//=============================================================================

void ThrowException(ErrorCode errorCode, const std::string& errorText, const std::string& errorSource)
{
	if (errorCode == david::OK) return;
	
	if (errorCode <= david::ConnectionProtocolErrors) throw david::ProtocolException(errorCode, errorText, errorSource);
	else if (errorCode <= david::ConnectionErrors) throw david::ConnectionException(errorCode, errorText, errorSource);
	else throw david::FunctionException(errorCode, errorText, errorSource);
}


//=============================================================================
// ErrorExceptionConverter
//=============================================================================

int ErrorExceptionConverter::operator=(int errorCode)
{
	++m_autoId;
	
	if (errorCode < 0)
	{
		std::string errorText = GetDefaultErrorText((david::ErrorCode)errorCode);
		std::string errorSource = m_source.empty() ? "ErrorExceptionConverter" : m_source;
		if (!m_context.empty()) errorSource += " (" + m_context + ")";
		std::stringstream ss;
		ss << m_autoId;
		errorSource += " [" + ss.str() + "]";

		ThrowException((david::ErrorCode)errorCode, errorText, errorSource);
	}
	
	return errorCode;
}

ErrorExceptionConverter& ErrorExceptionConverter::operator() (const std::string& context)
{
	m_context = context;
	m_autoId = 0;
	return *this;
}

} // namespace

