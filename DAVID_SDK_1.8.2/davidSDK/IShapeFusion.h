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

#ifndef DAVID_SDK_I_SHAPE_FUSION_H
#define DAVID_SDK_I_SHAPE_FUSION_H

#include "davidSDK/Common.h"
#include <vector>
#include <limits>

namespace david {

//*****************************************************************************
/// @addtogroup InterfaceGroup
/// @{
/// @defgroup ShapeFusionGroup Shape Fusion
/// Provides functions and types related 'Shape Fusion'.
/// @{
//*****************************************************************************

//=============================================================================
// TransformType
//=============================================================================

/// Defines transform types used by ShapeFusion::Translate and ShapeFusion::Rotate.
enum TransformType
{
	TransformGlobalX = 0,   ///< Transform around or along global (world) x axis.
	TransformGlobalY,       ///< Transform around or along global (world) y axis.
	TransformGlobalZ,       ///< Transform around or along global (world) z axis.
	TransformMeshX,         ///< Transform around or along mesh's x axis.
	TransformMeshY,         ///< Transform around or along mesh's y axis.
	TransformMeshZ,         ///< Transform around or along mesh's z axis.
};


//=============================================================================
// CoordinateSystem
//=============================================================================

/// Selection of coordinate system.
enum CoordinateSystem
{
	WorldCoordinates = 0,  ///< Coordinates are given with respect to world (global coordinates).
	LocalCoordinates,      ///< Coordinates are given with respect to local object coordinate system.
};


//=============================================================================
// SubmeshBufferType
//=============================================================================

/// Buffer selection.
enum SubmeshBufferType
{
	/// The vertex position buffer stores the vertex positions as an array of tuples.
	/// Each vertex position is described by a tuple (x,y,z) of three 32bit floats .
	/// This buffer is always present.
	/// @note Coordinates are always with respect to the local coordinate system of the mesh.
	SubmeshVertexPositionBuffer,

	/// The vertex normal buffer stores the vertex normals as an array of tuples.
	/// Each vertex normal is described by a tuple (nx,ny,nz) of three 32bit floats.
	/// This buffer is always present.
	/// @note Coordinates are always with respect to the local coordinate system of the mesh.
	SubmeshVertexNormalBuffer,
	
	/// The texture coordinates buffer stores texture coordinates as an array of tuples.
	/// Each vertex texture coordinate is described a tuple (u,v) of two 32bit floats.
	/// This buffer is only optional.
	/// @note Coordinates should be in range [0,1].
	SubmeshVertexTexCoordBuffer,

	/// The vertex color buffer stores vertex colors as an array of tuples.
	/// Each vertex texture coordinate is described a RGBA tuple (red, green, blue, alpha) of four 8bit unsigned chars.
	/// This buffer is only optional.
	/// @note Coordinates should be in range [0,1].
	SubmeshVertexColorBuffer,

	/// The triangle index buffer stores triangles as an array of vertex indices.
	/// Each triangle is described by tuple (v0, v1, v2) of three 32bit integer vertex indices that are zero based.
	/// This buffer is always present.
	SubmeshTriangleIndexBuffer,

	/// The vertex quality buffer stores a quality value for each vertex.
	/// This buffer is only optional.
	/// @note Quality values should be in range [0,1].
	SubmeshVertexQualityBuffer
};


//=============================================================================
/// @name Classes and functions related to MotionInfo.
/// @{
//=============================================================================

/// Provides user knowledge about the motion between two scans.
/// In most cases the motion can be described by a simple rotation around an axis, 
/// where the pitch along the (motion=rotation) axis is zero.
/// The following helper functions are available:
/// - david::GetMotionInfo_Free
/// - david::GetMotionInfo_OnTable
/// - david::GetMotionInfo_AngleOnly
/// - david::GetMotionInfo_TurntablePartlyCalibrated
struct MotionInfo
{
	bool     angleKnown;      ///< Is rotation 'angle' around motion axis known?.
	bool     axisDirKnown;    ///< Is direction vector 'axisDir' of motion axis is known?
	double   angleDeg;        ///< Angle of rotation in degrees.
	double   angleToleranceDeg; ///< Tolerance of angleDeg in degrees. Won't work well with too low tolerances; recommended: 2 deg.
	double   axisDir[3];      ///< 3D direction vector (x,y,z) of motion axis w.r.t. world coordinates.

