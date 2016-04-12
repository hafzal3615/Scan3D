#include "SimpleTurntable.h"
#include "SimpleCalibration.h"
#include "SimpleStructuredLightScanner.h"
#include "SimpleShapeFusion.h"
#include "TimedScans.h"

//ih8git

#include <stdio.h>  // required for printf
#include <conio.h>  // required for _getch


void main() 
{
	//examples::main_SimpleTurntable();
	examples::main_SimpleCalibration();
	//examples::main_SimpleStructuredLightScanner();
	//examples::main_SimpleShapeFusion();
	//examples::main_TimedScans(30.0, 3);
	
	printf("Press any key to exit\n");
	_getch();  // waits for key
}
