#!/bin/bash
path=$(dirname $(pwd))
echo $path
cd $(dirname $(pwd))
for ((  i = 100; i < 1000 ; i+=100))
do
	./get_k_maxth $i
#	g++ -c usercf_ml.cpp -o usercf
	if [ $? -eq 0 ]
	then
		./usercf $i
	fi
	echo $i
done
