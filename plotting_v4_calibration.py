import numpy as np
import pandas as pd
import pyqtgraph as pg
import socket
import threading
from pyqtgraph.Qt import QtCore
from collections import deque

# Calibration 
data_train = pd.read_csv('./calibration_push.csv')
X_train =  data_train.loc[:,'cali1']
y_train =  data_train.loc[:,'cali2']
X_train = np.array(X_train).reshape(-1,1)

from sklearn.linear_model import  LinearRegression
lr1 = LinearRegression()
lr1.fit(X_train,y_train)
LinearRegression(copy_X=True, fit_intercept=True, n_jobs=None)

# Set up the server
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("localhost", 12345))  # Must match the C++ program
server_socket.listen(1)

fz_values = deque([0]*9600, maxlen=9600)
t = np.linspace(-8, 0, 9600) 

# Plotting 
app = pg.mkQApp("Plotting")
win = pg.GraphicsLayoutWidget(show=True, title="Basic plotting")
win.resize(1000,600)
win.setWindowTitle('Force Visualize')

pg.setConfigOptions(antialias=True)

p1 = win.addPlot(title="Push & Pull recent 8s")
p1.setYRange(-300, -200)
p1.setLabel('bottom', 'Time', 's')
p1.setXRange(-8, 0)

curve = p1.plot()
ptr = 0

def update():
    global curve, ptr, p1,t 
    curve.setData(t, fz_values)
    #p1.enableAutoRange('xy', False)
timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(50)


def get_sensor_data():
    global fz_values
    while True:
        client_socket, address = server_socket.accept()
        data = client_socket.recv(1024)
        while data:
            data_values = data.decode('utf-8').split('\n')
            for value in data_values:
                if value:
                    new_value = np.array(float(value)).reshape(-1, 1)
                    cali_new_value =  lr1.predict(new_value)
                    fz_values.append(cali_new_value[0])

            data = client_socket.recv(1024)
        client_socket.close()

if __name__ == '__main__':
    data_thread = threading.Thread(target=get_sensor_data)
    data_thread.start()
    pg.exec()