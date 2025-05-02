import numpy as np


# 方法2：正态分布随机数（更接近真实音频特征）
data_normal = np.random.normal(loc=0.0, scale=0.3, size=256).clip(-1.0, 1.0)

# 打印结果（使用方法1）
print("\n".join(map("{:.4f}".format, data_normal)))
