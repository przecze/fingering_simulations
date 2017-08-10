import numpy as np
import sys
import os
if "SSH_CONNECTION" in os.environ:
    import matplotlib
    matplotlib.use('GTK')
import matplotlib.pyplot as plt


if len(sys.argv) < 2:
    file_path = "out/pe_vs_properties.out"
else:
    file_path = sys.argv[1]

if not os.path.exists(file_path):
    sys.exit('ERROR: Datafile %s was not found!' % file_path)
data = []
file_in = open(file_path, 'r')
with open(file_path, 'r') as f:
      content = f.readlines()
content = [map(float, x.strip().split()) for x in content]
fig=plt.figure() 
n = np.transpose(np.array(content))
for i in range(n.shape[0]-1):
    ax = plt.subplot(str(n.shape[0]-1)+"1"+str(i+1))
    s =n[i+1,:]
    ax.plot(n[0,:], s)
    ax.scatter(n[0,:], s)
plt.show()
