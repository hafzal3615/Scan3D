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

#ifndef DAVID_SDK_MEASURE_JSON_RPC_H
#define DAVID_SDK_MEASURE_JSON_RPC_H

#include "davidSDK/ClientJsonRpc.h"
#include <vector>

namespace david {


//=============================================================================
// MeasureJsonRpc
//=============================================================================

/// Implements interface functions of IMeasure.
/// All calls are delegated to a JSON-RPC 2.0 client.
class MeasureJsonRpc : public IMeasure
{
public:
	/// Constructor.
	/// @param[in,out] client Valid pointer to client implementation.
	MeasureJsonRpc(ClientJsonRpc* client);
	
	/// Destructor.
	~MeasureJsonRpc();

	// See IMeasure
	void ComputeSurfaceDistances(std::vector<float>& distances, int meshID1, int meshID2, double distThresh);

private:
	//-------------------------------------------------------------------------
	// Private member variables
	//-------------------------------------------------------------------------

	ClientJsonRpc*     m_client;     ///< Client implementation
};


} // namespace

#endif // DAVID_SDK_MEASURE_JSON_RPC_H
