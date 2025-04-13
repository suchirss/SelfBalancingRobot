import matplotlib.pyplot as plt
import numpy as np
import serial

ser = serial.Serial('COM5', 57600, timeout=0.1) 

plt.ion()
fig = plt.figure()
ax = fig.add_subplot(111)
plt.ylim([-10, 385])
plt.xlim([0, 100])

dataxyz = [[], [], [], [], [], [], [], [], [], []]

line_tilt_angle_acc, = ax.plot([], [], label='Accelerometer Tilt Angle', color = 'red')
line_tilt_angle_gyro, = ax.plot([], [], label='Gyroscope Tilt Angle', color = 'blue')
line_complementary_angle, = ax.plot([], [], label='Complementary Angle', color = 'green')
# line_tilt_angle, = ax.plot([], [], label='Calculated Tilt Angles vs Number of Readings', color = 'yellow')

# ax.set_xlabel('Time (ms)')
ax.set_xlabel('Last 100 Readings')
# ax.set_ylabel('Data')
plt.ylabel("Tilt Angle (Degrees)")
ax.set_title('Realtime Data Plot')
ax.legend(handles = [line_tilt_angle_acc, line_tilt_angle_gyro, line_complementary_angle], loc='upper right')

update_interval, counter = 10, 0
flag = True
ref_angle = []
prev_time = [0]
isComplementaryAngleInitialized = False

def isCalibrated(tilt_angle):
    if abs(tilt_angle) < 3: #TODO: make it greater than 3 when mounted on a robot
        # print(f"calibrated! tilt angle: {tilt_angle}")
        dataxyz[8].append(tilt_angle)
        return True
    return False

def calculateComplementaryAngle(k, tilt_angle_gyro, tilt_angle_acc):
    if len(dataxyz[8]) > 0:
        lastComplementaryAngle = dataxyz[8][-1]
        lCA = lastComplementaryAngle # for short variable name
        # print(f"last ComplementaryAngle: {lCA}")
        newComplementaryAngle = k*(lCA + tilt_angle_gyro) + (1 - k)*tilt_angle_acc
        # print(f"internal new comp angle: {newComplementaryAngle}")
        dataxyz[8].append(newComplementaryAngle)
        # print(f"internal tilt g: {tilt_angle_gyro}")
        # print(f"internal tilt g: {tilt_angle_acc}")

def map_acc_tilt_angle(acc_tilt_angle):
    if 0 <= acc_tilt_angle < 90:
            if z >= 0 and y >= 0:
                mapped_acc_tilt_angle = acc_tilt_angle
            else:
                mapped_acc_tilt_angle = acc_tilt_angle + 180
    elif -90 < acc_tilt_angle < 0:
        if y >= 0 and z < 0:
            mapped_acc_tilt_angle = acc_tilt_angle + 180
        else:
            mapped_acc_tilt_angle = acc_tilt_angle + 360

    return mapped_acc_tilt_angle


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

        mapped_acc_angle = map_acc_tilt_angle(tilt_angle_deg)

        if counter % update_interval == 1:
            # print(f"tilt angle: {tilt_angle_deg}")
            # print(f"mapped tilt angle a: {mapped_acc_angle}")
            pass

        # send tilt angle back to Arduino
        if len(dataxyz[8]) > 0:
            comp_angle_to_send = dataxyz[8][-1]
            ser.write(f"{comp_angle_to_send:.4f}\n".encode())

        dataxyz[0].append(x)
        dataxyz[1].append(y)
        dataxyz[2].append(z)
        dataxyz[3].append(mapped_acc_angle)
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
        
        if len(ref_angle) == 1:
            new_angle = ref_angle[0] + (dataxyz[7][-1]/1000 - prev_time[0]) * dataxyz[4][-1] # ref_angle + time difference * z-gyroscope
            ref_angle[0] = new_angle
            mapped_gyro_angle = 360 - (new_angle % 360)
            dataxyz[9].append(mapped_gyro_angle)
            prev_time[0] = dataxyz[7][-1]/1000
            if counter % update_interval == 1:
                # print(f"tilt angle g: {new_angle}")
                # print(f"tilt angle g: {mapped_gyro_angle}")
                pass

    if(len(ref_angle) >= 1):            
        calculateComplementaryAngle(0.05, mapped_gyro_angle, mapped_acc_angle)

    if len(dataxyz[8]) > 1: 
        compAngle = dataxyz[8][-1]

    if counter % update_interval == 1:
        # print(f"Calculate Complementary Angle: {compAngle}")
        pass

    if len(dataxyz[2]) > 100:
        dataxyz[0] = dataxyz[0][-100:]
        dataxyz[1] = dataxyz[1][-100:]
        dataxyz[2] = dataxyz[2][-100:]
        dataxyz[3] = dataxyz[3][-100:]
        dataxyz[4] = dataxyz[4][-100:]
        dataxyz[5] = dataxyz[5][-100:]
        dataxyz[6] = dataxyz[6][-100:]
        dataxyz[7] = dataxyz[7][-100:]
        dataxyz[8] = dataxyz[8][-100:]
        dataxyz[9] = dataxyz[9][-100:]

    if counter % update_interval == 1:
        line_tilt_angle_acc.set_ydata(dataxyz[3])
        line_tilt_angle_gyro.set_ydata(dataxyz[9])
        line_complementary_angle.set_ydata(dataxyz[8])
        # line_tilt_angle.set_ydata(dataxyz[3])

        line_tilt_angle_acc.set_xdata(np.arange(len(dataxyz[3])))
        line_tilt_angle_gyro.set_xdata(np.arange(len(dataxyz[9])))
        line_complementary_angle.set_xdata(np.arange(len(dataxyz[8])))
        # line_tilt_angle.set_xdata(np.arange(len(dataxyz[3])))

        plt.draw()
        plt.pause(0.1)


