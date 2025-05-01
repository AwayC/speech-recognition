//
// Created by AWAY on 25-4-30.
//

#include "mcff.h"

void Mcff::init()
{
    signal.resize(in_len);
    hamming_window_init();
    mel_init();
    dct_init();
}

void Mcff::pre_emphasis(float alpha)
{
    for (int i = in_len - 1;i > 0;i --)
    {
        signal[i] = signal[i] - signal[i - 1] * alpha;
    }
}

void Mcff::hamming_window_init()
{
    ham_win.resize(in_len);
    float k = 2.0 / (float) in_len;
    float w;
    for (int i = 0;i < in_len;i ++)
    {
        w = 0.54 - 0.46 * cosf(PI * i * k);
        ham_win[i] = w;
    }
}

void Mcff::mel_init()
{
    float low_f = 0;
    float high_f = freq / 2;
    high_f = 2595 * log10(1 + high_f / 700);
    float mel_scale = high_f / (mel_num + 1);
    mel_filter.resize(mel_num + 2);
    double ret;
    for (int i = 1;i <= mel_num + 1;i ++)
    {
        ret = mel_scale * (i);
        ret = 700 * (pow(10, ret / 2595) - 1);
        mel_filter[i] = floor((in_len + 1) * ret / freq);
    }
}

void Mcff::dct_init()
{
    dct_cos.resize(mel_num);
    for (int i = 0;i < mel_num;i ++)
    {
        dct_cos[i].resize(mel_num);
        float k = (float)(i + 1) / ((mel_num) * 2.0f);
        for (int j = 0;j < mel_num;j ++)
        {
            dct_cos[i][j] = cos(k * PI * (2.0f * j + 1));
        }
    }
}

void Mcff::hamming_window()
{
    for (int i = 0;i < in_len;i ++)
    {
        signal[i] = signal[i] * ham_win[i];
    }
}

void Mcff::get_mcff(float* input, size_t ilen, mcVec& output)
{
    assert(ilen == in_len);
    std::vector<Complex> sig(in_len);
    for (int i = 0;i < in_len;i ++)
        signal[i] = input[i];
    //emphasis
    pre_emphasis();
    //hamming
    hamming_window();
    // fft
    for (int i = 0;i < in_len;i ++)
        sig[i] = Complex(signal[i], 0);
    FastFourier fft(in_len);
    fft.transform(sig);
    for (int i = 0;i < in_len;i ++)
        signal[i] = sig[i].abs() / in_len;
    //mel
    std::vector<float> result(mel_num);
    for (int i = 1;i <= mel_num;i ++)
    {
        int f_m_mn = mel_filter[i - 1];
        int f_m = mel_filter[i];
        int f_m_mx = mel_filter[i + 1];
        float sum = 0;
        for (int j = f_m_mn;j < f_m;j ++)
        {
            sum += signal[j] * (j - f_m_mn) / (f_m - f_m_mn);
        }
        for (int j = f_m;j < f_m_mx;j ++)
        {
            sum += signal[j] * (f_m_mx - j) / (f_m_mx - f_m);
        }
        result[i - 1] = log10(sum);
    }
    // dct
    output.resize(mel_num / 2);
    for (int i = 0;i < mel_num / 2;i ++)
    {
        float sum = 0;
        for (int j = 0;j < mel_num;j ++)
        {
            sum += result[j] * dct_cos[i][j];
        }
        output[i] = sum * sqrtf(2.0f / (float)mel_num) * (1.0f + 0.5f * mel_num * sinf(PI * i / mel_num));
    }

}