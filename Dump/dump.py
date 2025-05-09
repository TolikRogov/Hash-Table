import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
data = pd.read_csv('Dump/data/CRC32.dat', sep=' ')
fig = plt.figure(figsize=(20, 10))
ax = fig.add_subplot()
ax.patch.set_facecolor('#addaed')
ax.patch.set_alpha(0.2)
ax.bar(data['x'], data['y'], width = 2, color = '#82b2fa')
ax.set_title('CRC32 dump')
ax.set_xlabel('Bucket number')
ax.set_ylabel('Bucket size')
fig.text(0.11, 0.05, f"Dispersion: {np.var(data['y']):.2f}")
plt.savefig('Dump/img/CRC32.svg', dpi=300)
plt.show()
