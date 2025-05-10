//
// Created by AWAY on 25-5-3.
//
#include <iostream>
#include "mfcc.h"
#include <cstring>
#include <iomanip>

#include "dtw.h"

// a16.wav is the template
float data[2000];
#define pr(x) cerr << #x << " " << x << endl
#define prs(x) cerr << x << endl;
#define frame_len 256
#define frame_step 128
#define frame_num 62
#define mel_num 13
using namespace std;
using mcVec = vector<float>;
vector<vector<mcVec>> frames;
int main()
{
    string s;
    while (cin >> s)
    {
        vector<mcVec> vec;
        float val;
        for (int i = 0;i < frame_num;i ++)
        {
            if (i) cin >> s;
            mcVec v;
            for (int j = 0;j < mel_num;j ++)
            {
                cin >> val;
                v.push_back(val);
            }
            vec.push_back(v);
        }
        frames.push_back(vec);
    }
    Dtw dtw;
    for (int i = 0;i < frames.size();i ++)
    {
        float sum = 0;
        for (int j = 0;j < frames.size();j ++)
        {
            float ret = dtw.get_cost(frames[i], frames[j], 10);
            cerr << ret << " ";
            sum += ret;
        }
        cerr << endl;
        cerr << "sum " << sum << endl;
    }
}