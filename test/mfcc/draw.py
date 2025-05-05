import matplotlib.pyplot as plt
import numpy as np

# 读取数据文件（假设数据存储在 data_conver.txt 中）
with open('data_conver.out', 'r') as f:
    lines = [line.strip() for line in f.readlines()]

# 创建画布和子图 (6行1列对应a1-a6)
fig, axs = plt.subplots(nrows=10, ncols=1, figsize=(12, 18))
plt.subplots_adjust(hspace=0.5)  # 调整子图间距

# 为每个音频文件绘制波形
for idx, ax in enumerate(axs):
    # 转换数据格式
    data = np.array([float(x) for x in lines[idx].split()])
    
    # 绘制波形
    ax.plot(data, linewidth=1)
    ax.set_title(f'a{idx+1}.wav MFCC C1', fontsize=10)
    ax.set_xlabel('Frame Index')
    ax.set_ylabel('MFCC Value')
    ax.grid(alpha=0.3)

# 保存合成图
plt.tight_layout()
plt.savefig('combined_plot.png', dpi=300)
plt.show()
print('所有波形图已保存至 combined_plot.png')
