import numpy as np
import cv2
import serial
import time
from picamera import PiCamera
from picamera.array import PiRGBArray

port = '/dev/ttyACM0'

arduino = serial.Serial(port, 9600)

camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))
time.sleep(0.1)
count = 0
for frame in camera.capture_continuous(rawCapture, format='bgr', use_video_port=True):
    image = frame.array
    if count == 0:
        last_image = image
        count += 1
    diff = cv2.absdiff(last_image, image)
    gray = cv2.cvtColor(diff, cv2.COLOR_BGR2GRAY)

    (y_len, x_len) = gray.shape

    (y_arr, x_arr) = np.where(gray > 20)

    fx, fy = 0, 0
    if x_arr.size and y_arr.size:
        x = int(np.sum(x_arr) / x_arr.size)
        y = int(np.sum(y_arr) / y_arr.size)

        fx = 100 - int(x * 100 / 640)
        fy = int(y * 100 / 480)

        fx = max(20, min(80, fx))
        fy = max(30, min(70, fy))
    if fx == 0 and fy == 0:
        fx, fy = 50, 50
    print(fx, fy)
    arduino.write((str(fx) + ',' + str(fy) + ',' + str(100) + '\n').encode('utf-8'))

    rawCapture.truncate(0)

    if cv2.waitKey(33) >= 0:
        break

    last_image = image
    count += 1

cv2.destroyAllWindows()
