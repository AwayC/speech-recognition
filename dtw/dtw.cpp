#include "dtw.h"

#include <cassert>
#include <vector>
#include <utility>
#include <iostream>
#define pr(x) std::cerr << #x << ": " << x << std::endl;

int Dtw::get_cost(const std::vector<uint16_t>& src, const std::vector<uint16_t> &t, int radius) {
    Ts s = {0, src.size()}; 
    Ts ts = {src.size(), t.size()}; 
    buf_len = src.size() + t.size(); 
    buf.resize(src.size() * 2 + t.size() * 2);
    for(int i = 0;i < src.size();i ++)
        buf[i] = src[i];
    for (int i = ts.head;i < ts.head + ts.len;i ++)
        buf[i] = t[i - ts.head];
    int ret = Fast_dtw(s, ts, radius);
    buf_len = 0;
    buf.clear(); 
    path.clear();
    return ret; 
}

int Dtw::Fast_dtw(Ts &src, Ts &t, int radius) {
    if(src.len <= radius || t.len <= radius) {
        Win* w = new Win(src.len); 
        for(int i = 0;i < src.len;i ++) { 
            w->at(i) = std::make_pair(0, t.len - 1);
        }
        double ret = Path_dtw(src, t, w);
        delete w;
        // pr(ret);
        // pr(src.len);
        return ret; 
    }

    Ts s1 = {buf_len, (src.len + 1) / 2};
    Ts s2 = {buf_len + (src.len + 1) / 2, (1 + t.len) / 2};
    buf_len += (src.len + 1) / 2 + (t.len + 1) / 2;
    assert(buf_len <= buf.size());
    coarse(src, s1);
    coarse(t, s2);

    int ret = Fast_dtw(s1, s2, radius);

    if (ret == -1)
    {
        buf_len -= (src.len + 1) / 2 + (t.len + 1) / 2;
        return -1;
    }

    Win* w = new Win(src.len); 
    coarse_path(src, t, w, radius);

    ret = Path_dtw(src, t, w);
    // pr(ret);
    // pr(src.len);
    delete w; 
    return ret;
}

#define PATH_MX 0x3f3f3f3f
#define COST_MX 1e9
int Dtw::Path_dtw(Ts &src, Ts &t, Win *w)  {
    size_t len1 = src.len, len2 = t.len;
    int brk = break_val * (len1 + len2);
    bool brk_flag = false;
    std::vector<std::vector<int>> cost(len1, std::vector<int>());
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
        prev[0][i] = {0, act - 1};
    }

    for(int i = 1;i < len1;i ++) {
        int mn = COST_MX;
        for (int j = 0;j < cost[i].size();j ++)
        {
            int act = j + w->at(i).first;
            int mn_cost = COST_MX;
            pathPoint mn_prev = {-1, -1};
            int pre_l = w->at(i - 1).first, pre_r = w->at(i - 1).second;
            if (act >= pre_l && act <= pre_r && cost[i - 1][act - pre_l] < mn_cost) // up
            {
                mn_cost = cost[i - 1][act - pre_l];
                mn_prev = {i - 1, act};
            }
            if (act - 1 >= pre_l && act - 1 <= pre_r && cost[i - 1][act - pre_l - 1] < mn_cost) // up left
            {
                mn_cost = cost[i - 1][act - pre_l - 1];
                mn_prev = {i - 1, act - 1};
            }
            if (j > 0 && cost[i][j - 1] < mn_cost) // left
            {
                mn_cost = cost[i][j - 1];
                mn_prev = {i, act - 1};
            }

            cost[i][j] = mn_cost + distance(buf[src.head + i], buf[act + t.head]);
            prev[i][j] = mn_prev;
            mn = std::min(mn, cost[i][j]);
        }
        if (mn > brk)
        {
            brk_flag = true;
            break;
        }
    }
    if (brk_flag)
    {
        path.clear();
        cost.clear();
        prev.clear();
        return -1;
    }
    path.clear();
    int x = len1 - 1, y = len2 - 1;
    while (~x && ~y)
    {
        path.push_back({x, y});
        int pos = y - w->at(x).first;
        y = prev[x][pos].y;
        x = prev[x][pos].x;
    }

    reverse(path.begin(), path.end());
    double ret = cost[len1 - 1].back();
    cost.clear();
    prev.clear();
    return ret;
}

void Dtw::coarse(Ts &src, Ts &t) {
    for (int i = 0;i < src.len;i += 2)
    {
        buf[t.head + (i >> 1)] = (buf[src.head + i] + buf[src.head + i + 1]) / 2;
    }
    if (src.len & 1)
    {
        buf[t.len + t.head] = (buf[src.head + src.len - 2] + buf[src.head + src.len - 1]) / 2;
    }
}

void Dtw::coarse_path(Ts &src, Ts &t, Win* w, int radius)
{
    assert(path.size());
    size_t ptr = 0;
    for (int i = 0;i < src.len + 1;i += 2)
    {
        int left = path[ptr].y;
        while (ptr + 1 < path.size() && path[ptr + 1].x == i / 2) ptr ++;
        int right = path[ptr].y;
        left <<= 1; right = (right << 1) | 1;
        left = (left < radius) ? 0 : left - radius;
        right = (t.len > radius + right) ? radius + right : int(t.len) - 1;
        if (i < src.len) w->at(i) = std::make_pair(left, right);
        if (i + 1 < src.len) w->at(i + 1) = std::make_pair(left, right);
    }
}