import numpy as np
import wave
import matplotlib.pyplot as plt
from scipy.fftpack import dct

def pre_emphasis(signal, alpha=0.97):
    """
    预加重处理
    :param signal: 输入信号
    :param alpha: 预加重系数
    :return: 预加重后的信号
    """
    return np.append(signal[0], signal[1:] - alpha * signal[:-1])

def framing(signal, frame_length, frame_step):
    """
    分帧处理
    :param signal: 输入信号
    :param frame_length: 帧长
    :param frame_step: 帧移
    :return: 分帧后的信号
    """
    signal_length = len(signal)
    frame_length = int(round(frame_length))
    frame_step = int(round(frame_step))
    if signal_length <= frame_length:
        num_frames = 1
    else:
        num_frames = 1 + int(np.ceil((1.0 * signal_length - frame_length) / frame_step))

    pad_signal_length = (num_frames - 1) * frame_step + frame_length
    z = np.zeros((pad_signal_length - signal_length))
    pad_signal = np.append(signal, z)

    indices = np.tile(np.arange(0, frame_length), (num_frames, 1)) + np.tile(
        np.arange(0, num_frames * frame_step, frame_step), (frame_length, 1)).T
    indices = np.array(indices, dtype=np.int32)
    frames = pad_signal[indices]
    return frames

def hamming_window(frame_length):
    """
    生成汉明窗
    :param frame_length: 窗长
    :return: 汉明窗
    """
    return 0.54 - 0.46 * np.cos(2 * np.pi * np.arange(frame_length) / (frame_length - 1))

def mel_filterbank(num_filters, NFFT, sample_rate):
    """
    生成 Mel 滤波器组
    :param num_filters: 滤波器数量
    :param NFFT: FFT 点数
    :param sample_rate: 采样率
    :return: Mel 滤波器组
    """
    low_freq_mel = 0
    high_freq_mel = 2595 * np.log10(1 + (sample_rate / 2) / 700)  # 转换为 Mel 频率
    mel_points = np.linspace(low_freq_mel, high_freq_mel, num_filters + 2)  # 均匀分布 Mel 频率
    hz_points = 700 * (10**(mel_points / 2595) - 1)  # 转换回 Hz 频率
    bin = np.floor((NFFT + 1) * hz_points / sample_rate)

    fbank = np.zeros((num_filters, int(np.floor(NFFT / 2 + 1))))
    for m in range(1, num_filters + 1):
        f_m_minus = int(bin[m - 1])   # 左
        f_m = int(bin[m])             # 中
        f_m_plus = int(bin[m + 1])    # 右

        for k in range(f_m_minus, f_m):
            fbank[m - 1, k] = (k - bin[m - 1]) / (bin[m] - bin[m - 1])
        for k in range(f_m, f_m_plus):
            fbank[m - 1, k] = (bin[m + 1] - k) / (bin[m + 1] - bin[m])
    return fbank

# 读取音频文件
try:
    f = wave.open(r"amtls.wav", "rb")
except FileNotFoundError:
    print("Audio file not found, please check the file path.")
else:
    params = f.getparams()
    nchannels, sampwidth, framerate, nframes = params[:4]
    str_data = f.readframes(nframes)
    signal = np.frombuffer(str_data, dtype=np.short)
    signal = signal * 1.0 / (max(abs(signal)))

    # 预加重
    alpha = 0.97
    emphasized_signal = pre_emphasis(signal, alpha)

    # 分帧参数
    frame_length = 0.025 * framerate
    frame_step = 0.01 * framerate
    frames = framing(emphasized_signal, frame_length, frame_step)

    # 加窗
    win = hamming_window(int(frame_length))
    frames = frames * win

    # FFT 参数
    NFFT = 512
    mag_frames = np.absolute(np.fft.rfft(frames, NFFT))

    # 功率谱
    pow_frames = ((1.0 / NFFT) * ((mag_frames) ** 2))

    # Mel 滤波器组参数
    num_filters = 26
    fbank = mel_filterbank(num_filters, NFFT, framerate)
    filter_banks = np.dot(pow_frames, fbank.T)
    filter_banks = np.where(filter_banks == 0, np.finfo(float).eps, filter_banks)  # 数值稳定性
    filter_banks = 20 * np.log10(filter_banks)  # dB

    # DCT 参数
    num_ceps = 12
    mfcc_coeffs = dct(filter_banks, type=2, axis=1, norm='ortho')[:, 1: (num_ceps + 1)]

    # 绘图
    time = np.arange(0, len(signal)) / framerate

    # 原始信号
    plt.figure(figsize=(12, 10))
    plt.subplot(4, 1, 1)
    plt.plot(time, signal)
    plt.title('Original Audio Signal')
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude')

    # 预加重后的信号
    plt.subplot(4, 1, 2)
    plt.plot(time, emphasized_signal)
    plt.title('Pre - emphasized Audio Signal')
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude')

    # 分帧加窗后的一帧信号
    plt.subplot(4, 1, 3)
    plt.plot(frames[4])
    plt.title('A Framed and Windowed Signal')
    plt.xlabel('Sample')
    plt.ylabel('Amplitude')

    # Mel 滤波器组
    plt.subplot(4, 1, 4)
    for filter in fbank:
        plt.plot(filter)
    plt.title('Mel Filter Bank')
    plt.xlabel('Frequency (bin)')
    plt.ylabel('Amplitude')

    plt.tight_layout()
    plt.savefig('audio_processing_steps.png', dpi=300)  # 保存第一个图表
    plt.show()

    # MFCC 系数热力图
    plt.figure(figsize=(10, 6))
    plt.imshow(np.swapaxes(mfcc_coeffs, 0, 1), cmap='viridis', origin='lower', 
               extent=[0, mfcc_coeffs.shape[0], 1, mfcc_coeffs.shape[1]])
    plt.title('MFCC Coefficients')
    plt.xlabel('Frame')
    plt.ylabel('MFCC Coefficient')
    plt.colorbar()
    plt.savefig('mfcc_coefficients.png', dpi=300)  # 保存第二个图表
    plt.show()

    print("MFCC coefficients shape:", mfcc_coeffs.shape)