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
i = 0
ax = plt.subplot(111)
s = np.square(n[i+1,:])
ax.plot(n[0,:], s)
ax.scatter(n[0,:], s)
ax.set_xlabel('Pe')
ax.set_ylabel('Square of front velocity [dx^2/dt^2]')
plt.savefig("fig2.pdf", bbox_inches='tight')
plt.show()
