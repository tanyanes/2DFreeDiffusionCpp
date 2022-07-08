#include <iostream>
#include <vector>

using namespace std;

class Particle {
	//private variables
	double radius; //radius

	vector<vector<double>> p; //position
	vector<vector<double>> v; //velocity

	string ID; //ID

	public:
	//Public variables
	Particle(); //generic constructor
	Particle(double r, double p_x, double p_y, double v_x, double v_y, string ID_in); //formal constructor
	double getRadius(); //radius getter
	string getID(); //ID getter
	void printPosition();
	void printVelocity();
	vector<double> getCurrentPosition();
	void redoCurrentPosition(vector<double> new_pos);
	void redoCurrentVelocity(vector<double> new_vel);
	void updateCurrentPosition(vector<double> new_pos);
	void updateCurrentVelocity(vector<double> new_vel);
	int getTrajLength();
	~Particle(); //destructor
};