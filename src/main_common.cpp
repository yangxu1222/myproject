/*************************************************************************
	> File Name: main_common.cpp
	> Author: yangxu07
	> Mail: yangxu07@baidu.com 
	> Created Time: 2014年02月21日 星期五 14时03分00秒
 ************************************************************************/

#include<iostream>
#include "common_header.h"
#include "common.cpp"
#include<string.h>
#include<vector>
#include<iterator>
#include<algorithm>
#define USER_NUM 943 //10K:943 1M:6040
#define ITEM_NUM 1682 //10K:1682 1M:3900

using namespace std;

int main(){
	string s = "1	3	56	90";
	const char *p = "\t";
	vector<string> ret;
	explode(p,s,ret);
	copy(ret.begin(),ret.end(),ostream_iterator<string>(cout,"\n"));
	cout << endl;
}
