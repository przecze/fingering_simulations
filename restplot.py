import numpy as np
from scipy import stats
from scipy.optimize import curve_fit
import sys
import os
if "SSH_CONNECTION" in os.environ:
    import matplotlib
    matplotlib.use('GTKCairo')
import matplotlib.pyplot as plt

def func(x, a, b):
  return (b - np.sqrt(a*x))/np.sqrt(a*x)


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
mask = n[0,:]<3
nnew = n[:,mask]
pes = nnew[0,:]
u = nnew[1,:]
w = nnew[3,:]
d = nnew[2,:]-w
n = np.array([pes, w**2/(d+w)**2, w])
labels = ["dupa", r"(w/(w+d))$^{\mathregular{2}}$", r"$w$"]

for i in range(n.shape[0]-1):
    print i
    ax = plt.subplot(str(n.shape[0]-1)+"1"+str(i+1))
    s =n[i+1,:]
    plt.ylabel(labels[i+1], rotation='vertical')
    print labels[i+1]
    ax.scatter(n[0,:], s)
    ax.set_xlim([0, np.max(n[0,:])*1.1])
    if i==-1:
        popt, pcov = curve_fit(func, n[0,:], s) 
        lab = "A = %.2g" %popt[0] + "B = %.2g" %popt[1]
        print lab
        ax.plot(np.sort(n[0,:]), func(np.sort(n[0,:]), popt[0], popt[1]), label = lab)
    else:
        slope, intercept, dummy,dummy,dummy = stats.linregress(n[0,:], s)
        lab = "%.2g" %intercept+r'+'+"%.2g"%slope+r'*Pe'
        print lab
        ax.plot(n[0,:], intercept + slope*n[0,:], label=r'linear fit, '+lab)
    ax.legend(loc="upper left")
    if i==0:
      ax.legend(loc="lower right")
    ax.set_ylim([0, np.max(s)*1.1])
file_name = "rplt"
plt.xlabel(r'$Pe$')
print "saving to "+file_name
plt.savefig(file_name+".png")
plt.savefig(file_name+".pdf", bbox_inches='tight')
plt.show()
