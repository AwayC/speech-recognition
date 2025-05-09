import numpy as np
from fastdtw import fastdtw
from scipy.spatial.distance import euclidean

def load_frames(file_path, frame_size=62):
    """加载数据文件并分割为MFCC帧"""
    with open(file_path, 'r') as f:
        # 解析带帧号的数据行（格式："行号: 系数1 系数2..."）
        lines = [line.strip().split(': ')[-1] for line in f if line.strip()]
    
    # 按帧尺寸分割并显示帧数
    frames = [lines[i:i+frame_size] for i in range(0, len(lines), frame_size)]
    print(f"共加载 {len(frames)} 个MFCC帧，每帧包含 {frame_size} 行数据")
    
    # 将每帧的多行MFCC合并为单个数组
    return [np.array([float(x) for line in frame for x in line.split()]) for frame in frames]

def compute_dtw_matrix(frames):
    """计算帧间DTW距离矩阵"""
    n = len(frames)
    matrix = np.zeros((n, n))
    
    for i in range(n):
        for j in range(i, n):
            distance, _ = fastdtw(frames[i], frames[j], dist=euclidean)
            matrix[i][j] = distance
            matrix[j][i] = distance  # 保持矩阵对称性
            
    return matrix

if __name__ == "__main__":
    frames = load_frames('data/max9814.out')
    dtw_matrix = compute_dtw_matrix(frames)
    for i in range(len(frames)):
        for j in range(len(frames)):
            print(f"{dtw_matrix[i][j]:.2f}", end=" ")
        print(f"\n")
    print(f"生成{len(frames)}x{len(frames)}的DTW距离矩阵已保存")