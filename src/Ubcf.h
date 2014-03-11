/*************************************************************************
	> File Name: Ubcf.h
	> Author: yangxu07
	> Mail: yangxu07@baidu.com 
	> Created Time: 2014年03月06日 星期四 16时30分43秒
 ************************************************************************/
#ifndef _UBCF_H_ 
#define _UBCF_H_

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cmath>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <ctime>

using namespace std;

const unsigned int USER_NUM=943; //10K:943 1M:6040
const unsigned int ITEM_NUM=1682; //10K:1682 1M:3900
const unsigned int MIN_RATE=1;    // the minimum of the rate
const unsigned int MAX_RATE=5;    // the maximum of the rate

struct rateNode{
	short item;
	short rate;
	long timestamp;
};
struct testSetNode{
	int user;
	short item;
	short rate;
	long timestamp;
};

class Ubcf{
	public:
		float mae;
	public:
		Ubcf(){
		};
		~Ubcf(){
		}; 
		void load_train(const char* file_name,vector<vector<rateNode> >& rate_matrix,const char* separator);
		void load_test(const char* file_name,vector<testSetNode>& probe_set,const char* separator);
		void load_sim_matrix(float s[USER_NUM+1][USER_NUM+1],const char *sim_file);
		void load_k_max(float k_max[USER_NUM+1],const char* file_name,const char* separator);
		double MAEProbe(vector<testSetNode>& probe_set,int dim,vector<vector<rateNode> > rate_matrix,float s[USER_NUM+1][USER_NUM+1],float kmax[USER_NUM+1]);
		float predict_rate(int user,int item,vector<vector<rateNode> > rate_matrix,vector<double> mean_rate,float s[USER_NUM+1][USER_NUM+1],float kmax[USER_NUM+1]);

	    vector<double> get_user_mean(const vector<vector<rateNode> > &rate_matrix);	

};


#endif //RECOMMENDER_H_
