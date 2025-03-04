import matplotlib.pyplot as plt
import numpy as np
import serial

ser = serial.Serial('COM5', 57600, timeout=0.1) 

plt.ion()
fig = plt.figure()
ax = fig.add_subplot(111)
plt.ylim([-370, 370])
plt.xlim([0, 100])

dataxyz = [[], [], [], [], [], [], [], [], []]

# linexg, = ax.plot([], [], label='X Data', color = 'red')
# lineyg, = ax.plot([], [], label='Y Data', color = 'blue')
# linezg, = ax.plot([], [], label='Z Data', color = 'green')
line_gyro_tilt_angle, = ax.plot([], [], label='Gyrometer Tilt Angle', color = 'red')
# line_tilt_angle, = ax.plot([], [], label='Tilt Angle Accelerometer (rad)', color = 'yellow')

# ax.set_xlabel('Time (ms)')
# ax.set_ylabel('Data')
ax.set_xlabel('Last 100 Readings')
ax.set_ylabel('Gyroscope Tilt Angle (Degrees)')
ax.set_title('Realtime Data Plot')
# ax.legend(handles = [linexg, lineyg, linezg, line_tilt_angle], loc='upper right')
ax.legend(handles = [line_gyro_tilt_angle], loc='upper right')

update_interval, counter = 10, 0
flag = True
ref_angle = []
prev_time = [0]
new_angle = 0

def isCalibrated(tilt_angle):
    if abs(tilt_angle) < 3: #TODO: make it greater than 3 when mounted on a robot
        print(f"calibrated! tilt angle: {tilt_angle}")
        return True
    return False

while True:
    
    # new_data = np.random.randint(0, 1023)
    ser.flushInput()
    raw_data = ser.readline().decode().strip()
    counter += 1
    # print(raw_data)

    split_data = raw_data.split("\t")
    if len(split_data) == 7:
        # print(split_data)
        x = float(split_data[0])  # Convert X to float
        y = float(split_data[1])  # Convert Y to float
        z = float(split_data[2])  # Convert Z to float
        xg = float(split_data[3])  # Convert X to float
        yg = float(split_data[4])  # Convert Y to float
        zg = float(split_data[5])  # Convert Z to float   
        time = float(split_data[6]) # time     
        # print(f"x: {split_data[0]}, y: {split_data[1]}, z: {split_data[2]}")

        tilt_angle_deg = 0

        if z != 0:
            yzratio = float(y/z)
            if abs(z) <=0.04 and abs(y) <=0.04:
                tilt_angle = yzratio
            else:
                tilt_angle = np.arctan(yzratio)
                tilt_angle_deg = tilt_angle * 180.0 / np.pi
        else:
            continue
        
        if counter % update_interval == 0:
            # print(f"tilt angle a: {tilt_angle_deg}")
            pass

        # print(z)
        dataxyz[0].append(x)
        dataxyz[1].append(y)
        dataxyz[2].append(z)
        dataxyz[3].append(tilt_angle)
        dataxyz[4].append(xg)
        dataxyz[5].append(yg)
        dataxyz[6].append(zg)
        dataxyz[7].append(time)

        if flag:
            if isCalibrated(tilt_angle_deg):
                ref_angle.append(tilt_angle_deg)
                flag = False
                prev_time[0] = dataxyz[7][-1]/1000
            else:
                continue
        
        if len(ref_angle) == 1: # if calibrated run this 
            new_angle = ref_angle[0] + (dataxyz[7][-1]/1000 - prev_time[0]) * dataxyz[4][-1] # ref_angle + time difference * z-gyroscope
            ref_angle[0] = new_angle
            prev_time[0] = dataxyz[7][-1]/1000
            dataxyz[8].append(new_angle)
            if counter % update_interval == 0:
                print(f"tilt angle g: {new_angle}")
                
    if len(dataxyz[2]) > 100:
        dataxyz[0] = dataxyz[0][-100:]
        dataxyz[1] = dataxyz[1][-100:]
        dataxyz[2] = dataxyz[2][-100:]
        dataxyz[3] = dataxyz[3][-100:]
        dataxyz[4] = dataxyz[4][-100:]
        dataxyz[5] = dataxyz[5][-100:]
        dataxyz[6] = dataxyz[6][-100:]
        dataxyz[8] = dataxyz[8][-100:]

    if counter % update_interval == 0:
        line_gyro_tilt_angle.set_ydata(dataxyz[8])
        # lineyg.set_ydata(dataxyz[5])
        # linezg.set_ydata(dataxyz[6])
        # line_tilt_angle.set_ydata(dataxyz[3])

        # linexg.set_xdata(np.arange(len(dataxyz[0])))
        # lineyg.set_xdata(np.arange(len(dataxyz[1])))
        # linezg.set_xdata(np.arange(len(dataxyz[2])))
        # line_tilt_angle.set_xdata(np.arange(len(dataxyz[3])))
        line_gyro_tilt_angle.set_xdata(np.arange(len(dataxyz[8])))


        plt.draw()
        plt.pause(0.1)


