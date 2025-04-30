import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv('Dump/data/CRC32Intrinsic.dat', sep=' ')
plt.figure(figsize=(20, 10))
plt.grid(True, linestyle="--", alpha=0.5)
plt.fill_between(data['x'], data['y'], color="blue")
plt.plot(data['x'], data['y'], 'b-', label='Data')
plt.xlabel('Bucket number')
plt.ylabel('Bucket size')
plt.title('Buckets dump')
plt.legend()
plt.savefig('Dump/img/CRC32Intrinsic.svg', dpi=300)
plt.show()
