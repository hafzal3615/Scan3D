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

#include "davidSDK/ShapeFusionD4Lib.h"
#include "davidSDK/FastVector.h"
#include <algorithm>

namespace david { 


int D4ConvertSubmeshBufferType(enum SubmeshBufferType bufferType)
{
	ErrorExceptionConverter eec("D4ConvertSubmeshBufferType");
	
	switch (bufferType)
	{
	case SubmeshVertexPositionBuffer:
		return d4mesh_VertexPositionBuffer;
	case SubmeshVertexNormalBuffer:
		return d4mesh_VertexNormalBuffer;
	case SubmeshVertexTexCoordBuffer:
		return d4mesh_VertexTexCoordBuffer;
	case SubmeshVertexColorBuffer:
		return d4mesh_VertexTexCoordBuffer;
	case SubmeshTriangleIndexBuffer:
		return d4mesh_TriangleIndexBuffer;
	default:
		eec = Error_InvalidArgument;
	}

	return -1;
}

//=============================================================================
// ShapeFusionD4Lib
//=============================================================================

ShapeFusionD4Lib::ShapeFusionD4Lib()
{
}


ShapeFusionD4Lib::~ShapeFusionD4Lib()
{

}


void ShapeFusionD4Lib::ExportMesh(int meshID, const std::string& filename)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::ExportMesh");
	eec = CheckMeshID(meshID);
	eec = d4mesh_export(GetMesh(meshID), filename.c_str());
}


int ShapeFusionD4Lib::ImportMesh(const std::string& filename)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::ImportMesh");
	D4Ptr<d4mesh_ptr> mesh(d4mesh_newMesh());
	eec = d4mesh_import(mesh, filename.c_str());
	return AddToList(mesh);
}


void ShapeFusionD4Lib::DeleteAllMeshes()
{
	Guard guard(m_mutex);
	m_scanlist.clear();
}


int ShapeFusionD4Lib::NewMesh()
{
	Guard guard(m_mutex);
	D4Ptr<d4mesh_ptr> mesh(d4mesh_newMesh());
	return AddToList(mesh);
}


int ShapeFusionD4Lib::DuplicateMesh(int sourceMeshID)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::DuplicateMesh");

	eec = CheckMeshID(sourceMeshID);
	D4Ptr<d4mesh_ptr> sourceMesh = GetMesh(sourceMeshID);
	D4Ptr<d4mesh_ptr> targetMesh(d4mesh_newMesh());
	eec = d4mesh_copy(targetMesh, sourceMesh);
	return AddToList(targetMesh);
}


void ShapeFusionD4Lib::DeleteMesh(int meshID)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::DeleteMesh");
	eec = CheckMeshID(meshID);
	m_scanlist.erase(meshID);
}


void ShapeFusionD4Lib::GetVertexPositions(std::vector<float>& positions, int meshID, enum CoordinateSystem coordinateSystem)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetVertexPositions");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);
	int vertexCount = 0;
	eec = d4mesh_getVertexCount(mesh, &vertexCount, -1);
	positions.resize(vertexCount*3); // three floats per vertex
	if (positions.size() > 0)
	{
		eec = d4mesh_getBuffer(mesh, &positions[0], positions.size()*sizeof(float), d4mesh_VertexPositionBuffer, -1);

		// Transform into world coordinates.
		if (coordinateSystem == WorldCoordinates)
		{
			double world_T_local[16];
			eec = d4mesh_getPose(mesh, world_T_local);
			for (int i=0; i < vertexCount; ++i)
			{
				FxV(world_T_local, &positions[i*3], 1.0);
			}
		}
	}
}


void ShapeFusionD4Lib::GetVertexQualities(std::vector<float>& qualities, int meshID)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetVertexQualities");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);
	int vertexCount = 0;
	eec = d4mesh_getVertexCount(mesh, &vertexCount, -1);
	qualities.resize(vertexCount); // one float per vertex
	if (qualities.size() > 0)
	{
		eec = d4mesh_getBuffer(mesh, &qualities[0], qualities.size()*sizeof(float), d4mesh_VertexQualityBuffer, -1);
	}
}