	/// Standard constructor. Initializes with #GetMotionInfo_Free.
	MotionInfo();
};


/// Nothing is known. All parameters are set to unknown.
/// @return MotionInfo
MotionInfo GetMotionInfo_Free();

/// Object was moved (translated and rotated) on a planar table without lifting it.
/// @param[in] axisDir 3D vector w.r.t. to world coordinates describing a vector orthogonal to the table plane.
/// @return MotionInfo
MotionInfo GetMotionInfo_OnTable(const double axisDir[3]);

/// Only rotation angle is known. Axis is unknown.
/// @param[in] angleDeg Angle in degrees.
/// @param[in] angleToleranceDeg Tolerance of angleDeg in degrees. Won't work well with too low tolerances; recommended: >=2 deg.
/// @return MotionInfo
MotionInfo GetMotionInfo_AngleOnly(double angleDeg, double angleToleranceDeg=2.0);

/// Object was rotated with the help of a turntable.
/// The direction of the rotation axis is known, but not the position of the rotation axis.
/// @param[in] angleDeg Angle in degrees.
/// @param[in] axisDir Direction vector of the turntable w.r.t. to world coordinates (orthogonal to the turntable surface plane).
/// @param[in] angleToleranceDeg Tolerance of angleDeg in degrees. Won't work well with too low tolerances; recommended: >=2 deg.
/// @return MotionInfo
MotionInfo GetMotionInfo_TurntablePartlyCalibrated(double angleDeg, const double axisDir[3], double angleToleranceDeg=2.0);


//=============================================================================
// Contact Regions for Alignment
//=============================================================================
/// Index to define a vertex in a submesh
struct SubmeshVertexIndex
{
	int submeshIndex;         ///< which submesh contains the vertex
	int vertexIndex;          ///< vertex index in that submesh

	SubmeshVertexIndex();     ///< Standard constructor. Initializes with indices with -1 = invalid
	SubmeshVertexIndex(int submeshID, int vertexID);  ///< Constructor that directly assigns values
};


/// A pair of contact regions, defined as lists of SubmeshVertexIndex
struct ContactRegionPair
{
	std::vector<SubmeshVertexIndex> vertexIndices1;  ///< indices in 'mesh1', the mesh to be aligned/moved
	std::vector<SubmeshVertexIndex> vertexIndices2;  ///< indices in 'mesh2', the other/fixed mesh
};


/// A list of poses (given as 4*4 matrices) plus tolerance values for translation and rotation.
struct PosesWithTolerances
{
	std::vector<double> poses;   ///< List of poses, each a 4*4 matrix of doubles (row by row).
	double toleranceTrans;       ///< Translational tolerance [mm] of the poses (0 to deactivate).
	double toleranceDeg;         ///< Rotational tolerance [degree] of the poses (0 to deactivate).

	PosesWithTolerances();       ///< Standard constructor.
};


/// Parameter set for Coarse Alignment, see IShapeFusion::AlignPairCoarse.
struct CoarseAlignParams
{
	MotionInfo motionInfo;           ///< Additional information about motion between the two meshes. See MotionInfo for more details.
	int texturePercentage;           ///< Percentage of texture influence on alignment (0-99). Only used if value is higher than 0.
	float qualityFactor;             ///< 1 is recommended. Larger values like 5 or 10 may create better results for symmetric objects, but increase computation time. Lower values like 0.01 to 0.1 decrease computation time, but may result in wrong alignment results.

	ContactRegionPair contactRegionPair;  ///< Pair of Contact Regions, one region on mesh '1' and one on mesh '2' which are to be in contact with the other mesh. Empty lists mean: Use entire mesh.

	PosesWithTolerances blacklist1;  ///< Blacklist of (rough) poses that mesh1 may not be moved to (can be empty).
	PosesWithTolerances whitelist1;  ///< Whitelist of (rough) poses that mesh1 must be moved to (can be empty)

