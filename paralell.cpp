#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <ctime>
#include <omp.h>

using namespace std;


vector<vector<int>> initialize(int &n, int &m) {
    char c;   
    string s;

    while (true)
    {
        cin >> c;
        if (c == 'c') 
        {
            getline(cin, s); 
        }
        else 
        {
            cin >> s; 
            break;
        }
    }
    cin >> n;
    cin >> m;

    vector<vector<int>> clauses;
    clauses.resize(m);

    int literal; 
    for (int i = 0; i < m; i++)
    {
        while (true) 
        {
            cin >> literal;
            if (literal != 0) {
                clauses[i].push_back(literal);
            }
            else
            {
                break; 
            }
        }
    }

    return clauses;
}


int main() {  
    int n = 0;
    int m = 0;
    
    vector<vector<int>> clauses = initialize(n,m);

    vector<vector<int>> subsets;
    int subset_size = pow(2,n);
    subsets.resize(subset_size);

    for (int i = 0; i < subset_size; i++){
        string binary = std::bitset<32>(i).to_string(); //to binary
        
        for (int j = 31; j > (31-n); j--)
        {
            if (binary[j] == '1')
                subsets[i].push_back(1);
            else
                subsets[i].push_back(-1);
        }        
    }
    

    int counter = 0;
    int counter_index = -1;
    bool isStop = false;
    clock_t start = clock(), finish;

    #pragma omp parallel for firstprivate(counter)
    for (int i = 0; i < subsets.size(); i++)
    {
        if (isStop) continue;
        int flag = 0;
        for (int j = 0; j < clauses.size(); j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (clauses[j][k] * subsets[i][abs(clauses[j][k])-1] > 0){
                    flag = 1;
                    break;
                };
            }
            if(flag == 0){
                break;
            }      
            else {
                flag = 0;
                counter++;
            }     
        }
        
        
        if (counter == m) {
            //#pragma omp critical
            counter_index = i;
            isStop = true;
        }
        counter = 0;
    }   
    finish = clock();
    
    if (counter_index != -1){
        cout << "SAT" << endl;
        for (int k = 0; k < n; k++)
        {
            cout << (k+1) * subsets[counter_index][k] << " ";          
        }     
        cout << endl;
    }
    else {
        cout << "UNSAT" << endl;
    }
    
    cout << "Time : " << (float)(finish - start) / CLOCKS_PER_SEC << endl;
    

    return 0;
}