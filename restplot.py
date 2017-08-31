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
mask = n[0,:]<=3
nnew = n[:,mask]
pes = nnew[0,:]
u = nnew[1,:]
d = nnew[2,:]
w = nnew[3,:]
n = np.array([pes, u, d-w, w])
labels = ["dupa", r"$u^{\mathregular{2}}$", r"$d$", r"$w$"]

for i in range(n.shape[0]-1):
    print i
    ax = plt.subplot(str(n.shape[0]-1)+"1"+str(i+1))
    s =n[i+1,:]
    plt.ylabel(labels[i+1], rotation='vertical')
    print labels[i+1]
    slope, intercept, dummy,dummy,dummy = stats.linregress(n[0,:], s)
    ax.scatter(n[0,:], s)
    ax.set_xlim([0, np.max(n[0,:])*1.1])
    lab = "%.2g" %intercept+r'+'+"%.2g"%slope+r'*Pe'
    print lab
    ax.plot(n[0,:], intercept + slope*n[0,:], label=r'linear fit, '+lab)
    ax.legend(loc="best")
    ax.set_ylim([0, np.max(s)*1.1])
file_name = "rplt"
plt.xlabel(r'$Pe$')
print "saving to "+file_name
plt.savefig(file_name+".png")
plt.savefig(file_name+".pdf", bbox_inches='tight')
plt.show()
