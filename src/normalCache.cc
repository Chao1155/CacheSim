/*
 * normalCache.cc
 *
 * Created on: Oct 20, 2015
 *     Author: Chao
 */

#include "normalCache.h"

bool 
NormalCache::access(Request* req, unsigned* latency){
    bool hit = accessTag(req);
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
    return true;
}