void ShapeFusionD4Lib::GetVertexNormals(std::vector<float>& normals, int meshID, enum CoordinateSystem coordinateSystem)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetVertexNormals");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);
	int vertexCount = 0;
	eec = d4mesh_getVertexCount(mesh, &vertexCount, -1);
	normals.resize(vertexCount*3); // three floats per vertex
	if (normals.size() > 0)
	{
		eec = d4mesh_getBuffer(mesh, &normals[0], normals.size()*sizeof(float), d4mesh_VertexNormalBuffer, -1);

		// Transform into world coordinates.
		if (coordinateSystem == WorldCoordinates)
		{
			double world_T_local[16];
			eec = d4mesh_getPose(mesh, world_T_local);
			for (int i=0; i < vertexCount; ++i)
			{
				FxV(world_T_local, &normals[i*3], 0.0);
			}
		}
	}
}


void ShapeFusionD4Lib::GetVertexTexCoords(std::vector<float>& texCoords, int meshID)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetVertexTexCoords");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);
	int vertexCount = 0;
	eec = d4mesh_getVertexCount(mesh, &vertexCount, -1);
	texCoords.resize(vertexCount*2); // two floats per vertex
	if (texCoords.size() > 0)
	{
		eec = d4mesh_getBuffer(mesh, &texCoords[0], texCoords.size()*sizeof(float), d4mesh_VertexTexCoordBuffer, -1);
	}
}


void ShapeFusionD4Lib::GetTriangles(std::vector<int>& triangles, int meshID)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetTriangles");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);
	int triangleCount = 0;
	eec = d4mesh_getTriangleCount(mesh, &triangleCount, -1);
	triangles.resize(triangleCount*3); // three indices per triangle
	if (triangles.size() > 0)
	{
		eec = d4mesh_getBuffer(mesh, &triangles[0], triangles.size()*sizeof(int), d4mesh_TriangleIndexBuffer, -1);
	}
}


void ShapeFusionD4Lib::GetTextureImageData(void* data, size_t dataSizeInBytes, int meshID, int submeshIndex)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetTextureImageData");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);
	D4Ptr<d4img_ptr> texture(d4mesh_getTexture(mesh, submeshIndex), false);
	if (texture)
	{
		int width, height;
		eec = d4img_getImageFormat(texture, &width, &height, NULL);
		if (width > 0 && height > 0)
		{
			size_t expectedSize = width * height * 3;
			if (expectedSize != dataSizeInBytes) eec = d4error_InvalidArgument;
			eec = d4img_getImageData(texture, data, dataSizeInBytes, d4img_RGB24);
		}
	}
}


bool ShapeFusionD4Lib::GetTextureImageFormat(int& width, int& height, enum PixelFormat& pixelFormat, int meshID, int submeshIndex)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetTextureImageFormat");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);
	D4Ptr<d4img_ptr> texture(d4mesh_getTexture(mesh, submeshIndex), false);
	if (texture)
	{
		int _pixelFormat;
		eec = d4img_getImageFormat(texture, &width, &height, &_pixelFormat);
		pixelFormat = (enum PixelFormat) _pixelFormat;
		return true;
	}
	else return false;
}


void ShapeFusionD4Lib::SetTexture(const void* data, size_t dataSizeInBytes, int width, int height, enum PixelFormat pixelFormat, int meshID, int submeshIndex)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::SetTexture");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	// Remove old texture.
	eec = d4mesh_setTexture(mesh, NULL, submeshIndex);
	
	// New texture?
	if (data == NULL) return;

	// Check data size.
	if (dataSizeInBytes != GetImageSize(width, height, pixelFormat)) 
	{
		eec = Error_InvalidArgument;
		return;
	}

	// Create new texture and set it.
	D4Ptr<d4img_ptr> texture(d4img_newImage());
	eec = d4img_setImageFormat(texture, width, height, int(pixelFormat));
	eec = d4img_setImageData(texture, data, dataSizeInBytes, int(pixelFormat));
	eec = d4mesh_setTexture(mesh, texture, submeshIndex);
}


void ShapeFusionD4Lib::GetPose(double world_T_local[16], int meshID)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetPose");
	eec = CheckMeshID(meshID);
	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);
	eec = d4mesh_getPose(mesh, world_T_local);
}


void ShapeFusionD4Lib::SetPose(const double world_T_local[16], int meshID)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::SetPose");
	eec = CheckMeshID(meshID);
	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);
	eec = d4mesh_setPose(mesh, world_T_local);
}


