#include "SimpleTurntable.h"
#include "SimpleCalibration.h"
#include "SimpleStructuredLightScanner.h"
#include "SimpleShapeFusion.h"
#include "TimedScans.h"
#include "Perimetre.h"
#include <iostream>
#include <locale.h>

#include <stdio.h>  // required for printf
#include <conio.h>  // required for _getch

void main() 
{
	examples::main_Perimetre();
	
	printf("Press any key to exit\n");
	_getch();  // waits for key
}
