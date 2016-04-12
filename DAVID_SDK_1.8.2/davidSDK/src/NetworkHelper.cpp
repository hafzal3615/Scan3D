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

#include "davidSDK/NetworkHelper.h"

#include <string.h>

namespace david {

//=============================================================================
// NetworkHelper
//=============================================================================

NetworkHelper::NetworkHelper()
	: m_initOk(true)
{
#if defined(_WIN32) && !defined(DAVID_EXTERNAL_WSA_STARTUP)
	WSAData wsaData;
	m_initOk = (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0);
#endif
}


NetworkHelper::~NetworkHelper()
{
#if defined(_WIN32) && !defined(DAVID_EXTERNAL_WSA_STARTUP)
	if (m_initOk)
	{
		WSACleanup();
	}
#endif
}


SOCKET NetworkHelper::Connect(sockaddr_storage* sockaddr, socklen_t* addrlen, const std::string& address, uint16_t port)
{
	struct addrinfo hints;
	struct addrinfo* res = NULL;
	struct addrinfo* p = NULL;
	char service[8];
	SOCKET sock = -1;

	if(!port || address == "")
	{
		return -1;
	}

	_snprintf_s(service, sizeof(service), sizeof(service), "%u", port);
	service[sizeof(service)-1] = 0x00;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = 0;

	if(getaddrinfo(address.c_str(), service, &hints, &res) != 0)
	{
		return -1;
	}

	for(p = res ; p ; p = p->ai_next)
	{
		sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

		if(sock == -1)
		{
			continue;
		}

		if(::connect(sock, (struct sockaddr*)p->ai_addr, (int)p->ai_addrlen) == -1)
		{
			::closesocket(sock);
			sock = -1;
			continue;
		}

		if(sockaddr)
		{
			memcpy(sockaddr, p->ai_addr, p->ai_addrlen);
		}

		if(addrlen)
		{
			*addrlen = (socklen_t)p->ai_addrlen;
		}

		break;
	}

	freeaddrinfo(res);
	p = NULL;

	return sock;
}


SOCKET NetworkHelper::Bind(sockaddr_storage* sockaddr, socklen_t* addrlen, const std::string& address, uint16_t port)
{
	if (address.empty() || address == "INADDR_ANY") 
	{
		// create socket:
		struct sockaddr_in server_address;
		SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == sock) return -1;

		// prepare bind:
		memset(&server_address, 0, sizeof(struct sockaddr_in));
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = htonl(INADDR_ANY);
		server_address.sin_port = htons(port);

		// bind:
		if (0 != bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)))
		{
			::closesocket(sock);
			return -1;
		}

		return sock;
	}

	struct addrinfo hints;
	struct addrinfo* res = NULL;
	struct addrinfo* p = NULL;
	char service[8];
	SOCKET sock = -1;

	if(!port || address.empty())
	{
		return -1;
	}

	_snprintf_s(service, sizeof(service), sizeof(service), "%u", port);
	service[sizeof(service)-1] = 0x00;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if(getaddrinfo(address.c_str(), service, &hints, &res) != 0)
	{
		return -1;
	}

	for(p = res ; p ; p = p->ai_next)
	{
		int on = 1;

		sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

		if(sock == -1)
		{
			continue;
		}

#ifndef _WIN32
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

		/* accept IPv6 OR IPv4 on the same socket */
		on = 1;
		setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on));
#else
		on = 0;
#endif

		if(::bind(sock, p->ai_addr, (int)p->ai_addrlen) == -1)
		{
			::closesocket(sock);
			sock = -1;
			continue;
		}

		if(sockaddr)
		{
			memcpy(sockaddr, p->ai_addr, p->ai_addrlen);
		}

		if(addrlen)
		{
			*addrlen = (socklen_t)p->ai_addrlen;
		}

		break;
	}

	freeaddrinfo(res);
	p = NULL;

	return sock;
}


} // namespace
