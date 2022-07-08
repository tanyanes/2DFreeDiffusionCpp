#include <iostream>
#include "particle.h"
#include "simulation.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <random>
#include <math.h>
#include <algorithm>
using namespace std;

Simulation::Simulation() {
	vector<vector<Particle>> collision_pairs;
	vector<vector<Particle>> persistence_pairs;
	placeSpheres();

	int time_steps = 0;

	int N = 10000;

	default_random_engine generator;
	uniform_real_distribution<double> noise_uniform(0, 1);
	normal_distribution<double> noise_normal(1.0, 0.1);

	handleCollisions();
	int numCollisions = collision_pairs.size();

	//cout << "started with: " << numCollisions << " collisions. " << endl;

	while (time_steps < N) {
		//cout << "***********************" << endl;
		printCurrentParticlePositions();
		cout << " time_step: " << time_steps << endl;

		for (int i = 0; i < sphere_list.size(); i++) {
			moveSphere(sphere_list.at(i), noise_uniform(generator), noise_normal(generator));
		}

		handleCollisions();
		
		time_steps = time_steps + 1;
	}

}

//Destructor
Simulation::~Simulation() {
	sphere_list.clear();
}

void Simulation::placeSpheres() {

	default_random_engine generator;

	uniform_real_distribution<double> x_1(0, 3000);
	uniform_real_distribution<double> x_2(0, 3000);

	uniform_real_distribution<double> y_1(2.5, 97.5);
	uniform_real_distribution<double> y_2(2.5, 97.5);

	for (int i = 0; i < 10; i++) {

		Particle FtsIW = Particle(2.5, x_1(generator), y_1(generator), 0, 0, "A" + to_string(i));
		Particle FtsQLB = Particle(2.5, x_2(generator), y_2(generator), 0, 0, "B" + to_string(i));

		sphere_list.push_back(FtsIW);
		sphere_list.push_back(FtsQLB);
	}
}

void Simulation::printCurrentParticlePositions() {
	ofstream outfile;
	outfile.open("positions2.dat",std::ios_base::app);
	outfile << "*" << endl;

	for (int i = 0; i < sphere_list.size(); i++) {
		Particle Sphere = sphere_list[i];
		vector<double> pos = Sphere.getCurrentPosition();
		outfile << pos.at(0) << " " << pos.at(1) << endl;
	}
	outfile.close();
}

vector<vector<int>> Simulation::getCombos(int len) {
	vector<vector<int>> combos;
	for (int i = 1; i <= len; i++) {
		for (int j = 2; j <= len; j++) {
			if (i != j) {
				//cout << i << "," << j << "," << endl;
				combos.push_back({ i,j });
			}
		}
	}
	return combos;
}

void Simulation::checkCollisions() {
	
	vector<vector<int>> combos = getCombos(sphere_list.size());

	for (int i = 0; i < combos.size(); i++) {
		int combo1 = combos[i].at(0);
		int combo2 = combos[i].at(1);
		double r1 = (sphere_list.at(combo1-1)).getRadius();
		double r2 = (sphere_list.at(combo2-1)).getRadius();
		vector<double> pos1 = (sphere_list.at(combo1-1)).getCurrentPosition();
		vector<double> pos2 = (sphere_list.at(combo2-1)).getCurrentPosition();

		double x = pos1.at(0) - pos2.at(0); //calculating number to square in next step
		double y = pos1.at(1) - pos2.at(1);
		double dist;

		dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
		dist = sqrt(dist);

		//If within collision distance, then add to collision pairs
		if (dist < (r1 + r2)) {
			Particle P1 = sphere_list.at(combo1 - 1);
			Particle P2 = sphere_list.at(combo2 - 1);

			collision_pairs.push_back({ (combo1 - 1),(combo2 - 1) });
		}

		//If within persistence distance, then add to persistance pairs
		if (dist < (10.0)) {
			Particle P1 = sphere_list.at(combo1 - 1);
			Particle P2 = sphere_list.at(combo2 - 1);

			vector<Particle> pair1 = { P1,P2 };
			vector<Particle> pair2 = { P2,P1 };

			persistence_pairs.push_back({ sphere_list.at(combo1 - 1),sphere_list.at(combo2 - 1) });
		}
	}
}

vector<vector<double>> Simulation::changeAfterCollision(Particle &sphereA, Particle &sphereB) {

	vector<vector<double>> displacements;

	//UPDATING POSITION

	//acquire centroids of particles
	vector<double> c1_old = sphereA.getCurrentPosition();
	vector<double> c2_old = sphereB.getCurrentPosition();

	//get the midpoint between the two particle centroids
	vector<double> midpoint = { (c1_old.at(0) + c2_old.at(0)) / 2.0, (c1_old.at(1) + c2_old.at(1)) / 2.0 };
	
	//displacement vectors
	vector<double> d1 = { (c1_old.at(0) - midpoint.at(0)), (c1_old.at(1) - midpoint.at(1)) };
	vector<double> d2 = { (c2_old.at(0) - midpoint.at(0)), (c2_old.at(1) - midpoint.at(1)) };

	displacements.push_back(d1);
	displacements.push_back(d2);

	//update positions to new centroids
	vector<double> c1_new = { (c1_old.at(0) + d1.at(0)), (c1_old.at(1) + d1.at(1)) };
	vector<double> c2_new = { (c2_old.at(0) + d2.at(0)), (c2_old.at(1) + d2.at(1)) };

	sphereA.redoCurrentPosition(c1_new);
	sphereB.redoCurrentPosition(c2_new);

	//UPDATING VELOCITY

	vector<double> vAi = { (c1_new.at(0) - c1_old.at(0)) / dt, (c1_new.at(1) - c1_old.at(1)) / dt } ;
	vector<double> vBi = { (c2_new.at(0) - c2_old.at(0)) / dt, (c2_new.at(1) - c2_old.at(1)) / dt };

	//use properties of elastic collisions between equally sized spheres to update
	vector<double> vAf = vBi;
	vector<double> vBf = vAi;

	sphereA.redoCurrentVelocity(vAf);
	sphereB.redoCurrentVelocity(vBf);

	return displacements;
}

