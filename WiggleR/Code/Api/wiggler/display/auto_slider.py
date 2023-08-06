from itertools import cycle
import tkinter as tk
from pathlib import Path
from PIL import Image
from PIL import ImageTk
import glob

BASE_FOLDER = Path.home() / 'WiggleR'
IMG_FOLDER = f"{BASE_FOLDER}/Pictures"

class App(tk.Tk):

    '''constructor to initialize and define'''
    def __init__(self,image_files,x,y,delay):
        #form a Tkinter window
        tk.Tk.__init__(self)

        #assign customized geometryraspberrypizero.local
        self.geometry(f'{x}x{y}')

        #assign custom time between two images
        self.delay = delay

        #create iterator for picture
        self.pictures = cycle((ImageTk.PhotoImage(Image.open(image)),image) for image in image_files)

        #create lable to display pictures
        self.pictures_display = tk.Label(self)
        self.pictures_display.pack()
    
    '''function to display the slides'''
    def show_slides(self):
        #display next item in iterator
        image_object, image_name = next(self.pictures)

        #display the images with title after specified time
        self.pictures_display.config(image=image_object)
        self.title(image_name)
        self.after(self.delay,self.show_slides)
    
    '''function to run the window'''
    def run(self):
        self.mainloop()

'''main function''' 
def main(image_files):
    delay = 100 #time between two images in seconds

    #dimensions
    x = 1024
    y = 768

    #call the App
    app = App(image_files,x,y,delay)
    app.show_slides()
    app.run()

if __name__ == '__main__':
    image_files = sorted(glob.glob(f"${IMG_FOLDER}/*.jpg"))
    main(image_files)
