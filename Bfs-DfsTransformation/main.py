import matplotlib.pyplot as plt
import numpy as np
import re

def parse_multiple_sections(file_path):
    sections = {}
    current_section = None
    graph_sizes = {}
    algorithms = {}
    performance_data = {}

    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()

        current_graph = None
        for line in lines:
            line = line.strip()
            if not line:
                continue

            # Başlık satırı
            if line.startswith("BFS") or line.startswith("DFS"):
                current_section = line
                sections[current_section] = []
                graph_sizes[current_section] = []
                algorithms[current_section] = []
                performance_data[current_section] = {}
                continue

            # graph satırı
            if line.startswith('graph'):
                current_graph = int(line.replace('graph', '').strip())
                if current_graph not in graph_sizes[current_section]:
                    graph_sizes[current_section].append(current_graph)
                continue

            # algoritma satırı
            if current_graph is not None and ':' in line:
                algo_name, time_str = line.split(':', 1)
                algo_name = algo_name.strip()

                time_match = re.search(r'([\d.eE+-]+)', time_str)
                if not time_match:
                    continue

                time_value = float(time_match.group(1)) * 1_000_000

                if algo_name not in algorithms[current_section]:
                    algorithms[current_section].append(algo_name)

                if algo_name not in performance_data[current_section]:
                    performance_data[current_section][algo_name] = {}

                performance_data[current_section][algo_name][current_graph] = time_value

        return graph_sizes, algorithms, performance_data

    except Exception as e:
        print(f"Dosya okuma hatası: {e}")
        return {}, {}, {}

# Dosya yolu
file_path = "result.txt"

# Verileri ayrıştır
graph_sizes_map, algorithms_map, performance_data_map = parse_multiple_sections(file_path)

# Renk paleti
colors = ['#3366CC', '#FF5733', '#FFC300', '#33A02C', '#A020F0']

# Her bölüm için ayrı grafik çiz
for section_title in graph_sizes_map:
    graph_sizes = graph_sizes_map[section_title]
    algorithms = algorithms_map[section_title]
    performance_data = performance_data_map[section_title]

    rows = 2
    cols = 3
    fig, axes = plt.subplots(rows, cols, figsize=(18, 12))
    axes = axes.flatten()

    for i, size in enumerate(graph_sizes):
        if i < len(axes):
            ax = axes[i]
            performance_values = [performance_data[algo][size] for algo in algorithms]
            
            bars = ax.bar(algorithms, performance_values, color=colors[:len(algorithms)])

            for bar in bars:
                height = bar.get_height()
                ax.text(bar.get_x() + bar.get_width()/2., height * 1.01,
                        f'{height:.3f} µs',
                        ha='center', va='bottom', rotation=0, fontsize=10)

            ax.set_title(f'Graph Vertex Size: {size}', fontsize=14)
            ax.set_xticklabels(algorithms, rotation=45, ha='right', fontsize=10)
            ax.grid(True, axis='y', linestyle='--', alpha=0.7)

    for j in range(i+1, len(axes)):
        axes[j].set_visible(False)

    # Genel başlık
    fig.suptitle(section_title, fontsize=16)

    # Yerleşim
    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    save_name = section_title.lower().replace(" ", "_").replace(":", "") + ".png"
    plt.savefig(save_name, dpi=300)
    plt.show()
