/*************************************************************************
	> File Name: test.cpp
	> Author: yangxu07
	> Mail: yangxu07@baidu.com 
	> Created Time: 2014年03月10日 星期一 13时46分01秒
 ************************************************************************/

#include<iostream>
#include <ctime>
using namespace std;

int main(int args,char ** argvs){
	time_t cur;
	struct tm *cur_info;
	cur = time(NULL);
	cur_info = localtime(&cur);
	string cur_str = asctime(cur_info);
	cout << (long)cur<< endl;
}
	

