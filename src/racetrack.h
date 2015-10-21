/*
 * racetrack.h
 *
 *  Created on: Apr 17, 2015
 *      Author: chao
 */

#ifndef RACETRACK_H_
#define RACETRACK_H_

#include <vector>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

class RacetrackArray;

/*
 * This basic class represents a racetrack group.
 * This only includes basic operation, without timing or energy.
 * This class take lazy port migration as default.
 */
class Racetrack {
	friend RacetrackArray;

	unsigned domain_num;
	unsigned port_num;
	unsigned segment_length;
	unsigned cur_position;

public:
	Racetrack(unsigned _domain_num, unsigned _port_num) {
		// Ignore odd domains, if the number cannot be divided by port.
		domain_num = _domain_num;
		port_num = _port_num;
		segment_length = domain_num / port_num;
		cur_position = segment_length / 2 - 1;
	};

	// Move port head to required position.
	unsigned moveTo(unsigned dest) {
		unsigned delta = abs(dest - cur_position);
		cur_position = dest;
		return delta;
	}

	// get the shift_distance;
	// WARN: this may cause race when two requests get to same rt but different domain.
	unsigned willShift(unsigned dest){
		return abs(dest - cur_position);
	}
};

/*
 * This class gives the mapping between logical index of a cache block to a list of racetrack group.
 * Hack the get*Index function to implement your mapping!
 *
 */
class RacetrackArray {
	unsigned spread_k;
	unsigned domain_length;
	unsigned ways;
	vector<Racetrack> racetrackList;
	unsigned rtListSize;

public:
	RacetrackArray(unsigned _sets, unsigned _ways, unsigned _domain_num,
			unsigned _port_num) {
		spread_k = _ways;
		assert(_ways % spread_k == 0);
		ways = _ways;
		domain_length = _domain_num / _port_num;
		racetrackList.resize(_sets * _ways / domain_length, Racetrack(_domain_num, _port_num));
		rtListSize = racetrackList.size();
	}

	unsigned getRTIndex(unsigned set_index, unsigned way_index) {
		return way_index % spread_k + set_index * ways / domain_length;
	}
	unsigned getDomainIndex(unsigned set_index, unsigned way_index) {
		return ((set_index * ways + way_index) / spread_k) % domain_length;
	}
	unsigned getSize() {
		return racetrackList.size();
	}
	unsigned moveTo(unsigned rt_index, unsigned domain_index){
		return racetrackList[rt_index].moveTo(domain_index);
	}
	unsigned willShift(unsigned rt_index, unsigned domain_index){
		return racetrackList[rt_index].willShift(domain_index);
	}
	void print(){
		for (unsigned i = 0; i < rtListSize; i++) {
			cout << i << ": ";
			cout << setw(0) << racetrackList[i].cur_position << endl;
		}
	}

};

#endif /* RACETRACK_H_ */
