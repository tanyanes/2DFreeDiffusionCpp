#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class Simulation {
	//Private variables
	vector<Particle> sphere_list;
	vector<vector<int>> collision_pairs;
	vector<vector<Particle>> persistence_pairs;
	double dt = pow(10,-5);

public:
	//Public variables
	Simulation(vector<Particle> newList) {
		sphere_list = newList;
	}

	void placeSpheres();
	void printCurrentParticlePositions();
	void checkCollisions();
	vector<vector<int>> getCombos(int len);
	vector<vector<double>> changeAfterCollision(Particle& sphereA, Particle& sphereB);
	void handleCollisions();
	void moveSphere(Particle& sphere, double nu, double nn);
	Simulation();
	~Simulation();
};