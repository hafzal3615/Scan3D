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

#ifndef DAVID_SDK_I_MEASURE_H
#define DAVID_SDK_I_MEASURE_H

#include "davidSDK/Common.h"
#include <vector>

namespace david {

//*****************************************************************************
/// @addtogroup InterfaceGroup 
/// @{
/// @defgroup MeasureGroup Measurements
/// Functions and types related to 'Measure'.
/// @{
//*****************************************************************************

//=============================================================================
// IMeasure
//=============================================================================

/// Interface for 'Measurement' functions.
///
class IMeasure
{
public:
	/// Destructor.
	virtual ~IMeasure() {}

	/// Computes signed distance between 'test object' and 'reference object'. 
	/// For each vertex of mesh 'test object' the signed distance to the mesh 'reference object' is computed. 
	/// @param[out] distances Receives the distance values. One signed distance value for each vertex of 'test object'.
	/// @param[in] meshID1 ID of the mesh that is the test object.
	/// @param[in] meshID2 ID of the other mesh that is the reference object.
	/// @param[in] distThresh Distance threshold: Distance values are limited to the range [-distThresh, distThresh].
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	virtual void ComputeSurfaceDistances(std::vector<float>& distances, int meshID1, int meshID2, double distThresh) = 0;
};

/// @} MeasureGroup
/// @} InterfaceGroup

} // namespace

#endif // DAVID_SDK_I_MEASURE_H
