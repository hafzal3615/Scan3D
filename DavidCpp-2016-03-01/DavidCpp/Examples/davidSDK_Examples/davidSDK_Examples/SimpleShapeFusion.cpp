/// @example SimpleShapeFusion.cpp
///
/// Shows usage of david::ShapeFusion class.

#include "davidSDK/david.h"

namespace examples {

/// Shows usage of david::ShapeFusion class.
///
void main_SimpleShapeFusion()
{
	try
	{
		// INITIALIZE, CONNECT TO SERVER:
		david::Client david;
		david.Connect();

		// IMPORT SOME SCANS:
		// File paths are relative to SERVER application folder:
		int id1 = david.fusion().ImportMesh("Scan01.obj");
		int id2 = david.fusion().ImportMesh("Scan02.obj");
		int id3 = david.fusion().ImportMesh("Scan03.obj");

		// Best to use absolute paths like "C:/Scans/Scan01.obj".

		// ALIGN SCAN 2 to SCAN 1, then SCAN 3 to SCAN 2:
		david.fusion().AlignPairCoarse(id2, id1, david::CoarseAlignParams(david::GetMotionInfo_Free()));  // first, coarse alignment (free)
		david.fusion().AlignPairFine(id2, id1, david::FineAlignParams());  // then fine alignment

		david.fusion().AlignPairCoarse(id3, id2, david::CoarseAlignParams(david::GetMotionInfo_Free()));  // first, coarse alignment (free)
		david.fusion().AlignPairFine(id3, id2, david::FineAlignParams());  // then fine alignment

		// FUSE ALL SCANS:
		int fusionMeshID = david.fusion().Fuse(500);
		
		// EXPORT FUSION RESULT:
		david.fusion().ExportMesh(fusionMeshID, "FusionResult.obj");
	}
	catch (david::Exception& e)
	{
		e.PrintError();	
	}
}

} // namespace