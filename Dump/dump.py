import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv('data.dat', sep='\s+')
plt.plot(data['x'], data['y'], 'b-', label='Data')
plt.xlabel('Bucket number')
plt.ylabel('Bucket size')
plt.title('Buckets dump')
plt.grid(True)
plt.legend()
plt.savefig('plot.svg', dpi=300)
plt.show()
