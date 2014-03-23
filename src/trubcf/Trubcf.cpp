/*************************************************************************
	> File Name: Trubcf.cpp
	> Author: yangxu07
	> Mail: yangxu07@baidu.com 
	> Created Time: 2014年03月14日 星期五 14时07分45秒
 ************************************************************************/

#include<iostream>
#include<cmath>
#include "Trubcf.h"

using namespace std;

map<float,vector<float> > Trubcf::trust_mode(vector<vector<rateNode> > rate_matrix,
		float s[USER_NUM+1][USER_NUM+1],vector<double> mean_rate){
	float eta = 0;
	map<float,vector<float> > ret;
	while (eta <= 5){
		cout << "eta:" << eta <<endl;
		vector<int> recSet(944,0);
		vector<int> correctSet(944,0);
		for( int c =1; c < rate_matrix.size(); ++c){//用户i
//			cout << "c:" << c << endl;
			for (int p = 1; p < rate_matrix.size(); ++p){
		//		cout << "p:" << p << endl;
				if (c!=p && s[c][p] != 0) {
					vector<rateNode> c_tmp = rate_matrix[c];
					vector<rateNode>::iterator it = c_tmp.begin();
					for(;it != c_tmp.end(); ++it){
						int item = (*it).item;
						int true_r = (*it).rate;
						recSet[p]++;
						//获取p对item的rate
						int p_item_r = 0;
						vector<rateNode> p_tmp = rate_matrix[p];
						for (int i =0;i < p_tmp.size(); ++i){
							if (item == p_tmp[i].item){
								p_item_r = p_tmp[i].rate;
								break;
							}
						}
						if (p_item_r == 0){
							continue;
						}
						float pred_r = mean_rate[c] + ((p_item_r - mean_rate[p]) * s[c][p] )/s[c][p];
						if( abs(pred_r - true_r) < eta) correctSet[p]++;
					}
				}
			}
		}
		vector<float> trust(944,0);
		for (int u = 1; u < rate_matrix.size(); ++u){
			float t = correctSet[u] / recSet[u];
			trust[u] = t;
		}
		ret.insert(make_pair(eta,trust));

		eta += 0.1;
	}
	map<float,vector<float> >::iterator it = ret.begin();
	for (; it != ret.end(); ++it){
		cout << it->first << endl;
		cout << (it->second).size() << endl;
	}
	return ret;
}

map<int,float> Trubcf::cal_trust(vector<vector<rateNode> > rate_matrix,
		float s[USER_NUM+1][USER_NUM+1],vector<double> mean_rate){
	float eta = 1.8;
	map<int,float> ret;
	vector<int> recSet(944,0);
	vector<int> correctSet(944,0);
	for( int c =1; c < rate_matrix.size(); ++c){//用户i
//		cout << "c:" << c << endl;
		for (int p = 1; p < rate_matrix.size(); ++p){
		//	cout << "p:" << p << endl;
			if (c!=p && s[c][p] != 0) {
				vector<rateNode> c_tmp = rate_matrix[c];
				vector<rateNode>::iterator it = c_tmp.begin();
				for(;it != c_tmp.end(); ++it){
					int item = (*it).item;
					int true_r = (*it).rate;
					recSet[p]++;
				    //获取p对item的rate
					int p_item_r = 0;
					vector<rateNode> p_tmp = rate_matrix[p];
					for (int i =0;i < p_tmp.size(); ++i){
						if (item == p_tmp[i].item){
							p_item_r = p_tmp[i].rate;
							break;
						}
					}
					if (p_item_r == 0){
						continue;
					}
					float pred_r = mean_rate[c] + ((p_item_r - mean_rate[p]) * s[c][p] )/s[c][p];
					if( abs(pred_r - true_r) < eta) correctSet[p]++;
				}
			}
		}
	}
	
	for (int u = 1; u < rate_matrix.size(); ++u){
//		cout << correctSet[u] << ":" <<recSet[u] << endl;
		float t = (float)correctSet[u] / (float)recSet[u];
//		cout << "t:" << t << endl;
		ret.insert(make_pair(u,t));
	}

	string dst = string("/home/yangxu/work/myproject/output/user_trust");
	ofstream to(dst.c_str());
	map<int,float>::iterator it = ret.begin();
	for (; it != ret.end(); ++it){
//		cout << it->first << ":"<<it->second << endl;
		to<<it->first<<"\t"<<it->second<<endl;
	}
	to.close();

	return ret;
}

//compute the mae of test set
double Trubcf::MAEProbe(vector<testSetNode>& probeSet,
		int dim,vector<vector<rateNode> > rate_matrix,
		float s[USER_NUM+1][USER_NUM+1],
	    float kmax[USER_NUM+1],
		map<int,float>trust)
{
    int probeSize = probeSet.size();
    float pRate,err;
    long double mae = 0;

	vector<double> mean_rate = get_user_mean(rate_matrix);

    for(int i = 0; i < probeSize; ++i) {
        pRate = predict_rate(probeSet[i].user,probeSet[i].item,rate_matrix,mean_rate,s,kmax,trust);      //predict rate
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
float Trubcf::predict_rate(int user, int item,
		vector<vector<rateNode> >rate_matrix,
		vector<double> mean_rate,
		float s[USER_NUM+1][USER_NUM+1],
	    float kmax[USER_NUM+1],
		map<int,float> trust)
{
    int r_num = rate_matrix[user].size(); //the num of items rated by user(用户user打过分的item数目)
    double sum_numerator = 0.0;
    double sum_denominator = 0.0;
   
	for (int user_i = 1; user_i < USER_NUM; ++user_i){
		int rate = 0;
	    float similarity = s[user][user_i];
		if (similarity < kmax[user])continue;
		else{

			float wight = (2*trust[user_i] * similarity) / (trust[user_i] + similarity ) ;					
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
