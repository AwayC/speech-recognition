
#ifndef __DTW_H
#define __DTW_H


// #ifdef __cplusplus
// extern "C" {
// #endif

#include <cmath>
#include <vector>
typedef struct { 
    size_t head;  
    size_t len; 
} Ts; 
typedef struct { 
    int x, y; 
} pathPoint; 

#define MAX_BREAK (1 << 14)

class Dtw { 
    using Win = std::vector<std::pair<int, int> > ; 
public: 
    Dtw(){}; 
    ~Dtw(){}; 
    int get_cost(const std::vector<uint16_t>& src,
                    const std::vector<uint16_t> &t,
                    int radius);
    int Fast_dtw(Ts &src, Ts &t, int radius);
    int Path_dtw(Ts &src, Ts &t, Win *w);
    void set_brkVal(int x)
    {
        break_val = x;
    };
    

private: 
    int radius;
    int break_val = MAX_BREAK;
    std::vector<uint16_t> buf;
    size_t buf_len; 
    std::vector<pathPoint> path; 
    static int distance(uint16_t a, uint16_t b) { return abs(a - b); };
    void coarse(Ts &src, Ts &t); 
    void coarse_path(Ts &src, Ts &t, Win* w, int radius);
    
}; 


// #ifdef __cplusplus
// }
// #endif

#endif