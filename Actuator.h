#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "Arduino.h"

typedef enum { DONE, MOVINGUP, MOVINGDOWN, AUTOMOVEUP, AUTOMOVEDOWN } ActuatorState;

class Actuator {
	public :
		Actuator(int extendPin, int retractPin, int positionPin);
		void goUp();
		void goDown();
		void goTo(int position);
		void doNothing();
		int getCurrentPosition();
		
	private:
		ActuatorState state;
		int destination;
		int extensionPin, retractionPin, positioningPin;
		void moveUp();
		void moveDown();
		void setPinsToNeutral();
};

#endif
