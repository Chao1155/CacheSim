/*
 * cache.h
 *
 *  Created on: Apr 13, 2015
 *      Author: chao
 */

#ifndef CACHE_H_
#define CACHE_H_

#include <iostream>
#include <vector>
#include "global.h"

#define MAX_REF 65500
using namespace std;

/*
 * cache line block class.
 * This now only contains tag, so I take it as a tag class.
 */
class Blk {
public:
	long long tag;
	bool valid;
	bool dirty;
	int refs;
	Blk() {
		tag = 0;
		valid = false;
		dirty = false;
		refs = 0;
	}
	void reset(unsigned _tag) {
		tag = _tag;
		valid = 1;
		dirty = 0;
		refs = 1;
	}
	friend ostream& operator<<(ostream& os, Blk& blk) {
		os << blk.tag;
		return os;
	}
};
enum CMD {
	READ, WRITE, RESPONSE, OTHER
};

/*
 * request class read from the trace files.
 */
class Request {
	friend class Scheduler;
public:
	CMD cmd;
	long long addr;
	long long time;
	Request(CMD _cmd, long long _addr, long long _time) {

		cmd = _cmd;
		addr = _addr;
		time = _time;
	}
	;
	friend ostream& operator<<(ostream& os, Request& req) {
		os << (req.cmd == READ ? "READ" : "WRITE") << ", " << req.addr << endl;
		return os;
	}
};

/*
 * The basic cache simulator class.
 * It contains basic tag compare and search operations.
 * This is a time irrelevant cache class.
 * Implement timing in descendants, please.
 */
class BaseCache {
private:
    // basic operation
	unsigned num_way, num_set;
    // cache bank
    unsigned num_bank, bankIntlvBit, bankIntlvLowBit, bank_move;
    unsigned long long bankIntlvMask;

	unsigned setIndexLength;
	vector<vector<Blk> > cache;

public:
	// temporary var.
	unsigned cur_set_index, cur_way_index;

	// Stats
	unsigned cache_hits;
	unsigned cache_misses;
	unsigned total_read, total_write, total_other;
	unsigned dirty_blocks, dirty_evicts;
    vector<unsigned> * bank_access;

    BaseCache(unsigned _num_set, unsigned _num_way, unsigned _num_bank = 1, unsigned _num_bank_move = 0);
	// Initial the cache stats;
	void InitialStats() {
		cache_hits = 0;
		cache_misses = 0;
		total_read = 0;
		total_write = 0;
		total_other = 0;
		dirty_blocks = 0;
		dirty_evicts = 0;
        bank_access = new vector<unsigned>(num_bank, 0);
	}
	;

	// Calculate the corresponding tag of the request.
	unsigned calTag(long long addr);
	// Calculate the corresponding set of the request.
	unsigned calSet(long long addr);

    // Calculate the bank index of the request.
    unsigned calBank(long long addr);

	// response with its hit and latency.
	bool accessTag(Request* req);

	// evict the dirty cache block to cout..
	void evictDirty(unsigned set_index, unsigned way_index);

	// replace a block for the incoming request.
	unsigned replace(long long addr);

	// find out the LRU block in a set
	unsigned findLRU(unsigned set_index);

	// find out the required block in a set
	// -1 indicates not found.
	int findBlk(unsigned set_index, unsigned req_tag);

	//print the content of the cache Tag.
	void printTag();

	//print the content of the cache bank.
	void printBank();
	
    // print the stats in this objects;
	void printStats();
};

#endif /* CACHE_H_ */