void ShapeFusionD4Lib::Rotate(int meshID, enum TransformType transformType, double degrees)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::Rotate");
	eec = CheckMeshID(meshID);
	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);
	const double rad = degrees / 180.0 * PI;

	// Get current transform.
	double world_T_local[16];
	eec = d4mesh_getPose(mesh, world_T_local);

	// Setup transform.
	bool local = false;
	double transform[16];
	switch (transformType)
	{
	case TransformGlobalX:
		Frotx(transform, rad);
		break;
	case TransformGlobalY:
		Froty(transform, rad);
		break;
	case TransformGlobalZ:
		Frotz(transform, rad);
		break;
	case TransformMeshX:
		Frotx(transform, rad);
		local = true;
		break;
	case TransformMeshY:
		Froty(transform, rad);
		local = true;
		break;
	case TransformMeshZ:
		Frotz(transform, rad);
		local = true;
		break;
	default:
		Fidentity(transform);
	}

	// Transform.
	double frame[16];
	if (local) FxF(frame, world_T_local, transform);
	else       FxF(frame, transform, world_T_local);
	
	// Write back.
	eec = d4mesh_setPose(mesh, frame);
}


void ShapeFusionD4Lib::Translate(int meshID, enum TransformType transformType, double amount)
{
	Guard guard(m_mutex);
	switch (transformType)
	{
	case TransformGlobalX: return Translate(meshID, amount, 0.0, 0.0, WorldCoordinates);
	case TransformGlobalY: return Translate(meshID, 0.0, amount, 0.0, WorldCoordinates);
	case TransformGlobalZ: return Translate(meshID, 0.0, 0.0, amount, WorldCoordinates);
	case TransformMeshX:   return Translate(meshID, amount, 0.0, 0.0, LocalCoordinates);
	case TransformMeshY:   return Translate(meshID, 0.0, amount, 0.0, LocalCoordinates);
	case TransformMeshZ:   return Translate(meshID, 0.0, 0.0, amount, LocalCoordinates);
	default:;
	}
}


void ShapeFusionD4Lib::ReduceMeshDensity(int meshID, float factor)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::ReduceMeshDensity");
	eec = CheckMeshID(meshID);
	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);
	eec = d4mesh_reduceMeshDensity(mesh, factor);
}


void ShapeFusionD4Lib::RemoveVerticesByVirtualCamMask(const std::vector<bool>& mask, int width, int height, double f, const double world_T_cam[16])
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::RemoveVerticesByVirtualCamMask");
	// Anything to do?
	if (m_scanlist.empty()) return;
	
	eec("get raw handles");
	std::vector<d4mesh_ptr> meshes;
	for (Scanlist::iterator it = m_scanlist.begin(); it != m_scanlist.end(); ++it)
	{
		d4mesh_ptr mesh = it->second;
		meshes.push_back(mesh);
	}
	if (meshes.size() == 0) return;

	eec("remove vertices");
	std::vector<uint8_t> _mask(mask.begin(), mask.end());
	eec = d4mesh_removeVerticesByVirtualCamMask(meshes.data(), (int) meshes.size(), _mask.data(), width, height, f, world_T_cam, false);
}


void ShapeFusionD4Lib::RemoveSelectedVertices(const std::vector<SubmeshVertexIndex>& selectedVertices, int meshID)
{
	if (selectedVertices.size()<1) return;  // nothing to do

	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::RemoveSelectedVertices");
	eec = CheckMeshID(meshID);
	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	std::vector<d4mesh_SubmeshVertexIndex> _selectedVertices(selectedVertices.size());
	for (size_t i=0; i < selectedVertices.size(); ++i)
	{
		_selectedVertices[i].submeshIndex = selectedVertices[i].submeshIndex;
		_selectedVertices[i].vertexIndex = selectedVertices[i].vertexIndex;
	}
	eec = d4mesh_removeSelectedVertices(mesh, _selectedVertices.data(), (int)_selectedVertices.size());
}


void ShapeFusionD4Lib::InvertTriangleOrientation(int meshID)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::InvertTriangleOrientation");
	eec = CheckMeshID(meshID);
	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);
	eec = d4mesh_invertTriangleOrientation(mesh);
}


