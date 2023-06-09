import numpy as np
import pandas as pd
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore
import socket
from collections import deque
import threading

# Calibration 
data_train_pull = pd.read_csv('./cali_pull.csv')
X_train_pull =  data_train_pull.loc[:,'cali2']
y_train_pull =  data_train_pull.loc[:,'cali1']
X_train_pull = np.array(X_train_pull).reshape(-1,1)

data_train_push = pd.read_csv('./cali_push.csv')
X_train_push =  data_train_push.loc[:,'cali2']
y_train_push =  data_train_push.loc[:,'cali1']
X_train_push = np.array(X_train_push).reshape(-1,1)

from sklearn.linear_model import  LinearRegression
lr_pull = LinearRegression()
lr_push = LinearRegression()
lr_pull.fit(X_train_pull,y_train_pull)
lr_push.fit(X_train_push,y_train_push)
LinearRegression(copy_X=True, fit_intercept=True, n_jobs=None)

# Set up the server
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("localhost", 12345))  
server_socket.listen(1)

fz_values = deque([0]*9600, maxlen=9600)
t = np.linspace(-8, 0, 9600) 

#get sensor data
def get_sensor_data():
    global fz_values, new_value, cali_new_value
    while True:
        client_socket, address = server_socket.accept()
        data = client_socket.recv(1024)
        while data:
            data_values = data.decode('utf-8').split('\n')
            for value in data_values:
                if value:
                    new_value = np.array(float(value)).reshape(-1, 1)

                    # 値別のキャリブレーション
                    if new_value[0][0] < 0:
                        cali_new_value =  lr_push.predict(new_value)
                        fz_values.append(cali_new_value[0])
                    elif new_value[0][0] > 0:
                        cali_new_value =  lr_pull.predict(new_value)
                        fz_values.append(cali_new_value[0])

            data = client_socket.recv(1024) 
        client_socket.close()

# Plotting 
app = pg.mkQApp("Plotting")
win = pg.GraphicsLayoutWidget(show=True, title="Basic plotting")
win.resize(1000,600)
win.setWindowTitle('Force Visualize')

pg.setConfigOptions(antialias=True)

p1 = win.addPlot(title="Push & Pull 8s")
p1.setYRange(100, -100)
curve = p1.plot()
ptr = 0
def update1():
    global curve, ptr, p1, t
    curve.setData(t, fz_values)
    #p1.enableAutoRange('xy', False)

#win.nextRow()
p2 = win.addPlot(title="Push & Pull")
p2.setDownsampling(mode='peak')
p2.setClipToView(True)
p2.setYRange(200, -200)
curve2 = p2.plot()

data2 = np.empty(100)
ptr2 = 0

def update2():
    global data2, ptr2
    try:
        data2[ptr2] = cali_new_value[0]
        ptr2 += 1
        if ptr2 >= data2.shape[0]:
            tmp = data2
            data2 = np.empty(data2.shape[0] * 2)
            data2[:tmp.shape[0]] = tmp
        curve2.setData(data2[:ptr2])
    except NameError:
        pass

def update():
    update1()
    update2()

timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(50)

if __name__ == '__main__':
    data_thread = threading.Thread(target=get_sensor_data)
    data_thread.start()
    pg.exec()