import numpy as np
import sys
import os
if "SSH_CONNECTION" in os.environ:
    import matplotlib
    matplotlib.use('GTK')
import matplotlib.pyplot as plt

if len(sys.argv) < 2:
    file_path = "out/sim000"
else:
    file_path = sys.argv[1]

if not os.path.exists(file_path):
    sys.exit('ERROR: Datafile %s was not found!' % sys.argv[1])
file_in = open(file_path, 'r')
fig=plt.figure() 
init = True
print "plotting.."
data = []
data2 = []
tip_nums = []
bifs =[]
bifs2 =[]
while(file_in):
    plt.clf()
    try:
        try:
            tip_data_string = file_in.readline()
        except:
            print "eof"
            raise ValueError("eof")
        print tip_data_string
        step, tip_num, lapl_val, vel, parent = map(float, tip_data_string.split())
        if (int(vel)>1000):
            print "adj vel", vel
            vel  = 0
        try:
            i = tip_nums.index(tip_num)
        except:
            tip_nums.append(tip_num)
            i = tip_nums.index(tip_num)
            data.append([])
            data2.append([])
        data[i].append([step, np.sqrt(int(vel))])
        data2[i].append([step, lapl_val])
        if (parent != tip_num and int(parent)!=-1):
            j = tip_nums.index(parent)
            bifs.append( [ data[i][-1], data[j][-2] ] )
            bifs2.append( [ data2[i][-1], data2[j][-2] ] )
        #print tip_nums
    except ValueError as err:
        print 'done', err.message
        ax = plt.subplot("111")
        for d in data:
            n = np.array(d)
            ax.plot(n[:,0], n[:,1])
        for b in bifs:
            bn = np.array(b)
            ax.scatter(bn[:,0], bn[:,1])
            ax.plot(bn[:,0], bn[:,1], color='brown', linewidth=3)
        #ax = plt.subplot("212")
        #for d in data2:
        #    n = np.array(d)
        #    ax.plot(n[:,0], n[:,1])
        #for b in bifs2:
        #    bn = np.array(b)
        #    ax.scatter(bn[:,0], bn[:,1])
        #    ax.plot(bn[:,0], bn[:,1], color='brown', linewidth=3)
            
        plt.show()
        break
