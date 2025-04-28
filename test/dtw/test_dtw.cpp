//
// Created by AWAY on 25-4-28.
//
#include "dtw.h"
#include "iostream"

#define DTW_LEN 1024
using namespace std;

vector<double> s1(DTW_LEN), s2(DTW_LEN);
void read()
{
    for (int i = 0;i < DTW_LEN;i ++)
    {
        cin >> s1[i];
    }
    for (int i = 0;i < DTW_LEN;i ++)
    {
        cin >> s2[i];
    }
}

void write()
{
    for (int i = 0;i < DTW_LEN;i ++) cout << s1[i] << " ";
    cout << endl;
    for (int i = 0;i < DTW_LEN;i ++) cout << s2[i] << " ";
    cout << endl;
}

int main() {
    freopen("py.out", "r", stdin);
    freopen("test.out", "w", stdout);
    Dtw dtw;
    read();
    // write();

    double ret = dtw.get_cost(s1, s2, 10);
    cout << ret << endl;
    return 0;
}