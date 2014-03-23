/*************************************************************************
	> File Name: Trtubcf.cpp
	> Author: yangxu07
	> Mail: yangxu07@baidu.com 
	> Created Time: 2014年03月10日 星期一 12时36分33秒
 ************************************************************************/

#include<iostream>
#include <limits>
#include <cmath>
#include "Trtubcf.h"
using namespace std;

double Trtubcf::MAEProbe(vector<testSetNode>& probeSet,
		int dim,vector<vector<rateNode> > rate_matrix,
		float s[USER_NUM+1][USER_NUM+1],
	    float kmax[USER_NUM+1],
		map<int,float> wt,
		map<int,float> trust,
		float beta)
{
    int probeSize = probeSet.size();
    float pRate,err;
    long double mae = 0;

	vector<double> mean_rate = get_user_mean(rate_matrix);

    for(int i = 0; i < probeSize; ++i) {
        pRate = predict_rate(probeSet[i].user,probeSet[i].item,rate_matrix,mean_rate,s,kmax,wt,trust,beta);      //predict rate
//	    cout << "pRate:" << pRate<< endl;
        err = fabsf(pRate-probeSet[i].rate);
        mae += err;
    }
    mae = mae / probeSize;
//	string retfile("retfile");
//	ofstream to(retfile.c_str(),ios_base::app);
//	to << dim <<":"<<mae<<endl;
    cout<<"beta:" << beta << " MAE:"<<mae<<" probeNum:"<<probeSize<<endl;
    return mae;
}

/**
 * predict the rate
 */
float Trtubcf::predict_rate(int user, int item,
		vector<vector<rateNode> >rate_matrix,
		vector<double> mean_rate,
		float s[USER_NUM+1][USER_NUM+1],
	    float kmax[USER_NUM+1],
		map<int,float> wt,
		map<int,float> trust,
		float beta)
{
    int r_num = rate_matrix[user].size(); //the num of items rated by user(用户user打过分的item数目)
    double sum_numerator = 0.0;
    double sum_denominator = 0.0;
   
	for (int user_i = 1; user_i < USER_NUM; ++user_i){
		int rate = 0;
	    float similarity = s[user][user_i];
		if (similarity < kmax[user])continue;
		else{
			float w = beta * wt[user_i] + (1.0-beta) * trust[user_i];
//			float w1 = (2 * wt[user_i] * similarity) / (wt[user_i] + similarity ) ;					
//			float w2 = (2 * trust[user_i] * similarity) / (trust[user_i] + similarity ) ;					
//			float wight = beta * w1 + (1-beta) * w2;
			float wight = (2 * w * similarity) / (w + similarity ) ;					
//			float wight =  w * similarity;					
			sum_denominator += fabs(wight);
			for (int p = 0; p < rate_matrix[user_i].size();++p){
				if (item == rate_matrix[user_i][p].item){
					rate = rate_matrix[user_i][p].rate;
//					cout << "rate:"<< rate<<"," <<user_i<<":"<< mean_rate[user_i] << endl;
					sum_numerator += wight * (rate-mean_rate[user_i]);
//					cout << "mean_rate[user]:"<<mean_rate[user_i]<<endl;
					break;
				}
			}
		}
	}
//	cout << "sum_numerator: "<< sum_numerator<<endl;
//	cout << "sum_denominator:"<<sum_denominator<<endl;
//	cout << "mean_rate[user]:"<<mean_rate[user]<<endl;
    float ret =0.0;
    if(sum_denominator > 0.002) ret =  mean_rate[user] + (sum_numerator/sum_denominator);
    else ret =  mean_rate[user];
    if(ret < MIN_RATE) ret = MIN_RATE;
    if(ret > MAX_RATE) ret = MAX_RATE;
//	cout << "ret: "<<ret<<endl;
    return ret;
}
