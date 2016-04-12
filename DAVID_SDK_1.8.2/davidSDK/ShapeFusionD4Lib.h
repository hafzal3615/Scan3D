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

#ifndef DAVID_SDK_SHAPE_FUSION_D4LIB_H
#define DAVID_SDK_SHAPE_FUSION_D4LIB_H

#include "davidSDK/IShapeFusion.h"
#include "davidSDK/d4lib.h"
#include "davidSDK/D4Ptr.h"
#include <limits>
#include <map>
#include <mutex>

namespace david {


//=============================================================================
// ShapeFusionD4Lib
//=============================================================================

/// Implements interface functions of IShapeFusion using DAVID Low Level Library.
class ShapeFusionD4Lib : public IShapeFusion
{
public:
	//-------------------------------------------------------------------------
	/// @name (De-)Construction
	/// @{
	//-------------------------------------------------------------------------

	/// Constructor.
	ShapeFusionD4Lib();
	
	/// Destructor.
	virtual ~ShapeFusionD4Lib();

	/// @}

	//-------------------------------------------------------------------------
	/// @name Interface functions from IShapeFusion
	/// @{
	//-------------------------------------------------------------------------

	// See IShapeFusion
	virtual void ExportMesh(int meshID, const std::string& filename);

	// See IShapeFusion
	virtual int ImportMesh(const std::string& filename);

	// See IShapeFusion
	virtual int NewMesh();

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
	virtual void GetTextureImageData(void* data, size_t dataSizeInBytes, int meshID, int submeshIndex = 0);

	// See IShapeFusion
	virtual bool GetTextureImageFormat(int& width, int& height, enum PixelFormat& pixelFormat, int meshID, int submeshIndex = 0);

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
	virtual void RemoveVerticesByVirtualCamMask(const std::vector<bool>& mask, int width, int height, double f, const double world_T_cam[16]);

	// See IShapeFusion
	virtual void RemoveSelectedVertices(const std::vector<SubmeshVertexIndex>& selectedVertices, int meshID);

	// See IShapeFusion
	virtual void InvertTriangleOrientation(int meshID);

	// See IShapeFusion
	virtual double AlignPairCoarse(int meshID1, int meshID2, const CoarseAlignParams &params);

	// See IShapeFusion
	virtual void AlignPairFine(int meshID1, int meshID2, const FineAlignParams &params);

	// See IShapeFusion
	virtual void AlignGlobalFine(const GlobalFineAlignParams &params);

	// See IShapeFusion
	virtual int Fuse(int resolution, float holeSizeThresRel=1.f, int sharpness=1);

	// See IShapeFusion
	virtual int GetFuseResolution(double metricResolution, int maxResolution);

	// See IShapeFusion
	virtual int CombineMeshes(const std::vector<int> &meshIDs);

	// See IShapeFusion
	virtual std::vector<int> UncombineMeshes(int meshIDgroup);

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

	//-------------------------------------------------------------------------
	/// @name Special functions
	/// @{
	//-------------------------------------------------------------------------

	/// Add a new mesh to the list of scans.
	/// @param[in,out] mesh Valid handle to mesh. The mesh is stored directly, no copy.
	/// @return ID of the mesh.
	int AddToList(D4Ptr<d4mesh_ptr> mesh);

	/// Translate (move) mesh into direction (dx, dy, dz) with respect to choosen coordinate system.
	/// @param[in] meshID Valid mesh ID.
	/// @param[in] dx Amount of translation in x direction.
	/// @param[in] dy Amount of translation in y direction.
	/// @param[in] dz Amount of translation in z direction.
	/// @param[in] coordinateSystem See #CoordinateSystem.
	void Translate(int meshID, double dx, double dy, double dz, enum CoordinateSystem coordinateSystem);

	/// @}

protected:
	//-------------------------------------------------------------------------
	// Typedefs
	//-------------------------------------------------------------------------

	/// Used mutex.
	typedef std::recursive_mutex  Mutex;
	
	/// Guard for automatic unlocking.
	typedef std::lock_guard<std::recursive_mutex>  Guard;

	//-------------------------------------------------------------------------
	// Private helper functions
	//-------------------------------------------------------------------------

	/// Check mesh ID.
	/// @param[in] meshID ID of the mesh.
	/// @return #d4ok, #d4error_InvalidMeshId
	int CheckMeshID(int meshID);

	/// Get mesh by 'meshID'.
	/// @param[in] meshID Valid ID of the mesh. Use 0 for fusion result.
	/// @return Reference to mesh pointer.
	D4Ptr<d4mesh_ptr>& GetMesh(int meshID);

	/// Helper function which converts vertex indices
	/// from "indices per submesh" (SubmeshVertexIndex)
	/// to "total indices" over all submeshes
	void Helper_AlignPairCoarse_ConvertContactVertices(std::vector<int> &contactVertices, int meshID, const std::vector<SubmeshVertexIndex> &svi);

	//-------------------------------------------------------------------------
	// Private member variables
	//-------------------------------------------------------------------------

	/// Scans are stored with an associated ID.
	typedef std::map<int, D4Ptr<d4mesh_ptr> > Scanlist;

	Scanlist            m_scanlist;         ///< List of scans.
	D4Ptr<d4mesh_ptr>   m_fusionResult;     ///< Mesh for fusion result.
	Mutex               m_mutex;            ///< Used for synchronization
};


} // namespace

#endif // DAVID_SDK_SHAPE_FUSION_D4LIB_H
