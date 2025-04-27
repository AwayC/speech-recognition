#include "dtw.h"
#include <vector>
#include <utility>

double Dtw::get_cost(const std::vector<double>& src, const std::vector<double> &t, int radius) {
    Ts s = {0, src.size()}; 
    Ts ts = {src.size(), t.size()}; 
    buf_len = src.size() + t.size(); 
    buf.resize(src.size() * 2 + t.size() * 2); 
    for(int i = 0;i < src.size();i ++) { 
        buf[i] = src[i];
        buf[i + src.size()] = src[i];
    }
    double ret = Fast_dtw(s, ts, radius); 
    buf.clear(); 
    path.clear();
    return ret; 
}

double Dtw::Fast_dtw(Ts &src, Ts &t, int radius) {
    if(src.len <= radius || t.len <= radius) {
        Win* w = new Win(src.len); 
        for(int i = 0;i < src.len;i ++) { 
            w->at(i) = std::make_pair(0, t.len);
        }
        double ret = Path_dtw(src, t, w);
        delete w;
        return ret; 
    }

    Ts s1 = {buf_len, src.len / 2};
    Ts s2 = {buf_len + src.len / 2, t.len / 2};
    buf_len += src.len / 2 + t.len / 2; 
    coarse(src, s1);
    coarse(t, s2);

    double ret = Fast_dtw(s1, s2, radius); 
    Win* w = new Win(src.len); 
    coarse_path(src, t, w); 

    ret = Path_dtw(src, t, w);

    delete w; 
    return ret; 

}

#define PATH_MX 0x3f3f3f3f
#define COST_MX 1e9
double Dtw::Path_dtw(Ts &src, Ts &t, Win *w)  { 
    size_t len1 = src.len, len2 = t.len; 
    std::vector<std::vector<double>> cost(len1, std::vector<double>());
    std::vector<std::vector<pathPoint>> prev(len1, std::vector<pathPoint>());

    for (int i = 0;i < len1;i ++) {
        cost[i].resize(w->at(i).second - w->at(i).first + 1);
        prev[i].resize(w->at(i).second - w->at(i).first + 1);
    }
    cost[0][0] = distance(buf[src.head], buf[t.head]);
    prev[0][0] = {-1, -1};
    for (int i = 1;i < cost[0].size();i ++)
    {
        int act = i + w->at(0).first;
        cost[0][i] = cost[0][i - 1] + distance(buf[src.head], buf[act + t.head]);
        prev[0][i] = {act - 1, 0};
    }
    for(int i = 1;i < len1;i ++) {
        for (int j = 0;j < cost[i].size();j ++)
        {
            int act = j + w->at(i).first;
            double mn_cost = COST_MX;
            pathPoint mn_prev = {-1, -1};
            int pre_l = w->at(i - 1).first, pre_r = w->at(i - 1).first;
            if (act >= pre_l && act <= pre_r && cost[i - 1][act - pre_l] < mn_cost) // up
            {
                mn_cost = cost[i - 1][act - pre_l];
                mn_prev = {i - 1, act};
            }
            if (act - 1 >= pre_l && act - 1 <= pre_r && cost[i - 1][act - 1] < mn_cost) // up left
            {
                mn_cost = cost[i - 1][act - 1];
                mn_prev = {i - 1, act - 1};
            }
            if (j > 0 && cost[i][j - 1] < mn_cost) // left
            {
                mn_cost = cost[i][j - 1];
                mn_prev = {i, act - 1};
            }

            cost[i][j] = mn_cost + distance(buf[src.head + i], buf[act + t.head]);
            prev[i][j] = mn_prev;
        }
    }
    path.clear();
    int x = len1 - 1, y = len2 - 1;
    while (~x && ~y)
    {
        path.push_back({x, y});
        int pos = y - w->at(x).first;
        x = prev[x][pos].x;
        y = prev[x][pos].y;
    }

    reverse(path.begin(), path.end());
    double ret = cost[len1 - 1][len2 - 1];
    cost.clear();
    prev.clear();
    return cost[len1 - 1][len2 - 1];
}

void Dtw::coarse(Ts &src, Ts &t) {
    
}

void Dtw::coarse_path(Ts &src, Ts &t, Win* w)
{

}