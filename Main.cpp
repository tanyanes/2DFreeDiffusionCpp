#include <iostream>
#include <vector>
#include "particle.h"
#include "simulation.h"

using namespace std;

int main() {
	cout << "hello!" << endl;
	Particle particleA = Particle(2.5, 1, 1, 2, 1, "A3");

	particleA.printVelocity();

	Simulation A = Simulation();
	//A.printCurrentParticlePositions();
}