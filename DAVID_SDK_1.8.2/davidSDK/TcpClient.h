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

#ifndef DAVID_SDK_TCP_CLIENT_H
#define DAVID_SDK_TCP_CLIENT_H

#include "davidSDK/Common.h"
#include "davidSDK/NetworkHelper.h"

namespace david {

//=============================================================================
// TcpClient
//=============================================================================

/// Implementation of a TCP client.
class TcpClient
{
public:
	//-------------------------------------------------------------------------
	// (De-)Initialization
	//-------------------------------------------------------------------------

	/// Constructor.
	/// @param[in] address Remote IP address. Use 127.0.0.1 for local host.
	/// @param[in] port Remote port.
	TcpClient(const std::string& address, uint16_t port);
	
	/// Destructor.
	virtual ~TcpClient();
	
	/// Connect to the remote machine.
	/// True if successful, false otherwise.
	bool Connect();
	
	/// Close socket.
	void Close();

	//-------------------------------------------------------------------------
	// Info
	//-------------------------------------------------------------------------

	/// Get remote IP address.
	/// @return IP address.
	std::string GetAddress() const;

	//-------------------------------------------------------------------------
	// Data transfer
	//-------------------------------------------------------------------------

	/// Receive data from the network. Function blocks until data is received.
	/// @param[out] data Gets the received data.
	/// @return Number of bytes received or -1 in case of an error.
	int64_t Recv(std::string& data);

	/// Receive binary data from the network.
	/// @note This method will blocked until 'numBytes' were received.
	/// @param[out] data Destination buffer with size of at least 'numBytes'.
	/// @param[in] numBytes Number of bytes to be received.
	/// @return True, if 'numBytes' were received.
	bool RecvBinary(void* data, size_t numBytes);

	/// Send data.
	/// @param[in] data Data to be send.
	/// @return Number of bytes sent or -1 in case of an error.
	int64_t Send(const std::string& data);

	/// Send binary data to network.
	/// @note This method will blocked until 'numBytes' were send.
	/// @param[in] data Source buffer with size of at least 'numBytes'.
	/// @param[in] numBytes Number of bytes to be send.
	/// @return True, if 'numBytes' were send.
	bool SendBinary(const void* data, size_t numBytes);

private:
	//-------------------------------------------------------------------------
	// Member variables
	//-------------------------------------------------------------------------

	std::string              m_address;            ///< Network address
	uint16_t                 m_port;               ///< Local port
	SOCKET                   m_sock;               ///< Socket descriptor.
	socklen_t                m_sockaddrlen;        ///< Remote socket address length.
	sockaddr_storage         m_sockaddr;           ///< Remote socket address.
};


} // namespace

#endif // DAVID_SDK_TCP_CLIENT_H
