/* 
 *
 * This file contains some common operations of movielens dataset, for example reading the test set
 * and reading the test set
 *
 * the ratings of  training set store in the array (USER X ITEM, user is the row ,and item is the column)
 */
#ifndef MLBASE_CPP_
#define MLBASE_CPP_

/**
 * load the training set of movielens dataset
 * 
 */
void load_rating(char * file_name, vector< vector<rateNode> >& rate_matrix_local, const char * separator)
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
            cout<<str_temp<<"#####################userId:"<<user_id<<" itemId:"<<item_id<<" rate:"<<rate<<endl;
            exit(1);
        }
        //³õÊ¼»¯rateMatrix
        try {
            rateNode tmp_node;
            tmp_node.item = item_id;
            tmp_node.rate = (short)rate;
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
void load_probe(char * file_name,vector<testSetNode>& probe_set, const char* separator)
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
    
    while(in.getline(rate_str,256)){
        str_temp = rate_str;
        if(str_temp.length() < 4) continue;
        int i = 0;
        pch = strtok (rate_str,separator);
        while (pch != NULL) {
            if(0 == i) user_id = atoi(pch);
            else if(1 == i) item_id = atoi(pch);
            else if(2 == i) rate_value = atoi(pch);
            else if(i > 2) break;
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
#endif // MLBASE_CPP_ 
