#ifndef BFS_BITWISE__TEMP_HPP
#define BFS_BITWISE__TEMP_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdint>

// Bitwise implementation from custom implementation
#define block_t uint64_t
#define BLOCK_INDEX(bit_index) ((bit_index)/(sizeof(block_t)*8))
#define MASK_INDEX(bit_index) ((bit_index)%(sizeof(block_t)*8))
#define MASK(bit_index) ((block_t)1 << MASK_INDEX(bit_index))
#define TOTAL_BLOCK_NUM(vertex_num) (BLOCK_INDEX(vertex_num-1)+1)
#define SET_EDGE(vertex1, vertex2, edges) edges[vertex1][BLOCK_INDEX(vertex2)] |= MASK(vertex2); edges[vertex2][BLOCK_INDEX(vertex1)] |= MASK(vertex1);

using namespace std;

class BFSBtwTemp {
private:
    int num_nodes;
    int edge_count;
    vector<vector<block_t>> edges; // bitwise operations

public:
    BFSBtwTemp() : num_nodes(0), edge_count(0) {}

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
                edges.resize(num_nodes, std::vector<block_t>(TOTAL_BLOCK_NUM(num_nodes), 0)); // bitwise matrix 
            } else if (line[0] == 'a') {
                std::stringstream ss(line);
                char a;
                int u, v;
                ss >> a >> u >> v;
                u--; // 1 tabanlıdan 0 tabanlıya
                v--;
                SET_EDGE(u, v, edges);
            }
        }
        file.close();
    }

    std::vector<int> bfs_matrix(int start) {
        std::vector<int> dist(num_nodes, -1);
        std::queue<int> q;
        dist[start] = 0;
        q.push(start);
        
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            
            const auto &row = edges[node];
            for (size_t block_index = 0; block_index < row.size(); ++block_index) {
                block_t bits = row[block_index];
                while (bits) {
                    int offset = __builtin_ctzll(bits);
                    int v = block_index * 64 + offset;
                    if (v >= num_nodes) break;
                    if (dist[v] == -1) {
                        dist[v] = dist[node] + 1;
                        q.push(v);
                    }
                    bits &= bits - 1; // işlenen biti sıfırla
                }
            }
        }
        return dist;
    }

    void compute(const std::string& output_filename, int iterations = 10) {
        std::ofstream file(output_filename);
        if (!file.is_open()) {
            std::cerr << "Sonuç dosyası açılamadı!" << std::endl;
            return;
        }
        
        int start_node = 0;
        double total_time_custom = 0.0; 
        
        for (int i = 0; i < iterations; i++) {
            auto start_time = std::chrono::high_resolution_clock::now();
            std::vector<int> distances = bfs_matrix(start_node);
            auto end_time = std::chrono::high_resolution_clock::now();
            double elapsed_time = std::chrono::duration<double>(end_time - start_time).count();
            total_time_custom += elapsed_time; 
            
            if (i == 0) {
                for (int j = 0; j < num_nodes; j++) {
                    if (j != start_node) {
                        file << "Node " << start_node << " -> " << j
                             << " shortest distance: " << distances[j] << "\n";
                    } 
                }
            }
        }
        
        double avg_custom = total_time_custom / iterations;
        std::cout << "BTW Temp Avarage: " << avg_custom << " second\n";
        file.close();
        ofstream file2("result.txt",  ios::app);
        if (!file2.is_open()) {
            cerr << "File couldn't open!" << endl;
            return;
        }
        file2 << "Btw : " << avg_custom << " s" << endl;
        file2.close();
    } 
};

#endif // BFS_BITWISE_H
