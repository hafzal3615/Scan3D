/// Shows usage of StructuredLightScanner class.

#include "davidSDK/david.h"
#include <iostream>

namespace examples {

void main_SimpleStructuredLightScanner()
{
	try
	{
		// Connect to DAVID server.
		// david::Client::Disconnect is automatically called in destructor of david::Client.
		david::Client david;
		david.Connect();

		// Select screen.
		david.sls().SetScreenID(2);

		// I) Select camera directly:
		// david.sls().SelectCamera("DAVID-CAM-3-M (8F6DEE1E)"); 
		
		// II) Select camera based on common name: Select any DAVID cam.
		std::vector<std::string> cameraNames = david.sls().GetAvailableCameraNames();
		for (size_t i=0; i < cameraNames.size(); ++i)
		{
			if (cameraNames[i].find("DAVID-CAM") != std::string::npos)
			{
				david.sls().SelectCamera(cameraNames[i]); 
				break;
			}
		}
		
		// Get current exposure for scanning.
		std::cout << "Exposure: " << david.sls().GetCameraProperty("exposure") << std::endl;

		// Tells DAVID server to make a new scan and save it under 'filename'.
		int num = david.sls().Scan();
		std::cout << "New scan consists of " << num << " 3D points\n";

		david.sls().ExportMesh("scan.obj");
	}
	catch (david::Exception& e)
	{
		e.PrintError();
	}
}


} // namespace examples
