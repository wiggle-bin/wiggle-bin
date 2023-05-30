from __future__ import print_function
import cv2
import argparse
import time

parser = argparse.ArgumentParser(description='This program shows how to use background subtraction methods provided by \
                                              OpenCV. You can process both videos and images.')
parser.add_argument('--input', type=str, help='Path to a video or a sequence of image.', default='./input/wormsRedCleaned.mov')
parser.add_argument('--algo', type=str, help='Background subtraction method (KNN, MOG2).', default='MOG2')
args = parser.parse_args()

video = cv2.VideoCapture(0)

# backSub = cv2.createBackgroundSubtractorMOG2() # better on small particles 
backSub = cv2.createBackgroundSubtractorKNN() # better on selecting worms
capture = cv2.VideoCapture(cv2.samples.findFileOrKeep(args.input))

frame_width = int(capture.get(3))
frame_height = int(capture.get(4))
   
size = (frame_width, frame_height)

out = cv2.VideoWriter(
    './outputs/Day2-255-255-255.mov',
    cv2.VideoWriter_fourcc(*'mp4v'), 10, 
    (frame_width, frame_height)
)

frameRate = int(capture.get(cv2.CAP_PROP_FPS))

if not capture.isOpened():
    print('Unable to open: ' + args.input)
    exit(0)
while True:
    ret, frame = capture.read()
    if frame is None:
        break
    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    fgMask = backSub.apply(gray)
    blur = cv2.GaussianBlur(fgMask, (11, 11), 0)
    canny = cv2.Canny(blur, 30, 150, 3)
    dilated = cv2.dilate(canny, (1, 1), iterations=0)
    
    (cnt, hierarchy) = cv2.findContours(dilated.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cv2.drawContours(frame, cnt, -1, (255, 0, 0), 1)
    
    cv2.imshow('Contours', dilated)
    cv2.imshow('Worm Contours', frame)
    
    time.sleep(1/frameRate) 
    
    out.write(frame)

    keyboard = cv2.waitKey(30)
    if keyboard == ord('q') or keyboard == 27:
        break

capture.release()
cv2.destroyAllWindows()

print("The video was successfully saved")