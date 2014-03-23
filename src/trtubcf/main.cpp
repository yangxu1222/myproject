/*************************************************************************
	> File Name: main_utbcf.cpp
	> Author: yangxu07
	> Mail: yangxu07@baidu.com 
	> Created Time: 2014年03月11日 星期二 11时48分33秒
 ************************************************************************/

#include<iostream>
#include "Trtubcf.h"
#include "../trubcf/Trubcf.h"
#include "../tubcf/Tubcf.h"

using namespace std;

const char*  TRAINING_SET="/home/yangxu/work/myproject/input/movielens/u1.base"; //training set
const char*  PROBE_SET="/home/yangxu/work/myproject/input/movielens/u1.test";  //test set
const char*  RATE_SP="\t";  //rate Separator
const char* USER_SIM="/home/yangxu/work/myproject/output/user_sim"; 

vector<vector<rateNode> > rate_matrix(USER_NUM+1);   //store training set
vector<testSetNode> probe_row;                            //store test set

void model(int k_num){

	char buffer[20];
	vector<double> mean_rate;             
	int    biNum[USER_NUM+1] = {0.0};            
	float s[USER_NUM+1][USER_NUM+1] = {0.0};     
	float kmax[USER_NUM+1] = {0};              
	float mean = 0.0;

	sprintf(buffer,"%d",k_num);
	const string k_max_file = string("/home/yangxu/work/myproject/output/user_") + string(buffer) + string("_max");
	//cout << k_max_file << endl;	
	
	Trtubcf *rec = new Trtubcf(); 
	rec->load_train(TRAINING_SET,rate_matrix,RATE_SP);  //load training set
	rec->load_test(PROBE_SET,probe_row,RATE_SP);   //load test set
	
	rec->load_sim_matrix(s, USER_SIM);               //load sim matrix
	rec->load_k_max(kmax, k_max_file.c_str(),RATE_SP);                //load k-max

	mean_rate = rec->get_user_mean(rate_matrix);         //calculate the mean of each item
	
	map<int,float> t ;
	float alpha = 0.6;
	Tubcf *tubcf = new Tubcf();
	t = tubcf->wight_time(alpha,rate_matrix);
	cout << "caculate wt finished" << endl;

	cout <<"begin cal trust " << endl; 
	Trubcf *trubcf = new Trubcf();	
	map<int,float> trust = trubcf->cal_trust(rate_matrix,s,mean_rate);
    cout <<"end cal trust" << endl;

	cout <<"begin MAE " << endl;
	for (float beta = 0.0; beta < 1.05; beta += 0.05 ){
		rec->mae = rec->MAEProbe(probe_row,k_num,rate_matrix,s,kmax,t,trust,beta);
		cout <<"end MAE" << endl;
	}
}

int main(int argc,char ** argv){	
	int k_num = atoi(argv[1]);
	model(k_num);
	return 0;
}
