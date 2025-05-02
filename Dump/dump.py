import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv('Dump/data/CRC32Intrinsic.dat', sep=' ')
fig = plt.figure(figsize=(20, 10))
ax = fig.add_subplot()
ax.patch.set_facecolor('#addaed')
ax.patch.set_alpha(0.2)
ax.bar(data['x'], data['y'], width = 2, color = '#82b2fa')
ax.set_title('Buckets dump')
ax.set_xlabel('Bucket number')
ax.set_ylabel('Bucket size')
plt.savefig('Dump/img/CRC32Intrinsic.svg', dpi=300)
plt.show()
