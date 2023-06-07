import sys
import time
from collections import deque
from pyqtgraph.Qt import QtGui, QtCore
import numpy as np
import pyqtgraph as pg

app = pg.mkQApp("Realtime plotter")
win = pg.GraphicsLayoutWidget(show=True, title="For Test")
win.resize(1200,600)
win.setWindowTitle('Realtime plotter')

# Enable antialiasing for prettier plots
pg.setConfigOptions(antialias=True)

p1 = win.addPlot(title="Fz plotter")
curve = p1.plot(pen='y')
# data = np.random.normal(size=(10,1000))
window = 10
data = deque(maxlen=window * 1000 // 50)

ptr = 0
def update():
    global curve, data, ptr, p1
    curve.setData(data[ptr%10])

    if ptr == 0:
        p1.enableAutoRange('xy', False)  ## stop auto-scaling after the first data set is plotted
    ptr += 1

    line = sys.stdin.readline()
    if line:
        _, Fz = map(float, line.strip().split(","))
        data.append(Fz)
        curve.setData(data)
    
timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(50)
