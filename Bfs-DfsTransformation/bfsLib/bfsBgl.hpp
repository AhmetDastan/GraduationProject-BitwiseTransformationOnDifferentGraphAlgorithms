#ifndef BFS_BGL_HPP
#define BFS_BGL_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <chrono>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>

using namespace std;

class BFSBgl {
public:
    using BoostGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
    using Vertex = boost::graph_traits<BoostGraph>::vertex_descriptor;

private:
    BoostGraph boostGraph;
    std::vector<std::vector<int>> adjList;
    int num_nodes = 0;
    int edge_count = 0;
    std::vector<int> distances;

public:
    void readGraphFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Dosya açılamadı: " << filename << std::endl;
            exit(1);
        }

        std::string line;
        while (getline(file, line)) {
            if (line[0] == 'c') continue; 
            
            if (line[0] == 'p') {
                std::stringstream ss(line);
                std::string tmp;
                ss >> tmp >> tmp >> num_nodes >> edge_count;
                
                adjList.resize(num_nodes);
                boostGraph = BoostGraph(num_nodes + 1);
            } 
            else if (line[0] == 'a') {
                std::stringstream ss(line);
                char a;
                int u, v;
                ss >> a >> u >> v;
                --v;
                --u;
                adjList[u].push_back(v);
                adjList[v].push_back(u);
                
                boost::add_edge(u, v, boostGraph);
            }
        }
        file.close();
    }

    void compute(const std::string& outputFile, int iterations = 10) { 
        std::ofstream file(outputFile);
        if (!file.is_open()) {
            std::cerr << "Dosya açılamadı: " << outputFile << std::endl;
            return;
        }
        
        int startVertex = 0;  // Başlangıç düğümü doğrudan 0 olarak ayarlandı
        double total_time = 0.0; 
        
        for (int i = 0; i < iterations; i++) {
            // Zamanlama başlangıcı
            auto start_time = std::chrono::high_resolution_clock::now();
            
            // Mesafe vektörünü sıfırlama
            distances.clear();
            distances.resize(num_nodes, -1);
            startVertex = 0;

            // Başlangıç düğümü için mesafeyi sıfırla
            distances[startVertex] = 0;
            
            // BFS kuyruğu
            std::queue<Vertex> bfsQueue;
            bfsQueue.push(startVertex);
            
            // BFS algoritması
            while (!bfsQueue.empty()) {
                Vertex currentVertex = bfsQueue.front();
                bfsQueue.pop();
                for (auto [neighborIt, neighborEnd] = boost::adjacent_vertices(currentVertex, boostGraph);
                     neighborIt != neighborEnd; ++neighborIt) {
                    Vertex neighbor = *neighborIt;
                    if (distances[neighbor] == -1) {
                        bfsQueue.push(neighbor);
                        distances[neighbor] = distances[currentVertex] + 1;
                    }
                }
            }
            
            // Zamanlama bitişi
            auto end_time = std::chrono::high_resolution_clock::now();
            double elapsed_time = std::chrono::duration<double>(end_time - start_time).count();
            total_time += elapsed_time; 
            
            // Sadece ilk çalıştırmada dosyaya yaz
            if (i == 0) {
                for (int v = 0; v < num_nodes; ++v) {
                    if (v != startVertex) {
                        file << "Node " << startVertex << " -> " << v
                        << " shortest distance: " << distances[v] << "\n";
                    }
                }
            }
        }
        
        double avg_time = total_time / iterations;
        std::cout << "BGL Avarage: " << avg_time << " second\n";
        file.close();
        ofstream file2("result.txt",  ios::app);
        if (!file2.is_open()) {
            cerr << "File couldn't open!" << endl;
            return;
        }
        file2 << "BGL : " << avg_time << " s" << endl;
        file2.close();
    }
};
 
#endif // BFS_BGL_HPP