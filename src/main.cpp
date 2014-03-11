/*************************************************************************
	> File Name: ubcf_1.cpp
	> Author: yangxu07
	> Mail: yangxu07@baidu.com 
	> Created Time: 2014年03月06日 星期四 18时24分11秒
 ************************************************************************/

#include<iostream>
#include "Ubcf.h"

//#include "common.h"

using namespace std;

const char*  TRAINING_SET="../input/movielens/u1.base"; //training set
const char*  PROBE_SET="../input/movielens/u1.test";  //test set
const char*  RATE_SP="\t";  //rate Separator
const char* USER_SIM="../output/user_sim"; 

vector<vector<rateNode> > rate_matrix(USER_NUM+1);   //store training set
vector<testSetNode> probe_row;                            //store test set

void model(int k_num)
{
	char buffer[20];
	vector<double> mean_rate;             
	//store the mean rating of each user 
	int    biNum[USER_NUM+1] = {0.0};            
	//the rating num of every (打过item i分的用户总数)
	float s[USER_NUM+1][USER_NUM+1] = {0.0};     
	//item-item similarity matrix(user-user相似矩阵)
	float kmax[USER_NUM+1] = {0};              
	//k-max array ,the k-max similarity of each user
	float mean = 0.0;

	sprintf(buffer,"%d",k_num);
	const string k_max_file = string("../output/user_") + string(buffer) + string("_max");
	
	Ubcf rec; 
	cout << "begin initialization" << endl;
	rec.load_sim_matrix(s, USER_SIM);               //load sim matrix
	cout << "load_sim finished" << endl;
	rec.load_k_max(kmax, k_max_file.c_str(),RATE_SP);                //load k-max
    
	rec.load_train(TRAINING_SET,rate_matrix,RATE_SP);  //load training set
	rec.load_test(PROBE_SET,probe_row,RATE_SP);   //load test set
	
	cout << "len of rate_matrix: "<< rate_matrix.size() <<endl;
//    mean = set_mean_rating(USER_NUM,rate_matrix); //calculate the mean ????
	
	mean_rate = rec.get_user_mean(rate_matrix);         //calculate the mean of each item
    cout <<"begin MAE " << endl; 
	rec.mae = rec.MAEProbe(probe_row,k_num,rate_matrix,s,kmax);
    cout <<"end MAE" << endl;
    return;
}

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
    model(k_num);
	
	end = time(NULL);
    duration = (end-start);
    time_end_info = localtime(&end);
	string time_end_str = asctime(time_end_info);
//    cout << "Start at "<<time_start_str<<", and end at "<< time_end_str;
    cout << "duration:"<<duration <<" s!" <<endl;
    return 0;
}
