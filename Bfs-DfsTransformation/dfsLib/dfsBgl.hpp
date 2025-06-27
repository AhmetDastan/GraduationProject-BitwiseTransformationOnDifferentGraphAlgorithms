#ifndef DFS__BOOST__HPP
#define DFS__BOOST__HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <chrono>
#include <numeric>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

using namespace std;
using namespace boost;

class DFSBgl {
private:
    typedef adjacency_list<vecS, vecS, undirectedS> Graph;
    Graph g;
    int num_nodes;
    int edge_count;

public:
    DFSBgl() : num_nodes(0), edge_count(0) {}

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
                // Graph'ı baştan oluşturmak
                g = Graph(num_nodes);
            } else if (line[0] == 'a') {
                std::stringstream ss(line);
                char a;
                int u, v;
                ss >> a >> u >> v;
                --u;
                --v;
                add_edge(u, v, g);  // Kenar ekle
            }
        }
        file.close();
    }

    void compute(const std::string& output_filename, int iterations = 10) {
        ofstream file(output_filename);
        if (!file.is_open()) {
            cerr << "File couldn't open!" << endl;
            return;
        }

        std::vector<double> durations;
        
        for (int it = 0; it < iterations; ++it) {
            vector<int> nodePos(num_nodes, -1);
            std::vector<bool> visited(num_nodes, false);  
            int counter = 0;
            auto start = std::chrono::high_resolution_clock::now();

            // DFS iterative
            std::stack<int> s;
            s.push(0); // 0'dan başla

            while (!s.empty()) {
                int u = s.top();
                s.pop();
                if (visited[u]) continue;

                visited[u] = true; 

                ++counter;
                if(it == 0) nodePos.push_back(u); if (nodePos[u] == -1) { 
                    nodePos[u] = counter;
                }

                graph_traits<Graph>::adjacency_iterator adj, adj_end;
                for (tie(adj, adj_end) = adjacent_vertices(u, g); adj != adj_end; ++adj) {
                    int v = *adj;
                    if (!visited[v]) {
                        s.push(v);
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

        double avg = std::accumulate(durations.begin(), durations.end(), 0.0) / iterations;
        std::cout << "bgl Ortalama süre: " << avg << " s" << std::endl;
        ofstream file2("result.txt",  ios::app);
        if (!file2.is_open()) {
            cerr << "File couldn't open!" << endl;
            return;
        }
        file2 << "BGL:" << avg << " s" << endl;
        file2.close();
    }
};

#endif
