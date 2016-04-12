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

#ifndef DAVID_SDK_D4_PTR_H
#define DAVID_SDK_D4_PTR_H

#include "davidSDK/d4lib.h"
#include "davidSDK/Exceptions.h"

namespace david {

//=============================================================================
// D4Ptr
//=============================================================================

/// Helper class that automatically decrements reference count of a d4lib object.
template <class T>
class D4Ptr
{
public:
	/// Constructor.
	D4Ptr() : m_ptr(NULL) {}

	/// Copy constructor.
	/// @param[in,out] ptr Reference to other smart pointer.
	D4Ptr(const D4Ptr& ptr) 
	{
		m_ptr = ptr.m_ptr;
		d4mem_incRefCount(m_ptr);
	}

	/// Constructor.
	/// @param[in] ptr Valid pointer of a d4lib object.
	/// @param[in] checkPtr Check pointer? If true and 'ptr' is NULL, an exception will be thrown.
	/// @throw david::Exception if 'ptr'==NULL and 'checkPtr'==true.
	explicit D4Ptr(T ptr, bool checkPtr=true) : m_ptr(ptr) 
	{
		if (checkPtr && ptr == NULL) ThrowException(Error_InvalidArgument, "Pointer is NULL.", "D4Ptr");
	}

	/// Destructor.
	~D4Ptr() 
	{
		Reset();
	}

	/// Convert to native handle type.
	operator T() {return m_ptr;}

	/// Assign pointer and increment reference count.
	/// @param[in,out] ptr Reference to other smart pointer.
	/// @return Reference to *this.
	D4Ptr& operator=(D4Ptr& ptr) 
	{
		if (m_ptr == ptr.m_ptr) return *this;
		Reset();
		m_ptr = ptr.m_ptr;
		if (m_ptr) d4mem_incRefCount(m_ptr);
		return *this;
	}

	/// Reset the smart pointer. 
	/// Decrement reference count and set internal pointer to NULL.
	void Reset()
	{
		if (m_ptr) d4mem_decRefCount(m_ptr);
		m_ptr = NULL;
	}

private:
	T      m_ptr;     ///< Native pointer.
};


} // namespace

#endif // DAVID_SDK_D4_PTR_H
