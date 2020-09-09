#ifndef ROBOT_CLASS_H
#define ROBOT_CLASS_H

// terminals io library (non blocking)
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <string>

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/*-------------------------------------------------
 initialize special non blocking non echoing Keyboard 
 ------------------------------------------------*/
struct termios orig_term, raw_term;

void init_keyboard(void)
{
  // Get terminal settings and save a copy for later
  tcgetattr(STDIN_FILENO, &orig_term);
  raw_term = orig_term;

  // Turn off echoing and canonical mode
  raw_term.c_lflag &= ~(ECHO | ICANON);

  // Set min character limit and timeout to 0 so read() returns immediately
  // whether there is a character available or not
  raw_term.c_cc[VMIN] = 0;
  raw_term.c_cc[VTIME] = 0;

  // Apply new terminal settings
  tcsetattr(STDIN_FILENO, TCSANOW, &raw_term);
}

void deinit_keyboard(void)
{
	// Restore original terminal settings
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
}



class Robot
{
public: 
	// Class Members
	bool move;
	std::string command;

	//Class Methods. (short definitions, hence not in the source file)
	// encapsulation functions
	int getX()						     {return x_;}     			//return x position
	int getY()						     {return y_;}     			//return y position
	void setX(int x) 				     {x_ = x;} 		  			//set x position
	void setY(int y) 				     {y_ = y;} 		  			//set y position
	void setDir(std::string direction)   {direction_ = direction;}  //set direction
	std::string getDir()            	 {return direction_;}		//return direction
	void setScore(int score)             {score_ = score;}
	int getScore()						 {return score_;}
	void incScore()						 {score_ = score_ + 1;}

	//Constructor for the robot
	Robot(int position[2], std::string direction){ 
		x_ = position[0];
		y_ = position[1];
		direction_ = direction;
		move = false;
		command = "steady";
		score_ = 0;
	}

private:
	//  Class Members
	int x_;
	int y_;
	std::string direction_;
	int score_;

	// Class Methods
	void moveForward();//It will change the position depending on the direction the robot facing
	void moveBackward();//It will change the position depending on the direction the robot facing
	void turnLeft();//Change direction counterclockwise
	void turnRight();//Change direction clockwise

};


#endif
