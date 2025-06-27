#ifndef BFS_CUSTOM_HPP
#define BFS_CUSTOM_HPP

// Custom BFS implementation
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <chrono>
 
using namespace std;

class BFSCstm {
private:
    vector<vector<int>> adjList;
    int num_nodes;
    int edge_count;

public:
    BFSCstm() : num_nodes(0), edge_count(0) {}

    // Grafiği dosyadan oku
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

    // BFS implementasyonu
    vector<int> bfs(int start) { 
        vector<int> dist(num_nodes, -1);
        std::queue<int> q;
        dist[start] = 0;
        q.push(start);
        
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            for (int neighbor : adjList[node]) {
                if (dist[neighbor] == -1) {
                    dist[neighbor] = dist[node] + 1;
                    q.push(neighbor);
                }
            }
        }
        return dist;
    }

    // BFS'leri test et
    void compute(const std::string& output_filename, int iterations = 10) {
        ofstream file(output_filename);
        if (!file.is_open()) {
            cerr << "File couldnt oppen!" << endl;
            return;
        }
        
        int start_node = 0; // 0-indexed olarak değiştirildi
        double total_time_custom = 0.0; 
        
        for (int i = 0; i < iterations; i++) {
            // Kendi BFS süresi 
            auto start_time = std::chrono::high_resolution_clock::now();
            vector<int> distances = bfs(start_node);
            auto end_time = std::chrono::high_resolution_clock::now();
            double elapsed_time = std::chrono::duration<double>(end_time - start_time).count();
            total_time_custom += elapsed_time; 
            
            // Sadece ilk çalıştırmada dosyaya yaz
            if (i == 0) {
                for (int j = 0; j < num_nodes; j++) {
                    if (j != start_node) {
                        file << "Node " << start_node << " -> " << j
                        << " shortest distance: " << distances[j] << "\n";
                    } 
                }
                file.close();
            }
        }
        
        double avg_custom = total_time_custom / iterations;
        std::cout << "Custom WAvarage: " << avg_custom << " second\n";
        file.close();
        ofstream file2("result.txt",  ios::app);
        if (!file2.is_open()) {
            cerr << "File couldn't open!" << endl;
            return;
        }
        file2 << "Custom : " << avg_custom << " s" << endl;
        file2.close();
    }
     
};

#endif // BFS_CUSTOM_HPP