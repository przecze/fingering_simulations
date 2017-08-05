import numpy as np
import sys
import os
if "SSH_CONNECTION" in os.environ:
    import matplotlib
    matplotlib.use('GTKCairo')
import matplotlib.pyplot as plt
if len(sys.argv)<3:
    start_viewing_step = 0
else:
    start_viewing_step = int(sys.argv[2])
    print "start from", start_viewing_step
if len(sys.argv) < 2:
    file_path = "out/sim000"
else:
    file_path = sys.argv[1]

if not os.path.exists(file_path):
    sys.exit('ERROR: Datafile %s was not found!' % sys.argv[1])
file_in = open(file_path, 'r')
fig=plt.figure() 
init = True
viewing = False
while(file_in):
#for i in range(3):
    plt.clf()
    try:
        step_word, step_no = file_in.readline().split()
        step_no = int(step_no)
        if(step_no >= start_viewing_step and not viewing):
            viewing = True
            print step_no, "start viewing"
        for field in range(3):
            name = file_in.readline()
            try:
                size_x, size_y = file_in.readline().split()
            except:
                print "eof"
                raise ValueError("eof")
            size_x = int(size_x)
            size_y = int(size_y)
            if init:
                data = np.zeros((3,size_x, size_y))
            if viewing:
                for i in range(size_x):
                    data[field,i,:] = file_in.readline().split()
                ax = plt.subplot("22"+str(field+1))
                cax = ax.imshow(data[field], interpolation='nearest')
                if field==0:
                        cax.set_clim(vmin=0, vmax=0.1)
                        cbar = fig.colorbar(cax, orientation='vertical')
                else:
                        cax.set_clim(vmin=0, vmax=1)
                        cbar = fig.colorbar(cax, ticks=[0,0.3, 0.5,1], orientation='vertical')
                print "added ", name,  'max= ', np.max(data[field,:,:])
                init = False
            else:
                for i in range(size_x):
                    dummy=file_in.readline()
        if viewing:
            ax = plt.subplot("224")
            print data[0,:, size_y/2].shape
            print np.arange(size_x).shape
            ax.plot(data[0,:,size_y/2][::-1], np.arange(size_x))
            plt.suptitle("Step "+str(step_no))
            plt.pause(0.01)
    except ValueError as err:
        print 'done', err.message
        for field in range(3):
            ax = plt.subplot("22"+str(field+1))
            cax = ax.imshow(data[field], interpolation='nearest')
            if field==0:
                    cax.set_clim(vmin=0, vmax=0.1)
                    cbar = fig.colorbar(cax, orientation='vertical')
            else:
                    cax.set_clim(vmin=0, vmax=1)
                    cbar = fig.colorbar(cax, ticks=[0,0.3, 0.5,1], orientation='vertical')
            print "added ", name,  'max= ', np.max(data[field,:,:])
        plt.show()
        break
