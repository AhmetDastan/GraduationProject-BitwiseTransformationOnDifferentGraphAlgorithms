#ifndef DFS__BTW__TEMP__HPP
#define DFS__BTW__TEMP__HPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <chrono>
#include <numeric>

// Bitwise implementation from custom implementation
#define block_t __uint64_t
#define BLOCK_INDEX(bit_index) ((bit_index)/(sizeof(block_t)*8))
#define MASK_INDEX(bit_index) ((bit_index)%(sizeof(block_t)*8))
#define MASK(bit_index) ((block_t)1 << MASK_INDEX(bit_index))
#define TOTAL_BLOCK_NUM(vertex_num) (BLOCK_INDEX(vertex_num-1)+1)

#define SET_EDGE(vertex1, vertex2, edges) edges[vertex1][BLOCK_INDEX(vertex2)] |= MASK(vertex2); edges[vertex2][BLOCK_INDEX(vertex1)] |= MASK(vertex1);
#define CHECK_BIT(arr, index) (arr[BLOCK_INDEX(index)] & MASK(index)) 
#define SET_TRUE(arr, index) (arr[BLOCK_INDEX(index)] |= MASK(index))

using namespace std;

class DFSBtwTemp {
private:
    vector<vector<block_t>> adjMatrix;
    int num_nodes;
    int edge_count;
public:
    DFSBtwTemp() : num_nodes(0), edge_count(0) {}

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
                // Komşuluk matrisini başlangıçta sıfırlarla doldur
                adjMatrix.resize(num_nodes, std::vector<block_t>(TOTAL_BLOCK_NUM(num_nodes), 0)); // bitwise operations
                
            } else if (line[0] == 'a') {
                std::stringstream ss(line);
                char a;
                int u, v;
                ss >> a >> u >> v;
                --u;
                --v;
                SET_EDGE(u, v, adjMatrix);
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
        
        std::vector<double> durations;
        vector<int> nodePos(num_nodes, -1);
        
        for (int it = 0; it < iterations; ++it) {
            vector<block_t> visited(TOTAL_BLOCK_NUM(num_nodes), 0);
            int counter = 0;
            auto start = std::chrono::high_resolution_clock::now();
            
            // DFS iterative with adjacency matrix
            std::stack<int> s;

            s.push(0); // 0'dan başla 
            while (!s.empty()) {
                int topElement = s.top();
                s.pop();
                
                if (CHECK_BIT(visited, topElement)) continue; 
                SET_TRUE(visited, topElement); 
                ++counter;
                
                if (nodePos[topElement] == -1) { 
                    nodePos[topElement] = counter;
                } 
                
                for (size_t block_index = 0; block_index < TOTAL_BLOCK_NUM(num_nodes); ++block_index) {  
                    block_t bits = adjMatrix[topElement][block_index]; // get spesific block
                    while (bits) { // till to 0
                        int offset = __builtin_ctzll(bits); // get LSB position
                        int v = (block_index << 6) + offset; // calculate spesific index

                        if (~CHECK_BIT(visited, v)) s.push(v);                        
                        bits &= bits - 1; // deleteLSB
                    }
                }

            }
            
            auto end = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration<double>(end - start).count(); 
            durations.push_back(duration);

            if (it == 0) { 
                for (int j = 1; j < num_nodes; ++j) {
                    file << "Node 0 " << " -> " << j
                         << " Shortest path: " << nodePos[j] << "\n";
                }
                file.close();
            }
        }
        
        double avg = std::accumulate(durations.begin(), durations.end(), 0.0) / iterations;
        std::cout << "temp btw Ortalama süre: " << avg << " s" << std::endl;
        ofstream file2("result.txt",  ios::app);
        if (!file2.is_open()) {
            cerr << "File couldn't open!" << endl;
            return;
        }
        file2 << "Btw : " << avg << " s" << endl;
        file2.close();
    }
};

#endif
