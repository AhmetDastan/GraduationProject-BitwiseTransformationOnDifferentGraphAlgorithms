#include <iostream>
#include "string"


#include "bfsLib/bfsBtwTemp.hpp" 
#include "bfsLib/bfsBgl.hpp"
#include "bfsLib/bfsCstm.hpp"

#include "dfsLib/dfsCstm.hpp"
#include "dfsLib/dfsBgl.hpp" 
#include "dfsLib/dfsBtwTemp.hpp" 

using namespace std;

int main() {
    cout <<"main function" << endl;

    // create result file
    ofstream file2("result.txt");
    if (!file2.is_open()) {
        cerr << "File couldn't open!" << endl;
        return 0;
    }
    file2 << "BFS Graph Comparison with -O3 Flag Microsecond Results" << endl;
    file2.close();
    int graphSize[6] = { 50, 100, 500, 1000, 2000, 5000};

    for(int i = 0; i < 6;++i){
        string filename = "graphs/graph" + to_string(graphSize[i]) + ".txt";

        ofstream file2("result.txt",  ios::app);
        if (!file2.is_open()) {
            cerr << "File couldn't open!" << endl;
            return 0;
        }
        file2 << "graph" << to_string(graphSize[i]) << endl;
        file2.close();
        
        BFSBgl bfsBgl;
        bfsBgl.readGraphFromFile(filename);
        bfsBgl.compute("output_bfs_Bgl.txt", 10);

        BFSCstm bfsCstm;
        bfsCstm.readGraphFromFile(filename);
        bfsCstm.compute("output_bfs_Cstm.txt", 10);
 
        BFSBtwTemp bfsBtwTemp;
        bfsBtwTemp.readGraphFromFile(filename); 
        bfsBtwTemp.compute("output_bfs_BtwTemp.txt", 10);
    }

    ofstream file1("result.txt",  ios::app);
    if (!file1.is_open()) {
        cerr << "File couldn't open!" << endl;
        return 0;
    }
    file1 << "DFS Graph Comparison with -O3 Flag Microsecond Results" << endl;
    file1.close();
    for(int i = 0; i < 6; ++i){
        string filename = "graphs/graph" + to_string(graphSize[i]) + ".txt";

        ofstream file2("result.txt",  ios::app);
        if (!file2.is_open()) {
            cerr << "File couldn't open!" << endl;
            return 0;
        }
        file2 << "graph" << to_string(graphSize[i]) << endl;
        file2.close();
        
        DFSBgl dfsBgl;
        dfsBgl.readGraphFromFile(filename);
        dfsBgl.compute("output_dfs_Bgl.txt", 10);

        DFSCstm dfsCstm;
        dfsCstm.readGraphFromFile(filename);
        dfsCstm.compute("output_dfs_Cstm.txt", 10);
 
        DFSBtwTemp dfsBtwTemp;
        dfsBtwTemp.readGraphFromFile(filename); 
        dfsBtwTemp.compute("output_dfs_BtwTemp.txt", 10);
    }
    
    return 0;
}
