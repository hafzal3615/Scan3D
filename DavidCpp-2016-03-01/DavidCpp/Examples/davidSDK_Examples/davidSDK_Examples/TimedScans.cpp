/// @example TimedScans.cpp
///
/// Takes a scan every x minute.

#include "davidSDK/david.h"
#include <time.h>
#include <iostream>
#include <Windows.h>


namespace examples {


/// Take a scan every 'waitTimeInSeconds' seconds.
/// This example assumes that the scanner is calibrated and camera and projector are connected and correctly configured.
/// The scans are saved in the current directory.
///
/// @param[in] waitTimeInSeconds Wait time in seconds until next scan.
/// @param[in] scanCount Total number of scans to be taken.
void main_TimedScans(double waitTimeInSeconds, int scanCount)
{
	try
	{
		david::Client david;
		david.Connect();
		david.fusion().DeleteAllMeshes();
				
		for (int i=0; i < scanCount; ++i)
		{
			// Get filename and save current time.
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
			//david.sls().AddScanToShapeFusion();

			// Wait.
			double dif = 0.0;
			while (dif < waitTimeInSeconds)
			{
				dif = difftime(time(0), begin);
				Sleep(100);
			}
		}
	}
	catch (david::Exception& e)
	{
		e.PrintError();	
	}
}

} // namespace