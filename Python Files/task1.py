import matplotlib.pyplot as plt
import numpy as np
import serial

ser = serial.Serial('COM5', 57600, timeout=0.1) 

plt.ion()
fig = plt.figure()
ax = fig.add_subplot(111)
plt.ylim([-3, 3])
plt.xlim([0, 100])

# data = np.random.randint(0, 1023, size = (50,))

# data = ser.readline().decode().strip()
# if data:
#     split_data = data.split("\t")
#     if len(split_data) == 3:
#         print(f"x: {split_data[0]}, y: {split_data[1]}, z: {split_data[2]}")
dataxyz = [[], [], []]
# line, = ax.plot(split_data[0])
linex, = ax.plot([], [], label='X Data', color = 'red')
liney, = ax.plot([], [], label='Y Data', color = 'blue')
linez, = ax.plot([], [], label='Z Data', color = 'green')

ax.set_ylabel('Accelerometer Readings (m/s^2*g)')
ax.set_xlabel('Last 100 Readings')

# ax.set_ylabel('Data')
ax.set_title('Realtime Data Plot')
ax.legend(handles = [linex, liney, linez], loc='upper right')

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

        # print(z)
        dataxyz[0].append(x)
        dataxyz[1].append(y)
        dataxyz[2].append(z)

    if len(dataxyz[2]) > 100:
        dataxyz[0] = dataxyz[0][-100:]
        dataxyz[1] = dataxyz[1][-100:]
        dataxyz[2] = dataxyz[2][-100:]


    if counter % update_interval == 0:
        linex.set_ydata(dataxyz[0])
        liney.set_ydata(dataxyz[1])
        linez.set_ydata(dataxyz[2])

        linex.set_xdata(np.arange(len(dataxyz[0])))
        liney.set_xdata(np.arange(len(dataxyz[1])))
        linez.set_xdata(np.arange(len(dataxyz[2])))

        plt.draw()
        plt.pause(0.1)



