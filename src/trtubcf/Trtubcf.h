/*************************************************************************
	> File Name: UTbcf.cpp
	> Author: yangxu07
	> Mail: yangxu07@baidu.com 
	> Created Time: 2014年03月10日 星期一 12时26分49秒
 ************************************************************************/

#include<iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cmath>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include "../Ubcf.h"

using namespace std;

class Trtubcf : public Ubcf{
	private:
		float predict_rate(int user,int item,vector<vector<rateNode> > rate_matrix,vector<double> mean_rate,float s[USER_NUM+1][USER_NUM+1],float kmax[USER_NUM+1],map<int,float> wt,map<int,float> trust,float beta);

	public:
		Trtubcf(){
		};
		~Trtubcf(){
		};
		map<int,float> wight_time(float alpha,vector<vector<rateNode> > rate_matrix);
		double MAEProbe(vector<testSetNode>& probe_set,int dim,vector<vector<rateNode> > rate_matrix,float s[USER_NUM+1][USER_NUM+1],float kmax[USER_NUM+1],map<int,float> wt,map<int,float> trust,float beta);
};