	/// Constructors:
	CoarseAlignParams(const MotionInfo &motionInfo=GetMotionInfo_Free(), int texturePercentage=0, float qualityFactor=1.f, const ContactRegionPair &contactRegionPair=ContactRegionPair(), const PosesWithTolerances &blacklist1=PosesWithTolerances(), const PosesWithTolerances &whitelist1=PosesWithTolerances());
};


/// Parameter set for Fine Alignment, see IShapeFusion::AlignPairFine.
struct FineAlignParams
{
	int maxNumIterations;       ///< Number of iterations to run maximally (recommended: 100)
	int texturePercentage;      ///< Percentage of texture influence on alignment. Only used if value is higher than 0.
	double maxTranslation;      ///< Maximum allowed drift[mm] of mesh center (0 = no limit). Alignment will stop when this limit is reached.
	double maxRot_deg;          ///< Maximum allowed rotation of mesh in degree (0 = no limit). Alignment will stop when this limit is reached.

	/// Constructors:
	FineAlignParams(int maxNumIterations=100, int texturePercentage=0, double maxTranslation=0, double maxRot_deg=0);
};


/// Parameter set for Global Fine Alignment, see IShapeFusion::AlignGlobalFine.
struct GlobalFineAlignParams
{
	int texturePercentage;      ///< Percentage of texture influence on alignment. Only used if value is higher than 0.
	int numIterations;          ///< Number of iterations to run (recommended: 20)
	double maxTranslation;      ///< Maximum allowed drift[mm] of any mesh center (0 = no limit). Global Fine Alignment will not move any scan more than this.
	double maxRot_deg;          ///< Maximum allowed rotation of any mesh in degree (0 = no limit). Global Fine Alignment will not rotate any scan more than this.

	/// Constructors:
	GlobalFineAlignParams();    ///< Standard constructor.
};

/// @}

//=============================================================================
// IShapeFusion
//=============================================================================

/// Interface class for 'Shape Fusion'.
/// Example:
/// @include SimpleShapeFusion.cpp
class IShapeFusion
{
public:
	//-------------------------------------------------------------------------
	/// @name Construction, import, export, deconstruction
	/// @{
	//-------------------------------------------------------------------------

	/// Destructor.
	virtual ~IShapeFusion() {}

	/// Exports the selected mesh.
	/// See documentation of DAVID software for supported file formats.
	///
	/// @warning File paths like "c:\\meshes\\myobject.obj" are interpreted as files located on the server.
	/// For files located on the client you have to use a shared network path like "\\\\mycomputer\\meshes\\myobject.obj".
	/// 
	/// @param[in] meshID ID of the mesh.
	/// @param[in] filename Filename of the mesh to be exported.
	/// @exception david::Error_NoAccess (david::FunctionException) No access to path.
	/// @exception david::Error_InvalidPath (david::FunctionException) Specified path is invalid.
	/// @exception david::Error_MissingFilename (david::FunctionException) Specified path misses file name.
	/// @exception david::Error_DirectoryNotFound (david::FunctionException) Specified directory does not exist.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	virtual void ExportMesh(int meshID, const std::string& filename) = 0;

	/// Imports a mesh into the mesh list from a file given by 'filename'.
	/// See documentation of DAVID software for supported file formats.
	///
	/// @warning File paths like "c:\\meshes\\myobject.obj" are interpreted as files located on the server.
	/// For files located on the client you have to use a shared network path like "\\\\mycomputer\\meshes\\myobject.obj".
	/// 
	/// @param[in] filename Filename of the mesh to be imported.
	/// @return ID of the imported mesh.
	/// @exception david::Error_NoAccess (david::FunctionException) No access to path.
	/// @exception david::Error_InvalidPath (david::FunctionException) Specified path is invalid.
	/// @exception david::Error_MissingFilename (david::FunctionException) Specified path misses file name.
	/// @exception david::Error_DirectoryNotFound (david::FunctionException) Specified directory does not exist.
	/// @exception david::Error_FileNotFound (david::FunctionException) Specified file does not exist.
	virtual int ImportMesh(const std::string& filename) = 0;

