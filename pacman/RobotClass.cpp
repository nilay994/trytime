#include "RobotClass.h"

//Methods
void Robot::Move(char move) {
		switch (move) {
			case 'w': Robot::moveForward();	 break;
			case 'a': Robot::turnLeft();     break;
			case 'd': Robot::turnRight();    break;
			case 's': Robot::moveBackward(); break;
			default: break;
		}
}

void Robot::moveForward()
{
	switch(direction_) {
		case North:	y_--; break;
		case East:	x_++; break;
		case South: y_++; break;
		case West:  x_--; break;
		default:break;
	}
}

void Robot::moveBackward()
{
	switch(direction_) {
		case North:	y_++; break;
		case East:	x_--; break;
		case South:	y_--; break;
		case West:  x_++; break;
		default:break;
	}
}

void Robot::turnLeft()
{
	switch(direction_) {
		case North:	direction_ = West;  break;
		case East:  direction_ = North; break;
		case South:	direction_ = East;	break;
		case West:  direction_ = South;	break;
		default: break;
	}
}

void Robot::turnRight()
{
	switch(direction_) {
		case North:	direction_ = East;  break;
		case East:	direction_ = South; break;
		case South:	direction_ = West;  break;
		case West:  direction_ = North; break;
		default:break;
	}
}
