#include <string>
#include <string.h>
#include <vector>
#include <sstream> //istd::stringstream
#include <iostream> // std::std::cout
#include <fstream> // ifstream

// typedef
enum waypoint_type_t {
	WAYPOINT=0,
	GATE=1,
	FINISH=2,
	START=4,
	SKIP_GATE=8,
};


// flightplan waypoint
struct waypoint_t {
	double x;
	double y;
	double z;
	double psi; 		// heading towards gate
	double gate_psi; // heading gate's front panel
	double v_sp; 	// velocity at the gate
	waypoint_type_t type;
};

 
std::vector<std::vector<double>> parse_csv_file(std::string inputFileName) {
 
    std::vector<std::vector<double>> data;
    std::ifstream inputFile(inputFileName);
    int l = 0;
 
    while (inputFile) {
        l++;
        std::string s;
        if (!getline(inputFile, s)) break;
        if (s[0] != '#') {
            std::istringstream ss(s);
            std::vector<double> record;
 
            while (ss) {
                std::string line;
                if (!getline(ss, line, ','))
                    break;
                try {
                    record.push_back(stof(line));
                }
                catch (const std::invalid_argument e) {
                    std::cout << "NaN found in file " << inputFileName << " line " << l
                         << std::endl;
                    e.what();
                }
            }
 
            data.push_back(record);
        }
    }
 
    if (!inputFile.eof()) {
        std:: cerr << "Could not read file " << inputFileName << "\n";
        std::__throw_invalid_argument("File not found.");
    }
 
    return data;
}


std::vector<waypoint_t> wp;

// add waypoint to the fligbtplan
void add_wp(float x, float y, float z, float psi, float gate_psi, float v_sp, waypoint_type_t type) {
    waypoint_t wp_tmp = {x, y, z, psi, gate_psi, v_sp, type};
    wp.push_back(wp_tmp);
}

int main() {
    
    std::vector<std::vector<double>> data = parse_csv_file("log-flights/austin-1/waypoints.csv"); //bmore-env-change/waypoints-perturb-set1-1.csv");
    
    for(std::vector<std::vector<double>>::iterator it = begin(data); it != end(data); ++it) {
        std::vector<double> wp = *it;
        waypoint_t new_wp;
        new_wp.x                = wp[0];
        new_wp.y                = wp[1];
        new_wp.z                = wp[2];
        new_wp.psi              = wp[3];
        new_wp.gate_psi         = wp[4];
        new_wp.type             = (waypoint_type_t) wp[5];
        new_wp.v_sp             = wp[6];
        add_wp(new_wp.x, new_wp.y, new_wp.z, new_wp.psi, new_wp.gate_psi, new_wp.v_sp, new_wp.type);
    }


    for (int i = 0; i < (int) wp.size(); i++) {
	    printf("%f,%f,%f,%f,%f,%d,%f,%d\n", wp[i].x, wp[i].y, wp[i].z, wp[i].psi, wp[i].gate_psi, wp[i].type, wp[i].v_sp, i);
	}
 
    return 0;
}