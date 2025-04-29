import numpy as np

def generate_series(length=32):
    # 生成基础数列
    base_series = np.random.randn(length)
    # 将基础数列的值缩放到 uint16 范围 (0-65535)
    base_series = np.random.randint(0, 30000, length, dtype=np.uint16)
    # 生成小噪声，范围在 0-65535 之间
    noise = np.random.randint(0, 30000, length, dtype=np.uint16)
    # 生成相近数列
    series1 = base_series
    series2 = base_series + noise
    # 确保 series2 中的值不超过 uint16 的最大值
    return series1, series2

if __name__ == "__main__":
    series1, series2 = generate_series()
    # 保存数据到文件
    for i in series1 :
        print(i, end=" ")
    for i in series2 :
        print(i, end=" ")
