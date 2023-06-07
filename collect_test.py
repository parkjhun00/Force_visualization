import sys
import time
import numpy as np
from collections import deque

window = 10
data = deque(maxlen=window * 1000 // 50)

ptr = 0
last_printed = time.time()

def update():
    global curve, data, ptr, p1, last_printed

    line = sys.stdin.readline()
    if line:
        Fz = float(line.strip())
        data.append(Fz)
    
    current_time = time.time()
    if current_time - last_printed >= 1.0:  # 1초 이상 경과한 경우에만 출력합니다.
        print(Fz)
        sys.stdout.flush()
        last_printed = current_time  # 마지막으로 출력한 시간을 업데이트합니다.

# 루프를 사용하여 데이터를 계속 읽어들입니다.
while True:
    update()