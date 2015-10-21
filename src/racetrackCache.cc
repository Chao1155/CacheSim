/*
 * racetrackCache.cc
 *
 *  Created on: Apr 17, 2015
 *      Author: chao
 */


#include "racetrackCache.h"
#include <iomanip>

void
RacetrackCache::updateQuota(){
	// only refill the quota at the end of interval.
	long long time = clock.currentClock();
	if (time >= last_refill_time + MAX_REFILL_TIME){
		shift_quota = MAX_QUOTA;
		last_refill_time = time / MAX_REFILL_TIME * MAX_REFILL_TIME;
		quota_refill_time = 0;
	}else if (time >= last_refill_time){
		quota_refill_time = last_refill_time + MAX_REFILL_TIME - time;
	}
	else{
		cout << "[WARN] updateQuota before last refill!" << endl;
	}
}
bool
RacetrackCache::access(Request* req, unsigned* latency){

	clock.updateClock(req);
	// update quota before access.
	updateQuota();
	bool hit = accessTag(req);
	// count the shift first.
	unsigned rt_index = rtList->getRTIndex(cur_set_index, cur_way_index);
	unsigned domain_index = rtList->getDomainIndex(cur_set_index, cur_way_index);
	unsigned shift_dist = rtList->willShift(rt_index, domain_index);

	if (shift_dist < shift_quota){
		// quota is enough
		unsigned delta_time = 0;
		rtList->moveTo(rt_index, domain_index);
		shift_quota -= shift_dist;
		total_shift_latency += shift_dist * shift_latency;
		delta_time += shift_dist * shift_latency;
		// update hit/mis
		if (hit){
			total_latency += shift_dist * shift_latency ;
		}
		else{
			total_latency += shift_dist * shift_latency + miss_latency;
			delta_time += miss_latency;
		}
		clock.updateClock(delta_time);
	}
	else{
		// quota refill.
		total_refill_time += quota_refill_time;
		total_latency += quota_refill_time;
		// delay the this request.
		clock.reschedule(req, quota_refill_time);
		return false;
	}

	// count the read/write second.

	if (req->cmd == READ){
		total_latency += read_latency;
	}else if (req->cmd == WRITE){
		total_latency += write_latency;
	}
	else{
		total_latency += write_latency;
		//std:: cout << "[Warn] unrec command timing is ignored." <<endl;
	}
	// update read/write
	clock.updateClock(req->cmd == READ ? read_latency: write_latency);
	return true;
}

void
RacetrackCache::print(){
	cout << "The context of the Tags:" << endl;
	printTag();
	cout << "The context of the Racetracks:" << endl;
	rtList->print();
	return;
}

void
RacetrackCache::printStats(){
	long long total_clock = clock.currentClock();
	((BaseCache*)this)->printStats();
	cout << "total_latency:\t" << setw(16) << total_latency;
	cout << "(" << setw(16) << setprecision(4) << (double)total_latency/total_clock * 100 <<"%)"<< endl;
	cout << "shift_latency:\t" << setw(16) << total_shift_latency;
	cout << "(" << setw(16) << setprecision(4) << (double)total_shift_latency/total_clock * 100 <<"%)"<< endl;
	cout << "refill_time:\t" << setw(16) << total_refill_time;
	cout << "(" << setw(16) << setprecision(4) << (double)total_refill_time/total_clock * 100 <<"%)" << endl;
	cout << "cache_clock:\t" << setw(16) << total_clock << endl;
}



