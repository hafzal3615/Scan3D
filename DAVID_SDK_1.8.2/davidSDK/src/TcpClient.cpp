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

#include "davidSDK/TcpClient.h"
#include <iostream>
#include <string.h>

namespace david {

//=============================================================================
// TcpClient
//=============================================================================

TcpClient::TcpClient(const std::string& address, uint16_t port)
	: m_address(address)
	, m_port(port)
	, m_sock(-1)
	, m_sockaddrlen(0)
{
	memset(&m_sockaddr, 0, sizeof(struct sockaddr_storage));
}


TcpClient::~TcpClient()
{
	if(m_sock != -1)
	{
		Close();
	}
}


bool TcpClient::Connect()
{
	m_sock = NetworkHelper::Connect(&m_sockaddr, &m_sockaddrlen, m_address, m_port);
	return m_sock != -1;
}


void TcpClient::Close()
{
	::closesocket(m_sock);
	m_sock = -1;
}


std::string TcpClient::GetAddress() const
{
	return m_address;
}


int64_t TcpClient::Recv(std::string& data)
{
	char buf[1500];
	int64_t nb = -1;

	if((nb = ::recv(m_sock, buf, sizeof(buf), 0)) == -1)
	{
		std::cerr << "ERROR - Json::Rpc::TcpClient::Recv - Error while receiving" << std::endl;
		return -1;
	}

	data = std::string(buf, size_t(nb));

	return nb;
}


bool TcpClient::RecvBinary(void* data, size_t numBytes)
{
	const size_t maxTransmitSize = 1000000000; // 1GB
		
	if(data == NULL) 
	{
		std::cerr << "ERROR - Json::Rpc::TcpClient::RecvBinary - Invalid input parameter." << std::endl;
		return false;
	}
		
	if(numBytes > maxTransmitSize)
	{
		std::cerr << "ERROR - Json::Rpc::TcpClient::RecvBinary - Maximum transmit size of " << maxTransmitSize << " bytes is exceeded." << std::endl;
		return false;
	}
		
	if(numBytes == 0) return true;
		
	int bytes_recv = 0;
	do
	{
		const int cur_num_bytes = int(numBytes) - bytes_recv;
		const int cur_bytes_recv = recv(m_sock, ((char*)data)+bytes_recv, cur_num_bytes, 0);
		if(cur_bytes_recv > 0)
		{
			bytes_recv += cur_bytes_recv;
		}
		else if(cur_bytes_recv < 0)
		{
			std::cerr << "ERROR - Json::Rpc::TcpClient::RecvBinary - Did not receive enough bytes. Connection lost?" << std::endl;
			return false;
		}
		else
		{
			bytes_recv = cur_bytes_recv;
			break;
		}
	} while(bytes_recv < int(numBytes));
		
	return true;
}


int64_t TcpClient::Send(const std::string& data)
{
	return ::send(m_sock, data.c_str(), (int)data.length(), 0);
}


bool TcpClient::SendBinary(const void* data, size_t numBytes)
{
	const size_t maxTransmitSize = 1000000000; // 1GB
		
	// Input parameters ok?
	if(data == NULL) 
	{
		std::cerr << "ERROR - Json::Rpc::TcpClient::SendBinary - Invalid input parameter." << std::endl;
		return false;
	}
	if(numBytes > maxTransmitSize)
	{
		std::cerr << "ERROR - Json::Rpc::TcpClient::SendBinary - Maximum transmit size of " << maxTransmitSize << " bytes is exceeded." << std::endl;
		return false;
	}
		
	// Send bytes.
	const int bytes_sent = send(m_sock, (const char*) data, (int)numBytes, MSG_DONTROUTE);
		
	// Check.
	if(bytes_sent == int(numBytes)) return true;
	else
	{
		std::cerr << "ERROR - Json::Rpc::TcpClient::SendBinary - Transmit failed. Connection lost?" << std::endl;
		return false;
	}
}


} // namespace
