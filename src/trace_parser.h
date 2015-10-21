/*
 * trace_parser.h
 *	This header is used to collect trace from files.
 *	The context is buffered in a string first, and then put into queue filled by Request class.
 *  Created on: Apr 14, 2015
 *      Author: chao
 */

#ifndef TRACE_PARSER_H_
#define TRACE_PARSER_H_
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <cerrno>
#include <string>

using namespace std;
class Request;

class Parser{
private:
	string stringinputdata;
	istringstream infile;
	queue<Request> req_queue;
    unsigned long num_parsed;

public:
    Parser(){
        num_parsed = 0;
    }
	void get_file_contents(const char *filename)
	{
		//get the contents of file to the string.
		std::ifstream in(filename, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);		// Set to the end of instream.
			stringinputdata.resize(in.tellg());	// Set the size of in to string.
			in.seekg(0, std::ios::beg);		// Set the pointer at the begin.
			in.read(&stringinputdata[0], stringinputdata.size());	// read from in.
			in.close();
			// cut the final empty line.
			stringinputdata.resize(stringinputdata.size() -1);
			reset();
            cout <<"File loaded." << endl;
			return;
		}
		throw(errno);
	}
	void reset(){
		infile.clear();
		infile.str(stringinputdata);
		infile.seekg(0, std::ios::beg);
		//std::cout << infile.str();
        num_parsed = 0;
	}
	void input_request(){
		long long lastreq_time = 0;
		while (!infile.eof()) {
			//string line_buf = "";
			char cmd_c = 'E';
			long long addr = 0;
			long long time = 0;
			long long dontcare = 0;
            
            // put the contents into a line
            string linebuffer;
            getline(infile, linebuffer);
            istringstream line_in (linebuffer);
			// read from the line to prevent unfortunate endings.
            line_in >> cmd_c;
            line_in.ignore(1);
			line_in >> addr;
            line_in.ignore(1);
			line_in >> dontcare;
            line_in.ignore(1);
			line_in >> dontcare;
            line_in.ignore(1);
            line_in >> time;
            // judge depending on the contents.
            // there are multiple trace formats:
            //  for one is; r,2138574848,64,512,5223946179000
            //              cmd, addr, size, tag, time(ps)
			CMD cmd;
			if (cmd_c == 'r')
				cmd = READ;
			else if (cmd_c == 'w')
				cmd = WRITE;
			else if (cmd_c == 'b')
				cmd = RESPONSE;
			else if (cmd_c == 'u')
				cmd = OTHER;
			else{
				cout << "[Error] Command unrec:" << cmd_c << endl;
                exit(-2);
            }
	        if (num_parsed == 0)
                lastreq_time = time;
            //cout << time<< endl;
			time -= lastreq_time;
			lastreq_time += time;
			// NOTE: the time here is redefined as the interval distance between two requests.
            //cout << "CMD " << cmd_c << ",ADDR " << addr << ",TIME " << time << endl;
			req_queue.push(Request(cmd, addr, time));
            num_parsed ++;
//            if (num_parsed % 1000 == 0) cout << num_parsed << endl;
		}
        cout << "Requests parsed (" << req_queue.size()  << ")."<< endl;
	}
	Request& get_request(){
		return req_queue.front();
	}
	void pop_request(){
		req_queue.pop();
	}
	bool queue_empty(){
		return req_queue.empty();

	}
	long long queue_size(){
		return req_queue.size();
	}

};
#endif /* TRACE_PARSER_H_ */
