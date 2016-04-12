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

#ifndef DAVID_SDK_EXCEPTIONS_H
#define DAVID_SDK_EXCEPTIONS_H

#include "davidSDK/ErrorCodes.h"
#include <string>
#include <exception>
#include <stdexcept>

namespace david { 


//=============================================================================
// Exception
//=============================================================================

/// Base exception class. All other DAVID exceptions are derived from this class.
class Exception : public std::runtime_error
{
public:
	/// Constructor.
	/// @param[in] errorCode Error code.
	/// @param[in] errorText Textual description of the error.
	/// @param[in] errorSource Source of the error.
	Exception(int errorCode, const std::string& errorText, const std::string& errorSource);
	
	/// Destructor.
	virtual ~Exception() throw();

	/// Returns the error code. 
	/// @return Error code (see david::ErrorCode).
	int GetErrorCode() const;

	/// Returns a description for the error.
	/// @return Error text.
	std::string GetErrorText() const;

	/// Returns information about the error source.
	/// @return Error source.
	std::string GetErrorSource() const;

	/// Prints error information to error stream.
	void PrintError();

private:
	int             m_errorCode;     ///< Code of the error (see david::ErrorCode).
	std::string     m_errorText;     ///< Textual description of the error.
	std::string     m_errorSource;   ///< Source of the error.
};


//=============================================================================
// ConnectionException
//=============================================================================

/// Exception thrown in case of connection errors.
/// Associated error codes: All below david::CONNECTION_ERRORS.
class ConnectionException : public Exception
{
public:
	/// Constructor.
	/// @param[in] errorCode Error code.
	/// @param[in] errorText Textual description of the error.
	/// @param[in] errorSource Source of the error.
	ConnectionException(int errorCode, const std::string& errorText, const std::string& errorSource);
};


//=============================================================================
// ProtocolException
//=============================================================================

/// Special connection exception that indicates a problem with the communication protocol.
/// Incompatible implementations or wrong/incomplete implementation is likely.
/// Associated error codes: All below david::PROTOCOL_ERRORS.
class ProtocolException : public ConnectionException
{
public:
	/// Constructor.
	/// @param[in] errorCode Error code.
	/// @param[in] errorText Textual description of the error.
	/// @param[in] errorSource Source of the error.
	ProtocolException(int errorCode, const std::string& errorText, const std::string& errorSource);
};


//=============================================================================
// FunctionException
//=============================================================================

/// The remote procedure call was excecuted, but caused an error.
/// This should be the more 'typical' exception.
/// Associated error codes: All above david::CONNECTION_ERRORS.
class FunctionException : public Exception
{
public:
	/// Constructor.
	/// @param[in] errorCode Error code.
	/// @param[in] errorText Textual description of the error.
	/// @param[in] errorSource Source of the error.
	FunctionException(int errorCode, const std::string& errorText, const std::string& errorSource);
};


//=============================================================================
// Global functions
//=============================================================================

/// Throws an exception.
/// @param[in] errorCode Error code.
/// @param[in] errorText Textual description of the error.
/// @param[in] errorSource Source of the error.
void ThrowException(ErrorCode errorCode, const std::string& errorText, const std::string& errorSource);


//=============================================================================
// ErrorExceptionConverter
//=============================================================================

/// Easy conversion from error codes to exceptions.
/// The error code has to be one of david::ErrorCode.
///
/// Example:
/// @code
/// ErrorExceptionConverter eec;
/// eec = AnyFunctionWithErrorCodeAsReturnValue();  // will throw an exception, if return value < 0
/// @endcode
class ErrorExceptionConverter
{
public:
	/// Constructor.
	/// @param[in] source Optional source information.
	ErrorExceptionConverter(const std::string& source = "") : m_source(source), m_autoId(0) {}

	/// Destructor.
	virtual ~ErrorExceptionConverter() {}

	/// Throws an exception, if 'errorCode' is less then zero. 
	/// An assignment will also increment the automatic Id.
	/// The automatic Id will will be available in source info of an exception.
	/// @param[in] errorCode See david::ErrorCode.
	/// @return Error code.
	int operator=(int errorCode);

	/// Set more context information to source.
	/// The context information will be available in source info of an exception.
	/// The automatic Id will be reset to zero.
	/// @param[in] context More context information.
	/// @return Reference to itself.
	ErrorExceptionConverter& operator() (const std::string& context);

private:
	std::string     m_source;      ///< Information about error source.
	std::string     m_context;     ///< More context information.
	int             m_autoId;      ///< Automatic Id.
};


} // namespace

#endif // DAVID_SDK_EXCEPTIONS_H
