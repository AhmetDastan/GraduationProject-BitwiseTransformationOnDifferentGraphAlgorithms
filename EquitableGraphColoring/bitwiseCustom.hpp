#ifndef CUSTOM_BITWISE_HPP
#define CUSTOM_BITWISE_HPP
#include <iostream> 
#include <string.h>
#include <time.h>
#include <ctime> 


#define block_t __uint64_t 

using namespace std;
struct var{
    int vertexAmount; // vertex sayısı
    int totalBlockAmount;  // blok sayısı
    int insideOfBlock; // blok içindeki sayı sayısı
    int bitWidth = 1;     // bit genişliği

    void init(int vertexAmount) {
        this->vertexAmount = vertexAmount;
        this->bitWidth = (((vertexAmount) <= 1) ? 1 : (64 - __builtin_clzll((vertexAmount) - 1)));
        this->insideOfBlock = (sizeof(block_t) * 8 / this->bitWidth);
        this->totalBlockAmount = (((vertexAmount) + this->insideOfBlock - 1) / this->insideOfBlock);
    }
    void printInfo() const {
        cout <<"vertexAmount: " << vertexAmount << ", totalBlockAmount: " << totalBlockAmount
             << ", insideOfBlock: " << insideOfBlock << ", bitWidth: " << bitWidth << endl;
    }
}config;


#define BLOCK_INDEX(index) ((index) / config.insideOfBlock) // which line
#define MASK_INDEX(index) ((index) % config.insideOfBlock) // which row


#define SET_VALUE(array, index, value) do { \
    size_t block_i = BLOCK_INDEX(index); \
    size_t bit_i = MASK_INDEX(index) * config.bitWidth; \
    block_t mask = ((1ULL << config.bitWidth) - 1) << bit_i; \
    array[block_i] &= ~mask; \
    array[block_i] |= ((block_t)((value) & ((1U << config.bitWidth) - 1)) << bit_i); \
} while (0)

// Değer oku
#define GET_VALUE(array, index) ( \
    ((array[BLOCK_INDEX(index)] >> (MASK_INDEX(index) * config.bitWidth)) & \
    ((1ULL << config.bitWidth) - 1)) \
)


void bulk_copy_compressed_values(block_t* dest, const block_t* source) {
    for (int i = 0; i < config.totalBlockAmount; ++i) {
        dest[i] = source[i];  // Direkt blok kopyalama
    }
}

void bulk_compress_from_ints(block_t* dest, const int* source) {
    for (int i = 0; i < config.vertexAmount; ++i) {
        size_t block_i = BLOCK_INDEX(i);
        size_t bit_i = MASK_INDEX(i) * config.bitWidth;
        block_t mask = ((1ULL << config.bitWidth) - 1) << bit_i;

        dest[block_i] &= ~mask;  // Hedef alandaki eski değeri temizle
        dest[block_i] |= ((block_t)(source[i] & ((1U << config.bitWidth) - 1))) << bit_i;  // Yeni değeri yaz
    }
}


#endif