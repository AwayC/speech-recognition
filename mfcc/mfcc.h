//
// Created by AWAY on 25-4-30.
//

#ifndef __MFCC_H
#define __MFCC_H

#include "fft/fft.h"
#include "cmath"
#define PI acos(-1.0)

class Mfcc {
  using mcVec = std::vector<float>;
public:
    Mfcc(size_t ilen, int olen,  int f) : freq(f), in_len(ilen), mel_num(olen) {};
    
    void init();
    void get_mfcc(float* input, size_t ilen, mcVec& output);

private:
    int freq;
    size_t in_len;
    int mel_num = 26;
    std::vector<float> signal;
    std::vector<float> ham_win;
    std::vector<int> mel_filter;
    std::vector<std::vector<float>> dct_cos;
    void pre_emphasis(float alpha = 0.97);
    void hamming_window_init();
    void hamming_window();
    void mel_init();
    void dct_init();


};



#endif //__Mfcc_H
