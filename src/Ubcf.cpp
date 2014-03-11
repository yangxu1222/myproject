/*************************************************************************
	> File Name: Ubcf.cpp
	> Author: yangxu07
	> Mail: yangxu07@baidu.com 
	> Created Time: 2014年03月06日 星期四 17时40分33秒
 ************************************************************************/

#include<iostream>
#include "Ubcf.h"
using namespace std;

//float mean_rate[USER_NUM+1] = {0.0};             //store the mean rating of each user 
//int    biNum[USER_NUM+1] = {0.0};            //the rating num of every (打过item i分的用户总数)
//float s[USER_NUM+1][USER_NUM+1] = {0};     //item-item similarity matrix(user-user相似矩阵)
//float kmax[USER_NUM+1] = {0};              //k-max array ,the k-max similarity of each user
//float mean = 0.0;


/**
 * load the training set of movielens dataset
 * 
 */
void Ubcf::load_train(const char * file_name, 
		vector< vector<rateNode> >& rate_matrix_local, 
		const char * separator)
{
    char rate_str[256];
    char* pch;   
    int file_num = 0;
    std::ifstream from (file_name);
    if (!from.is_open()) {
        cout << "can't open input file!\n";
        exit(1);
    }
        
    int item_id = 0, user_id = 0, rate = 0;
	long timestamp = 0;
    string str_temp = "";
    
    while(from.getline(rate_str,256)){
        string str_temp(rate_str);
        if(str_temp.length() < 3)continue;
        int i = 0;
        pch = strtok (rate_str,separator);
        while (pch != NULL) {
            if(0 == i) user_id = atoi(pch);
            else if(1 == i) item_id = atoi(pch);
            else if(2 == i) rate = atoi(pch);
			else if(3 == i) timestamp = atol(pch);
            else if(i > 3) break;
            ++i;
            pch = strtok (NULL,separator);
          }
        if(0 == item_id || 0 == user_id ||  0 == rate ) {
            cout<<str_temp<<"#####################userId:"<<user_id<<" itemId:"<<item_id<<" rate:"<<rate<<endl;
            exit(1);
        }
        //初始化rateMatrix
        try {
            rateNode tmp_node;
            tmp_node.item = item_id;
            tmp_node.rate = (short)rate;
			tmp_node.timestamp = timestamp;
            rate_matrix_local[user_id].push_back(tmp_node);
        }
        catch (bad_alloc& ba){
            cerr << "bad_alloc caught: " << ba.what() << endl;
        }
    }
    from.close();
    cout<<"read training set file sucessfully!"<<endl;
    return;
}


//load test set
void Ubcf::load_test(const char * file_name,vector<testSetNode>& probe_set, const char* separator)
{
    ifstream in(file_name);
    if (!in.is_open()) {
        cout << "can't open test set file!\n";
        exit(1);
    }
    char rate_str[256];
    char* pch ; // store a token of a string

    string str_temp;
    int rate_value,item_id,user_id,probe_num=0;
	long timestamp = 0;    
    while(in.getline(rate_str,256)){
        str_temp = rate_str;
        if(str_temp.length() < 4) continue;
        int i = 0;
        pch = strtok (rate_str,separator);
        while (pch != NULL) {
            if(0 == i) user_id = atoi(pch);
            else if(1 == i) item_id = atoi(pch);
            else if(2 == i) rate_value = atoi(pch);
			else if(3 == i) timestamp = atol(pch); 
			else if(i > 3) break;
            ++i;
            pch = strtok (NULL,separator);
        }
        try {
            testSetNode tmp_node;
            tmp_node.item = item_id;
            tmp_node.rate = (short)rate_value;
            tmp_node.user = user_id;
            probe_set.push_back(tmp_node);
            ++probe_num;
        }
        catch (bad_alloc& ba) {
            cerr << "bad_alloc caught: " << ba.what() << endl;
            cout << "Can't allocate the momery!" << endl; exit(1);
        }
    }
    cout << "Load " << probe_num << " test ratings successfully!"<<endl;
    in.close(); 
}

