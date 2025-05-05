import numpy as np
import wave
import matplotlib.pyplot as plt
from scipy.fftpack import dct
import librosa


all_data = np.loadtxt('save.out')

def librosa_mfcc(y, sr) :
    return librosa.feature.mfcc(
        y = y[:256], 
        sr = sr, 
        n_mfcc = 13, 
        n_fft = 256,
        win_length = 256,
        hop_length = 257,
        window = 'hamming',
        n_mels = 28,
        htk = True,
        power = 1.0,
        center = False,
        dct_type=2,
        norm = None,
    )

mfcc_librosa = librosa_mfcc(all_data, 8000)

# print(mfcc_librosa)
y_emph = librosa.effects.preemphasis(all_data[:256], coef=0.97)
print("预加重结果样本:", y_emph[:5])

# 2. 获取分帧加窗后的帧
stft = librosa.core.spectrum._spectrogram(
    y=y_emph[:256],
    n_fft=256,
    hop_length=257,
    win_length=256,
    window='hamming',
    center=False
)
frames = stft[0]  # 获取分帧加窗后的数据
print("分帧加窗后的前5帧:", frames[:10])
print("加窗后首帧能量:", np.sum(frames**2))

# 3. 获取幅度谱
D = np.abs(librosa.stft(
    all_data[:256],
    n_fft=256,
    hop_length=257,
    win_length=256,
    window='hamming',
    center=False
)) / 256
D2 = np.abs(librosa.stft(
    all_data[:256],
    n_fft=256,
    hop_length=257,
    win_length=256,
    window='hamming',
    center=False
))
print("幅度谱前5点:", D[:5, 0])

# 4. 获取梅尔滤波器组
mel_basis = librosa.filters.mel(
    sr=8000,
    n_fft=256,
    n_mels=28,
    htk=True,
    fmin=0,
    fmax=4000,
    norm=None
)

print("首个梅尔滤波器:", mel_basis[1, :10], sum(mel_basis[2])); 
print("Mel中心频率:", librosa.mel_frequencies(n_mels=28, fmin=0, fmax=4000, htk=True))
# 5. 计算梅尔能量
mel_energy = np.dot(mel_basis, D[:, 0])
print("梅尔能量uem:", mel_energy)

mel_energyu2 = np.dot(mel_basis, D2[:, 0])
print("梅尔能量u2:", mel_energyu2)

mel_energy2 = np.dot(mel_basis, frames[:, 0])
print("梅尔能量:", mel_energy2)

# 6. 获取DCT输入
log_mel = np.log10(mel_energy)
print("对数梅尔能量u:", log_mel)

log_mel2 = np.log10(mel_energy2)
print("对数梅尔能量:", log_mel2)    

log_melu2 = np.log10(mel_energyu2)
print("对数梅尔能量:", log_melu2) 

# 7. 执行DCT变换
mfcc = dct(log_mel, type=2, norm=None)
print("原始DCT结果u:", mfcc[:13])
print(mfcc_librosa)

mfcc2 = dct(log_mel2, type=2, norm="ortho")
print("原始DCT结果:", mfcc2[:13])
print(mfcc_librosa)

mfccu2 = dct(log_melu2, type=2, norm="ortho")
print("原始DCT结果:", mfccu2[:13])
print(mfcc_librosa)

