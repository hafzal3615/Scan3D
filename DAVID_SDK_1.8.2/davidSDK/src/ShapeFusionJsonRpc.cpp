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

#include "davidSDK/ShapeFusionJsonRpc.h"

namespace david { 

//=============================================================================
// Global functions
//=============================================================================

std::string ConvertTransformType(enum TransformType transformType)
{
	if      (transformType == TransformGlobalX) return "TransformGlobalX";
	else if (transformType == TransformGlobalY) return "TransformGlobalY";
	else if (transformType == TransformGlobalZ) return "TransformGlobalZ";
	else if (transformType == TransformMeshX)   return "TransformMeshX";
	else if (transformType == TransformMeshY)   return "TransformMeshY";
	else if (transformType == TransformMeshZ)   return "TransformMeshZ";
	else return "";
}


std::string ConvertCoordinateSystem(enum CoordinateSystem coordinateSystem)
{
	if      (coordinateSystem == WorldCoordinates) return "WorldCoordinates";
	else if (coordinateSystem == LocalCoordinates) return "LocalCoordinates";
	else return "";
}


//=============================================================================
// ShapeFusionJsonRpc
//=============================================================================

ShapeFusionJsonRpc::ShapeFusionJsonRpc(ClientJsonRpc* client)
	: m_client(client)
{
}


ShapeFusionJsonRpc::~ShapeFusionJsonRpc()
{

}


void ShapeFusionJsonRpc::ExportMesh(int meshID, const std::string& filename)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::ExportMesh");
	rpc["params"]["meshID"] = meshID;
	rpc["params"]["filename"] = filename;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


int ShapeFusionJsonRpc::ImportMesh(const std::string& filename)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::ImportMesh");
	rpc["params"]["filename"] = filename;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	return m_client->ExtractJsonParam<int>(response, "result");
}


int ShapeFusionJsonRpc::DuplicateMesh(int sourceMeshID)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::DuplicateMesh");
	rpc["params"]["sourceMeshID"] = sourceMeshID;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	return m_client->ExtractJsonParam<int>(response, "result");
}


int ShapeFusionJsonRpc::NewMesh()
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::NewMesh");
	return -1;
}


void ShapeFusionJsonRpc::DeleteAllMeshes()
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::DeleteAllMeshes");
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


void ShapeFusionJsonRpc::DeleteMesh(int meshID)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::DeleteMesh");
	rpc["params"]["meshID"] = meshID;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


void ShapeFusionJsonRpc::GetVertexPositions(std::vector<float>& positions, int meshID, enum CoordinateSystem coordinateSystem)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::GetMeshBuffer");
	rpc["params"]["meshID"] = meshID;
	rpc["params"]["bufferType"] = "VertexPositions";
	rpc["params"]["coordinateSytem"] = ConvertCoordinateSystem(coordinateSystem);
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
	
	Json::Value result = response["result"];
	
	const uint32_t vertexCount = m_client->ExtractJsonParam<uint32_t>(result, "vertexCount");
	const uint32_t size = m_client->ExtractJsonParam<uint32_t>(result, "size");
		
	if (size == vertexCount*3*4)
	{
		positions.resize(3*vertexCount);
		if (size > 0)
		{
			m_client->ReadBinaryData(&positions[0], size);
		}
	}
	else
	{
		ThrowException(david::Error_InconsistentBuffer, "Inconsistent buffer size received from server.", "david::ShapeFusionJsonRpc::GetVertexPositions");
	}
}


void ShapeFusionJsonRpc::GetVertexQualities(std::vector<float>& qualities, int meshID)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::GetVertexQualities");
}


void ShapeFusionJsonRpc::GetVertexNormals(std::vector<float>& normals, int meshID, enum CoordinateSystem coordinateSystem)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::GetMeshBuffer");
	rpc["params"]["meshID"] = meshID;
	rpc["params"]["bufferType"] = "VertexNormals";
	rpc["params"]["coordinateSytem"] = ConvertCoordinateSystem(coordinateSystem);

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	Json::Value result = response["result"];
		
	const uint32_t vertexCount = m_client->ExtractJsonParam<uint32_t>(result, "vertexCount");
	const uint32_t size = m_client->ExtractJsonParam<uint32_t>(result, "size");
		
	if (size == vertexCount*3*4)
	{
		normals.resize(3*vertexCount);
		if (size > 0)
		{
			m_client->ReadBinaryData(&normals[0], size);
		}
	}
	else
	{
		ThrowException(david::Error_InconsistentBuffer, "Inconsistent buffer size received from server.", "david::ShapeFusionJsonRpc::GetVertexNormals");
	}
}


