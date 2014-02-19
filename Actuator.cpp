#include "Actuator.h"

Actuator::Actuator(int extendPin, int retractPin, int positionPin){
	extensionPin = extendPin;
	retractionPin = retractPin;
	positioningPin = positionPin;
	
}
	

int Actuator::getCurrentPosition(){
	return analogRead(positioningPin);
}

void Actuator::goUp(){
	state = MOVINGUP;
	moveUp();
}

void	Actuator::goDown(){
	state = MOVINGDOWN;
	moveDown();
}

//this happens when nothing is pressed. In the case of us being in most states, that means we stop. If we are in an automove state, then we have to check if we're where we need to be and if so, turn off the movement pins
void Actuator::doNothing()
{
	int currentPosition = getCurrentPosition();
	if (state == AUTOMOVEDOWN)
	{
		if (currentPosition <= destination){
			state = DONE;
			setPinsToNeutral();
		}
	}
	else if (state == AUTOMOVEUP)
	{
		if (currentPosition >= destination){
			state = DONE;
			setPinsToNeutral();
		}
	}
	else
	{
		state = DONE;
		setPinsToNeutral();
	}
}


void Actuator::goTo(int position){
	destination = position;
	int currentPosition = getCurrentPosition();
	if (currentPosition > destination)
	{
		state = AUTOMOVEDOWN;
		moveDown();
	}
	else if (currentPosition < destination)
	{
		state = AUTOMOVEUP;
		moveUp();
	}
	else
	{
		return;
	}
	
}

void Actuator::moveDown(){
	digitalWrite(retractionPin, HIGH);
	digitalWrite(extensionPin, LOW);
}

void Actuator::moveUp(){
	digitalWrite(extensionPin, HIGH);
	digitalWrite(retractionPin, LOW);
}


void Actuator::setPinsToNeutral()
{
	digitalWrite(extensionPin, LOW);
	digitalWrite(retractionPin, LOW);
}


	
