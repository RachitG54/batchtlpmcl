#!/bin/bash
for t in {100000000,1};
do
	for i in {3,4,5}; 
	do 
		./bin/testlibs trivial $((i*100)) $t > "logs/trivial"$((i*100))"time"$t; 
	done 
done