void ShapeFusionJsonRpc::GetVertexTexCoords(std::vector<float>& texCoords, int meshID)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::GetMeshBuffer");
	rpc["params"]["meshID"] = meshID;
	rpc["params"]["bufferType"] = "VertexTexCoords";

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	Json::Value result = response["result"];
		
	const uint32_t vertexCount = m_client->ExtractJsonParam<uint32_t>(result, "vertexCount");
	const uint32_t size = m_client->ExtractJsonParam<uint32_t>(result, "size");
		
	if (size != 2*sizeof(float)*vertexCount) ThrowException(david::Error_InconsistentBuffer, "Inconsistent buffer size received from server.", "david::ShapeFusionJsonRpc::GetVertexTexCoords");

	texCoords.resize(2*vertexCount);
	if (size > 0) m_client->ReadBinaryData(&texCoords[0], size);
}


void ShapeFusionJsonRpc::GetTriangles(std::vector<int>& triangles, int meshID)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::GetMeshBuffer");
	rpc["params"]["meshID"] = meshID;
	rpc["params"]["bufferType"] = "Triangles";

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	Json::Value result = response["result"];
	
	const uint32_t triangleCount = m_client->ExtractJsonParam<uint32_t>(result, "triangleCount");
	const uint32_t size = m_client->ExtractJsonParam<uint32_t>(result, "size");
	
	if (size == triangleCount*3*4)
	{
		triangles.resize(3*triangleCount);
		if (size > 0)
		{
			m_client->ReadBinaryData(&triangles[0], size);
		}
	}
	else
	{
		ThrowException(david::Error_InconsistentBuffer, "Inconsistent buffer size received from server.", "david::ShapeFusionJsonRpc::GetTriangles");
	}
}


void ShapeFusionJsonRpc::GetTextureImageData(void* data, size_t dataSizeInBytes, int meshID, int submeshIndex)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::GetTextureImage");
	rpc["params"]["meshID"] = meshID;
	rpc["params"]["submeshIndex"] = submeshIndex;
	rpc["params"]["sendData"] = true;

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	Json::Value result = response["result"];
	const size_t size   = m_client->ExtractJsonParam<uint32_t>(result, "size");
	int width  = m_client->ExtractJsonParam<int>(result, "width");
	int height = m_client->ExtractJsonParam<int>(result, "height");
	
	if (size != uint32_t(width*height*3)) ThrowException(david::Error_InconsistentBuffer, "Inconsistent buffer size received from server.", "david::ShapeFusionJsonRpc::GetTextureImageData");
	if (dataSizeInBytes != size) ThrowException(david::Error_InvalidArgument, "User data buffer has invalid size.", "david::ShapeFusionJsonRpc::GetTextureImageData");

	if (size > 0)
	{
		m_client->ReadBinaryData(data, size);
	}
}


bool ShapeFusionJsonRpc::GetTextureImageFormat(int& width, int& height, enum PixelFormat& pixelFormat, int meshID, int submeshIndex)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::GetTextureImage");
	rpc["params"]["meshID"] = meshID;
	rpc["params"]["submeshIndex"] = submeshIndex;
	rpc["params"]["sendData"] = false;

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	Json::Value result = response["result"];
	const uint32_t size   = m_client->ExtractJsonParam<uint32_t>(result, "size");
	width  = m_client->ExtractJsonParam<int>(result, "width");
	height = m_client->ExtractJsonParam<int>(result, "height");
	
	if ((int)size == width*height*3 && size > 0)
	{
		pixelFormat = RGB24;
		return true;
	}
	else
	{
		pixelFormat = UnknownPixelFormat;
		return false;
	}
}


void ShapeFusionJsonRpc::SetTexture(const void* data, size_t dataSizeInBytes, int width, int height, enum PixelFormat pixelFormat, int meshID, int submeshIndex)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::SetTexture");
}


void ShapeFusionJsonRpc::GetPose(double world_T_local[16], int meshID)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::GetPose");
	rpc["params"]["meshID"] = meshID;

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	Json::Value result = response["result"];

	world_T_local[0]  = m_client->ExtractJsonParam<double>(result, "nx");
	world_T_local[1]  = m_client->ExtractJsonParam<double>(result, "ny");
	world_T_local[2]  = m_client->ExtractJsonParam<double>(result, "nz");
	world_T_local[3]  = 0.0;
	world_T_local[4]  = m_client->ExtractJsonParam<double>(result, "ox");
	world_T_local[5]  = m_client->ExtractJsonParam<double>(result, "oy");
	world_T_local[6]  = m_client->ExtractJsonParam<double>(result, "oz");
	world_T_local[7]  = 0.0;
	world_T_local[8]  = m_client->ExtractJsonParam<double>(result, "ax");
	world_T_local[9]  = m_client->ExtractJsonParam<double>(result, "ay");
	world_T_local[10] = m_client->ExtractJsonParam<double>(result, "az");
	world_T_local[11] = 0.0;
	world_T_local[12] = m_client->ExtractJsonParam<double>(result, "px");
	world_T_local[13] = m_client->ExtractJsonParam<double>(result, "py");
	world_T_local[14] = m_client->ExtractJsonParam<double>(result, "pz");
	world_T_local[15] = 1.0;
}


