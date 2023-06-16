import numpy as np
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore
import socket
from collections import deque
import threading


# Set up the server
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("localhost", 12345))  # Must match the C++ program
server_socket.listen(1)

fz_values = deque([-250]*9600, maxlen=9600)

#get sensor data
def get_sensor_data():
    global fz_values, new_value
    while True:
        client_socket, address = server_socket.accept()
        data = client_socket.recv(1024)
        while data:
            data_values = data.decode('utf-8').split('\n')
            for value in data_values:
                if value:
                    new_value = float(value)
                    fz_values.append(new_value)
            data = client_socket.recv(1024) 
        client_socket.close()

app = pg.mkQApp("Plotting")
win = pg.GraphicsLayoutWidget(show=True, title="Basic plotting")
win.resize(1000,600)
win.setWindowTitle('Force Visualize')

pg.setConfigOptions(antialias=True)

p1 = win.addPlot(title="Push & Pull 10s")
p1.setYRange(-300, -200)
curve = p1.plot()
ptr = 0
def update1():
    global curve, ptr, p1
    curve.setData(fz_values)
    #p1.enableAutoRange('xy', False)

#win.nextRow()
p2 = win.addPlot(title="Push & Pull")
p2.setDownsampling(mode='peak')
p2.setClipToView(True)
p2.setYRange(-300, -200)
curve2 = p2.plot()

data2 = np.empty(100)
ptr2 = 0

def update2():
    global data2, ptr2
    #data2[ptr2] = new_value
    data2[ptr2] = np.random.normal()
    ptr2 += 1
    if ptr2 >= data2.shape[0]:
        tmp = data2
        data2 = np.empty(data2.shape[0] * 2)
        data2[:tmp.shape[0]] = tmp
    curve2.setData(data2[:ptr2])

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