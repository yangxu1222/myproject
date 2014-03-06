/* 
 *
 * knn_base.cpp contains some common functions .
 */
#ifndef KNN_KNNBASE_CPP_
#define KNN_KNNBASE_CPP_
#include "../common_header.h"
#include "../common.cpp"
#include "../ml_base.cpp"

#define USER_NUM 943 //10K:943 1M:6040
#define ITEM_NUM 1682 //10K:1682 1M:3900
#define MIN_RATE 1    // the minimum of the rate
#define MAX_RATE 5    // the maximum of the rate
#define K_NUM  50     //dimension
#define TRAINING_SET "../dataset/movielens/u1.base" //training set
#define PROBE_SET "../dataset/movielens/u1.test"  //test set
#define RATE_SP "\t"  //rate Separator



namespace userCF{
    //use some global variables，s(similarity matrix) 
    double mean_rate[USER_NUM+1] = {0.0};             //store the mean rating of each user 
    int    biNum[USER_NUM+1] = {0.0};            //the rating num of every (打过item i分的用户总数)
    float s[USER_NUM+1][USER_NUM+1] = {0};     //item-item similarity matrix(user-user相似矩阵)
    float kmax[USER_NUM+1] = {0};              //k-max array ,the k-max similarity of each user
    //map<int,int> rate_matrix[ITEM_NUM+1];     //use a map array to store the sparse matrix of training-set rating 
    float mean = 0.0;
    
    vector < vector<rateNode> > rate_matrix(USER_NUM+1);   //store training set
    vector<testSetNode> probe_row;                            //store test set
    
    //load the similarity matrix from file
    void load_sim_matrix(float s[USER_NUM+1][USER_NUM+1], const char* sim_file)
    {
        char rate_str[256];
        char* pch;    
        int user_num = 0;
        std::ifstream from(sim_file);
        if (!from.is_open()) {
            cout << "can't open  operation failed!\n";
            exit(1);
        }
        char* separator = "\t";
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
    void load_k_max(float k_max_local[USER_NUM+1], const char* file_name, const char* separator)
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
    
    void get_user_mean()
    {
        //求mean_rate，即每个user的平均打分
        using namespace userCF;
        int i,j;
        for(i = 1; i < USER_NUM+1; ++i){
            double vSize = rate_matrix[i].size();
			double sum = 0;
            for(j=0; j < vSize; ++j) {
				sum += rate_matrix[i][j].rate;
            }
			mean_rate[i] = sum / vSize;
        }
        
    }
    
    void model(const char* sim_file, int k_num)
    {
		char buffer[20];
		
		sprintf(buffer,"%d",k_num);
		
		string k_max_file = string("user_") + string(buffer) + string("_max");
        cout << "begin initialization: " << endl;
        load_sim_matrix(s, sim_file);               //load sim matrix
        load_k_max(kmax, k_max_file.c_str(),"\t");                //load k-max
        
        load_rating(TRAINING_SET,rate_matrix,RATE_SP);  //load training set
        load_probe(PROBE_SET,probe_row,RATE_SP);   //load test set
        
        mean = set_mean_rating(USER_NUM,rate_matrix); //calculate the mean ????

		get_user_mean();         //calculate the mean of each item
		
        int i,u,j,k;
        cout <<"begin testRMSEProbe(): " << endl;
	//	RMSEProbe(probe_row,k_num);
        MAEProbe(probe_row,k_num);
        cout <<"end testRMSEProbe(): " << endl;
        return;
    }
};


/**
 * predict the rate
 */
float predictRate(int user, int item)
{
    using namespace userCF;
    
    int r_num = rate_matrix[user].size(); //the num of items rated by user(用户user打过分的item数目)
    double sum_numerator = 0.0;
    double sum_denominator = 0.0;
   
	for (int user_i = 1; user_i < USER_NUM; ++user_i){
		int rate = 0;
		
		if (s[user][user_i] < kmax[user])continue;
		else{
			sum_denominator += fabs(s[user][user_i]);

			for (int p = 0; p < rate_matrix[user_i].size();++p){
				if (item == rate_matrix[user_i][p].item){
					rate = rate_matrix[user_i][p].rate;
					sum_numerator += s[user][user_i] * (rate-mean_rate[user_i]);
					break;
				}
			}
		}
	}
//	cout << "sum_numerator: "<< sum_numerator<<endl;
	//cout << "sum_denominator:"<<sum_denominator<<endl;
	//cout << "mean_rate[user]:"<<mean_rate[user]<<endl;
	
    float ret =0.0;
    if(sum_denominator > 0.0000000002) ret =  mean_rate[user] + (sum_numerator/sum_denominator);
    else ret =  mean_rate[user];
//	cout << "ret: "<<ret<<endl;
    if(ret < MIN_RATE) ret = MIN_RATE;
    if(ret > MAX_RATE) ret = MAX_RATE;
    return ret;
}
#endif // KNN_KNNBASE_CPP_ 
