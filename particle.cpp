#include <iostream>
#include "particle.h"
using namespace std;

//Default constructor
Particle::Particle() {
	radius = 2.5;

	ID = "A1";

	p = { {0,0} };

	v = { {0,0} };

	//if you ever want to add to the vector...
	//p_x.push_back(1.0);
	//v_x.push_back(1.0);
	//p_y.push_back(1.0);
	//v_y.push_back(1.0);
}

//formal constructor
Particle::Particle(double r, double px, double py, double vx, double vy, string ID_in) {
	radius = r;

	ID = ID_in;

	p = { {px,py} };
	v = { {vx,vy} };
}

void Particle::redoCurrentPosition(vector<double> new_pos) {
	p.pop_back();
	p.push_back(new_pos);
}

void Particle::redoCurrentVelocity(vector<double> new_vel) {
	v.pop_back();
	v.push_back(new_vel);
}

void Particle::updateCurrentPosition(vector<double> new_pos) {
	p.push_back(new_pos);
}

void Particle::updateCurrentVelocity(vector<double> new_vel) {
	p.push_back(new_vel);
}

int Particle::getTrajLength() {
	return p.size();
}

double Particle::getRadius() {
	return radius;
}

string Particle::getID() {
	return ID;
}

vector<double> Particle::getCurrentPosition() {
	return p.back();
}

void Particle::printPosition() {
	int len1 = p.size();
	for (int i = 0; i < len1; i++) {
		int len2 = p[i].size();
		for (int j = 0; j < len2; j++) {
			cout << p[i][j] << " ";
		}
		cout << endl;
	}
}

void Particle::printVelocity() {
	int len1 = v.size();
	for (int i = 0; i < len1; i++) {
		int len2 = v[i].size();
		for (int j = 0; j < len2; j++) {
			cout << v[i][j] << " ";
		}
		cout << endl;
	}
}

//Destructor
Particle::~Particle() {
	p.clear();
	v.clear();
}