	/// Create a new empty mesh.
	/// @return ID of the created mesh.
	virtual int NewMesh() = 0;

	/// Duplicate mesh with mesh ID 'sourceMeshID'.
	/// @param[in] sourceMeshID ID of the mesh to be duplicated.
	/// @return ID of newly created mesh.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	virtual int DuplicateMesh(int sourceMeshID) = 0;

	/// Deletes all meshes in the mesh list. 
	virtual void DeleteAllMeshes() = 0;

	/// Deletes a specific mesh of the mesh list. 
	/// @param[in] meshID ID of the mesh to be deleted.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	virtual void DeleteMesh(int meshID) = 0;

	/// @}

	//-------------------------------------------------------------------------
	/// @name High level access to vertices and triangles
	/// @note These functions treat a mesh group with submeshes as single mesh to the user.
	/// @{
	//-------------------------------------------------------------------------

	/// Gets the vertex positions of a specific mesh. 
	/// @param[out] positions For each vertex float triples are written to this array. (x1,y1,z1,...,xn,yn,zn).
	/// @param[in] meshID ID of the mesh.
	/// @param[in] coordinateSystem 'positions' should be relative to which coordinate system? See CoordinateSystem.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	/// @exception david::Error_InconsistentBuffer (david::FunctionException) Buffer size differs from expected buffer size.
	virtual void GetVertexPositions(std::vector<float>& positions, int meshID, enum CoordinateSystem coordinateSystem = WorldCoordinates) = 0;

	/// Gets the vertex qualities [0..1] of a specific mesh. 
	/// Vertex qualities are not always available. At this time, only a Fusion result has vertex qualities.
	/// @param[out] qualities For each vertex, a float value [0..1] is written to this array
	/// @param[in] meshID ID of the mesh.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	/// @exception david::Error_MissingObject (david::FunctionException) Mesh does not have vertex qualities.
	/// @exception david::Error_InconsistentBuffer (david::FunctionException) Buffer size differs from expected buffer size.
	virtual void GetVertexQualities(std::vector<float>& qualities, int meshID) = 0;

	/// Gets the vertex normals of a specific mesh. 
	/// @param[out] normals For each vertex float triples are written to this array. (x1,y1,z1,...,xn,yn,zn).
	/// @param[in] meshID ID of the mesh.
	/// @param[in] coordinateSystem 'normals' should be relative to which coordinate system? See CoordinateSystem.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	/// @exception david::Error_InconsistentBuffer (david::FunctionException) Buffer size differs from expected buffer size.
	virtual void GetVertexNormals(std::vector<float>& normals, int meshID, enum CoordinateSystem coordinateSystem = WorldCoordinates) = 0;

	/// Gets the vertex texture coordinates (u,v) of a specific mesh. All texture coords are in [0,1].
	/// @param[out] texCoords For each vertex, a pair of floats is written to this array. (u1,v1, u2,v2, ..., un,vn).
	/// @param[in] meshID ID of the mesh.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	/// @exception david::Error_InconsistentBuffer (david::FunctionException) Buffer size differs from expected buffer size.
	virtual void GetVertexTexCoords(std::vector<float>& texCoords, int meshID) = 0;

	/// Gets the triangles of a specific mesh as an indexed face set. 
	/// Each triangle is defined by a triple of vertex indices (a,b,c).
	/// @param[out] triangles For each triangle int triples are written to this array. (a1,b1,c1,...,an,bn,cn).
	/// @param[in] meshID ID of the mesh.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	/// @exception david::Error_InconsistentBuffer (david::FunctionException) Buffer size differs from expected buffer size.
	virtual void GetTriangles(std::vector<int>& triangles, int meshID) = 0;

	/// @}

	//-------------------------------------------------------------------------
	/// @name Access to texture data
	/// @{
	//-------------------------------------------------------------------------


