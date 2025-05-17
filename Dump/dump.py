import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
data = pd.read_csv('Dump/data/CRC32Intrinsic.dat', sep=' ')
fig = plt.figure(figsize=(20, 10))
ax = fig.add_subplot()
ax.patch.set_facecolor('#addaed')
ax.patch.set_alpha(0.2)
ax.bar(data['x'], data['y'], width = 2, color = '#82b2fa')
ax.set_title('CRC32Intrinsic dump', fontsize=24)
ax.set_xlabel('Bucket number', fontsize=20)
ax.set_ylabel('Bucket size', fontsize=20)
fig.text(0.11, 0.05, f"Dispersion: {np.var(data['y']):.2f}", fontsize=20)
plt.savefig('Dump/img/CRC32Intrinsic.svg', dpi=300)
plt.show()
