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

#include "davidSDK/ClientJsonRpc.h"
#include "davidSDK/ShapeFusionJsonRpc.h"
#include "davidSDK/StructuredLightScannerJsonRpc.h"
#include "davidSDK/MeasureJsonRpc.h"
#include "davidSDK/TurntableJsonRpc.h"
#include "davidSDK/MainWindowJsonRpc.h"

namespace david {

//=============================================================================
// ClientJsonRpc
//=============================================================================

ClientJsonRpc::ClientJsonRpc()
	: m_sls(NULL)
	, m_fusion(NULL)
	, m_measure(NULL)
	, m_turntable(NULL)
	, m_mainWindow(NULL)
	, m_client(NULL)
	, m_binary(NULL)
	, m_messageID(1)
{
	
}


ClientJsonRpc::~ClientJsonRpc()
{
	Disconnect(false);
}


ServerInfo ClientJsonRpc::Connect(const std::string& ipAddress, uint16_t port)
{
	Json::Value connectResponse;
	
	// Disconnect from server. This also deletes the client.
	Disconnect(false);

	// Create interfaces.
	m_sls = new StructuredLightScannerJsonRpc(this);
	m_fusion = new ShapeFusionJsonRpc(this);
	m_measure = new MeasureJsonRpc(this);
	m_turntable = new TurntableJsonRpc(this);
	m_mainWindow = new MainWindowJsonRpc(this);

	// Create new client interface.
	m_client = new TcpClient(ipAddress, port);
		
	// Connect to server.
	if (m_client->Connect())
	{
		// Json channel to server is established. Now, check the channel.
		{
			Json::Value rpc;
			PrepareRpc(rpc, "david::Connect");
			rpc["params"]["clientVersion"] = david::DAVID_SDK_ClientVersion;
			Json::Value response;
			this->ExecuteRpc(connectResponse, rpc);
		}
					
		// Establish binary channel.
		{
			Json::Value rpc;
			PrepareRpc(rpc, "david::RequestBinaryChannel");
			Json::Value response;
			this->ExecuteRpc(response, rpc);
			
			// Evaluate response.
			Json::Value result = response["result"];
			// Get address and port.
			const std::string address = m_client->GetAddress();
			const uint16_t port = (uint16_t) result.get("port", 0).asInt();
				
			// New TCP/IP connection for binary channel.
			m_binary = new TcpClient(address, port);
			if (!m_binary->Connect())
			{
				delete m_binary;
				m_binary = NULL;
				ThrowException(Error_BinarySetup, "Could not establish binary channel with server.", "david::ClientJsonRpc::Connect");
			}
		}
	}
	else
	{
		delete m_client;
		m_client = NULL;
		ThrowException(Error_ConnectionFailed, "Could not connect to server.", "david::ClientJsonRpc::Connect");
	}

	// Get return value.
	Json::Value result = connectResponse["result"];
	ServerInfo info;
	info.licenseOk = ExtractJsonParam<bool>(result, "licenseOk");
	info.serverVersion = ExtractJsonParam<std::string>(result, "serverVersion");
	return info;
}


void ClientJsonRpc::Disconnect(bool stopServer)
{
	// Stop server.
	if (m_client && stopServer)
	{
		Json::Value rpc;
		PrepareRpc(rpc, "david::StopServer");
		Json::Value response;
		ExecuteRpc(response, rpc);
	}

	// Try to inform server that we disconnect.
	if (m_client)
	{
		try
		{
			Json::Value rpc;
			PrepareRpc(rpc, "david::Disconnect");
			Json::Value response;
			ExecuteRpc(response, rpc);
		}
		catch (david::Exception&) {}
	}
		
	// Delete binary channel.
	if (m_binary)
	{
		m_binary->Close();
		delete m_binary;
		m_binary = NULL;
	}
		
	// Delete JSON client interface.
	if (m_client)
	{
		m_client->Close();
		delete m_client;
		m_client = NULL;
	}

	delete m_sls; m_sls = NULL;
	delete m_fusion; m_fusion = NULL;
	delete m_measure; m_measure = NULL;
	delete m_turntable; m_turntable = NULL;
	delete m_mainWindow; m_mainWindow = NULL;
}


IStructuredLightScanner& ClientJsonRpc::sls()
{
	if (m_sls == NULL)
	{
		ThrowException(Error_MissingObject, "Invalid IStructuredLightScanner interface. Not connected to server?", "ClientJsonRpc::sls");
	}

	return *m_sls;
}


IShapeFusion& ClientJsonRpc::fusion()
{
	if (m_fusion == NULL)
	{
		ThrowException(Error_MissingObject, "Invalid IShapeFusion interface. Not connected to server?", "ClientJsonRpc::fusion");
	}
	
	return *m_fusion;
}


ITurntable& ClientJsonRpc::turntable()
{
	if (m_turntable == NULL)
	{
		ThrowException(Error_MissingObject, "Invalid ITurntable interface. Not connected to server?", "ClientJsonRpc::turntable");
	}
	
	return *m_turntable;
}


IMeasure& ClientJsonRpc::measure()
{
	if (m_measure == NULL)
	{
		ThrowException(Error_MissingObject, "Invalid IMeasure interface. Not connected to server?", "ClientJsonRpc::measure");
	}
	
	return *m_measure;
}


IMainWindow& ClientJsonRpc::mainWindow()
{
	if (m_mainWindow == NULL)
	{
		ThrowException(Error_MissingObject, "Invalid IMainWindow interface. Not connected to server?", "ClientJsonRpc::mainWindow");
	}
	
	return *m_mainWindow;
}


void ClientJsonRpc::ExecuteRpc(Json::Value& response, const Json::Value& rpc)
{
	response.clear();
		
	if (m_client)
	{
		// Send data.
		std::cout << "--> " << m_writer.write(rpc) << std::endl;
		const int64_t sendSize = m_client->Send(m_writer.write(rpc));
		std::cout << "Send size: " << sendSize << std::endl;
			
		// Receive data.
		if (sendSize > 0)
		{
			std::string responseStr;
			const int64_t recvSize = m_client->Recv(responseStr);
			std::cout << "<-- " << responseStr << std::endl;
			if (recvSize <= 0)
			{
				ThrowException(Error_ConnectionLost, "Lost connection to server (receive failed).", "david::ClientJsonRpc::ExecuteRpc");
			}
			else if (!m_reader.parse(responseStr, response))
			{
				ThrowException(Error_RpcParse, m_reader.getFormatedErrorMessages(), "david::ClientJsonRpc::ExecuteRpc");
			}
			else if (response.isMember("error"))
			{
				Json::Value jsonError = response["error"];
				const ErrorCode code = (ErrorCode) jsonError.get("code", david::Error_RpcParse).asInt();
				const std::string message = jsonError.get("message", "Unknown parse error.").asString();
				ThrowException(code, message, "david::ClientJsonRpc::ExecuteRpc");
			}
		}
		else ThrowException(Error_ConnectionLost, "Lost connection to server (send failed).", "david::ClientJsonRpc::ExecuteRpc");
	}
	else
	{
		ThrowException(Error_ConnectionFailed, "No connection to server.", "david::ClientJsonRpc::ExecuteRpc");
	}
}


void ClientJsonRpc::PrepareRpc(Json::Value& rpc, const std::string method)
{
	rpc["jsonrpc"] = "2.0";
	rpc["method"] = method;
	rpc["id"] = m_messageID++;
}


void ClientJsonRpc::ReadBinaryData(void* buffer, size_t bufferSize)
{
	if (m_client)
	{
		if (m_binary)
		{
			if (!m_binary->RecvBinary(buffer, bufferSize))
			{	
				ThrowException(Error_BinaryRead, "Receive failed.", "david::ClientJsonRpc::ReadBinaryData");
			}
		}
		else
		{
			ThrowException(Error_BinaryRead, "No binary channel available.", "david::ClientJsonRpc::ReadBinaryData");
		}
	}
}


} // namespace
