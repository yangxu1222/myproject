/*
 *
 * this programme calculate the similarity between two items 
 *
 * steps:
 * （1）pre-process: compute the mean rate of every user，and store in the mean array
 * （2）for every u，v，calculate the similarity between u and v 
 * （3）for every u,v , find the items which they both rated  and store in a set
 * （4）use Pearson similarity formula  to calculate the similarity of u and v and store in file
 *  
 *  note: the similarity matrix we get must be symmetrical,or there are something wrong.
 *    
 *  if the source is movielens, then get the similarity file by userCF::get_sim_matrix();
 *  if the source is netflix, then get the similarity file by userCF::get_sim_matrix("netflix");
 */
#include "./common_header.h"

#define TRAINING_SET "../input/movielens/u1.base"
#define RATE_SP "\t"  //rate Separator
#define USER_NUM 943 //10K:943 1M:6040
#define ITEM_NUM 1682 //10K:1682 1M:3900
#define USER_SIM_RET "../output/user_sim"

//namespace userCF{
vector<float> mean_rate(USER_NUM+1,0.0);         //store the mean rate of every user 用来存储每个user的评分的平均打分
float w[USER_NUM+1][USER_NUM+1] = {0.0};    //user-user similarity matrix (user-user相似矩阵)
map<int,short> rate_matrix_v2[USER_NUM+1];    //use a map to store the sparse rate matrix(使用一个map数组存储稀疏的打分矩阵)
float mean = 0.0;                           //mean of all ratings(全局的平均值)

//function declaration    
double get_sim(int user1,int user2);
void load_rating(char * file_path, map<int,short> rate_matrix_v2[],const char* separator);

void get_sim_matrix(const char* source="movielens")
{
    cout << "begin initialization: " << endl;   
    if("movielens" == source)load_rating(TRAINING_SET,rate_matrix_v2,RATE_SP);  //initialization,load ratings to rate_matrix_v2
    else if("netflix" == source){
		exit(1);
		//loadNetflixRating(TRAINING_SET,rate_matrix_v2,RATE_SP);
	}
    int i,u,j,k;
    
    //get the mean rate of every user 
    for(i = 1; i < USER_NUM+1; ++i){  
        map <int,short> ::iterator it = rate_matrix_v2[i].begin(); 
        map <int,short> ::iterator end = rate_matrix_v2[i].end();
        while(it != end) {
            mean_rate[i] += (*it).second;
            ++it;
        }
        if(rate_matrix_v2[i].size() > 0)mean_rate[i] = mean_rate[i]/rate_matrix_v2[i].size();
        else mean_rate[i] = 0.0;
    } 
    
    for(i = 1; i < USER_NUM+1; ++i){  //get similarity for every u and v
        for(j = i; j < USER_NUM+1; ++j) { 
			w[i][j] = get_sim(i,j);
            if(j != i)w[j][i] = w[i][j];
        }
        if( i % 100 == 0)cout <<i<< "  over!"<<endl;
    }

    //output the simean_ratelarity matrix
	//string out_put_file = string("user_sim");
    ofstream outputw(USER_SIM_RET);
    for(i=1; i < USER_NUM+1; ++i){
		outputw <<i<<":"<<endl;
		for(j=1; j < USER_NUM+1; ++j) {
			outputw << j << "\t" << w[i][j]<<endl;
		}
    }
    outputw.close();
    cout<<"successfully exit!"<<endl;
}

/**
 * load ratings in filePath to rate_matrix_v2
 */
void load_rating(char * file_path, map<int,short> rate_matrix_v2_local[],const char* separator)
{
    cout<<"begin load training rate:"<<endl;
    std::ifstream from (file_path);
    if (!from.is_open()) {
        cout << "can't open  operation failed!\n";
        exit(1);
    }
    char rate_str[256];//为什么256
    char* pch;
    int item_id = 0, user_id = 0, rate = 0;
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
            else if(i > 2) break;
            ++i;
            pch = strtok (NULL,separator);
        }
        if(0 == item_id || 0 == user_id ||  0 == rate ) {
            cout<<str_temp<<"#####################user_id:"<<user_id<<" item_id:"<<item_id<<" rate:"<<rate<<endl;
            exit(1);
        }
        rate_matrix_v2_local[user_id][item_id] = rate;
    }
    from.close();
    cout<<"end load training rate!"<<endl;
    return;
}


double get_sim(int user1, int user2) {
    //(1)find the items which the user1 and user2 both rated (找到打分的公共物品集合) 
    //(2)calculate the similarity according the pearson formula(按照公式计算)
    if(user1 == user2)return 1.0;
    if(0 == rate_matrix_v2[user1].size() || 0 == rate_matrix_v2[user2].size() ) return 0.0;
    map <int,short> ::iterator it = rate_matrix_v2[user1].begin(); 
    map <int,short> ::iterator end = rate_matrix_v2[user1].end(); 
    double sum1 = 0.0, sumSquare1 = 0.0, sumSquare2 = 0.0;
    for(; it != end; ++it) {
        int item = (*it).first;
        if(rate_matrix_v2[user2].find(item) != rate_matrix_v2[user2].end()) {
            //cout<<"common user:"<<user<<'\t'<<rate_matrix_v2[user1][user]<<'\t'<<rate_matrix_v2[user2][user]<<endl;
            //already find the common user(已经找到了公共元素)
            sum1 += (rate_matrix_v2[user1][item] - mean_rate[user1]) *(rate_matrix_v2[user2][item] - mean_rate[user2]);
            sumSquare1 += pow(rate_matrix_v2[user1][item] - mean_rate[user1],2); 
            sumSquare2 += pow(rate_matrix_v2[user2][item] - mean_rate[user2],2);
        }
    }
    double sim = 0.0;
    if(sumSquare1 > 0.0000000001  && sumSquare2 > 0.0000000001)sim = sum1 / sqrt( sumSquare1 * sumSquare2);
    return sim;
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
    get_sim_matrix();
    end = time(NULL);
    duration = (end-start);
    time_end_info = localtime(&end);
	string time_end_str = asctime(time_end_info);
    cout << "start at "<<time_start_str<<"end at "<<time_end_str; 
    cout << "duration:"<<duration <<" s!" <<endl;
    return 0;
}
