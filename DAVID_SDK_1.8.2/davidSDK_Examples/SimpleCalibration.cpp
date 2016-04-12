/// @example SimpleStructuredLightScanner.cpp
///
/// Shows usage of calibration with the StructuredLightScanner class.

#include "davidSDK/david.h"
#include <vector>
#include <time.h>
#include <string>

namespace examples {


/// Shows usage of calibration with the StructuredLightScanner class.
/// Simple example class for usage of StructuredLightScanner class via david::Client::sls interface.
void main_SimpleCalibration()
{
	try
	{
		// Connection to DAVID server is established within constructor.
		// david::Client::Disconnect is automatically called in destructor of david::Client.
		david::Client david;
		david.Connect();
		david.mainWindow().Show();

		/*
				std::vector<std::string> names = david.sls().GetAvailableCameraNames();
				for (int i = 0; i < names.size(); i++) {
					std::string name = names[i];
						printf("%s", name);
				}
		*/
		// Select screen and camera.
		david.sls().SetScreenID(2);
		david.sls().SelectCamera("Logitech HD Pro Webcam C920 (4076D97F)");

		// Tells DAVID server to calibrate, and save calibration parameters to .cal files.

		david.sls().Calibrate(150.0);
		//david.sls().ExportCalibration("camera.cal", "projector.cal");
		/*
		david.sls().SetCameraProperty("Gain", 0);
		david.sls().SetCameraProperty("Exposure", 0);
		david.sls().SetCameraProperty("Brightness", 0);
		david.sls().SetCameraProperty("Contrast", 0);

		int gain = david.sls().GetCameraProperty("Gain", david::CamPropsForScanning);
		int contrast = david.sls().GetCameraProperty("Contrast", david::CamPropsForScanning);
		int brightness = david.sls().GetCameraProperty("Brightness", david::CamPropsForScanning);
		int exposure = david.sls().GetCameraProperty("Exposure", david::CamPropsForScanning);

		printf("Gain: %d\n", gain);
		printf("Contrast: %d\n", contrast);
		printf("Brightness: %d\n", brightness);
		printf("Exposure: %d\n", exposure);
		*/

		// Re-import calibration (this step is not necessary here, it is just for demonstration/testing):
		//david.sls().ImportCalibration("camera.cal", "projector.cal");

		
		// david::Client::Disconnect is automatically called in destructor of david::Client
		// at the end of this { } block

		for (int i = 0; i < 10; i++) {
			Sleep(1000);
			printf("%d", 10 - i);
		}

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
