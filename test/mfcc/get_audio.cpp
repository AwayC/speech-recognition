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
#define frame_len 1024
#define frame_step 512
using namespace std;
using mcVec = vector<float>;
int main()
{
    int n ;
    cin >> n ;
    vector<vector<mcVec>> aud(n);
    for (int t = 0;t < n;t ++)
    {
        pr(t);
        string s;
        cin >> s;
        int freq, num;
        cin >> freq;
        cin >> s;
        cin >> num;
        pr(freq);
        pr(num);
        vector<float> audio(num);
        for (size_t i = 0;i < num;i ++)
        {
            cin >> audio[i];
        }
        Mfcc mfcc(frame_len, 26, 48000);
        mfcc.init();
        mcVec result;
        vector<mcVec> &frame = aud[t];
        for (int i = 0;i + frame_len < num;i += frame_step)
        {
            for (int j = 0;j < frame_len;j ++)
            {
                data[j] = audio[i + j];
            }

            int ret = mfcc.get_mfcc(data, frame_len, result);
            if (ret == MFCC_ENERGY_ZERO) continue;
            float mx = 0.0;
            for (int j = 0;j < result.size();j ++) { 
                mx = max(abs(result[j]), mx);
                cout << result[j] << ' ' ;
            }

            for (int j = 0;j < result.size();j ++)
            {
                result[j] = result[j] / mx;
            }
            cout << endl;
            frame.push_back(result);
        }
        cout << endl;
    }

    float dtw_mn = 1e16;
    int mn_pos = 0;
    for (int i = 0;i < n;i ++)
    {
        float cost_sum = 0;
        Dtw dtw;
        for (int j = 0;j < n;j ++)
        {
            if (j == i) continue;
            dtw.set_vec_len(13);
            float ret = dtw.get_cost(aud[i], aud[j] ,10);
            ret /= ((float)aud[i].size() + (float)aud[j].size());
            cerr << ret << " " ;
            cost_sum += ret;
        }
        cerr << endl;
        cerr << i << " cost_sum: " << cost_sum << endl;
        if (cost_sum > 0 && cost_sum < dtw_mn) mn_pos = i, dtw_mn = cost_sum;
    }
    cerr << setprecision(3);
    pr(mn_pos);
    pr(dtw_mn);


}