import numpy as np
from scipy import stats
import sys
import os
if "SSH_CONNECTION" in os.environ:
    import matplotlib
    matplotlib.use('GTKCairo')
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
#for i in range(n.shape[0]-1):
for i in range(1):
    ax = plt.subplot(111)
    s =n[i+1,:]
    ret = []
    slope, intercept, dummy,dummy,dummy = stats.linregress(n[0,:], s**2)
    ax.scatter(n[0,:], s**2)
    lab = "%.2g" %intercept+r'+'+"%.2g"%slope+r'*Pe'
    print lab
    ax.plot(n[0,:], intercept + slope*n[0,:], label=r'linear fit, '+lab)
    ax.set_ylim([0, np.max(s**2)*1.1])
    ax.set_xlim([0, np.max(n[0,:])*1.1])
    ax.legend(loc="best")
file_name = "vplt"
plt.xlabel(r'$Pe$')
plt.ylabel(r'$u^\mathregular{2}$')
plt.savefig(file_name+".png")
plt.savefig(file_name+".pdf", bbox_inches='tight')
plt.show()