// Helper function which converts vertex indices
// from "indices per submesh" (#SubmeshVertexIndex)
// to "total indices" over all submeshes
void ShapeFusionD4Lib::Helper_AlignPairCoarse_ConvertContactVertices(std::vector<int> &totalVertices, int meshID, const std::vector<SubmeshVertexIndex> &submeshVertices)
{
	// collect mesh info (submeshCount and vertex count for each submesh):
	int submeshCount = this->GetSubmeshCount(meshID);
	std::vector<int> submeshVertexCounts(submeshCount);
	for (int i=0; i<submeshCount; ++i) submeshVertexCounts[i] = GetSubmeshVertexCount(meshID, i);

	// compute index offsets for conversion, one offset value for each submesh:
	std::vector<int> offsets(submeshCount);
	int offset=0;
	for (int iSubm=0; iSubm<submeshCount; ++iSubm)
	{
		offsets[iSubm] = offset;
		offset += submeshVertexCounts[iSubm];
	}

	// copy/convert vertex indices:
	totalVertices.resize(submeshVertices.size());
	for (size_t i=0; i<submeshVertices.size(); ++i)  // for each vertex
	{
		// Validate values:
		const david::SubmeshVertexIndex& svi = submeshVertices[i];
		if (svi.submeshIndex<0) ThrowException(david::Error_InvalidArgument, "Invalid submeshIndex < 0 in ContactRegionPair", "ShapeFusionD4Lib::Helper_AlignPairCoarse_ConvertContactVertices");
		if (svi.vertexIndex<0)  ThrowException(david::Error_InvalidArgument, "Invalid vertexIndex < 0 in ContactRegionPair", "ShapeFusionD4Lib::Helper_AlignPairCoarse_ConvertContactVertices");
		if (svi.submeshIndex>=submeshCount)                         ThrowException(david::Error_InconsistentBuffer, "Invalid submeshIndex in ContactRegionPair", "ShapeFusionD4Lib::Helper_AlignPairCoarse_ConvertContactVertices");
		if (svi.vertexIndex>=submeshVertexCounts[svi.submeshIndex]) ThrowException(david::Error_InconsistentBuffer, "Invalid vertexIndex in ContactRegionPair", "ShapeFusionD4Lib::Helper_AlignPairCoarse_ConvertContactVertices");

		// Convert using offsets:
		totalVertices[i] = offsets[svi.submeshIndex] + svi.vertexIndex;
	}
}


double ShapeFusionD4Lib::AlignPairCoarse(int meshID1, int meshID2, const CoarseAlignParams &params)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::AlignPairCoarse");
	eec = CheckMeshID(meshID1);
	eec = CheckMeshID(meshID2);
	
	//------------------------------------------------------------
	// Convert from CoarseAlignParams to d4mesh_CoarseAlignParams
	//------------------------------------------------------------
	d4mesh_CoarseAlignParams myParams;

	myParams.texturePercentage = params.texturePercentage;
	myParams.qualityFactor = params.qualityFactor;

	// MotionInfo:
	d4mesh_MotionInfo mi;
	mi.angleDeg          = params.motionInfo.angleDeg;
	mi.angleKnown        = params.motionInfo.angleKnown;
	mi.angleToleranceDeg = params.motionInfo.angleToleranceDeg;
	VcV(mi.axisDir,        params.motionInfo.axisDir);
	mi.axisDirKnown      = params.motionInfo.axisDirKnown;
	myParams.motionInfo = &mi;

	// Contact Pairs:
	// Convert contact regions from "indices per submesh" to "total indices" over all submeshes:
	std::vector<int> contactVertices1(params.contactRegionPair.vertexIndices1.size());
	std::vector<int> contactVertices2(params.contactRegionPair.vertexIndices2.size());
	this->Helper_AlignPairCoarse_ConvertContactVertices(contactVertices1, meshID1, params.contactRegionPair.vertexIndices1);
	this->Helper_AlignPairCoarse_ConvertContactVertices(contactVertices2, meshID2, params.contactRegionPair.vertexIndices2);
	myParams.numContactVertices1 = contactVertices1.size();
	myParams.contactVertices1    = contactVertices1.data();
	myParams.numContactVertices2 = contactVertices2.size();
	myParams.contactVertices2    = contactVertices2.data();

	// Blacklisting:
	if (params.blacklist1.poses.size() % 16 != 0) ThrowException(david::Error_RpcInvalidParams, "Number of doubles in params.blacklist1.poses must be multiple of 16 - expecting 4*4 matrices", "ShapeFusionD4Lib::AlignPairCoarse");
	myParams.blacklist1.numPoses       = (int)params.blacklist1.poses.size()/16;
	myParams.blacklist1.poses          = (double*)params.blacklist1.poses.data();
	myParams.blacklist1.toleranceTrans = params.blacklist1.toleranceTrans;
	myParams.blacklist1.toleranceDeg   = params.blacklist1.toleranceDeg;

	// Whitelist:
	if (params.whitelist1.poses.size() % 16 != 0) ThrowException(david::Error_RpcInvalidParams, "Number of doubles in params.whitelist1.poses must be multiple of 16 - expecting 4*4 matrices", "ShapeFusionD4Lib::AlignPairCoarse");
	myParams.whitelist1.numPoses       = (int)params.whitelist1.poses.size()/16;
	myParams.whitelist1.poses          = (double*)params.whitelist1.poses.data();
	myParams.whitelist1.toleranceTrans = params.whitelist1.toleranceTrans;
	myParams.whitelist1.toleranceDeg   = params.whitelist1.toleranceDeg;

	// Mesh IDs:
	D4Ptr<d4mesh_ptr>& mesh1 = GetMesh(meshID1);
	D4Ptr<d4mesh_ptr>& mesh2 = GetMesh(meshID2);

	double resQuality=-1.0;
	eec = d4mesh_alignPairCoarse(mesh1, mesh2, &resQuality, &myParams);

	return resQuality;
}


