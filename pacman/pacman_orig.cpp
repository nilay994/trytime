#include <iostream>
#include <string>
#include <vector>
#include "RobotClass.h"

#include <string.h> //for memset
#include <stdlib.h> // srand, rand
#include <time.h>   // for picking seed of rand

//using namespace std;
int position[2] = {0,0}; 				// declare variable
std::vector<Robot> CCRobot; 			// create the vector with objects of CCRobot
direction_t orientation; 
int temp;								// (initialize to north maybe)	
Robot HCRobot(position,North);   		// create the class of the human controlled robot
int n = 0; 								// number of computer controlled robots;

char grid[22*14];						// declare global grid for all tasks

/*
 *   task1_userInput(): function populates the objects of the classes of Computer Controlled Robots and Human controlled robots
 */
void task1_userInput() {		

	//1. Ask for the number of robots (and read the number from STDIN).
	std::cout<<"Please, insert the number of computer controlled robots: \n";
	std::cin>>n;

	//2. Ask for the position and orientation of the human controlled robot.
	std::cout<<"Please indicate the position of the human controlled robot: \nX: ";
	std::cin>>position[0]; HCRobot.setX(position[0] % 22);
	std::cout<<"Y: ";
	std::cin>>position[1]; HCRobot.setY(position[1] % 14);
	std::cout<<"Please insert the orientation (North=0, West=3, Sout=2, East=1): ";
	std::cin>>temp; orientation = (direction_t) temp;
	HCRobot.setDir(orientation);

	//3. Ask for the position and orientation of each computer controlled robot.
	for(int i=1; i<=n; i++){
		std::cout<<"Please indicate the position of the computer controlled robot "<<i<<": \nX: ";
		std::cin>>position[0];
		position[0] = position[0] % 22;
		std::cout<<"Y: ";
		std::cin>>position[1];
		position[1] = position[1] % 14;
		std::cout<<"Please insert the orientation (North=0, West=3, Sout=2, East=1): ";
		std::cin>>temp; orientation = (direction_t) temp;
		CCRobot.push_back(Robot(position, orientation)); //Insert a new object for this robot
	}

	//4. For each computer controlled robot:
	//(a) Print out the robot’s position and orientation on a single line.
	std::cout<<"The position and orientation of every Computer Controlled Robot are:\n";
	
	for(int i=0; i<n; i++){
		std::cout<<"Robot "<<i<<"-- X: "<<CCRobot[i].getX()<<"; Y:"<<CCRobot[i].getY()<<"; Orientation: "<<CCRobot[i].getDir()<<"   \n";
	}

	//5. Print out the position and orientation of the human controlled robot on a single line.
	std::cout<<"Human controlled Robot -- X:"<<HCRobot.getX()<<"; Y:"<<HCRobot.getY()<<"; Orientation: "<<HCRobot.getDir()<<"   \n";
}

/*
 * take random inputs and free the users from the burden
 */
void task1_randomInput() {
	srand(time(NULL));
	n = rand() % 5 + 1;     	// Computer robots in the range 1 to 5
	position[0] = rand() % 22;  //x between 0 and 21
	HCRobot.setX(position[0]);
    position[1] = rand() % 14;  //y between 0 and 13
    HCRobot.setY(position[1]);
	orientation = North;

    // now for computer robots
	for(int i=0; i<n; i++){
		position[0] = rand() % 22;
		position[1] = rand() % 14;
		orientation = North;
		CCRobot.push_back(Robot(position, orientation)); //Insert a new object for this robot
	}
}


/*
 * print the global variable on the terminal
 */
void printGrid(char* grid) {
	for (int i = 0; i < 22*14; i++) {
		if(i % 22 == 0) {
			std::cout << "\033[0m" << std::endl;
		}
		if (grid[i] == '.') {
			std::cout << "\033[1;43m" << grid[i];
		}
		else if (grid[i] == 'H') {
			std::cout << "\033[1;42m" << grid[i] << "\033[1;43m";
		} 
		else if (grid[i] == 'R') {
			std::cout << "\033[0;41m" << grid[i] << "\033[1;43m";
		} 
	}
	std::cout <<"\033[0m\n" <<std::endl;
}

/*
 * update characters on the grid depending on robot's locations
 */
void modifyGrid(char* grid) {
	//remember, grid starts from zero
	int idx = HCRobot.getX() + HCRobot.getY()*22;
	grid[idx] = 'H';
	for (int cnt = 0; cnt < n; cnt++) {
		idx = CCRobot[cnt].getX() + CCRobot[cnt].getY()*22; 
		grid[idx] = 'R';
	}
}

/*
 * task 2 requires this plot
 */
void task2_printGrid(char* grid) {
	memset(grid, '.', 22*14);
	modifyGrid(grid);
	printGrid(grid);
}

/*
 * This function changes the location of the robots to catch you, first correct x and then y
 */
void robot_ai() {
	//for all the robots 
	for (int i = 0; i < n; i++) {
		int step_x = HCRobot.getX() - CCRobot[i].getX();  
		// calculate how far you are from them, correct x coordinate first, and then go for y
		if(step_x != 0 && CCRobot[i].getDir() != East && CCRobot[i].getDir() != West)  CCRobot[i].setDir(East);
		if(CCRobot[i].getDir() == East || CCRobot[i].getDir() == West) {
			if(step_x > 0) CCRobot[i].moveForward();
			if(step_x < 0) CCRobot[i].moveBackward();
		}
		if(step_x == 0) {
			int step_y = HCRobot.getY() - CCRobot[i].getY(); 
			if(step_y != 0 && CCRobot[i].getDir() != South && CCRobot[i].getDir() != North) CCRobot[i].setDir(North);
			if(CCRobot[i].getDir() == North || CCRobot[i].getDir() == South) {
				if(step_y > 0) CCRobot[i].moveBackward();
				if(step_y < 0) CCRobot[i].moveForward();
			}
		}
	}	
}

/*
 * function implements basic game loop
 */
void task3_game(char* grid) {
	char move;
	bool game_over = false;
	
	while(HCRobot.getX() >= 0 && HCRobot.getX() <= 21 && 
		  HCRobot.getY() >= 0 && HCRobot.getY() <= 13 && 
		  game_over == false) {
		
		std::cout<<"Insert move (w,a,d,s):\n";
		std::cin>>move;
		HCRobot.Move(move);    // move the Robot
		move = 'n'; 		   // change move to nothing

		robot_ai();
		task2_printGrid(grid);

		// is a robot sitting on you?
		for(int i=0; i<n; i++) { 
			if(CCRobot[i].getX() == HCRobot.getX() && 
			   CCRobot[i].getY() == HCRobot.getY()) {
				game_over = true;
				std::cout<<"\033[1;31m You lost, try playing it again!\033[0m\n\n";
			}
		}	
	}	
	// Tell the user about winning
	if(game_over == false) {
		std::cout<<"\n\033[1;34mYOU WIN!\033[0m \n\n\n";
	}
}

int main() {
	std::cout << "press Y to enter the game directly, N to give inputs" << std::endl;
	char choice;
	std::cin >> choice;
	if (choice == 'Y'|| choice == 'y') {
		task1_randomInput();
	}	
	else if (choice = 'N' || choice == 'n') {
	 	task1_userInput();
	}
	else {
		std::cerr << "OUT";
	}

	//use if Segmentation faults
	//grid = malloc(((22*14)+1)*sizeof(char));
	task2_printGrid(grid);

	task3_game(grid);

return 0;
}
