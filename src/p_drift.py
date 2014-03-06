#!/usr/bin/env python
# coding=utf-8

import sys
import os

from time import localtime

#input_data = open('/home/yangxu/dataSet/movielens/ml-100k/u.data')
input_data = open('/home/yangxu/dataSet/movielens/ml-1m/ratings.dat')
d = dict()
for line in input_data:
	line = line.strip()
	user,item,rate,time = line.split('::',-1)
	i_user = int(user)
	if not d.has_key(user):
		d[i_user] = list()
	d[i_user].append((rate,time))

for index in range(1,944):
	temp_set = set()
	for i in d[index]:
		year = localtime(int(i[1])).tm_mon
		temp_set.add(year)
#		print i[0],year
	print index,len(temp_set)
