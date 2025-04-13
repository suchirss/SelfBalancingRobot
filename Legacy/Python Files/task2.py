import matplotlib.pyplot as plt
import numpy as np
import serial

ser = serial.Serial('COM5', 57600, timeout=0.1) 

plt.ion()
fig = plt.figure()
ax = fig.add_subplot(111)
plt.ylim([-100, 100])
plt.xlim([0, 100])

dataxyz = [[], [], [], []]

# linex, = ax.plot([], [], label='X Data', color = 'red')
# liney, = ax.plot([], [], label='Y Data', color = 'blue')
# linez, = ax.plot([], [], label='Z Data', color = 'green')
line_tilt_angle, = ax.plot([], [], label='Tilt Angle Accelerometer (degrees)', color = 'yellow')

ax.set_ylabel('Accelerometer Tilt Angle (degrees)')
ax.set_xlabel('Last 100 Readings')
# ax.set_ylabel('Data')
ax.set_title('Realtime Data Plot')
# ax.legend(handles = [linex, liney, linez, line_tilt_angle], loc='upper right')
ax.legend(handles = [line_tilt_angle], loc='upper right')

update_interval = 10
counter = 0

while True:
    
    # new_data = np.random.randint(0, 1023)
    ser.flushInput()
    raw_data = ser.readline().decode().strip()
    counter += 1
    # print(raw_data)

    split_data = raw_data.split("\t")
    if len(split_data) == 3:
        x = float(split_data[0])  # Convert X to float
        y = float(split_data[1])  # Convert Y to float
        z = float(split_data[2])  # Convert Z to float
        # print(f"x: {split_data[0]}, y: {split_data[1]}, z: {split_data[2]}")

        tilt_angle_deg = 0

        if z != 0:
            yzratio = float(y/z)
            # if abs(z) <=0.04 or abs(y) <=0.04:
            if abs(yzratio) <= 0.04:
                tilt_angle = yzratio
            else:
                tilt_angle = np.arctan(yzratio)
                tilt_angle_deg = tilt_angle * 180.0 / np.pi
        else:
            continue
        
        print(f"tilt angle: {tilt_angle_deg}")

        # print(z)
        dataxyz[0].append(x)
        dataxyz[1].append(y)
        dataxyz[2].append(z)
        dataxyz[3].append(tilt_angle_deg)

    if len(dataxyz[2]) > 100:
        dataxyz[0] = dataxyz[0][-100:]
        dataxyz[1] = dataxyz[1][-100:]
        dataxyz[2] = dataxyz[2][-100:]
        dataxyz[3] = dataxyz[3][-100:]

    if counter % update_interval == 0:
        # linex.set_ydata(dataxyz[0])
        # liney.set_ydata(dataxyz[1])
        # linez.set_ydata(dataxyz[2])
        line_tilt_angle.set_ydata(dataxyz[3])

        # linex.set_xdata(np.arange(len(dataxyz[0])))
        # liney.set_xdata(np.arange(len(dataxyz[1])))   
        # linez.set_xdata(np.arange(len(dataxyz[2])))
        line_tilt_angle.set_xdata(np.arange(len(dataxyz[3])))


        plt.draw()
        plt.pause(0.1)
