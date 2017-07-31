import numpy as np
import sys
import os
if "SSH_CONNECTION" in os.environ:
    import matplotlib
    matplotlib.use('GTK')
import matplotlib.pyplot as plt

if len(sys.argv) < 2:
    file_path = "out.txt"
else:
    file_path = sys.argv[1]

if not os.path.exists(file_path):
    sys.exit('ERROR: Datafile %s was not found!' % sys.argv[1])
file_in = open(file_path, 'r')
fig=plt.figure() 
init = True
print "plotting.."
while(file_in):
    plt.clf()
    step_no = file_in.readline()
    for field in range(3):
        name = file_in.readline()
        size_x, size_y = file_in.readline().split()
        size_x = int(size_x)
        size_y = int(size_y)
        if init:
            data = np.zeros((3,size_x, size_y))
        for i in range(size_x):
            data[field,i,:] = file_in.readline().split()
        ax = plt.subplot("22"+str(field+1))
        cax = ax.imshow(data[field], interpolation='nearest')
        cax.set_clim(vmin=0, vmax=1)
        print "added ", name,  'max= ', np.max(data[field,:,:])
        cbar = fig.colorbar(cax, ticks=[0,0.3, 0.5,1], orientation='vertical')
        init = False
    ax = plt.subplot("22"+"4")
    fourier = np.absolute(np.fft.fft(data[2]))
    rev_four = np.absolute(np.fft.ifft(fourier))
    #cax = ax.imshow(fourier, interpolation='nearest')
    #print np.min(fourier), np.max(fourier)
    #cax.set_clim(vmin=-2, vmax=2)
    #cax.set_clim(vmin=0, vmax=1)
    cbar = fig.colorbar(cax, orientation='vertical')
    cut = fourier[:,1:50]
    cax = ax.imshow(cut, interpolation='nearest', aspect='auto')
    cax.set_clim(np.min(cut),np.max(cut))
    #plt.plot(cut)
    #ax.set_ylim(np.min(cut),np.max(cut))
    print "added special"
    plt.pause(0.1)
