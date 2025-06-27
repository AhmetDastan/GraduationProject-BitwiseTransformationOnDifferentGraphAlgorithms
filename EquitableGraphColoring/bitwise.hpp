
// adjList to adjMax
#define block_t __uint64_t 
#define BLOCK_INDEX(bit_index) ((bit_index)/(sizeof(block_t)*8))
#define TOTAL_BLOCK_NUM(vertex_num) (BLOCK_INDEX(vertex_num-1)+1) // return max of block num 
#define MASK_INDEX(bit_index) ((bit_index)%(sizeof(block_t)*8))
#define MASK(bit_index) ((block_t)1 << MASK_INDEX(bit_index)) 
#define SET_EDGE(vertex1, vertex2, edges) edges[vertex1][BLOCK_INDEX(vertex2)] |= MASK(vertex2); edges[vertex2][BLOCK_INDEX(vertex1)] |= MASK(vertex1);
#define CHECK_BIT(arr, index) (arr[BLOCK_INDEX(index)] & MASK(index)) 
#define SET_TRUE(arr, index) (arr[BLOCK_INDEX(index)] |= MASK(index))

