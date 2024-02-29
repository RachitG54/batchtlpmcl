#!/bin/bash
for t in {1000000,5000000,10000000,50000000,100000000,500000000,1000000000};
do
	for i in {1..5}; 
	do 
		./bin/testlibs optimized $((i*100)) $t > "logs/optimized"$((i*100))"time"$t; 
	done 
done