	/// Get the texture image of a mesh as binary data.
	/// Each pixel is represented according to the pixel format in #GetTextureImageFormat.
	/// The pixels are ordered row by row, from top to bottom, each row from left to right.
	///
	/// Usage example for pixelFormat == david::RGB24:
	/// @code
	/// // Import a mesh:
	/// int meshID = david.fusion().ImportMesh("C:\\Scans\\someScan.obj");  // Import mesh (should have a texture)
	/// 
	/// // Get texture format:
	/// int width, height;
	/// david::PixelFormat pixelFormat;
	/// bool hasTexture = david.fusion().GetTextureImageFormat(width, height, pixelFormat, meshID);
	/// if (!hasTexture || pixelFormat != david::RGB24) return;
	/// 
	/// // Get texture data:
	/// std::vector<unsigned char> imgData(david::GetImageSize(width, height, pixelFormat));
	/// david.fusion().GetTextureImageData(imgData.data(), imgData.size(), meshID);  // Retrieve texture image
	/// printf("Texture size is %i*%i\n", width, height);
	/// 
	/// // Print some example pixel colors in the middle:
	/// int x=width/2, y=height/2;
	/// unsigned char* myPixel = &imgData[(y*width+x)*3];
	/// unsigned char blue  = myPixel[0];
	/// unsigned char green = myPixel[1];
	/// unsigned char red   = myPixel[2];
	/// printf("Pixel XY(%i,%i) has color BGR(%u,%u,%u)\n", x,y, blue,green,red);
	/// 
	/// @endcode
	///
	/// @param[out] data Valid pointer to an array that will be filled with the pixel data.
	/// @param[in] dataSizeInBytes Size of 'data' in bytes. Has to be in compliance with #GetTextureImageFormat.
	/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #GetSubmeshCount.
	/// @param[in] meshID ID of the mesh.
	virtual void GetTextureImageData(void* data, size_t dataSizeInBytes, int meshID, int submeshIndex = 0) = 0;

	/// Get the format of the texture image.
	/// @param[out] width Width of the image in [px]
	/// @param[out] height Height of the image in [px]
	/// @param[out] pixelFormat See #PixelFormat. Will be david::UnknownPixelFormat, if no texture is set.
	/// @param[in] meshID ID of the mesh.
	/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #GetSubmeshCount.
	/// @return True, if texture is available. False, if no texture is available.
	virtual bool GetTextureImageFormat(int& width, int& height, enum PixelFormat& pixelFormat, int meshID, int submeshIndex = 0) = 0;

	/// Set texture of a mesh.
	/// @param[in] data Valid pointer to an array of pixel data.
	/// @param[in] dataSizeInBytes Size of 'data' in bytes. Has to be in compliance with given texture format.
	/// @param[in] width Width of the image in [px]
	/// @param[in] height Height of the image in [px]
	/// @param[in] pixelFormat See #PixelFormat. Will be david::UnknownPixelFormat, if no texture is set.
	/// @param[in] meshID ID of the mesh.
	/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #GetSubmeshCount.
	virtual void SetTexture(const void* data, size_t dataSizeInBytes, int width, int height, enum PixelFormat pixelFormat, int meshID, int submeshIndex = 0) = 0;

	/// @}

	//-------------------------------------------------------------------------
	/// @name Object pose and transformations
	/// @{
	//-------------------------------------------------------------------------

	/// Get the position and orientation of a mesh as a homogeneous transformation matrix.
	/// @param[out] world_T_local Valid pointer to an array of 16 doubles representing a homogeneous transformation matrix
	///                           that transforms local coordinates into world coordinates: p_world = world_T_local * p_local.
	///                           Values will be stored in the following order: nx, ny, nz, 0, ox, oy, oz, 0, ax, ay, az, 0, px, py, pz, 1.
	/// @param[in] meshID ID of the mesh.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	virtual void GetPose(double world_T_local[16], int meshID) = 0;

	/// Set the position and orientation of a mesh as a homogeneous transformation matrix.
	/// @param[out] world_T_local Valid pointer to an array of 16 doubles representing a homogeneous transformation matrix
	///                           that transforms local coordinates into world coordinates: p_world = world_T_local * p_local.
	///                           Values will be stored in the following order: nx, ny, nz, 0, ox, oy, oz, 0, ax, ay, az, 0, px, py, pz, 1.
	/// @param[in] meshID ID of the mesh.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	virtual void SetPose(const double world_T_local[16], int meshID) = 0;

