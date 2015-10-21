/*
 * global.cc
 *
 *  Created on: Apr 17, 2015
 *      Author: chao
 */

#include <string>
#include <iostream>
#include <stdlib.h>
using namespace std;


// ceil to integer of the result.
unsigned log2(unsigned x) {
	unsigned n = 0;
	while (x > 1) {
		x = x / 2;
		n++;
	};
	return n;
}


int translateSize (string str_size)
{
	int size_value = -1;
//std::cout << "translating: " << str_size << '\n';
	int found_mb = str_size.find_last_of("Mm");
	int found_kb = str_size.find_last_of("Kk");
	int found_b = str_size.find_last_of("Bb");

	if (found_mb != -1){
		size_value = atoi(str_size.substr(0,found_mb).c_str()) * 1024 * 1024;
	} else if (found_kb != -1){
		size_value = atoi(str_size.substr(0,found_kb).c_str()) * 1024;
	}else if (found_b != -1){
		size_value = atoi(str_size.c_str());
	} else{
		cout << "Size translate warning." << endl;
	}
	return size_value;
}
