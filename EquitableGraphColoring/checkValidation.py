#!/usr/bin/env python3
"""
Simple Graph Coloring Verification Script
==========================================

This script verifies graph coloring solutions using simple string input.
Just paste your colors as space-separated numbers.

Usage:
1. Place your graph file (DSJC125.9.col) in the same directory
2. Edit the COLORING_STRING variable below with your colors
3. Set the K value (number of colors expected)
4. Run: python verify_coloring.py
"""

def parse_graph_file(filename):
    """Parse the DIMACS format graph file and return adjacency list"""
    edges = []
    num_vertices = 0
    num_edges = 0
    
    try:
        with open(filename, 'r') as file:
            for line in file:
                line = line.strip()
                if line.startswith('c'):  # Comment line
                    continue
                elif line.startswith('p'):  # Problem line
                    parts = line.split()
                    if len(parts) >= 4 and parts[1] == 'edge':
                        num_vertices = int(parts[2])
                        num_edges = int(parts[3])
                elif line.startswith('e'):  # Edge line
                    parts = line.split()
                    if len(parts) >= 3:
                        u = int(parts[1]) - 1  # Convert to 0-indexed
                        v = int(parts[2]) - 1  # Convert to 0-indexed
                        edges.append((u, v))
    except FileNotFoundError:
        print(f"ERROR: Graph file '{filename}' not found!")
        print("Please ensure the graph file is in the same directory as this script.")
        return None, 0, 0
    
    # Create adjacency list
    graph = [[] for _ in range(num_vertices)]
    for u, v in edges:
        if 0 <= u < num_vertices and 0 <= v < num_vertices:
            graph[u].append(v)
            graph[v].append(u)
    
    return graph, num_vertices, len(edges)

def parse_coloring_string(coloring_string):
    """Parse space-separated color string into list of integers"""
    try:
        numbers = []
        for line in coloring_string.strip().split('\n'):
            line = line.strip()
            if line:
                numbers.extend([int(x) for x in line.split()])
        return numbers
    except ValueError as e:
        print(f"ERROR: Failed to parse coloring string: {e}")
        return None

def verify_coloring_validity(graph, coloring):
    """Verify if the coloring is valid (no adjacent vertices have same color)"""
    if not graph:
        return False, "Graph not loaded"
    
    num_vertices = len(graph)
    
    # Check if all vertices are colored
    if len(coloring) != num_vertices:
        return False, f"Coloring length mismatch: expected {num_vertices}, got {len(coloring)}"
    
    # Check adjacent vertices have different colors
    invalid_edges = []
    for u in range(num_vertices):
        for v in graph[u]:
            if u < v and coloring[u] == coloring[v]:  # Check each edge only once
                invalid_edges.append((u + 1, v + 1, coloring[u]))  # Convert back to 1-indexed
    
    if invalid_edges:
        return False, f"Found {len(invalid_edges)} invalid edges: {invalid_edges[:5]}{'...' if len(invalid_edges) > 5 else ''}"
    
    return True, "All adjacent vertices have different colors"

def verify_equitable_coloring(coloring, expected_k):
    """Verify if the coloring is equitable"""
    color_counts = {}
    
    # Count occurrences of each color
    for color in coloring:
        color_counts[color] = color_counts.get(color, 0) + 1
    
    # Get statistics
    counts = list(color_counts.values())
    colors_used = sorted(color_counts.keys())
    min_count = min(counts)
    max_count = max(counts)
    num_colors_used = len(colors_used)
    
    # Check equitability
    is_equitable = (max_count - min_count) <= 1
    
    # Check if we use the expected number of colors
    correct_k = (num_colors_used == expected_k) if expected_k else True
    
    return {
        'is_equitable': is_equitable,
        'correct_k': correct_k,
        'colors_used': colors_used,
        'num_colors_used': num_colors_used,
        'expected_k': expected_k,
        'min_count': min_count,
        'max_count': max_count,
        'color_counts': color_counts,
        'count_distribution': sorted(counts)
    }

