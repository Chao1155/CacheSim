/*
 * The Main entry of CacheSim
 * Functions:
 *      help manu
 *      main trace loop
 *
 *  Created on: Apr 13, 2015
 *  Modified on: Oct 13, 2015
 *      Author: CharleZ
 */

#include "racetrackCache.h"
#include "normalCache.h"
#include "trace_parser.h"
#include "global.h" // for translateSize
#include <unistd.h> // for getopt
#include <string>

//#define status

using namespace std;


int main(int argc, char *argv[]) {
    //cout << argc << endl;
    if (argc < 2){
        cerr << "You need options. Dear!" << endl;
        cerr << "Usage: " << argv[0] << " -iswdpt \n" << endl;
        exit(EXIT_FAILURE);
    }
        
	string file_name = "";
	long cache_size = 0;
    int mem_type = 0;   // 0: normal cache, 1: racetrack memory
	unsigned int num_set = 2, num_way = 2, num_RTdomain = 32, num_RTport = 1;
    unsigned int num_bank = 1, num_bank_move = 0;

	int optchar;
	while((optchar = getopt(argc,argv,"i:s:w:d:p:t:b:m:")) != -1){
		switch(optchar){
			case 'i':// input file path
				file_name = optarg;
				cout << "Reading " << file_name << endl;
				break;
			case 's':// cache size
				cache_size = translateSize((string)optarg); 
				break;
			case 'w':// associativity
				num_way = atoi(optarg);
				break;
			case 'd':// domain number
				num_RTdomain = atoi(optarg);
				break;
			case 'p':// port number
				num_RTport = atoi(optarg);
				break;
            case 't':// type of cache
                if (atoi(optarg) == 0)
                    mem_type = 0;
                else
                    mem_type = 1;   //@TODO: improve this when you have more types.
            case 'b':// number of banks
                num_bank = atoi(optarg);
            case 'm':// number of bank index left move
                num_bank_move = atoi(optarg);
			default:
                continue;
		}
	}

	num_set = cache_size / 64 / num_way;
	if (num_set == 0){
        fprintf(stderr, "[ERROR] The set number is 0. \n");
        exit(EXIT_FAILURE);
	}
	//cout << num_set << endl;
	Parser parser;
	parser.get_file_contents(file_name.c_str());
	//parser.get_file_contents(file_name);
	parser.input_request();
#ifdef status
	long long processed_requests = 0;
	long long total_requests = parser.queue_size();
#endif
	//Request & req = parser.get_request();
	//cout << req;
    if (mem_type == 0){
	    cout << "Cache config:\n" << "set: " << num_set << ", way: " <<num_way << endl;
        //BaseCache cache(num_set, num_way);
        NormalCache *cache = new NormalCache(num_set, num_way, num_bank, num_bank_move);
	    while (!parser.queue_empty()){
	    	unsigned latency = 0;
	    	bool not_delayed = cache->access(& parser.get_request(), &latency);
	    	parser.pop_request();
#ifdef status
	    	processed_requests ++;
	    	if (processed_requests % 100000 == 0){
	    		float processed_ratio =  (float) processed_requests / total_requests;
	    		cout << processed_requests << " requests simulated. (" << setprecision(3) << processed_ratio * 100  << "%)"<< endl;
	    	}
#endif
        }
	    cache->printStats();
        delete cache;
    }
    else{
	    cout << "Cache config:\n" << "set: " << num_set << ", way: " <<num_way 
            <<", domain: " << num_RTdomain <<", port: " << num_RTport << endl;
    	RacetrackCache *cache = new RacetrackCache(num_set, num_way, num_RTdomain, num_RTport);
    	while (!parser.queue_empty()){
    		unsigned latency = 0;
    		bool not_delayed = cache->access(& parser.get_request(), &latency);
    		if (not_delayed){
    			parser.pop_request();
    #ifdef status
    			processed_requests ++;
    			if (processed_requests % 10000 == 0){
    				float processed_ratio =  (float) processed_requests / total_requests;
    				cout << processed_requests << " requests simulated. (" << setprecision(3) << processed_ratio * 100  << "%)"<< endl;
    			}
    #endif
    		}
    		else{
    			//the request has been rescheduled to future, but still at the top of queue.
    			//So do nothing.
    
    		}
    	}
	    cache->printStats();
        delete cache;
    }
	//cache.print();
	return 0;

}

