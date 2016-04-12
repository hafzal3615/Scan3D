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

#ifndef DAVID_SDK_SHAPE_FUSION_JSON_RPC_H
#define DAVID_SDK_SHAPE_FUSION_JSON_RPC_H

#include "davidSDK/ClientJsonRpc.h"
#include <limits>

namespace david {


//=============================================================================
// ShapeFusionJsonRpc
//=============================================================================

/// Implements interface functions of IShapeFusion.
/// All calls are delegated to a JSON-RPC 2.0 client.
/// Example:
/// @include SimpleShapeFusion.cpp
class ShapeFusionJsonRpc : public IShapeFusion
{
public:
	/// Constructor.
	/// @param[in,out] client Valid pointer to client implementation.
	ShapeFusionJsonRpc(ClientJsonRpc* client);
	
	/// Destructor.
	virtual ~ShapeFusionJsonRpc();

	// See IShapeFusion
	virtual void ExportMesh(int meshID, const std::string& filename);

	// See IShapeFusion
	virtual int ImportMesh(const std::string& filename);

	// See IShapeFusion
	virtual int DuplicateMesh(int sourceMeshID);

	// See IShapeFusion
	virtual void DeleteAllMeshes();

	// See IShapeFusion
	virtual void DeleteMesh(int meshID);

	// See IShapeFusion
	virtual void GetVertexPositions(std::vector<float>& positions, int meshID, enum CoordinateSystem coordinateSystem = WorldCoordinates);

	// See IShapeFusion
	virtual void GetVertexQualities(std::vector<float>& qualities, int meshID);

	// See IShapeFusion
	virtual void GetVertexNormals(std::vector<float>& normals, int meshID, enum CoordinateSystem coordinateSystem = WorldCoordinates);

	// See IShapeFusion
	virtual void GetVertexTexCoords(std::vector<float>& texCoords, int meshID);

	// See IShapeFusion
	virtual void GetTriangles(std::vector<int>& triangles, int meshID);

	// See IShapeFusion
	virtual void GetTextureImageData(void* data, size_t dataSizeInBytes, int meshID, int submeshIndex=0);

	// See IShapeFusion
	virtual bool GetTextureImageFormat(int& width, int& height, enum PixelFormat& pixelFormat, int meshID, int submeshIndex=0);

	// See IShapeFusion
	virtual void SetTexture(const void* data, size_t dataSizeInBytes, int width, int height, enum PixelFormat pixelFormat, int meshID, int submeshIndex = 0);

	// See IShapeFusion
	virtual void GetPose(double world_T_local[16], int meshID);

	// See IShapeFusion
	virtual void SetPose(const double world_T_local[16], int meshID);

	// See IShapeFusion
	virtual void Rotate(int meshID, enum TransformType transformType, double degrees);

	// See IShapeFusion
	virtual void Translate(int meshID, enum TransformType transformType, double amount);

	// See IShapeFusion
	virtual void ReduceMeshDensity(int meshID, float factor);

	// See IShapeFusion
	virtual double AlignPairCoarse(int meshID1, int meshID2, const CoarseAlignParams &params);

	// See IShapeFusion
	virtual void AlignPairFine(int meshID1, int meshID2, const FineAlignParams &params);

	// See IShapeFusion
	virtual void AlignGlobalFine(const GlobalFineAlignParams &params);

	// See IShapeFusion
	virtual int Fuse(int resolution, float holeSizeThresRel=1.f, int sharpness=1);

	// See IShapeFusion
	virtual int CombineMeshes(const std::vector<int> &meshIDs);

	// See IShapeFusion
	virtual std::vector<int> UncombineMeshes(int meshIDgroup);

	//-------------------------------------------------------------------------
	/// @name Not implemented
	/// @warning The following functions are not implemented!
	/// @{
	//-------------------------------------------------------------------------

	// See IShapeFusion
	virtual void RemoveVerticesByVirtualCamMask(const std::vector<bool>& mask, int width, int height, double f, const double world_T_cam[16]);

	// See IShapeFusion
	virtual void RemoveSelectedVertices(const std::vector<SubmeshVertexIndex>& selectedVertices, int meshID);

	// See IShapeFusion
	virtual void InvertTriangleOrientation(int meshID);

	// See IShapeFusion
	virtual int GetFuseResolution(double metricResolution, int maxResolution);

	// See IShapeFusion
	virtual int NewMesh();

	// See IShapeFusion
	virtual int GetSubmeshCount(int meshID);

	// See IShapeFusion
	virtual void SetSubmeshCount(int count, int meshID);

	// See IShapeFusion
	virtual void GetSubmeshBuffer(void* data, size_t size, enum SubmeshBufferType bufferType, int meshID, int submeshIndex);

	// See IShapeFusion
	virtual void SetSubmeshBuffer(const void* data, size_t size, enum SubmeshBufferType bufferType, int meshID, int submeshIndex);

	// See IShapeFusion
	virtual bool HasSubmeshBuffer(enum SubmeshBufferType bufferType, int meshID, int submeshIndex);

	// See IShapeFusion
	virtual size_t GetSubmeshBufferSize(enum SubmeshBufferType bufferType, int meshID, int submeshIndex);

	// See IShapeFusion
	virtual int GetSubmeshVertexCount(int meshID, int submeshIndex);

	// See IShapeFusion
	virtual void SetSubmeshVertexCount(int vertexCount, int meshID, int submeshIndex);

	// See IShapeFusion
	virtual int GetSubmeshTriangleCount(int meshID, int submeshIndex);

	// See IShapeFusion
	virtual void SetSubmeshTriangleCount(int triangleCount, int meshID, int submeshIndex);

	/// @}

private:
	//-------------------------------------------------------------------------
	// Private member variables
	//-------------------------------------------------------------------------

	ClientJsonRpc*     m_client;     ///< Client implementation
};


} // namespace

#endif // DAVID_SDK_SHAPE_FUSION_JSON_RPC_H
