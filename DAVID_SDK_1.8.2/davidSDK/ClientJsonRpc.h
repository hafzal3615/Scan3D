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

#ifndef DAVID_SDK_CLIENT_JSON_RPC_H
#define DAVID_SDK_CLIENT_JSON_RPC_H

#include "davidSDK/IModules.h"
#include "davidSDK/TcpClient.h"
#include "davidSDK/json/json.h"
//#include "davidSDK/jsonrpc/jsonrpc.h"
//#include "davidSDK/JsonRpcFunctions.h"

namespace david {

//*****************************************************************************
/// @addtogroup ManagerGroup
/// @{
/// @defgroup ClientJsonRpcGroup Client for DAVID Enterprise Server
/// IModules implementation for connection to DAVID Enterprise Server.
/// @{
//*****************************************************************************

//=============================================================================
// ServerInfo
//=============================================================================

/// Information about the server.
struct ServerInfo
{
	bool            licenseOk;        ///< Is license ok?
	std::string     serverVersion;    ///< Describes the version of the server.

	/// Constructor.
	ServerInfo() : licenseOk(false) {}
};


//=============================================================================
// ClientJsonRpc
//=============================================================================

/// Implementation of IModules and client using Json RPC 2.0.
/// The client connects to a running DAVID Enterprise Server.
class ClientJsonRpc : public IModules
{
public:
	/// Constructor.
	ClientJsonRpc();
	
	/// Destructor.
	virtual ~ClientJsonRpc();
	
	/// Connect to a running 'DAVID Enterprise Server' instance.
	/// @param[in] address IP address of the running server. Use 127.0.0.1 for local host.
	/// @param[in] port Port of server connection.
	/// @return Some information about the server.
	/// @exception david::Error_RpcClientVersion (david::FunctionException) Client version not compatible.
	ServerInfo Connect(const std::string& address = "127.0.0.1", uint16_t port = DAVID_SDK_DefaultPort);
	
	/// Disconnect from server.
	/// @param[in] stopServer Stop remote server?
	virtual void Disconnect(bool stopServer);

	// See IModules
	virtual IStructuredLightScanner& sls();

	// See IModules
	virtual IShapeFusion& fusion();

	// See IModules
	virtual ITurntable& turntable();

	// See IModules
	virtual IMeasure& measure();

	// See IModules
	virtual IMainWindow& mainWindow();

	/// Execute a remote procedure call all via RPC channel. 
	/// @param[out] response Response from server.
	/// @param[in] rpc Remote procedure call.
	void ExecuteRpc(Json::Value& response, const Json::Value& rpc);

	/// Prepare a remote procedure call.
	///
	/// This following variables are set:
	/// @code
	/// rpc["jsonrpc"] = "2.0";
	/// rpc["method"] = method;
	/// rpc["id"] = newID;
	/// @endcode
	/// 
	/// @param[in,out] rpc Remote procedure call. 
	/// @param[in] method Name of the method.
	void PrepareRpc(Json::Value& rpc, const std::string method);
	
	/// Read binary data from binary channel into buffer.
	/// @param[out] buffer Destination buffer for binary data.
	/// @param[in] bufferSize Size of buffer in bytes.
	void ReadBinaryData(void* buffer, size_t bufferSize);

	/// Extract a parameter value from Json value.
	/// @param[in] jsonParams Subtree of Json values.
	/// @param[in] paramName Name of parameter to be extracted.
	/// @return Value of parameter.
	/// @exception david::Error_RpcInvalidParams Parameter not available or of wrong type.
	template<class T>
	T ExtractJsonParam(const Json::Value& jsonParams, const std::string& paramName)
	{
		T value;

		const Json::Value& v = jsonParams[paramName];
		if (v.type() == Json::nullValue) 
		{
			ThrowException(david::Error_RpcInvalidParams, "Missing parameter " + paramName, "ClientImpl::ExtractJsonParam");
		}

		try 
		{ 
			GetJsonParamValue(value, v); 
		} 
		catch (std::runtime_error&)   
		{
			ThrowException(david::Error_RpcInvalidParams, "Wrong type of parameter " + paramName, "ClientImpl::ExtractJsonParam"); 
		}

		return value;
	}

	/// Extract a parameter value from Json value.
	/// @param[out] optionalValue Gets the value, if one is available.
	/// @param[in] jsonParams Subtree of Json values.
	/// @param[in] paramName Name of parameter to be extracted.
	/// @exception david::Error_RpcInvalidParams Parameter is of wrong type.
	template<class T>
	void ExtractJsonParam(Optional<T>& optionalValue, const Json::Value& jsonParams, const std::string& paramName)
	{
		const Json::Value& v = jsonParams[paramName];
		if (v.type() != Json::nullValue) 
		{
			try 
			{ 
				T value;
				GetJsonParamValue(value, v); 
				optionalValue = value;
			} 
			catch (std::runtime_error&)   
			{
				ThrowException(david::Error_RpcInvalidParams, "Wrong type of parameter " + paramName, "ClientImpl::ExtractJsonParam"); 
			}
		}
	}

private:
	void GetJsonParamValue(int &result, const Json::Value &v) { result=v.asInt(); }
	void GetJsonParamValue(uint32_t &result, const Json::Value &v) { result=v.asUInt(); }
	void GetJsonParamValue(bool &result, const Json::Value &v) { result=v.asBool(); }
	void GetJsonParamValue(double &result, const Json::Value &v) { result=v.asDouble(); }
	void GetJsonParamValue(float &result, const Json::Value &v) { result=(float)v.asDouble(); }
	void GetJsonParamValue(std::string &result, const Json::Value &v) { result=v.asString(); }

	template <class T>
	void GetJsonParamValue(std::vector<T> &result, const Json::Value &v)
	{
		if (!v.isArray()) throw std::runtime_error("not an array");

		result.resize(v.size());
		for (Json::Value::UInt i=0; i<v.size(); ++i) GetJsonParamValue(result[i], v[i]);
	}


private:
	NetworkHelper               m_network;      ///< Helper for networking. 
	IStructuredLightScanner*    m_sls;          ///< IStructuredLightScanner implementation.
	IShapeFusion*               m_fusion;       ///< IShapeFusion implementation.
	IMeasure*                   m_measure;      ///< IMeasure implementation.
	ITurntable*                 m_turntable;    ///< ITurntable implementation.
	IMainWindow*                m_mainWindow;   ///< IMainWindow implementation.
	TcpClient*                  m_client;       ///< JsonRpc connection to server.
	TcpClient*                  m_binary;       ///< Binary connection to server.
	Json::StyledWriter          m_writer;       ///< Json writer.
	Json::Reader                m_reader;       ///< Json reader.
	int                         m_messageID;    ///< Every message gets a unique ID.
};

/// @} ClientJsonRpcGroup
/// @} ManagerGroup

} // namespace

#endif // DAVID_SDK_CLIENT_JSON_RPC_H
