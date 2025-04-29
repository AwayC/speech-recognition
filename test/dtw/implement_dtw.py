import numpy as np
from fastdtw import fastdtw

def dtw(s, t):
    """标准 DTW 算法实现"""
    n, m = len(s), len(t)
    dtw_matrix = np.full((n + 1, m + 1), np.inf)
    dtw_matrix[0][0] = 0

    for i in range(1, n + 1):
        for j in range(1, m + 1):
            cost = np.abs(s[i - 1] - t[j - 1])
            last_min = np.min([dtw_matrix[i - 1][j], dtw_matrix[i][j - 1], dtw_matrix[i - 1][j - 1]])
            dtw_matrix[i][j] = cost + last_min
    return dtw_matrix[n][m]

if __name__ == "__main__":
    # 从文件中读取数据
    all_data = np.loadtxt('series1.txt')
    
    # 提取前 256 个数到 s1
    s1 = all_data[:256]
    # 提取接下来的 256 个数到 s2
    s2 = all_data[256:512]

    # 使用标准 DTW 算法计算距离
    dtw_distance = dtw(s1, s2)

    # 使用 fastdtw 库计算距离
    fast_dtw_distance, path = fastdtw(s1, s2)

    print(f"标准 DTW 距离: {dtw_distance}")
    print(f"FastDTW 距离: {fast_dtw_distance}")