	/// Rotate a mesh around specified axis by given degrees.
	/// @param[in] meshID ID of mesh to be transformed.
	/// @param[in] transformType Type of transform. See david::TransformType.
	/// @param[in] degrees Amount of rotation in degrees.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	/// @exception david::Error_InvalidArgument (david::FunctionException) Invalid transform type.
	virtual void Rotate(int meshID, enum TransformType transformType, double degrees) = 0;

	/// Translate a mesh along specified axis by given amount.
	/// @param[in] meshID ID of mesh to be transformed.
	/// @param[in] transformType Type of transform. See david::TransformType.
	/// @param[in] amount Amount of translation. Typically in mm.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	/// @exception david::Error_InvalidArgument (david::FunctionException) Invalid transform type.
	virtual void Translate(int meshID, enum TransformType transformType, double amount) = 0;

	/// @}

	//-------------------------------------------------------------------------
	/// @name Filters
	/// @{
	//-------------------------------------------------------------------------

	/// Reduce the density of a mesh.
	/// @param[in] meshID ID of mesh to be transformed.
	/// @param[in] factor Factor of reduction, between 0 and 1.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	/// @exception david::Error_InvalidArgument (david::FunctionException) Invalid factor value.
	virtual void ReduceMeshDensity(int meshID, float factor) = 0;

	/// Remove any vertex i that when projected into a virtual camera view has a mask value of 'mask[i]==true'.
	/// Vertices outside view or with a mask value fo 'mask[i]==false' are not removed.
	/// If a vertex is removed, its referenced triangles are also removed.
	/// The virtual camera is described by a perspective camera model: 
	/// - Central z axis goes through center of image --> Center is at (width/2, height/2). 
	/// - Image plane has a size in x-direction of 'width and a size of 'height' in y-direction.
	/// - x-axis is from left to right and y-axis from top to down (with respect to image).
	/// - Camera looks along the positive z-axis.
	/// - Visible points are on positive side of z-axis.
	/// - Focal length is given by 'f'.
	/// - Pose of the camera with respect to world coordinates is given by 'world_T_cam'.
	///
	/// @param[in] mask Valid pointer to an boolean array of size 'width*height'. True: Vertex should be removed. False: Keep vertex.
	/// @param[in] width Width of the mask image [pixel]. Has to be > 0.
	/// @param[in] height Height of the mask image [pixel]. Has to be > 0.
	/// @param[in] f Focal length (distance of image plane to origin camera coordinate system).
	/// @param[in] world_T_cam Position and orientation (pose) of the camera with respect to world coordinates. 
	///                        The pose is represented as a homogenenous transformation matrix.
	///                        See #d4mesh_getPose for more information.
	/// @exception david::Error_InvalidArgument (david::FunctionException) At least one of the arguments is invalid.
	virtual void RemoveVerticesByVirtualCamMask(const std::vector<bool>& mask, int width, int height, double f, const double world_T_cam[16]) = 0;

	/// Remove selected vertices.
	/// @param[in] selectedVertices Index array of selected vertices.
	/// @param[in] meshID ID of mesh that 'selectedVertices' references.
	/// @exception david::Error_InvalidArgument (david::FunctionException) At least one of the arguments is invalid.
	virtual void RemoveSelectedVertices(const std::vector<SubmeshVertexIndex>& selectedVertices, int meshID) = 0;

	/// Invert orientation of all triangles in given mesh.
	/// @param[in] meshID ID of mesh to be transformed.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	virtual void InvertTriangleOrientation(int meshID) = 0;

	/// @}

	//-------------------------------------------------------------------------
	/// @name Alignment
	/// @{
	//-------------------------------------------------------------------------

	/// Coarse alignment of mesh '1' to another mesh '2'.
	/// @param[in] meshID1 ID of the mesh that is to be aligned.
	/// @param[in] meshID2 ID of the other mesh.
	/// @param[in] params Alignment parameter set. See CoarseAlignParams.
	/// @return Quality of Alignment result in range [0,1]=[bad,good].
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	/// @exception david::Error_AlignFailed (david::FunctionException) Alignment could not find a solution.
	virtual double AlignPairCoarse(int meshID1, int meshID2, const CoarseAlignParams &params) = 0;

