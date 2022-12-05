#include "accelMagGyro.h"

int main (void)
{	
	while(1){
		acc();
		Mag();
		Gyro();
		
	sleep(1);
	}

	return 0;
}
