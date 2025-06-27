#ifndef DFS__CSTM__HPP
#define DFS__CSTM__HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <chrono>
#include <numeric>

using namespace std;

class DFSCstm {
private:
    vector<vector<int>> adjList;
    int num_nodes;
    int edge_count;

public:
    DFSCstm() : num_nodes(0), edge_count(0) {}

    void readGraphFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Dosya açılamadı: " << filename << std::endl;
            exit(1);
        }
        
        std::string line;
        while (std::getline(file, line)) {
            if (line[0] == 'c') continue; // Yorum satırlarını atla
            if (line[0] == 'p') {
                std::stringstream ss(line);
                std::string tmp;
                ss >> tmp >> tmp >> num_nodes >> edge_count;
                adjList.resize(num_nodes);
            } else if (line[0] == 'a') {
                std::stringstream ss(line);
                char a;
                int u, v;
                ss >> a >> u >> v;
                --u;
                --v;
                adjList[u].push_back(v);
                adjList[v].push_back(u); 
            }
        }
        file.close();
    }

    void compute(const std::string& output_filename, int iterations = 10) {
        ofstream file(output_filename);
        if (!file.is_open()) {
            cerr << "File couldnt oppen!" << endl;
            return;
        } 
        int maxsize = 0;

        std::vector<double> durations; 
        for (int it = 0; it < iterations; ++it) {
            vector<int> nodePos(num_nodes, -1);
            vector<bool> visited(num_nodes, false);  
            int counter = 0;
            auto start = std::chrono::high_resolution_clock::now();

            // DFS iterative
            std::stack<int> s;
            s.push(0); // 0'dan başla

            while (!s.empty()) {
                if(s.size() > maxsize) {
                    maxsize = s.size();
                }
                int u = s.top();
                s.pop();

                if (visited[u]) continue;

                ++counter;
                visited[u] = true;
                if (nodePos[u] == -1) { 
                    nodePos[u] = counter;
                }
                    

                for (int v = 0; v < adjList[u].size(); ++v) {
                    int neighbor = adjList[u][v];  
                    if (!visited[neighbor]) {
                        s.push(neighbor); 
                    }
                }
            }

            auto end = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration<double>(end - start).count(); 
            durations.push_back(duration);

            if (it == 0) { 
                for (int j = 1; j < nodePos.size(); ++j) {
                    file << "Node 0 " << " -> " << j
                         << " Shortest path: " << nodePos[j] << "\n";
                }
                file.close();
            }
        }
        std::cout << "Max stack size: " << maxsize << std::endl;
        double avg = std::accumulate(durations.begin(), durations.end(), 0.0) / iterations;

        std::cout << " cstm Ortalama süre: " << avg << " s" << std::endl;
        ofstream file2("result.txt",  ios::app);
        if (!file2.is_open()) {
            cerr << "File couldn't open!" << endl;
            return;
        }
        file2 << "Adj List: " << avg << " s" << endl;
        file2.close();
    }
};

#endif