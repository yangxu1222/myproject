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

class UTbcf : public Ubcf{
	public:
		UTbcf(){
		};
		~UTbcf(){
		};
		void wight_time(int user,vector<vector<rateNode> > rate_matrix);
		map<int ,int> get_best_t(vector<vector<rateNode> >& rate_matrix);
};
		
