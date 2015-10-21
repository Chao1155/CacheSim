/*
 * cache.cc
 *
 *  Created on: Apr 13, 2015
 *      Author: chao
 */
// simple set associative cache

#include "baseCache.h"
#include <assert.h>
#include <iomanip>


using namespace std;
unsigned
BaseCache::calSet(long long addr){
	return (unsigned)((addr & 0xfffffffffffffc0) >> 6) % num_set;
}
unsigned
BaseCache::calTag(long long addr){
	return (addr & 0xffffffffffffffc0) >> (6+ setIndexLength);
}

unsigned
BaseCache::calBank(long long addr){
	return (addr & bankIntlvMask) >> bankIntlvLowBit;
}

BaseCache::BaseCache(unsigned _num_set, unsigned _num_way, unsigned _num_bank, unsigned _num_bank_move) {
	num_way = _num_way;
	num_set = _num_set;
	cur_set_index = 0;
	cur_way_index = 0;
	setIndexLength = log2(num_set);
	cache.resize(num_set);
	Blk InitialBlk;
	for (unsigned i = 0; i < num_set; i++) {
		cache[i].resize(num_way, InitialBlk);
	}

    num_bank = _num_bank;
    bankIntlvBit = log2(num_bank);
    bank_move = _num_bank_move;
    bankIntlvLowBit = log2(64) + bank_move;     // you can push the mask left by increasing move
    bankIntlvMask = (((unsigned long long)(1) << (bankIntlvBit + 1)) - 1)     // the length of the bank bit in between 00000xxx0 
        << (bankIntlvLowBit - 1);
    //cout << bankIntlvMask << endl;
    assert(bank_move >= 0);
    assert(bank_move <= setIndexLength - bankIntlvBit);
	InitialStats();
}
	
bool
BaseCache::accessTag(Request* req){
	long long addr = req->addr;
	unsigned set_index = calSet(addr);
	unsigned tar_tag = calTag(addr);
    unsigned bank_index = calBank(addr);
	bool hit;
	int found_way = findBlk(set_index, tar_tag);
	if (found_way != -1){// hit
		hit = true;
		cache[set_index][found_way].refs ++;
		cache_hits ++;
	}
	else{// mis
		hit = false;
		// miss response.
		found_way = replace(addr);
		cache_misses ++;
	}
	if (req->cmd == READ){
		total_read ++;
	}
	else if (req->cmd == WRITE){
		total_write ++;
		cache[set_index][found_way].dirty = true;
		dirty_blocks ++;
	}else{
		total_other ++;
	}
	cur_set_index = set_index;
	cur_way_index = found_way;

	//cout << addr << " " << (hit?"hit":"miss") << endl;
    bank_access->at(bank_index) ++;
	return hit;
}

void
BaseCache::evictDirty(unsigned set_index, unsigned way_index){
	//cout << cache[set_index][way_index];
	dirty_evicts ++;
	dirty_blocks --;

}

unsigned
BaseCache::findLRU(unsigned set_index){
	int temp = MAX_REF;
	unsigned LRU = MAX_REF;
	for (unsigned i=0; i<num_way ; i++){
		if (cache[set_index][i].refs < temp){
			temp = cache[set_index][i].refs;
			LRU = i;
		}
	}
	assert(LRU < MAX_REF);
	return LRU;
}

int
BaseCache::findBlk(unsigned set_index, unsigned req_tag){
	for (unsigned i=0; i<num_way ; i++){
		if (cache[set_index][i].tag == req_tag && cache[set_index][i].valid){
			return (int)i;
		}
	}
	return -1;
}

unsigned
BaseCache::replace(long long addr){
	unsigned set_index = calSet(addr);
	unsigned way_index = findLRU(set_index);

	if (cache[set_index][way_index].dirty)
		evictDirty(set_index,way_index);
	// assign the new data block.
	cache[set_index][way_index].reset(calTag(addr));
	return way_index;

}

void
BaseCache::printTag(){
	for (unsigned i = 0; i<num_set; i++){
		cout << i << ": ";
		for (unsigned j = 0; j < num_way; j++){
			cout << "(" << setw(5) << cache[i][j].tag << "," << setw(3) << cache[i][j].refs <<  ","<< setw(2) << cache[i][j].dirty << ")\t";
		}
		cout << endl;
	}
}
void
BaseCache::printBank(){
    cout << "Bank Access: " << endl;
    for (unsigned i=0; i<num_bank; i++){
        cout << bank_access->at(i) << " ";
    }
    cout << endl;
}

void
BaseCache::printStats(){
	cout << "cache_hits:\t" << setw(16) << cache_hits << endl;
	cout << "cache_misses:\t" << setw(16) << cache_misses << endl;
	cout << "total_read:\t" << setw(16) << total_read << endl;
	cout << "total_write:\t" << setw(16) << total_write << endl;
	cout << "total_other:\t" << setw(16) << total_other << endl;
	cout << "dirty_blocks:\t" << setw(16) << dirty_blocks << endl;
	cout << "dirty_evicts:\t" << setw(16) << dirty_evicts << endl;
}