d4mesh_FineAlignParams Helper_TransferFineRegParams(const FineAlignParams &params)
{
	d4mesh_FineAlignParams res;
	res.texturePercentage = params.texturePercentage;
	res.maxNumIterations  = params.maxNumIterations;
	res.maxTranslation    = params.maxTranslation;
	res.maxRot_deg        = params.maxRot_deg;
	return res;
}


d4mesh_GlobalFineAlignParams Helper_TransferGlobalFineRegParams(const GlobalFineAlignParams &params)
{
	d4mesh_GlobalFineAlignParams res;
	res.texturePercentage = params.texturePercentage;
	res.numIterations     = params.numIterations;
	res.maxTranslation    = params.maxTranslation;
	res.maxRot_deg        = params.maxRot_deg;
	return res;
}


void ShapeFusionD4Lib::AlignPairFine(int meshID1, int meshID2, const FineAlignParams &params)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::AlignPairFine");
	eec = CheckMeshID(meshID1);
	eec = CheckMeshID(meshID2);
	D4Ptr<d4mesh_ptr>& mesh1 = GetMesh(meshID1);
	D4Ptr<d4mesh_ptr>& mesh2 = GetMesh(meshID2);

	d4mesh_FineAlignParams _params = Helper_TransferFineRegParams(params);
	eec = d4mesh_alignPairFine(mesh1, mesh2, NULL, &_params);
}


void ShapeFusionD4Lib::AlignGlobalFine(const GlobalFineAlignParams &params)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::AlignGlobalFine");
	
	if (m_scanlist.empty()) return;

	std::vector<d4mesh_ptr> meshes;
	for (Scanlist::iterator it = m_scanlist.begin(); it != m_scanlist.end(); ++it)
	{
		meshes.push_back(it->second);
	}

	d4mesh_GlobalFineAlignParams _params = Helper_TransferGlobalFineRegParams(params);
	eec = d4mesh_alignGlobalFine(&meshes[0], (int) meshes.size(), &_params);
}


int ShapeFusionD4Lib::Fuse(int resolution, float holeSizeThresRel, int sharpness)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::Fuse");
	
	// Anything to do?
	if (m_scanlist.empty()) return d4ok;

	eec("get raw handles");
	std::vector<d4mesh_ptr> meshes;
	bool textureAvailable = false;
	for (Scanlist::iterator it = m_scanlist.begin(); it != m_scanlist.end(); ++it)
	{
		d4mesh_ptr mesh = it->second;
		meshes.push_back(mesh);
		bool hasTexture = false;
		eec = d4mesh_hasTexture(mesh, &hasTexture, 0);
		if (hasTexture) textureAvailable = true;
	}

	eec("fuse geometry");
	m_fusionResult = D4Ptr<d4mesh_ptr>(d4mesh_newMesh());
	eec = d4mesh_fuseGeometry(m_fusionResult, &meshes[0], (int) meshes.size(), resolution, holeSizeThresRel, sharpness);

	eec("fuse texture");
	if (textureAvailable)
	{
		eec = d4mesh_fuseTexture(m_fusionResult, &meshes[0], (int) meshes.size());
	}
	
	if (m_fusionResult) return 0;
	else return -1;
}