void ShapeFusionJsonRpc::SetPose(const double world_T_local[16], int meshID)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::SetPose");
	rpc["params"]["meshID"] = meshID;
	rpc["params"]["px"] = world_T_local[12];
	rpc["params"]["py"] = world_T_local[13];
	rpc["params"]["pz"] = world_T_local[14];
	rpc["params"]["nx"] = world_T_local[0];
	rpc["params"]["ny"] = world_T_local[1];
	rpc["params"]["nz"] = world_T_local[2];
	rpc["params"]["ox"] = world_T_local[4];
	rpc["params"]["oy"] = world_T_local[5];
	rpc["params"]["oz"] = world_T_local[6];
	rpc["params"]["ax"] = world_T_local[8];
	rpc["params"]["ay"] = world_T_local[9];
	rpc["params"]["az"] = world_T_local[10];

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


void ShapeFusionJsonRpc::Rotate(int meshID, enum TransformType transformType, double degrees)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::Rotate");
	rpc["params"]["meshID"] = meshID;
	rpc["params"]["transformType"] = ConvertTransformType(transformType);
	rpc["params"]["degrees"] = degrees;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


void ShapeFusionJsonRpc::Translate(int meshID, enum TransformType transformType, double amount)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::Translate");
	rpc["params"]["meshID"] = meshID;
	rpc["params"]["transformType"] = ConvertTransformType(transformType);
	rpc["params"]["amount"] = amount;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


void ShapeFusionJsonRpc::ReduceMeshDensity(int meshID, float factor)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::ReduceMeshDensity");
	rpc["params"]["meshID"] = meshID;
	rpc["params"]["factor"] = factor;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


double ShapeFusionJsonRpc::AlignPairCoarse(int meshID1, int meshID2, const CoarseAlignParams &params)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::AlignPairCoarse");
	rpc["params"]["meshID1"] = meshID1;
	rpc["params"]["meshID2"] = meshID2;
	rpc["params"]["texturePercentage"] = params.texturePercentage;
	rpc["params"]["qualityFactor"] = params.qualityFactor;

	rpc["params"]["motionInfo"]["angleKnown"] = params.motionInfo.angleKnown;
	rpc["params"]["motionInfo"]["axisDirKnown"] = params.motionInfo.axisDirKnown;
	rpc["params"]["motionInfo"]["angle"] = params.motionInfo.angleDeg;
	rpc["params"]["motionInfo"]["angleTolerance"] = params.motionInfo.angleToleranceDeg;
	{
		Json::Value av(Json::arrayValue);
		av.resize(3);
		for (Json::UInt i=0; i < 3; ++i) av[i] = params.motionInfo.axisDir[i];
		rpc["params"]["motionInfo"]["axisDir"] = av;
	}
	
	if (params.contactRegionPair.vertexIndices1.size()>0 || params.contactRegionPair.vertexIndices2.size()>0) ThrowException(david::Error_NotImplemented, "Specifying contact regions is not implemented.", "david::ShapeFusionJsonRpc::AlignPairCoarse");
	if (params.blacklist1.poses.size()>0) ThrowException(david::Error_NotImplemented, "Specifying blacklisted poses is not implemented.", "david::ShapeFusionJsonRpc::AlignPairCoarse");
	if (params.whitelist1.poses.size()>0) ThrowException(david::Error_NotImplemented, "Specifying whitelist poses is not implemented.", "david::ShapeFusionJsonRpc::AlignPairCoarse");

	if (params.blacklist1.poses.size() % 16 != 0) ThrowException(david::Error_RpcInvalidParams, "Number of doubles in params.blacklist1.poses must be multiple of 16 - expecting 4*4 matrices", "ShapeFusionJsonRpc::AlignPairCoarse");
	if (params.whitelist1.poses.size() % 16 != 0) ThrowException(david::Error_RpcInvalidParams, "Number of doubles in params.whitelist1.poses must be multiple of 16 - expecting 4*4 matrices", "ShapeFusionJsonRpc::AlignPairCoarse");

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	return 0.01 * m_client->ExtractJsonParam<int>(response, "result");  // convert from percent to [0,1]
}


