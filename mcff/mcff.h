//
// Created by AWAY on 25-4-30.
//

#ifndef __MCFF_H
#define __MCFF_H

#include "fft/fft.h"
#include "cmath"
#define PI acos(-1.0)

class Mcff {
  using mcVec = std::vector<float>;
public:
    Mcff(size_t ilen, size_t olen,  int f) : freq(f), in_len(ilen), out_len(olen) {};
    ~Mcff();
    void init();
    void get_mcff(float* input, size_t ilen, mcVec& output);

private:
    int freq;
    size_t in_len;
    size_t out_len;
    int mel_num = 22;
    std::vector<float> signal;
    std::vector<float> ham_win;
    std::vector<int> mel_filter;
    std::vector<std::vector<float>> dct_cos;
    void pre_emphasis(float alpha = 0.97);
    void hamming_window_init();
    void hamming_window();
    void mel_init();
    void dct_init();
    void dct();


};



#endif //__MCFF_H
