#ifndef __FFT_H
#define __FFT_H

#include "Complex.h"
#include <vector>
#include <cmath>
#include <cassert>

/*****  use exmple *****
 * 
 * std::vector<Complex> single = { 
 *  .... .....
 * }; 
 * 
 * FastFourier fft(8); 
 * 
 * fft.transform(single); 
 * fft.transform(single, true); 
 * 
 * 
*/
class FastFourier {
private:
    int N;
    std::vector<Complex> twiddle_factors;
    
    void bit_reverse(std::vector<Complex>& x) {
        int logn = log2(N);
        for (int i = 0; i < N; i++) {
            int rev = 0;
            for (int j = 0; j < logn; j++) {
                rev |= ((i >> j) & 1) << (logn - j - 1);
            }
            if (i < rev) {
                std::swap(x[i], x[rev]);
            }
        }
    }

public:
    FastFourier(int size) : N(size) {
        assert((N & (N-1)) == 0 && "Size must be power of 2");
    }

    void transform(std::vector<Complex>& x, bool inverse = false) {
        assert(x.size() >= N); 
        bit_reverse(x);
        int logn = log2(N);
        
        for (int s = 1; s <= logn; ++s) {
            int m = 1 << s;
            int m2 = m >> 1;
            double angle_sign = inverse ? 1.0 : -1.0;
            
            for (int k = 0; k < N; k += m) {
                for (int j = 0; j < m2; ++j) {
                    Complex twiddle(cos(angle_sign * 2 * M_PI * j / m),
                                   sin(angle_sign * 2 * M_PI * j / m));
                    
                    Complex t = x[k + j + m2] * twiddle;
                    Complex u = x[k + j];
                    
                    x[k + j] = u + t;
                    x[k + j + m2] = u - t;
                }
            }
        }

        if (inverse) {
            for (auto& c : x) {
                c = c / Complex(N, 0);
            }
        }
    }
};

#endif