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

#ifndef DAVID_SDK_NETWORK_HELPER_H
#define DAVID_SDK_NETWORK_HELPER_H

#include <string>

#ifdef _WIN32
	#include <stdint.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>

	/* to use getaddrinfo, _WIN32_WINNT have to
	 * equal at least 0x0501
	 */

	#define OLD_WIN32_WINNT _WIN32_WINNT

	#if (_WIN32_WINNT < 0x0501)
		#undef _WIN32_WINNT
		#define _WIN32_WINNT 0x501
	#endif

	#include <ws2tcpip.h>

	#if (_WIN32_WINNT != OLD_WIN32_WINNT)
		#undef _WIN32_WINNT
		#define _WIN32_WINNT OLD_WIN32_WINNT
	#endif

	typedef int socklen_t;

	#ifndef QT_VERSION
		#define close closesocket
	#endif

#else  // not _WIN32
	#include <stdint.h>

	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/time.h>

	#include <unistd.h>

	#include <netinet/in.h>

	#include <netdb.h>

	#define SOCKET int
	#define closesocket close
#endif


namespace david {

//=============================================================================
// NetworkHelper
//=============================================================================

/// Helper class for initialization and cleanup of network usage.
/// On Windows WSAStartup is called within constructor and WSACleanup within destructor.
/// Define DAVID_EXTERNAL_WSA_STARTUP, if you want to call WSAStartup externally.
class NetworkHelper
{
public:
	/// Constructor. Calls WSAStartup on Windows.
	NetworkHelper();
	
	/// Destructor. Calls WSACleanup on Windows in case of successful WSAStartup.
	virtual ~NetworkHelper();

	/// Connect to a remote machine.
	/// @param[out] sockaddr Gets the sockaddr if the function succeeds.
	/// @param[out] addrlen Length of sockaddr if the functions succeeds.
	/// @param[in] address Remote address.
	/// @param[in] port Remote port.
	/// @return Socket descriptor if successful, -1 otherwise.
	static SOCKET Connect(sockaddr_storage* sockaddr, socklen_t* addrlen, const std::string& address, uint16_t port);

	/// Bind on a local address.
	/// @param[out] sockaddr Gets the sockaddr if the function succeeds.
	/// @param[out] addrlen Length of sockaddr if the functions succeeds.
	/// @param[in] address Remote address.
	/// @param[in] port Remote port.
	/// @return Socket descriptor if successful, -1 otherwise.
	static SOCKET Bind(sockaddr_storage* sockaddr, socklen_t* addrlen, const std::string& address, uint16_t port);

private:
	bool    m_initOk;      ///< Initialization ok?
};


} // namespace

#endif // DAVID_SDK_NETWORK_HELPER_H
