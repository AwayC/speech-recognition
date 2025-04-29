import numpy as np

def generate_series(length=256):
    # 生成基础数列
    base_series = np.random.randn(length)
    # 生成小噪声
    noise = np.random.normal(0, 10, length)
    # 生成相近数列
    series1 = base_series
    series2 = base_series + noise
    return series1, series2

if __name__ == "__main__":
    series1, series2 = generate_series()
    # 保存数据到文件
    for i in series1 :
        print(i, end=" ")
    print("\n", file=open("series1.txt", "a"))
    for i in series2 :
        print(i, end=" ")
