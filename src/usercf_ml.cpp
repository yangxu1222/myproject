/* Copyright (C) 2011 Lv Hongliang. All Rights Reserved.
 * please maitain the copyright information completely when you redistribute the code.
 * 
 * If there are some bugs, please contact me via email honglianglv@gmail.com or submit the bugs 
 * in the google code project http://code.google.com/p/recsyscode/issues/list
 * 
 * my blog: http://lifecrunch.biz
 * my twitter: http://twitter.com/honglianglv
 * my google profile:https://profiles.google.com/honglianglv/about?hl=en
 *
 * It is free software; you can redistribute it and/or modify it under 
 * the license GPLV3.
 *
 * The purpose of this program is to implement the KNN model, using the movielens dataset.
 *
 */

#include "./usercf_base.cpp"
int main(int argc, char ** argv)
{
    time_t start,end;
    struct tm * time_start_info;
    struct tm * time_end_info;
    double duration; 
    start = time(NULL);
    time_start_info = localtime(&start);
    string time_start_str = asctime(time_start_info);
	
	int k_num = atoi(argv[1]);	
    userCF::model("user_sim",k_num);

	end = time(NULL);
    duration = (end-start);
    time_end_info = localtime(&end);
	string time_end_str = asctime(time_end_info);
    cout << "Start at "<<time_start_str<<", and end at "<< time_end_str;
    cout << "duration:"<<duration <<" s!" <<endl;
    return 0;
}
