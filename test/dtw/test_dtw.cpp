//
// Created by AWAY on 25-4-28.
//
#include "dtw.h"
#include "iostream"

#define DTW_LEN 32
using namespace std;

vector<uint16_t> s1(DTW_LEN), s2(DTW_LEN);
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
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    Dtw dtw;
    read();
    write();

    double ret = dtw.get_cost(s1, s2, 20);
    cout << ret << endl;
    return 0;
}