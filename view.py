import numpy as np
import matplotlib.pyplot as plt

file_in = open("out.txt", 'r')
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
    plt.pause(0.5)
    file_in.readline()
