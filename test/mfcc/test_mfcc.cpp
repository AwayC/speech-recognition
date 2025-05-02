//
// Created by AWAY on 25-5-1.
//
#include "../../mfcc/mfcc.h"
#include <vector>
#include <iostream>

#define DATA_LEN 256
#define FREQ 8000
#define MEL_NUM 26
float data[DATA_LEN + 10];

void read() {
    for(int i = 0;i < DATA_LEN;i ++) {
        std::cin >> data[i];
    }
    std::cerr << "read" << std::endl;
}

void write() {
    for(int i = 0;i < DATA_LEN;i ++) {
      std::cout << data[i] << ' ';
    }
}

int main()
{
    Mfcc mfcc(DATA_LEN, MEL_NUM, FREQ);
    mfcc.init();
    std::cerr << "INIT" << std::endl;
    read();
    // write();
    std::vector<float> result;
    mfcc.get_mfcc(data, DATA_LEN, result);
    std::cout << result.size() << std::endl;
    for (auto i : result)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
