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

#ifndef DAVID_SDK_DAVID_H
#define DAVID_SDK_DAVID_H

#ifdef DAVID_SDK_USE_D4LIB
#include "davidSDK/ClientD4Lib.h"
#else
#include "davidSDK/ClientJsonRpc.h"
#endif


/// Basic DAVID namespace. 
namespace david { 


//*****************************************************************************
/// @addtogroup ManagerGroup
/// @{
/// @defgroup ClientGroup Client switching
/// Easy switching between different client implementations.
/// @{
//*****************************************************************************

#ifdef DAVID_SDK_USE_D4LIB

typedef ClientD4Lib Client;

#else

/// The Client class manages all modules and gives access to them via david::IModules interface implementation. 
/// You can select between two different implementations of david::IModules:
/// 1. 'DAVID Enterprise Server' (default): 
///     - ClientJsonRpc is defined as Client
///     - Use ClientJsonRpc::Connect and ClientJsonRpc::Disconnect
/// 2. 'DAVID Low Level Library': 
///     - ClientD4Lib is defined as Client
///     - Use ClientD4Lib::InitLibrary and ClientD4Lib::ReleaseLibrary
///
/// @note If you want to switch to 'DAVID Low Level Library', just define DAVID_SDK_USE_D4LIB in your project.
typedef ClientJsonRpc Client;

#endif


} // namespace

#endif // DAVID_SDK_DAVID_H
