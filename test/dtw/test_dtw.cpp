//
// Created by AWAY on 25-4-28.
//
#include "dtw.h"
#include "iostream"

#define DTW_LEN 32
using namespace std;

using vec = vector<float>;
#define VEC_LEN 1
vector<vec> s1(DTW_LEN, vec(1)), s2(DTW_LEN, vec(1));
void read()
{
    for (int i = 0;i < DTW_LEN;i ++)
    {
        for (int j = 0;j < VEC_LEN;j ++)
            cin >> s1[i][j];
    }
    for (int i = 0;i < DTW_LEN;i ++)
    {
        for (int j = 0;j < VEC_LEN;j ++)
            cin >> s2[i][j];
    }
}

void write()
{
    for (int i = 0;i < DTW_LEN;i ++)
        for (int j = 0;j < VEC_LEN;j ++)
            cout << s1[i][j] << ' ' ;
    cout << endl;
    for (int i = 0;i < DTW_LEN;i ++)
        for (int j = 0;j < VEC_LEN;j ++)
            cout << s2[i][j] << ' ' ;
    cout << endl;
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    Dtw dtw;
    read();
    write();

    float ret = dtw.get_cost(s1, s2, 20);
    cout << ret << endl;
    return 0;
}