int ShapeFusionD4Lib::GetFuseResolution(double metricResolution, int maxResolution)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetFuseResolution");

	std::vector<d4mesh_ptr> meshes;
	for (Scanlist::iterator it = m_scanlist.begin(); it != m_scanlist.end(); ++it)
	{
		d4mesh_ptr mesh = it->second;
		meshes.push_back(mesh);
	}

	if (meshes.empty()) return maxResolution;

	int resolution = maxResolution;
	eec = d4mesh_getFuseGeometryResolution(&meshes[0], (int) meshes.size(), &resolution, metricResolution);

	return std::min(resolution, maxResolution);
}


int ShapeFusionD4Lib::CombineMeshes(const std::vector<int> &meshIDs)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::CombineMeshes");

	if (meshIDs.size() < 1) eec = Error_InvalidArgument;

	// Get mesh pointers.
	std::vector<d4mesh_ptr> meshes;
	for (size_t i=0; i < meshIDs.size(); ++i)
	{
		Scanlist::iterator it = m_scanlist.find(meshIDs[i]);
		if (it == m_scanlist.end()) eec = Error_InvalidMeshId;
		meshes.push_back(it->second);
	}

	// Combine.
	D4Ptr<d4mesh_ptr> mesh(d4mesh_newMesh());
	eec = d4mesh_combine(mesh, &meshes[0], int(meshes.size()));

	// Remove old meshes.
	for (size_t i=0; i < meshIDs.size(); ++i)
	{
		Scanlist::iterator it = m_scanlist.find(meshIDs[i]);
		m_scanlist.erase(it);
	}

	// Add combined mesh.
	return this->AddToList(mesh);
}


std::vector<int> ShapeFusionD4Lib::UncombineMeshes(int meshIDgroup)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::UncombineMeshes");

	// Get iterator to mesh in scanlist
	Scanlist::iterator itMesh = m_scanlist.find(meshIDgroup);
	if (itMesh == m_scanlist.end()) eec = Error_InvalidMeshId;

	// Get combine count.
	int combineCount = 0;
	eec = d4mesh_getSubmeshCount(itMesh->second, &combineCount);

	// Returns 'meshIDgroup' if there are no meshes combined.
	if (combineCount <= 1) return std::vector<int>(1, meshIDgroup);

	// Create new meshes for uncombine.
	std::vector< D4Ptr<d4mesh_ptr> > meshes(combineCount);
	for (int i=0; i < combineCount; ++i)
	{
		meshes[i] = D4Ptr<d4mesh_ptr>(d4mesh_newMesh());
	}

	// Uncombine.
	{
		std::vector<d4mesh_ptr> nativeMeshPtrs(combineCount);
		for (int i=0; i < combineCount; ++i)
		{
			nativeMeshPtrs[i] = meshes[i];
		}
		eec = d4mesh_uncombine(itMesh->second, &nativeMeshPtrs[0], combineCount);
	}

	// Add to scan list.
	std::vector<int> meshIDs;
	for (int i=0; i < combineCount; ++i)
	{
		int meshID = AddToList(meshes[i]);
		meshIDs.push_back(meshID);
	}

	// Remove old mesh.
	m_scanlist.erase(itMesh);

	return meshIDs;
}


int ShapeFusionD4Lib::GetSubmeshCount(int meshID)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetSubmeshCount");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	int count = 0;
	eec = d4mesh_getSubmeshCount(mesh, &count);
	return count;
}


void ShapeFusionD4Lib::SetSubmeshCount(int count, int meshID)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetSubmeshCount");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	eec = d4mesh_setSubmeshCount(mesh, count);
}


void ShapeFusionD4Lib::GetSubmeshBuffer(void* data, size_t size, enum SubmeshBufferType bufferType, int meshID, int submeshIndex)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetSubmeshBuffer");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	eec = d4mesh_getBuffer(mesh, data, size, D4ConvertSubmeshBufferType(bufferType), submeshIndex);
}


