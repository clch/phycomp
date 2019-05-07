import numpy as np
import cv2
import serial
import time

port = '/dev/cu.usbmodem14201'

arduino = serial.Serial(port, 9600)

cap = cv2.VideoCapture(0)

ret, last_frame = cap.read()

if last_frame is None:
    exit()

while(cap.isOpened()):
    ret, frame = cap.read()

    if frame is None:
        break

    diff = cv2.absdiff(last_frame, frame)
    gray = cv2.cvtColor(diff, cv2.COLOR_BGR2GRAY)

    (y_len, x_len) = gray.shape

    (y_arr, x_arr) = np.where(gray > 20)

    x = int(np.sum(x_arr) / x_arr.size)
    y = int(np.sum(y_arr) / y_arr.size)

    fx = int(x / x_len * 100)
    fy = int(y / y_len * 100)

    fx = max(20, min(80, fx))
    fy = max(30, min(70, fy))

    print(fx, fy)
    arduino.write((str(fx) + ',' + str(fy) + '\n').encode('utf-8'))

    cv2.imshow('diff', gray)

    if cv2.waitKey(33) >= 0:
        break

    last_frame = frame

cap.release()
cv2.destroyAllWindows()