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

		david.sls().SetScreenID(0);
		david.sls().SelectCamera("Logitech HD Pro Webcam C920 (4076D97F)"); 

		// Tells DAVID server to calibrate, and save calibration parameters to .cal files.
		david.sls().Calibrate(120.0);
		david.sls().ExportCalibration("camera.cal", "projector.cal");

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

		time_t begin = time(0);
		std::string filename;
		{
			tm tstruct;
			localtime_s(&tstruct, &begin);
			char buf[80];
			strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tstruct);	

			filename = buf;
			filename += ".obj";

			std::cout << "filename: " << filename << std::endl;
		}
		
		// Scan and export.
		david.sls().Scan();

		david.sls().ExportMesh(filename);

		// Re-import calibration (this step is not necessary here, it is just for demonstration/testing):
		//david.sls().ImportCalibration("camera.cal", "projector.cal");
		
		// david::Client::Disconnect is automatically called in destructor of david::Client
		// at the end of this { } block
	}
	catch (david::Exception& e)
	{
		e.PrintError();
	}
}


} // namespace examples
