import tkinter as tk
from tkinter import *
from pathlib import Path
from PIL import Image
from PIL import ImageTk
import glob
from functools import partial

# POT
import board
import busio
i2c = busio.I2C(board.SCL, board.SDA)
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn

ads = ADS.ADS1115(i2c)

def remap(x, in_min, in_max, out_min, out_max):
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

BASE_FOLDER = Path.home() / 'WiggleR'
IMG_FOLDER = f"{BASE_FOLDER}/Pictures"

chan = AnalogIn(ads, ADS.P2)
image_files = sorted(glob.glob(f"{IMG_FOLDER}/*.jpg"))

def changeImage(root, chan, l, label):
    mappedValue = remap(chan.voltage, 0, 5, 0, len(image_files))
    imageIndex = round(mappedValue)
    imageFile = image_files[imageIndex]
    stgImg = ImageTk.PhotoImage(Image.open(imageFile))
    l.config(image=stgImg)
    label.image = stgImg
    root.after(200, lambda : changeImage(root, chan, l, label))

'''main function''' 
def main():
    root=tk.Tk()
    root.geometry("1024x768")
    l=Label()
    l.pack()

    stgImg = ImageTk.PhotoImage(Image.open(image_files[0]))
    label=tk.Label(root, image=stgImg)

    changeImage(root, chan, l, label)
    root.mainloop()

if __name__ == '__main__':
    main()