	/// Fine alignment of mesh '1' to another mesh '2'.
	/// @param[in] meshID1 ID of the mesh that is to be aligned.
	/// @param[in] meshID2 ID of the other mesh.
	/// @param[in] params Alignment parameter set. See FineAlignParams.
	/// @exception david::Error_InvalidMeshId (david::FunctionException) Specified meshID does not exist.
	/// @exception david::Error_AlignFailed (david::FunctionException) Alignment could not find a solution.
	virtual void AlignPairFine(int meshID1, int meshID2, const FineAlignParams &params) = 0;

	/// Global fine alignment of all meshes.
	/// @param[in] params Alignment parameter set. See GlobalFineAlignParams.
	virtual void AlignGlobalFine(const GlobalFineAlignParams &params) = 0;

	/// @}

	//-------------------------------------------------------------------------
	/// @name Fusion
	/// @{
	//-------------------------------------------------------------------------

	/// Fuse all visible meshes of the mesh list.
	/// @param[in] resolution Limits the maximum resolution and thus the required memory usage.
	/// @param holeSizeThresRel Holes, which have a smaller area than holeSizeThresRel * size of original data surface, will be closed. 0 = open all holes. 1 = close all holes.
	/// @param[in] sharpness Fusion sharpness in [-3,+5]. Negative values for smoothing. Default is 1.
	/// @return Mesh ID of the fusion result.
	virtual int Fuse(int resolution, float holeSizeThresRel=1.f, int sharpness=1) = 0;

	/// Converts a metric resolution value into a resolution value required by #Fuse.
	/// It uses the maximum bounding box length of all meshes to be fused.
	/// @param[in] metricResolution Smallest possible element size in units (e.g. 0.1 [mm]).
	/// @param[in] maxResolution Returned value is bounded by this value (e.g. 2000).
	/// @return Resolution value that can be used in #Fuse.
	virtual int GetFuseResolution(double metricResolution, int maxResolution) = 0;

	/// @}

	//-------------------------------------------------------------------------
	/// @name Combine and uncombine of meshes
	/// Multiple meshes can be combined into one mesh group. 
	/// A mesh within a mesh group is called submesh. Normally, a mesh has one submesh.
	/// @{
	//-------------------------------------------------------------------------

	/// Combines several meshes into one group.
	/// @param[in] meshIDs List of the IDs of the meshes to be combined.
	/// @return Mesh ID of the combined mesh.
	virtual int CombineMeshes(const std::vector<int> &meshIDs) = 0;

	/// Uncombines a group of meshes into separate meshes.
	/// @param[in] meshIDgroup Mesh ID of the mesh group to be separated.
	/// @return List of the IDs of the separated meshes.
	virtual std::vector<int> UncombineMeshes(int meshIDgroup) = 0;

	/// Get the number of submeshes (combined meshes) in 'mesh'.
	/// A mesh has at least one submesh. This is also the standard case unless #CombineMeshes was used.
	/// @param[in] meshID Valid mesh ID.
	/// @return Number of submeshes (combined) meshes.
	/// @exception david::Error_InvalidArgument (david::FunctionException)
	virtual int GetSubmeshCount(int meshID) = 0;

	/// Set the number of submeshes (combined meshes) in 'mesh'.
	/// A mesh has at least one submesh. This is also the standard case unless #CombineMeshes was used.
	/// @param[in] count New number of submeshes (>0).
	/// @param[in] meshID Valid mesh ID.
	/// @exception david::Error_InvalidArgument (david::FunctionException)
	virtual void SetSubmeshCount(int count, int meshID) = 0;

	/// @}

	//-------------------------------------------------------------------------
	/// @name Direct access to submesh data
	/// @{
	//-------------------------------------------------------------------------

