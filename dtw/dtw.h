
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



class Dtw { 
    using Win = std::vector<std::pair<int, int> > ; 
public: 
    Dtw(){}; 
    ~Dtw(){}; 
    double get_cost(const std::vector<double>& src,
                    const std::vector<double> &t, 
                    int radius);
    double Fast_dtw(Ts &src, Ts &t, int radius); 
    double Path_dtw(Ts &src, Ts &t, Win *w);
    

private: 
    int radius; 
    std::vector<double> buf; 
    size_t buf_len; 
    std::vector<pathPoint> path; 
    static double distance(double a, double b) { return fabs(a - b); };
    void coarse(Ts &src, Ts &t); 
    void coarse_path(Ts &src, Ts &t, Win* w, int radius);
    
}; 


// #ifdef __cplusplus
// }
// #endif

#endif