void ShapeFusionD4Lib::SetSubmeshBuffer(const void* data, size_t size, enum SubmeshBufferType bufferType, int meshID, int submeshIndex)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::SetSubmeshBuffer");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	eec = d4mesh_setBuffer(mesh, data, size, D4ConvertSubmeshBufferType(bufferType), submeshIndex);
}


bool ShapeFusionD4Lib::HasSubmeshBuffer(enum SubmeshBufferType bufferType, int meshID, int submeshIndex)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::HasSubmeshBuffer");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	bool hasBuffer = false;
	eec = d4mesh_hasBuffer(mesh, &hasBuffer, D4ConvertSubmeshBufferType(bufferType), submeshIndex);
	return hasBuffer;
}


size_t ShapeFusionD4Lib::GetSubmeshBufferSize(enum SubmeshBufferType bufferType, int meshID, int submeshIndex)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetSubmeshBufferSize");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	size_t size = 0;
	eec = d4mesh_getBufferSize(mesh, &size, D4ConvertSubmeshBufferType(bufferType), submeshIndex);
	return size;
}


int ShapeFusionD4Lib::GetSubmeshVertexCount(int meshID, int submeshIndex)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetSubmeshVertexCount");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	int count = 0;
	eec = d4mesh_getVertexCount(mesh, &count, submeshIndex);
	return count;
}


void ShapeFusionD4Lib::SetSubmeshVertexCount(int vertexCount, int meshID, int submeshIndex)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::SetSubmeshVertexCount");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	eec = d4mesh_setVertexCount(mesh, vertexCount, submeshIndex);
}


int ShapeFusionD4Lib::GetSubmeshTriangleCount(int meshID, int submeshIndex)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetSubmeshTriangleCount");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	int count = 0;
	eec = d4mesh_getTriangleCount(mesh, &count, submeshIndex);
	return count;
}


void ShapeFusionD4Lib::SetSubmeshTriangleCount(int triangleCount, int meshID, int submeshIndex)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::SetSubmeshTriangleCount");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	eec = d4mesh_setTriangleCount(mesh, triangleCount, submeshIndex);
}


int ShapeFusionD4Lib::AddToList(D4Ptr<d4mesh_ptr> mesh)
{
	Guard guard(m_mutex);
	for (int i=1; i < std::numeric_limits<int>::max(); ++i)
	{
		if (m_scanlist.find(i) == m_scanlist.end())
		{
			Scanlist::value_type element(i, mesh);
			m_scanlist.insert(element);
			return i;
		}
	}
	return -1;
}


void ShapeFusionD4Lib::Translate(int meshID, double dx, double dy, double dz, enum CoordinateSystem coordinateSystem)
{
	Guard guard(m_mutex);
	ErrorExceptionConverter eec("ShapeFusionD4Lib::Translate");
	eec = CheckMeshID(meshID);

	D4Ptr<d4mesh_ptr>& mesh = GetMesh(meshID);

	double translation[3] = {dx, dy, dz};

	// Get current transform.
	double world_T_local[16];
	eec = d4mesh_getPose(mesh, world_T_local);

	// Translate
	double* position = world_T_local+12;
	if (coordinateSystem == WorldCoordinates)
	{
		VpV(position, translation);
	}
	else
	{
		double translation_world[3];
		FxV(translation_world, world_T_local, translation, 1.0);
		VpV(position, translation_world);
	}

	// Write back.
	eec = d4mesh_setPose(mesh, world_T_local);
}


int ShapeFusionD4Lib::CheckMeshID(int meshID)
{
	if (meshID != 0)
	{
		if (m_scanlist.find(meshID) != m_scanlist.end()) return d4ok;
		else return d4error_InvalidMeshId;
	}
	else
	{
		if (m_fusionResult) return d4ok;
		else return d4error_InvalidMeshId;
	}
}


D4Ptr<d4mesh_ptr>& ShapeFusionD4Lib::GetMesh(int meshID)
{
	ErrorExceptionConverter eec("ShapeFusionD4Lib::GetMesh");
	eec = CheckMeshID(meshID);
	if (meshID == 0) return m_fusionResult;
	else return m_scanlist[meshID];
}


} // namespace
