import sys
import time
import numpy as np
from collections import deque
from PyQt6.QtWidgets import QApplication
from PyQt6.QtCore import QTimer
import pyqtgraph as pg


class RealtimePlotter:
    def __init__(self, window=10):
        self.app = QApplication([])
        self.win = pg.GraphicsWindow(title="Realtime plotter")
        self.plot = self.win.addPlot(title="Fz over time")
        self.curve = self.plot.plot()
        self.data = deque(maxlen=window * 1000 // 50)
        self.timer = QTimer()
        self.timer.timeout.connect(self.update)

    def update(self):
        line = sys.stdin.readline()
        if line:
            _, Fz = map(float, line.strip().split(","))
            self.data.append(Fz)
            self.curve.setData(self.data)

    def run(self):
        self.timer.start(50)
        sys.exit(self.app.exec())


if __name__ == "__main__":
    plotter = RealtimePlotter(window=10)
    plotter.run()