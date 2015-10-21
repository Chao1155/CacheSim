/*
 * racetrackCache.h
 *
 *  Created on: Apr 17, 2015
 *      Author: chao
 */

#ifndef RACETRACKCACHE_H_
#define RACETRACKCACHE_H_

#include <vector>
#include "racetrack.h"
#include "baseCache.h"

using namespace std;

/*
 * responsible for request scheduling, global timing.
 */
class Scheduler{

	long long cache_time;
public:
	Scheduler(){
		cache_time = 0;

	}
	// increase the delta time of this request.
	void reschedule(Request * req, long long delay){req->time += delay;}

	// update the global_clock according to the given req or delta time.
	void updateClock(Request * req){cache_time += req->time;}
	void updateClock(unsigned delta_time){cache_time += delta_time;}

	long long & currentClock(){return cache_time;}
};

/*
 * This class implement a real racetrack cache, based on BaseCache.
 * It provides timing of read, write and shift.
 * This cache use racetracks ONLY as data, thus they move only when we access data.
 */
class RacetrackCache: public BaseCache{
	// rtList used to locate the data blocks.
	RacetrackArray * rtList;
	unsigned read_latency, write_latency, shift_latency, miss_latency;

	// time aware scheduler
	Scheduler clock;

	unsigned shift_quota;		// the remain avaible shift amount.
	long long quota_refill_time;	// refill time.
	long long last_refill_time;		// time unit is tick.

	// a const or a variable
	static const long MAX_QUOTA = 1000;
	static const unsigned CYCLE_TIME = 500;
	static const long long MAX_REFILL_TIME = 2000 * CYCLE_TIME;
public:
	//Stats
	long long total_latency, total_shift_latency;
	long long total_refill_time;


public:
	RacetrackCache(unsigned _num_set, unsigned _num_way, unsigned _domain_num, unsigned _port_num):
	BaseCache(_num_set, _num_way){
		write_latency = 2 * CYCLE_TIME;
		read_latency = 1 * CYCLE_TIME;
		shift_latency = 1 * CYCLE_TIME;
		miss_latency = 10 * CYCLE_TIME;

		rtList = new RacetrackArray(_num_set, _num_way, _domain_num, _port_num);

		total_latency = 0;
		total_shift_latency = 0;

		shift_quota = MAX_QUOTA;
		quota_refill_time = MAX_REFILL_TIME;
		last_refill_time = 0;
		total_refill_time = 0;
	};

	// update the quota status.
	void updateQuota();

	// access tag first, then data.
	// true if the time is ok
	// false if block.
	// The latency will returns a access latency;
	bool access(Request* req, unsigned* latency);

	//Debug & Output

	// print the content of the cache.
	void print();

	// print the stats in this objects;
	void printStats();
};


#endif /* RACETRACKCACHE_H_ */