	/// Copy requested buffer ('bufferType') content into 'data'.
	/// You can use #GetSubmeshVertexCount, #GetSubmeshTriangleCount, and #GetSubmeshBufferSize to
	/// compute the correct size of 'data'.
	/// Coordinates are given with respect to local coordinate system.
	/// @param[out] data Valid pointer to data buffer.
	/// @param[in] size Size of 'data' buffer in bytes.
	/// @param[in] bufferType Desired data buffer, see #SubmeshBufferType.
	/// @param[in] meshID Valid mesh ID.
	/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #GetSubmeshCount.
	/// @exception david::Error_InvalidArgument (david::FunctionException)
	virtual void GetSubmeshBuffer(void* data, size_t size, enum SubmeshBufferType bufferType, int meshID, int submeshIndex) = 0;

	/// Copy 'data' into the desired buffer ('bufferType').
	/// You can use #GetSubmeshVertexCount, #GetSubmeshTriangleCount, and #GetSubmeshBufferSize to
	/// compute the correct size of 'data'.
	/// Coordinates have to be given with respect to local coordinate system.
	/// @param[in] data Valid pointer to data buffer.
	/// @param[in] size Size of 'data' buffer in bytes.
	/// @param[in] bufferType Desired data buffer, see #d4mesh_BufferType.
	/// @param[in] meshID Valid mesh ID.
	/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #GetSubmeshCount.
	/// @exception david::Error_InvalidArgument (david::FunctionException)
	virtual void SetSubmeshBuffer(const void* data, size_t size, enum SubmeshBufferType bufferType, int meshID, int submeshIndex) = 0;

	/// Is the buffer 'bufferType' available?
	/// See #SubmeshBufferType for information about which buffers are optional.
	/// @param[in] bufferType Desired data buffer, see #d4mesh_BufferType.
	/// @param[in] meshID Valid mesh ID.
	/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #GetSubmeshCount.
	/// @return True, if submesh has a buffer of type 'bufferType'. False otherwise.
	/// @exception david::Error_InvalidArgument (david::FunctionException)
	virtual bool HasSubmeshBuffer(enum SubmeshBufferType bufferType, int meshID, int submeshIndex) = 0;

	/// Get size of buffer 'bufferType' in bytes.
	/// @param[in] bufferType Desired data buffer, see #SubmeshBufferType.
	/// @param[in] meshID Valid mesh ID.
	/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #GetSubmeshCount.
	/// @return Size of the buffer in bytes. It will be set to zero if buffer is not available.
	/// @exception david::Error_InvalidArgument (david::FunctionException)
	virtual size_t GetSubmeshBufferSize(enum SubmeshBufferType bufferType, int meshID, int submeshIndex) = 0;

	/// Get the number of vertices in submesh.
	/// @param[in] meshID Valid mesh ID.
	/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #GetSubmeshCount.
	/// @return Number of vertices in submesh.
	/// @exception david::Error_InvalidArgument (david::FunctionException)
	virtual int GetSubmeshVertexCount(int meshID, int submeshIndex) = 0;

	/// Set the number of vertices in submesh.
	/// @param[in] vertexCount New number of vertices in submesh. Old data remains in buffers, if 'vertexCount' increases.
	/// @param[in] meshID Valid mesh ID.
	/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #GetSubmeshCount.
	virtual void SetSubmeshVertexCount(int vertexCount, int meshID, int submeshIndex) = 0;

	/// Get the number of triangles in submesh.
	/// @param[in] meshID Valid mesh ID.
	/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #GetSubmeshCount.
	/// @return Number of triangles in submesh.
	/// @exception david::Error_InvalidArgument (david::FunctionException)
	virtual int GetSubmeshTriangleCount(int meshID, int submeshIndex) = 0;

	/// Set the number of triangles in submesh.
	/// @param[in] triangleCount New number of triangles in submesh.
	/// @param[in] meshID Valid mesh ID.
	/// @param[in] submeshIndex Zero based index to a submesh of 'mesh'. Also see #GetSubmeshCount.
	/// @exception david::Error_InvalidArgument (david::FunctionException)
	virtual void SetSubmeshTriangleCount(int triangleCount, int meshID, int submeshIndex) = 0;

	/// @}
};

/// @} // ShapeFusionGroup
/// @} // InterfaceGroup

} // namespace

#endif // DAVID_SDK_I_SHAPE_FUSION_H
