import numpy as np
from fastdtw import fastdtw

len = 256
# 生成基础数列
base_series = np.random.randn(len)

# 生成相近数列，添加小噪声
noise = np.random.normal(0, 1, len)
series1 = base_series
series2 = base_series + noise

# 使用 fastdtw 计算 DTW 距离
distance, path = fastdtw(series1, series2)

for i in series1 :
    print(i, end=" ")
print("\n")
for i in series2 :
    print(i, end=" ")
print("\n")
print(f"DTW 距离: {distance}")