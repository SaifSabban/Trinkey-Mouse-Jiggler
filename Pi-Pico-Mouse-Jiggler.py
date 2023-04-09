import usb_hid
import random
import time
from adafruit_hid.mouse import Mouse
from time import sleep

mouse = Mouse(usb_hid.devices)

Function_Interval = 25
interval_Timer = 0
Jiggle_interval = 0

while True:
    now = time.monotonic()
    if ((now - interval_Timer) >= Jiggle_interval):
        distance = random.randint(10, 400)
        x = random.randint(0,3) - 1
        y = random.randint(0,3) - 1
        
        for travel in range(distance):
            mouse.move(x, y, 0)

        Jiggle_interval = random.randint(round(Function_Interval - (Function_Interval / 2)), round(Function_Interval + (Function_Interval / 2)))
        interval_Timer = time.monotonic()