/*************************************************************************
	> File Name: Tubcf.cpp
	> Author: yangxu07
	> Mail: yangxu07@baidu.com 
	> Created Time: 2014年03月10日 星期一 12时36分33秒
 ************************************************************************/

#include<iostream>
#include <limits>
#include <cmath>
#include "Tubcf.h"
using namespace std;

map<int,int> Tubcf::get_best_t(vector< vector<rateNode> >& rate_matrix){
	
	const long T0 = 5 * 24*60*60;
	const long T = 10 * 24*60*60;
	const int N = 10;

	//设定当前时间为98年5月1日 因为数据集是97年9月17日 到98年4月22日
	time_t  cur_time;
	struct tm *cur_info;
	time(&cur_time);
	cur_info = localtime(&cur_time);

	cur_info->tm_year = 98;
	cur_info->tm_mon = 4;
	cur_info->tm_mday = 1;
	cur_info->tm_hour = 0;
	cur_info->tm_min = 0;
	cur_info->tm_sec = 0;
	
	long t_cur = (long)mktime(cur_info);
	//cout << "t_cur:" << t_cur << endl;

/*
	char now[64];
	time_t tt = 893952000;
	struct tm *ttime = localtime(&tt);
	strftime(now,64,"%Y-%m-%d %H:%M:%S",ttime);
	cout << "time is " << now << endl;
*/
	//获取user进入系统的时间
	map<int,int> ret;
	for (int i =1;i < rate_matrix.size(); ++i){
//		cout << "i: " << i << endl;
		vector<rateNode> tmp = rate_matrix[i];
		if (tmp.size() < 10) {
			ret.insert(pair<int,int>(i,10));
		    continue;	
		}
		for (long t = T ; t < numeric_limits<long>::max() ; t+=T0){
//			cout << "t:" << t/(24*60*60) << endl;
			int num = 0;	
			vector<rateNode>::iterator it = tmp.begin();
			for (;it != tmp.end(); ++it){
				long time = (*it).timestamp;
				long min = t_cur -t;
				if ( time >= min && time <= t_cur){
					num++;
				}
			}
//			cout << "num : " << num << endl;
			if (num >= N){
				int t_value = t / (24 *60*60);
				ret.insert(pair<int,int>(i,t_value));
				break;
			}
		}
	}
	
//	cout << "len of ret : " << ret.size() << endl;	
//	map<int,int>::iterator it = ret.begin();
//	for(;it != ret.end(); ++it){
//		cout << it->first << "=>" << it->second << endl;
//	}
	return ret;
}

map<int,long> Tubcf::get_first_time(vector<vector<rateNode> > rate_matrix){
	map<int,long> ret ; 
	for (int i =1;i < rate_matrix.size(); ++i){
		long min = numeric_limits<long>::max(); 
		vector<rateNode> tmp = rate_matrix[i];
		vector<rateNode>::iterator it = tmp.begin();
		for (; it!=tmp.end(); ++it){
			long time  = (*it).timestamp;
			if (time < min){
				min = time;
			}
		}
		ret.insert(pair<int,long>(i,min));
	}
	return ret;
}

map<int,float> Tubcf::wight_time(float alpha,vector<vector<rateNode> > rate_matrix){
	map<int,float> ret;
	
	map<int,long> first_time = get_first_time(rate_matrix);
	map<int,int> t = get_best_t(rate_matrix);

//	map<int,long>::iterator it = first_time.begin();
//	for(;it!= first_time.end(); ++it){
//		cout << it->first << "=>" << it->second << endl;
//	}
//
	//设定当前时间为98年5月1日 因为数据集是97年9月17日 到98年4月22日
	time_t  cur_time;
	struct tm *cur_info;
	time(&cur_time);
	cur_info = localtime(&cur_time);

	cur_info->tm_year = 98;
	cur_info->tm_mon = 4;
	cur_info->tm_mday = 1;
	cur_info->tm_hour = 0;
	cur_info->tm_min = 0;
	cur_info->tm_sec = 0;
	
	long t_cur = (long)mktime(cur_info);

	for (int i =1; i < rate_matrix.size(); ++i){
		long t_first = first_time[i];
		int duration = abs(t_cur-t_first)/(24*60*60);
//		cout << t_first << " : " << duration<< endl;
		float wt =  exp(-alpha * duration / t[i]);
//		cout << wt<< endl;
		ret.insert(make_pair(i,wt));
	}
	map<int,float>::iterator it = ret.begin();

//	for(;it!= ret.end(); ++it){
//		cout << it->first << "=>" << it->second << endl;
//	}

	return ret;
}

//compute the mae of test set
double Tubcf::MAEProbe(vector<testSetNode>& probeSet,
		int dim,vector<vector<rateNode> > rate_matrix,
		float s[USER_NUM+1][USER_NUM+1],
	    float kmax[USER_NUM+1],
		map<int,float> wt)
{
    int probeSize = probeSet.size();
    float pRate,err;
    long double mae = 0;

	vector<double> mean_rate = get_user_mean(rate_matrix);

    for(int i = 0; i < probeSize; ++i) {
        pRate = predict_rate(probeSet[i].user,probeSet[i].item,rate_matrix,mean_rate,s,kmax,wt);      //predict rate
//	    cout << "pRate:" << pRate<< endl;
        err = fabsf(pRate-probeSet[i].rate);
        mae += err;
    }
    mae = mae / probeSize;
//	string retfile("retfile");
//	ofstream to(retfile.c_str(),ios_base::app);
//	to << dim <<":"<<mae<<endl;
    cout<<"MAE:"<<mae<<" probeNum:"<<probeSize<<endl;
    return mae;
}

/**
 * predict the rate
 */
float Tubcf::predict_rate(int user, int item,
		vector<vector<rateNode> >rate_matrix,
		vector<double> mean_rate,
		float s[USER_NUM+1][USER_NUM+1],
	    float kmax[USER_NUM+1],
		map<int,float> wt)
{
    int r_num = rate_matrix[user].size(); //the num of items rated by user(用户user打过分的item数目)
    double sum_numerator = 0.0;
    double sum_denominator = 0.0;
   
	for (int user_i = 1; user_i < USER_NUM; ++user_i){
		int rate = 0;
	    float similarity = s[user][user_i];
		if (similarity < kmax[user])continue;
		else{

			float wight = (2*wt[user_i] * similarity) / (wt[user_i] + similarity ) ;					
//			float wight = wt[user_i] * similarity;					
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
