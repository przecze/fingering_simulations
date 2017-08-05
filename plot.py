import numpy as np
import sys
import os
if "SSH_CONNECTION" in os.environ:
    import matplotlib
    matplotlib.use('GTK')
import matplotlib.pyplot as plt


if len(sys.argv)<3:
    start_plotting_step = 0
else:
    start_plotting_step = int(sys.argv[2])
    print "start from", start_plotting_step
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
I_vel_x = 2
I_vel_y = 3
I_lapl = 4
I_cvel_x = -3
I_cvel_y = -2
I_cvel = -1
vel_cum_n = 20
while(file_in):
    plt.clf()
    try:
        try:
            tip_data_string = file_in.readline()
        except:
            print "eof"
            raise ValueError("eof")
        print tip_data_string
        tip_data = map(float, tip_data_string.split())
        try:
            step = tip_data.pop(0)
            tip_num = tip_data.pop(0)
            parent = tip_data.pop(0)
        except:
            print "eof"
            raise ValueError("eof")
        #print tip_data
        if (step<start_plotting_step):
            continue
        #if (10001>1000):#TODO adjust velocity here
        #    print "adj vel", vel
        #    vel  = 0
        try:
            i = tip_nums.index(tip_num)
        except:
            tip_nums.append(tip_num)
            i = tip_nums.index(tip_num)
            data.append([])
        #modify velocities
        tip_data.append(tip_data[I_vel_x]) #cum_vel_x
        tip_data.append(tip_data[I_vel_y]) #cum_vel_y
        if data[i]:
            tip_data[I_cvel_x] += data[i][-1][1][I_cvel_x]
            tip_data[I_cvel_y] += data[i][-1][1][I_cvel_y]
            if len(data[i]) > vel_cum_n:
                tip_data[I_cvel_x] += data[i][-vel_cum_n][1][I_vel_x]
                tip_data[I_cvel_y] += data[i][-vel_cum_n][1][I_vel_y]
        tip_data.append(np.sqrt(tip_data[I_cvel_y]**2 + tip_data[I_cvel_x]**2))
        print tip_data
        data[i].append([step, tip_data])
        if (parent != tip_num and int(parent)!=-1):
            j = tip_nums.index(parent)
            previous_step = data[j][-1][0]
            bifs.append( [step, previous_step, i, j] )
    except ValueError as err:
        print 'done', err.message
        data_indexes = [I_lapl, I_cvel]
        #data = [tip[0::10] for tip in data]
        for data_index in data_indexes:
            ax = plt.subplot(str(len(data_indexes))+"1"+str(data_indexes.index(data_index)))
            for tip in data:
                steps = [entry[0] for entry in tip] 
                n = np.array([entry[1] for entry in tip])
                ax.plot(steps, n[:,data_index])
            for b in bifs:
                step, previous_step, i, j = b
                steps = [entry[0] for entry in data[i]]
                steps2 = [entry[0] for entry in data[j]]
                step_index = steps.index(step)
                previous_step_index = steps2.index(previous_step)
                #step_index = (steps>=step).argmax()
                #previous_step_index = (steps2<=previous_step).argmin()-1
                #step = steps[step_index]
                #steps2 = steps2[previous_step_index]
                xs = [previous_step, step]
                ys = [data[j][previous_step_index][1][data_index],\
                      data[i][step_index][1][data_index] ]
                ax.scatter(xs, ys)
                ax.plot(xs, ys, color='brown', linewidth=3)
        plt.show()
        break