void Simulation::handleCollisions() {
	//fill up collision pairs and persistence pairs
	checkCollisions();

	int numCollisions = collision_pairs.size();

	if (numCollisions > 0) {
		cout << "num: " << numCollisions << endl;
	}

	while (numCollisions > 0) {
		cout << "in while loop" << endl;
		for (int i = 0; i < numCollisions; i++) {
			vector<int> pair = collision_pairs.at(i);
			vector<vector<double>> ds = changeAfterCollision(sphere_list.at(pair.at(0)), sphere_list.at(pair.at(1)));
			vector<vector<double>> positions = {sphere_list.at(pair.at(0)).getCurrentPosition(), sphere_list.at(pair.at(1)).getCurrentPosition() };

			for (int j = 0; j < 2; j++) {
				Particle sphere = sphere_list.at(pair.at(j));
				vector<double> position = positions.at(j);

				vector<double> displacement = ds.at(j);
				double radius = sphere.getRadius();
				bool out_of_x = (position.at(0) < 0 || position.at(0) > 3000);
				bool out_of_y = (position.at(1) < (0 + radius) || position.at(1) > (100 - radius));

				cout << "out of x: " << out_of_x << "out of y " <<  out_of_y << endl;

				if (out_of_x == 1 || out_of_y == 1) {
					if (out_of_x == 1 && out_of_y == 0) {
						//invoke the periodic boundary conditions
						if (position.at(0) < 0) {
							sphere_list.at(pair.at(j)).redoCurrentPosition({position.at(0) + 3000, position.at(1)});
						}
						if (position.at(0) > 3000) {
							sphere_list.at(pair.at(j)).redoCurrentPosition({ position.at(0) - 3000, position.at(1) });
						}
					}
					if (out_of_x == 0 && out_of_y == 1) {
						//invoke the reflective boundary conditions
						vector<double> old_position = { position.at(0) - displacement.at(0), position.at(1) - displacement.at(1)};
						sphere_list.at(pair.at(j)).redoCurrentPosition({ old_position.at(0) + displacement.at(0), old_position.at(1) - displacement.at(1)});
					}
					if (out_of_x == 1 && out_of_y == 1) {
						//invoke reflective boundary conditions first
						vector<double> old_position = { position.at(0) - displacement.at(0), position.at(1) - displacement.at(1) };
						sphere_list.at(pair.at(j)).redoCurrentPosition({ old_position.at(0) + displacement.at(0), old_position.at(1) - displacement.at(1) });

						//invoke periodic boundary conditions next
						if (position.at(0) < 0) {
							sphere_list.at(pair.at(j)).redoCurrentPosition({ position.at(0) + 3000, position.at(1) });
						}
						if (position.at(0) > 3000) {
							sphere_list.at(pair.at(j)).redoCurrentPosition({ position.at(0) - 3000, position.at(1) });
						}
					}
				}
			}

			//clear the list of collision pairs just handled
			collision_pairs.clear();

			//check for collisions again, fill up collision pairs
			checkCollisions();

			numCollisions = collision_pairs.size();

		}
	}
}

void Simulation::moveSphere(Particle &sphere, double nu, double nn) {
	double D = 40000.0; //diffusion constant, nm^2/s

	//get current position of sphere
	vector<double> position = sphere.getCurrentPosition();
	double x_curr = position.at(0);
	double y_curr = position.at(1);

	//cout << "before: " << x_curr << " " << y_curr << endl;

	default_random_engine generator;
	uniform_real_distribution<double> noise_uniform(0, 1);
	normal_distribution<double> noise_normal (1.0, 0.1);

	double pi = 3.14159265358979323846;

	//Use polar coordinates to generate a random direction of motion defined by angle theta
	double theta = 2 * pi * nu;

	//np.random.normal(1, 0.1, 1)
	//Use Brownian dynamics to generate amplitude of motion, r
	double r = sqrt(4*D*dt) * nn;

	//Calculate displacement vector using polar coordinates
	double dx = r * cos(theta);
	double dy = r * sin(theta);

	//Displace the particle using overdamped Langevin dynamics
	double x_new = x_curr + dx;
	double y_new = y_curr + dy;

	//Periodic boundary conditions on x
	if (x_new > 3000) {
		x_new = x_new - 3000;
	}
	if (x_new < 0) {
		x_new = 3000 + x_new;
	}

	double rad = sphere.getRadius();

	//Reflective boundary conditions on y
	if (y_new > (100 - rad) or y_new < (0 + rad)) {
		y_new = y_curr - dy;
	}
	//fine x_new and y_new are good

	//Final boundary check
	if ((y_new < (100 - rad) and y_new >(0 + rad)) and (x_new < 3000 and x_new > 0)) {
		vector<double> old_position = sphere.getCurrentPosition();

		//update with a new position...
		vector<double> new_position = { x_new,y_new };
		sphere.updateCurrentPosition(new_position);

		//good up to here...

		//int trajLength = sphere.getTrajLength();

		//if (trajLength >= 2) {
		//	vector<double> new_velocity = { (x_new - old_position.at(0)) / dt, (y_new - old_position.at(1)) / dt };

		//	sphere.updateCurrentVelocity(new_velocity);
		//}
	}

	position = sphere.getCurrentPosition();
	x_curr = position.at(0);
	y_curr = position.at(1);

	//cout << "after: " << x_curr << " " << y_curr << endl;
}

