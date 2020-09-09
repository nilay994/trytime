#ifndef ROBOT_CLASS_H
#define ROBOT_CLASS_H

#include <iostream>
#include <string>

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/*create an enum for better use of orientations*/
typedef enum {
	 North,  //0
	 South,  //1
	 West,   //2
	 East,   //3
} direction_t;


class Robot
{
	public: 
		// No class members

		// class methods. (short definitions, hence not in the source file)
		// encapsulation functions
		int getX()   {return x_;}	     				  //return x position
		int getY()   {return y_;}   	  			      //return y position
		void setX(int x) {x_ = x;} 		  				  //set x position
		void setY(int y) {y_ = y;} 		  				  //set y position
		void setDir(direction_t dir) {direction_ = dir;}  //set direction
		direction_t getDir() {return direction_;}		  //return direction
		
		// Move function
		void Move(char move);

		void moveForward();		//change the position depending on the direction the robot facing
		void moveBackward();	//change the position depending on the direction the robot facing
		void turnLeft();      	//change direction counterclockwise
		void turnRight();		//change direction clockwise

		//Constructor for the robot
		Robot(int position[2], direction_t dir) { 
			x_ = position[0];
			y_ = position[1];
			direction_ = dir;
		}

	private:
		//  Class Members
		int x_;
		int y_;
		direction_t direction_;


};

#endif