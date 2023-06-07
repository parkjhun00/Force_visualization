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

fz_values = deque([-250]*10000, maxlen=10000)

app = pg.mkQApp("Plotting")
win = pg.GraphicsLayoutWidget(show=True, title="Basic plotting")
win.resize(1000,600)
win.setWindowTitle('Force Visualize')

pg.setConfigOptions(antialias=True)

p1 = win.addPlot(title="Push & Pull")
curve = p1.plot(pen='y')
ptr = 0
def update():
    global curve, ptr, p1
    curve.setData(fz_values)
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
                    new_value = float(value)
                    fz_values.append(new_value)
            data = client_socket.recv(1024)
        client_socket.close()

if __name__ == '__main__':
    data_thread = threading.Thread(target=get_sensor_data)
    data_thread.start()
    pg.exec()