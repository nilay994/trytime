#include <iostream>
#include <string>
#include <vector>
#include "RobotClass_try.h"

#include <string.h> //for memset
#include <stdlib.h> /* srand, rand */
#include <time.h>   // for picking seed of rand

//using namespace std;
int position[2] = {0,0}; 				//declare variable
std::vector<Robot> CCRobot; 			//Create the vector with objects of CCRobot
std::string orientation; 				//declare variable (initialize to north maybe)	
Robot HCRobot(position, orientation);   //Create the class of the human controlled robot
int n=0; 								//number of computer controlled robots;

//grid for task2
char grid[22*14];

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
	std::cout<<"Please insert the orientation (North, West, South, East): ";
	std::cin>>orientation; HCRobot.setDir(orientation);	

	//3. Ask for the position and orientation of each computer controlled robot.
	for(int i=1; i<=n; i++){
		std::cout<<"Please indicate the position of the computer controlled robot "<<i<<": \nX: ";
		std::cin>>position[0];
		position[0] = position[0] % 22;
		std::cout<<"Y: ";
		std::cin>>position[1];
		position[1] = position[1] % 14;
		std::cout<<"Please insert the orientation (North, West, South, East): ";
		std::cin>>orientation;
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

void task1_randomInput() {
	srand(time(NULL));
	n = rand() % 5   + 1;     // Computer robots in the range 1 to 5

	position[0] = rand() % 22; //x between 0 and 21
	HCRobot.setX(position[0]);
    position[1] = rand() % 14; //y between 0 and 13
    HCRobot.setY(position[1]);

    // now for computer robots
	for(int i=0; i<n; i++){
		position[0] = rand() % 22;
		position[1] = rand() % 14;
		orientation = rand() % 4;
		CCRobot.push_back(Robot(position, orientation)); //Insert a new object for this robot
	}
}


void printGrid(char* grid) {
	for (int i = 0; i < 22*14; i++) {
		if(i % 22 == 0) {
			std::cout << "\033[0m" << std::endl;
		}
		if (grid[i] == ' ') {
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

void modifyGrid(char* grid) {
	//remember, grid starts from zero
	int idx = HCRobot.getX() + HCRobot.getY()*22;
	grid[idx] = 'H';
	for (int cnt = 0; cnt < n; cnt++) {
		idx = CCRobot[cnt].getX() + CCRobot[cnt].getY()*22; 
		grid[idx] = 'R';
	}
}

void task2_printGrid(char* grid) {
	memset(grid, ' ', 22*14);
	modifyGrid(grid);
	printGrid(grid);
}

/*
 * This function changes the location of the robots to catch you
 */
void robot_ai() {
		//for all the robots 
		for (int i = 0; i < n; i++) {

			int step_x = HCRobot.getX() - CCRobot[i].getX();   //calculate how far you are from them
			if (step_x != 0) {
				int x_moveto = CCRobot[i].getX() + sgn(step_x); //move one step
				if (step_x) CCRobot[i].setDir("East");
				else CCRobot[i].setDir("West");
				CCRobot[i].setX(x_moveto); 
			}
			int step_y = HCRobot.getY() - CCRobot[i].getY();  //calculate how far you are from them
			if (step_y != 0) {
				int y_moveto = CCRobot[i].getY() + sgn(step_y); //move one step
				if (step_y) CCRobot[i].setDir("North");
				else CCRobot[i].setDir("South");
				CCRobot[i].setY(y_moveto); 
			}
		}	
}


char move;				// move is a global, realtime keyboard input
int counter;			// to count the number of tries robot has to wait till
int esc = 0;
/*
 *  this task is the game loop, it modifies and prints the populated grid
 */
void task3_game_static_robots(char* grid) {

	while(!esc) {

		while(HCRobot.getX()>=0 && HCRobot.getX()<=21 && 
			  HCRobot.getY()>=0 && HCRobot.getY()<=13 && !esc) {
			
			int len = read(STDIN_FILENO, &move, 1); 
			switch (move) {
				case 'w':
					position[1] = HCRobot.getY()-1;
					HCRobot.setY(position[1]);
					break;
				case 'a':
					position[0] = HCRobot.getX()-1;
					HCRobot.setX(position[0]);
					break;
				case 'd':
					position[0] = HCRobot.getX()+1;
					HCRobot.setX(position[0]);
					break;
				case 's':
					position[1] =HCRobot.getY()+1;
					HCRobot.setY(position[1]);
					break;
				case 'q':
					esc = 1; break;
				default: break;
			}

			move = 'n'; //change move to nothing

			counter++; 

			// populate the grid with AI robots' steps.
			// you have 5 tries, robot has only one. You are slow. 
			if (counter > 5) {
				counter = 0;
				robot_ai();
			}

			// this smart cout clears the terminal to give a stop motion animation
			std::cout<<"\033[2J\033[1;1H";  	
			task2_printGrid(grid);
			std::cout<<"Insert move (w,a,d,s), or q to quit:\n";

			std::cout<<"You: "<<HCRobot.getScore()<<"\tRobot: "<<CCRobot[0].getScore()<<std::endl;

			//set the speed of the game
			usleep(80000);

			// is a robot sitting on you?
			for(int i=0; i<n; i++) {
				if(CCRobot[i].getX() == HCRobot.getX() && 
				   CCRobot[i].getY() == HCRobot.getY()) {
				   	for (int j= 0; j<n; j++) {CCRobot[j].incScore();} //increment score for all; not one
					position[0] = rand() % 22;
					position[1] = rand() % 14;
					CCRobot[i].setX(position[0]);
					CCRobot[i].setY(position[1]);
				}
			}	
		}	
		// Tell the gamer he won.
		// if(game_over == false) {
		// std::cout<<"\n\033[1;34mYOU WIN!\033[0m \n\n\n";
		HCRobot.incScore();
		position[0] = rand() % 22;
		position[1] = rand() % 14;
		HCRobot.setX(position[0]);
		HCRobot.setY(position[1]);
	}
}

int main() {
	std::cout << "press Y to play the game directly, press N for giving user inputs\n" << std::endl;
	char choice;
	std::cin >> choice;
	if (choice == 'Y'|| choice == 'y') {
		task1_randomInput();
	}	
	else if (choice = 'N' || choice == 'n') {
	 	task1_userInput();
	}

	//use if Segmentation faults
	//grid = malloc(((22*14)+1)*sizeof(char));
	else  std::cerr << "OUT";

	// init keyboard in a non-blocking mode
	init_keyboard();

	task2_printGrid(grid);
	task3_game_static_robots(grid);


	/* Make sure no characters are left in the input stream as
	plenty of keys emit ESC sequences, otherwise they'll appear
	on the command-line after we exit.*/
	while(read(STDIN_FILENO, &move, 1)==1);
	
	deinit_keyboard();

return 0;
}
