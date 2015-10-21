# CacheSim README #

This README would introduce all necessary informations you need to know about this simulator.

### What is CacheSim? ###

* CacheSim is a tracebased simple architecture simulator for cache simulation. 
* Compared to other widely used simulator, this one is a home-made, easy to read and modify, fast to run and better custermized choice. It's generally very hard to modify a sophisticated simulator based on inmature idea. It's very erronous and needs lot of effort to debug. With this one, you can simple implement your idea. The debug is very easy with eclipse or other IDE or gdb.
* Currently, it is a single port, signle level simulator, and supports only LRU replacement policy.
* The simulation framework is motivated from gem5 classic cache model, and can be taken as a reduced version of it. 
* The simulator is not validated against any others. So be careful about this.

### How to use this simulator? ###

Since this simulator is prety easy and writen by c++/python, you should be able to understand the codes in a short time.
To start with, you could go to the sample directory, in where the sample run files are located. Simply cammand:
> ./sample_run.py

The cacheSim has several options, please read the src/main.cc for reference.

### What files should I commit? ###

* This repo is based on mercurial. So you can use following command to clone the repo to your local directory.
> hg clone https://yourid@bitbucket.org/shuihan/cachesim
* To commit, please use hg addremove to add files, commit to creat a new version, and push to send it to central repo.

* Pay attention to the .hgignore file. This file contains a ignore list in which files will not be commited to this central repo. Do add unwanted files into it and list them here for others benefits.