def print_analysis(coloring, equitable_result):
    """Print analysis of the coloring"""
    print("\n" + "="*50)
    print("COLORING ANALYSIS")
    print("="*50)
    
    print(f"Total vertices: {len(coloring)}")
    print(f"Colors used: {equitable_result['num_colors_used']}")
    print(f"Expected colors (K): {equitable_result['expected_k']}")
    print(f"Color range: {min(equitable_result['colors_used'])} to {max(equitable_result['colors_used'])}")
    
    print(f"\nColor distribution:")
    print(f"  Min vertices per color: {equitable_result['min_count']}")
    print(f"  Max vertices per color: {equitable_result['max_count']}")
    print(f"  Difference: {equitable_result['max_count'] - equitable_result['min_count']}")
    
    print(f"\nFirst 10 vertex colors: {coloring[:10]}")
    print(f"Last 10 vertex colors: {coloring[-10:]}")

def main():
    """Main verification function"""
    
    # ========================================================================
    # EDIT THESE VARIABLES: Paste your colors and set K value
    # ========================================================================
    
    COLORING_STRING = """
13 0 40 35 36 11 29 20 23 3 21 4 1 31 35 26 34 16 15 33 14 20 9 25 25 28 13 28 14 32 24 24 24 29 31 33 22 4 8 16 35 2 41 18 25 34 42 27 9 20 0 31 21 5 23 37 19 43 19 15 7 38 38 2 37 17 21 13 22 36 8 11 26 22 30 5 14 12 32 30 27 7 16 15 6 29 39 41 27 41 39 8 0 43 6 43 39 36 5 17 42 7 38 30 3 26 11 10 2 9 12 28 40 23 18 1 6 18 33 34 19 10 40 3 4 """
    ## 24 43 4 37 0 40
    K_VALUE = 44  # Expected number of colors
    
    # ========================================================================
    
    print("Graph Coloring Verification Tool")
    print("=" * 40)
    
    # Load graph
    print("Loading graph file...")
    graph_file = 'DSJC125.9.col'
    graph, num_vertices, num_edges = parse_graph_file(graph_file)
    
    if graph is None:
        return
    
    print(f"✅ Graph loaded: {num_vertices} vertices, {num_edges} edges")
    
    # Parse coloring
    print("Parsing coloring...")
    coloring = parse_coloring_string(COLORING_STRING)
    
    if coloring is None:
        return
    
    print(f"✅ Parsed {len(coloring)} colors")
    
    # Take only the required number of vertices
    if len(coloring) > num_vertices:
        print(f"⚠️  Taking first {num_vertices} colors from {len(coloring)} provided")
        coloring = coloring[:num_vertices]
    elif len(coloring) < num_vertices:
        print(f"❌ Not enough colors: need {num_vertices}, got {len(coloring)}")
        return
    
    # Verify validity
    print("\nChecking validity...")
    is_valid, validity_message = verify_coloring_validity(graph, coloring)
    
    if is_valid:
        print("✅ VALIDITY: PASSED")
        print(f"   {validity_message}")
    else:
        print("❌ VALIDITY: FAILED")
        print(f"   {validity_message}")
        return
    
    # Verify equitability
    print("\nChecking equitability...")
    equitable_result = verify_equitable_coloring(coloring, K_VALUE)
    
    if equitable_result['is_equitable']:
        print("✅ EQUITABLE: PASSED (color counts differ by ≤ 1)")
    else:
        print("❌ EQUITABLE: FAILED (color counts differ by > 1)")
    
    if equitable_result['correct_k']:
        print(f"✅ COLOR COUNT: CORRECT ({equitable_result['num_colors_used']} colors)")
    else:
        print(f"❌ COLOR COUNT: WRONG (got {equitable_result['num_colors_used']}, expected {K_VALUE})")
    
    # Print analysis
    print_analysis(coloring, equitable_result)
    
    # Final result
    print("\n" + "="*50)
    print("FINAL RESULT")
    print("="*50)
    
    if is_valid and equitable_result['is_equitable'] and equitable_result['correct_k']:
        print("🎉 SUCCESS: Valid, equitable, and correct number of colors!")
    elif is_valid and equitable_result['is_equitable']:
        print("⚠️  PARTIAL: Valid and equitable but wrong color count")
    elif is_valid:
        print("⚠️  PARTIAL: Valid but not equitable")
    else:
        print("❌ FAILED: Invalid coloring")

if __name__ == "__main__":
    main()