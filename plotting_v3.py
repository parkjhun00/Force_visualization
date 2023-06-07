import numpy as np
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore
import socket
from collections import deque

# Set up the server
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("localhost", 12345))  # Must match the C++ program
server_socket.listen(1)

app = pg.mkQApp("Plotting")
#mw = QtWidgets.QMainWindow()
#mw.resize(800,800)

win = pg.GraphicsLayoutWidget(show=True, title="Basic plotting")
win.resize(1000,600)
win.setWindowTitle('Force Visualize')

# Enable antialiasing for prettier plots
pg.setConfigOptions(antialias=True)
fz_values = deque([0]*10000, maxlen=10000)
p1 = win.addPlot(title="Push & Pull")
curve = p1.plot(pen='y')
ptr = 0

def update():
    global curve, fz_values, p1
    curve.setData(fz_values)
    p1.enableAutoRange('xy', False)  ## stop auto-scaling after the first data set is plotted

timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(100)

if __name__ == '__main__':
    pg.exec()

## get sensor data
while True:
    # Wait for a connection
    client_socket, address = server_socket.accept()

    # Read data from the client
    data = client_socket.recv(1024)
    while data:
        # Split the received data by newline character
        data_values = data.decode('utf-8').split('\n')

        # Convert each value to float and append to fz_values
        for value in data_values:
            if value:  # check if the value is not an empty string
                new_value = float(value)
                fz_values.append(new_value)
                print(f"Received new Fz value: {new_value}")

        data = client_socket.recv(1024)

    client_socket.close()