from __future__ import print_function
import cv2 as cv
import argparse
import time

parser = argparse.ArgumentParser(description='This program shows how to use background subtraction methods provided by \
                                              OpenCV. You can process both videos and images.')
parser.add_argument('--input', type=str, help='Path to a video or a sequence of image.', default='./input/wormsRedCleaned.mov')
parser.add_argument('--algo', type=str, help='Background subtraction method (KNN, MOG2).', default='MOG2')
args = parser.parse_args()

video = cv.VideoCapture(0)

# backSub = cv.createBackgroundSubtractorMOG2() # better on small particles 
backSub = cv.createBackgroundSubtractorKNN() # better on selecting worms
capture = cv.VideoCapture(cv.samples.findFileOrKeep(args.input))

frame_width = int(capture.get(3))
frame_height = int(capture.get(4))
   
size = (frame_width, frame_height)

out = cv.VideoWriter(
    './outputs/Day2-255-255-255.mov',
    cv.VideoWriter_fourcc(*'mp4v'), 10, 
    (frame_width, frame_height)
)

frameRate = int(capture.get(cv.CAP_PROP_FPS))

if not capture.isOpened():
    print('Unable to open: ' + args.input)
    exit(0)
while True:
    ret, frame = capture.read()
    if frame is None:
        break
    
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    fgMask = backSub.apply(gray)
    blur = cv.GaussianBlur(fgMask, (11, 11), 0)
    canny = cv.Canny(blur, 30, 150, 3)
    dilated = cv.dilate(canny, (1, 1), iterations=0)
    
    (cnt, hierarchy) = cv.findContours(dilated.copy(), cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    cv.drawContours(frame, cnt, -1, (255, 0, 0), 1)
    
    cv.imshow('Contours', dilated)
    cv.imshow('Worm Contours', frame)
    
    time.sleep(1/frameRate) 
    
    out.write(frame)

    keyboard = cv.waitKey(30)
    if keyboard == ord('q') or keyboard == 27:
        break

capture.release()
cv.destroyAllWindows()

print("The video was successfully saved")