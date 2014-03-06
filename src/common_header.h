/* 
 * this file contains some  General Function include lib 
 */
#ifndef COMMONHEADER_H_
#define COMMONHEADER_H_

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <math.h>
#include <map>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <dirent.h>
#include <malloc.h>
#include <errno.h>
#include <time.h>

using namespace std;

struct rateNode
{
    short item;
    short rate;
};

struct testSetNode
{
    int user;
    short item;
    short rate;
};
float predictRate(int user,int item);
#endif // COMMONHEADER_H_
