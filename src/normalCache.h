/*
 * normalCache.h
 * Normal cache model, considering the timing.
 *
 *  Created on: Oct 13, 2015
 *      Author: CharleZ
 */


#ifndef NORMALCACHE_H_
#define NORMALCACHE_H_

#include <iostream>
#include "baseCache.h"
#include <iomanip>

using namespace std;

class NormalCache: public BaseCache{
private:
    static const unsigned CYCLE_TIME = 500;
public:
	//stats
	long long total_latency;
	unsigned read_latency, write_latency, miss_latency;



public:
	NormalCache(unsigned _num_set, unsigned _num_way, unsigned _num_bank, unsigned _num_bank_move):
	BaseCache(_num_set, _num_way, _num_bank, _num_bank_move){
		write_latency = 2 * CYCLE_TIME;
		read_latency = 1 * CYCLE_TIME;
		miss_latency = 10 * CYCLE_TIME;
		total_latency = 0;
	};

	// access tag first, then data.
	// true if the time is ok
	// false if block.
	// the latency will returns a access latency;
	bool access(Request* req, unsigned* latency);

	//debug & output

	// print the content of the cache.
	void print(){return;}

	// print the stats in this objects;
	void printStats(){
	    ((BaseCache*)this)->printBank();
	    ((BaseCache*)this)->printStats();
    	cout << "access_latency:\t" << setw(16) << total_latency << " (tick)"<< endl;
    }
};


#endif /* normalcache_h_ */
