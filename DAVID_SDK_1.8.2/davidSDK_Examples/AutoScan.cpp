#include "davidSDK/david.h"
#include <vector>
#include <time.h>
#include <string>

namespace examples {

	void main_AutoScan() {
		try {
			// Connection to DAVID server is established within constructor.
			// david::Client::Disconnect is automatically called in destructor of david::Client.
			david::Client david;
			david.Connect();
			david.mainWindow().Show();

			for (int i = 10; i > 0; i--) {
				printf("%d \n", i);
				Sleep(1000);
			}

			// Scan and export.
			david.sls().Scan();
		}
		catch (david::Exception& e) {
			e.PrintError();
		}
	}
}