void ShapeFusionJsonRpc::AlignPairFine(int meshID1, int meshID2, const FineAlignParams &params)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::AlignPairFine");
	rpc["params"]["meshID1"] = meshID1;
	rpc["params"]["meshID2"] = meshID2;
	rpc["params"]["texturePercentage"] = params.texturePercentage;
	rpc["params"]["maxNumIterations"]  = params.maxNumIterations;
	rpc["params"]["maxTranslation"]    = params.maxTranslation;
	rpc["params"]["maxRotation_deg"]   = params.maxRot_deg;

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


void ShapeFusionJsonRpc::AlignGlobalFine(const GlobalFineAlignParams &params)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::AlignGlobalFine");
	rpc["params"]["texturePercentage"] = params.texturePercentage;
	rpc["params"]["numIterations"]     = params.numIterations;
	rpc["params"]["maxTranslation"]    = params.maxTranslation;
	rpc["params"]["maxRotation_deg"]   = params.maxRot_deg;

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);
}


int ShapeFusionJsonRpc::Fuse(int resolution, float holeSizeThresRel, int sharpness)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::Fuse");
	rpc["params"]["resolution"] = resolution;
	rpc["params"]["holeSizeThresRel"] = holeSizeThresRel;
	rpc["params"]["sharpness"] = sharpness;
	
	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	return m_client->ExtractJsonParam<int>(response, "result");
}


void ShapeFusionJsonRpc::RemoveVerticesByVirtualCamMask(const std::vector<bool>& mask, int width, int height, double f, const double world_T_cam[16])
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::RemoveVerticesByVirtualCamMask");
}


void ShapeFusionJsonRpc::RemoveSelectedVertices(const std::vector<SubmeshVertexIndex>& selectedVertices, int meshID)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::RemoveSelectedVertices");
}


void ShapeFusionJsonRpc::InvertTriangleOrientation(int meshID)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::InvertTriangleOrientation");
}


int ShapeFusionJsonRpc::GetFuseResolution(double metricResolution, int maxResolution)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::GetFuseResolution");
	return 0;
}


int ShapeFusionJsonRpc::CombineMeshes(const std::vector<int> &meshIDs)
{
	Json::Value av(Json::arrayValue);
	av.resize((Json::UInt)meshIDs.size());
	for(Json::UInt i=0; i<meshIDs.size(); ++i) av[i] = meshIDs[i];
	
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::CombineMeshes");
	rpc["params"]["meshIDs"] = av;

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	return m_client->ExtractJsonParam<int>(response, "result");
}


std::vector<int> ShapeFusionJsonRpc::UncombineMeshes(int meshIDgroup)
{
	Json::Value rpc;
	m_client->PrepareRpc(rpc, "david::shapefusion::UncombineMeshes");
	rpc["params"]["meshID"] = meshIDgroup;

	Json::Value response;
	m_client->ExecuteRpc(response, rpc);

	return m_client->ExtractJsonParam< std::vector<int> >(response, "result");
}


int ShapeFusionJsonRpc::GetSubmeshCount(int meshID)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::GetSubmeshCount");
	return 0;
}


void ShapeFusionJsonRpc::SetSubmeshCount(int count, int meshID)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::SetSubmeshCount");
}


void ShapeFusionJsonRpc::GetSubmeshBuffer(void* data, size_t size, enum SubmeshBufferType bufferType, int meshID, int submeshIndex)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::GetSubmeshBuffer");
}


void ShapeFusionJsonRpc::SetSubmeshBuffer(const void* data, size_t size, enum SubmeshBufferType bufferType, int meshID, int submeshIndex)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::SetSubmeshBuffer");
}


bool ShapeFusionJsonRpc::HasSubmeshBuffer(enum SubmeshBufferType bufferType, int meshID, int submeshIndex)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::HasSubmeshBuffer");
	return false;
}


size_t ShapeFusionJsonRpc::GetSubmeshBufferSize(enum SubmeshBufferType bufferType, int meshID, int submeshIndex)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::GetSubmeshBufferSize");
	return 0;
}


int ShapeFusionJsonRpc::GetSubmeshVertexCount(int meshID, int submeshIndex)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::GetSubmeshVertexCount");
	return 0;
}

void ShapeFusionJsonRpc::SetSubmeshVertexCount(int vertexCount, int meshID, int submeshIndex)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::SetSubmeshVertexCount");
}


int ShapeFusionJsonRpc::GetSubmeshTriangleCount(int meshID, int submeshIndex)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::GetSubmeshTriangleCount");
	return 0;
}


void ShapeFusionJsonRpc::SetSubmeshTriangleCount(int triangleCount, int meshID, int submeshIndex)
{
	ThrowException(david::Error_NotImplemented, "Not implemented.", "david::ShapeFusionJsonRpc::SetSubmeshTriangleCount");
}


} // namespace
