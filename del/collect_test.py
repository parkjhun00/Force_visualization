import sys
import time
import numpy as np
from collections import deque
import threading

window = 10
data = deque(maxlen=window * 1000 // 50)

ptr = 0

def update():
    global curve, data, ptr, p1

    line = sys.stdin.readline()
    if line:
        _, Fz = map(float, line.strip().split(","))
        data.append(Fz)
    
    print(data)
    sys.stdout.flush()

    
    # Start the timer again
    timer = threading.Timer(0.05, update)  # 0.05 seconds = 50 milliseconds
    timer.start()

# Start the timer for the first time
timer = threading.Timer(0.05, update)
timer.start()