//load the similarity matrix from file
void Ubcf::load_sim_matrix(float s[USER_NUM+1][USER_NUM+1], const char* sim_file)
{
    char rate_str[256];
    char* pch;    
    int user_num = 0;
    std::ifstream from(sim_file);
    if (!from.is_open()) {
        cout << "can't open  operation failed!\n";
        exit(1);
    }
    const char* separator = "\t";
    int user_i = 0, user_j = 0;
    float sim = 0.0;
    while(from.getline(rate_str,256)){
        string str_temp(rate_str);
        int pos = str_temp.find(":");
        if(-1 != pos) {
            user_i = atoi(str_temp.substr(0,pos).c_str());
            
            if(0 == user_i ) {
                cout<<str_temp<<"#####################"<<endl;
                exit(1);
            }
            ++user_num;     
            if(user_num % 942 ==0) {
                cout<<"read item "<<user_num<<" sucessfully!"<<endl;
            }
            continue;
        }
        if(str_temp.length() < 3)continue;
        int i = 0;
        pch = strtok (rate_str,separator);
        while (pch != NULL) {
            if(0 == i) user_j = atoi(pch);
            else if(1 == i) sim = atof(pch);
            else if(i > 1) break;
            ++i;
            pch = strtok (NULL,separator);
        }
        if(0 == user_i || 0 == user_j) {
            cout<<str_temp<<"#####################"<<endl;
            exit(1);
        }        
        s[user_i][user_j] = sim;
    }
    from.close();
    cout<<"end load training rate!"<<endl;
}

//load k-max similarity value from file
void Ubcf::load_k_max(float k_max_local[USER_NUM+1], const char* file_name, const char* separator)
{
    char rate_str[256];
    char* pch;   
    int file_num = 0;
    std::ifstream from (file_name);
    if (!from.is_open()) {
        cout << "can't open input file!\n";
        exit(1);
    }
        
    int user_id = 0;
    float sim = 0.0;
    string str_temp = "";
    
    while(from.getline(rate_str,256)){
        string str_temp(rate_str);
        if(str_temp.length() < 3)continue;
        int i = 0;
        pch = strtok (rate_str,separator);
        while (pch != NULL) {
            if(0 == i) user_id = atoi(pch);
            else if(1 == i) sim = atof(pch);
            else if(i > 1) break;
            ++i;
            pch = strtok (NULL,separator);
        }
        if(0 == user_id) {
            cout<<str_temp<<"#####################"<<endl;
            exit(1);
        }
        k_max_local[user_id] = sim;
    }
    from.close();
    cout<<"read k-max file sucessfully!"<<endl;
    return;
}


//compute the mae of test set
double Ubcf::MAEProbe(vector<testSetNode>& probeSet,
		int dim,vector<vector<rateNode> > rate_matrix,
		float s[USER_NUM+1][USER_NUM+1],
	    float kmax[USER_NUM+1])
{
    int probeSize = probeSet.size();
    float pRate,err;
    long double mae = 0;

	vector<double> mean_rate = get_user_mean(rate_matrix);

    for(int i = 0; i < probeSize; ++i) {
        pRate = predict_rate(probeSet[i].user,probeSet[i].item,rate_matrix,mean_rate,s,kmax);      //predict rate
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
float Ubcf::predict_rate(int user, int item,
		vector<vector<rateNode> >rate_matrix,
		vector<double> mean_rate,
		float s[USER_NUM+1][USER_NUM+1],
	    float kmax[USER_NUM+1])
{
    
    int r_num = rate_matrix[user].size(); //the num of items rated by user(用户user打过分的item数目)
    double sum_numerator = 0.0;
    double sum_denominator = 0.0;
   
	for (int user_i = 1; user_i < USER_NUM; ++user_i){
		int rate = 0;
	    float similarity = s[user][user_i];
		if (similarity < kmax[user])continue;
		else{
			sum_denominator += fabs(similarity);

			for (int p = 0; p < rate_matrix[user_i].size();++p){
				if (item == rate_matrix[user_i][p].item){
					rate = rate_matrix[user_i][p].rate;
//					cout << "rate:"<< rate<<"," <<user_i<<":"<< mean_rate[user_i] << endl;
					sum_numerator += similarity * (rate-mean_rate[user_i]);
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

vector<double> Ubcf::get_user_mean(const vector<vector<rateNode> > & rate_matrix)
{
    //求mean_rate，即每个user的平均打分
	vector<double>  mean_rate;
    int i,j;
	mean_rate.push_back(0);
    for(i = 1; i < USER_NUM+1; ++i){
        double vSize = rate_matrix[i].size();
		double sum = 0;
        for(j=0; j < vSize; ++j) {
			sum += rate_matrix[i][j].rate;
        }
		double mean = sum / vSize;
		mean_rate.push_back(mean);
	}
//	for (int i = 0;i < mean_rate.size(); i++){
//		cout << "mean of " << i << ":" << mean_rate[i]<< endl;
//	}

	return mean_rate; 
}

