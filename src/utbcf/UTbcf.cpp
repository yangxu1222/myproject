/*************************************************************************
	> File Name: UTbcf.cpp
	> Author: yangxu07
	> Mail: yangxu07@baidu.com 
	> Created Time: 2014年03月10日 星期一 12时36分33秒
 ************************************************************************/

#include<iostream>
#include <limits>

#include "UTbcf.h"
using namespace std;

map<int,int> UTbcf::get_best_t(vector< vector<rateNode> >& rate_matrix){
	
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
			cout << i << endl;
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
	
	cout << "len of ret : " << ret.size() << endl;	
	map<int,int>::iterator it = ret.begin();
//	for(;it != ret.end(); ++it){
//		cout << it->first << "=>" << it->second << endl;
//	}
	return ret;
}
