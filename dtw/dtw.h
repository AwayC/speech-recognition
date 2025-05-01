
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

#define MAX_BREAK 1e5

class Dtw { 
    using Win = std::vector<std::pair<int, int> > ; 
    using mcVec = std::vector<float>; 
public: 
    Dtw(){}; 
    ~Dtw()
    {
        buf.~vector();
        path.~vector();
    };
    float get_cost(const std::vector<mcVec>& src,
                 const std::vector<mcVec> &t,
                 int radius);
    void set_vec_len(size_t len)
    {
        Vec_len = len;
    }
    float Fast_dtw(Ts &src, Ts &t, int radius);
    float Path_dtw(Ts &src, Ts &t, Win *w);
    void set_brkVal(int x)
    {
        break_val = x;
    };
    

private: 
    float break_val = MAX_BREAK;
    std::vector<mcVec> buf;
    size_t Vec_len = 1;
    size_t buf_len; 
    std::vector<pathPoint> path; 
    float distance(const mcVec& a, const mcVec& b);
    void coarse(Ts &src, Ts &t); 
    void coarse_path(Ts &src, Ts &t, Win* w, int radius);
    
}; 


// #ifdef __cplusplus
// }
// #endif

#endif