/// @example SimpleTurntable.cpp
///
/// Shows usage of david::Turntable, david::StructuredLightScanner, and david::ShapeFusion class.

#include "davidSDK/david.h"

namespace examples {

/// Shows usage of david::Turntable, david::StructuredLightScanner, and david::ShapeFusion class.
void main_SimpleTurntable() 
{
	try 
	{
		david::Client david;
		david.Connect();
		david.turntable().Setup(true);  // connect turntable

		const double step = 45.0;
		for (double angle=0.0; angle < 360.0; angle += step) 
		{
			david.sls().Scan();
			int meshID = david.sls().AddScanToShapeFusion();
			david.fusion().Rotate(meshID, david::TransformGlobalY, -angle);
			david.turntable().Rotate(step);
		}

		int fusionMeshID = david.fusion().Fuse(1500);
		david.fusion().ExportMesh(fusionMeshID, "FusionResult.obj");

		david.turntable().Setup(false);  // disconnect
	}
	catch (david::Exception& e) 
	{
		e.PrintError();
	}
}

}