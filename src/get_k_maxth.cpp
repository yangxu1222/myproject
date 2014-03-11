/* *
 * this programme get k-largetst similiar of each item and store it to the file
 *
 * steps:
 * (0)calculate the similarity matrix and store it to the file(getSim.cpp)
 * (1)get the similarity matrix from the file.
 * (2)for every item i，use the minimal heap to get k-max similarity value
 * (3)store the k-max similarity value to file
 *    
 *  
 */
#include "./common_header.h"
#include <sstream>
#define TRAINING_SET "../input/movielens/u1.base"
#define RATE_SP "\t"  //rate Separator
#define USER_NUM 943 //10K:943 1M:6040
#define ITEM_NUM 1682 //10K:1682 1M:3900
#define SIM_FILE "../output/user_sim"

//namespace userCF{
    vector<float> mean_rate(USER_NUM+1,0.0);         //store the mean rate of every user(用来存储每个user的平均打分)
    float w[USER_NUM+1][USER_NUM+1] = {0.0};    //item-item similarity matrix(user-user相似矩阵)
    map<int,short> rate_matrix_v2[USER_NUM+1];    //use a map to store the sparse rate matrix(使用一个map数组存储稀疏的打分矩阵)
    float mean = 0.0;                           //mean of all ratings(全局的平均值)
    
    //function declaration    
    float get_k_max(vector<float>& array, int K);
    bool cmp(double a, double b);
    
    void get_k_max_sim(int K, const char* source="movielens")
    {
        cout << "begin initialization: " << endl;
        char rate_str[256];
        char* pch;    
        vector<float> sim_array;
        int user_num = 0;
		char tmp_k[20];
		sprintf(tmp_k,"%d",K);
//		stringstream ss;
//		ss << K;
//		string tmp_k;
//		ss >> tmp_k;
        string dst = string("../output/user_")+string(tmp_k)+string("_max");
        std::ifstream from(SIM_FILE);
        ofstream to(dst.c_str());
        if (!from.is_open()) {
            cout << "can't open  operation failed!\n";
            exit(1);
        }
        int user_i = 0, user_j = 0;
        float sim = 0.0;
        while(from.getline(rate_str,256)){
            string str_temp(rate_str);
            int pos = str_temp.find(":");
            if(-1 != pos) {
                user_j = user_i; //store the last user
                user_i = atoi(str_temp.substr(0,pos).c_str());
                if(0 == user_j) {//the first line
                    continue;
                }
                if(0 == user_i ) {
                    cout<<str_temp<<"#####################"<<endl;
                    exit(1);
                }
//				std::cout<<"sim_array length:"<<sim_array.size()<<endl;
                float k_max_value = get_k_max(sim_array,K); //get the k-max of the last user
//				std::cout<<user_j<<'\t'<<k_max_value<<endl;
                to << user_j << '\t' << k_max_value <<endl;
                sim_array.clear(); //清空vector,为获取下一个user的k-max similarity做准备    
                ++user_num;     
                if(user_num % 943 ==0) {
                    cout<<"read user "<<user_num<<" sucessfully!"<<endl;
                }
                continue;
            }
            if(str_temp.length() < 3)continue;
            int i = 0;
            pch = strtok (rate_str,RATE_SP);
            while (pch != NULL) {
                if(0 == i)user_j = atoi(pch);
                else if(1 == i) sim = atof(pch);
                else if(i > 1) break;
                ++i;
                pch = strtok (NULL,RATE_SP);
            }
            if(0 == user_i || 0 == user_j) {
                cout<<str_temp<<"#####################"<<endl;
                exit(1);
            }        
            sim_array.push_back(sim);
        }
        from.close();
        cout<<"end load similarity!"<<endl;
        cout<<"successfully exit!"<<endl;
    }
    
    //下面的这个函数用来利用最小堆找出第k大的相似度
    //get the k-max largest similarity in the array  using the minimum heap
    float get_k_max(vector<float>& array, int K)
    {
        int arraySize = array.size();
        if(arraySize < K)return 0.0; //if size < K, then the k-max value is 0
        vector<float> heapTmp;
        for(int i=0; i < array.size(); ++i) {
            heapTmp.push_back(array[i]);
            if(i == K-1) {
                make_heap(heapTmp.begin(),heapTmp.end(),cmp);
            }
            else if(i >=K) {
                push_heap(heapTmp.begin(),heapTmp.end(),cmp);
                pop_heap(heapTmp.begin(),heapTmp.end(),cmp);
                heapTmp.pop_back();
            }
            //cout << i<<'\t'<<heapTmp.size()<<endl;
        }
        return heapTmp.front();
    }
    
    bool cmp(double a, double b)
    {
        return a > b;
    }
//};

int main(int argc, char ** argv)
{
    time_t start,end;
    struct tm * time_start_info;
    struct tm * time_end_info;
    double duration; 
    start = time(NULL);
    time_start_info = localtime(&start);
    string time_start_str = asctime(time_start_info);
	int k_num = atoi(argv[1]);
	get_k_max_sim(k_num);
	end = time(NULL);
    duration = (end-start);
    time_end_info = localtime(&end);
	string time_end_str = asctime(time_end_info);
    cout << "Start at "<<time_start_str<<", and end at "<< time_end_str;
    cout << "duration:"<<duration <<" s!" <<endl;
    return 0;
}
