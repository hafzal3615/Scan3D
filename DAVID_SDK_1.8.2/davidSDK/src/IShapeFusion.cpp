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

#include "davidSDK/IShapeFusion.h"

#include <string.h>

namespace david { 

//=============================================================================
/// MotionInfoGroup
//=============================================================================

MotionInfo::MotionInfo()
	: angleKnown(false)
	, axisDirKnown(false)
	, angleDeg(0.0)
	, angleToleranceDeg(2.0)
{
	for (int i=0; i < 3; ++i)
	{
		axisDir[i] = 0.0;
	}
}


MotionInfo GetMotionInfo_Free()
{
	return MotionInfo();
}


MotionInfo GetMotionInfo_OnTable(const double axisDir[3])
{
	MotionInfo mi;
	memcpy(mi.axisDir, axisDir, sizeof(double)*3);
	mi.axisDirKnown = true;
	return mi;
}


MotionInfo GetMotionInfo_AngleOnly(double angleDeg, double angleToleranceDeg)
{
	MotionInfo mi;
	mi.angleDeg = angleDeg;
	mi.angleToleranceDeg = angleToleranceDeg;
	mi.angleKnown = true;
	return mi;
}


MotionInfo GetMotionInfo_TurntablePartlyCalibrated(double angleDeg, const double axisDir[3], double angleToleranceDeg)
{
	MotionInfo mi;
	mi.angleDeg = angleDeg;
	mi.angleToleranceDeg = angleToleranceDeg;
	mi.angleKnown = true;
	memcpy(mi.axisDir, axisDir, sizeof(double)*3);
	mi.axisDirKnown = true;
	return mi;

}



SubmeshVertexIndex::SubmeshVertexIndex()
	: submeshIndex(-1)
	, vertexIndex(-1)
{}

SubmeshVertexIndex::SubmeshVertexIndex(int submeshID, int vertexID)
	: submeshIndex(submeshID)
	, vertexIndex(vertexID)
{}


PosesWithTolerances::PosesWithTolerances()
	: poses(0)
//	: numPoses(0)
//	, poses(nullptr)
	, toleranceTrans(0)
	, toleranceDeg(0)
{
}

CoarseAlignParams::CoarseAlignParams(const MotionInfo& _motionInfo, int _texturePercentage, float _qualityFactor, const ContactRegionPair& _contactRegionPair, const PosesWithTolerances& _blacklist1, const PosesWithTolerances& _whitelist1)
	: motionInfo(_motionInfo)
	, texturePercentage(_texturePercentage)
	, qualityFactor(_qualityFactor)
	, contactRegionPair(_contactRegionPair)
	, blacklist1(_blacklist1)
	, whitelist1(_whitelist1)
{
}


FineAlignParams::FineAlignParams(int _maxNumIterations, int _texturePercentage, double _maxTranslation, double _maxRot_deg)
	: maxNumIterations(_maxNumIterations)
	, texturePercentage(_texturePercentage)
	, maxTranslation(_maxTranslation)
	, maxRot_deg(_maxRot_deg)
{
}


GlobalFineAlignParams::GlobalFineAlignParams()
	: texturePercentage(0)
	, numIterations(20)
	, maxTranslation(0)
	, maxRot_deg(0)
{
}


} // namespace
