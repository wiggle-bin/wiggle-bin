import cv2
import numpy as np
import glob
from pathlib import Path
import os

backSub = cv2.createBackgroundSubtractorKNN() 

def createTimelapse(
    startTime: str = "2023-05-11_1949", 
    endTime: str = "2023-05-11_1956",
    showVideo: bool = True,
    showContours: bool = False
):
    videoPath = Path(__file__).parent / f"output/video/{startTime}-{endTime}.mp4"

    if os.path.exists(videoPath):
        os.remove(videoPath)

    allImages = sorted(glob.glob(str(Path(__file__).parent / f"input/images/*.jpg")))

    startIndex = [idx for idx, s in enumerate(allImages) if startTime in s][0]
    endIndex = [idx for idx, s in enumerate(allImages) if endTime in s][0]

    images = allImages[startIndex:endIndex + 1]

    img_array = []
    for filename in images:
        img = cv2.imread(filename)
        height, width, layers = img.shape
        size = (width,height)

        (dilated, cnt) = getContours(img)

        if showVideo and showContours:
            cv2.drawContours(img, cnt, -1, (255, 0, 0), 1)
            img_array.append(img)
        elif showContours:
            img_array.append(dilated)
        else:
            img_array.append(img)
    
    out = cv2.VideoWriter(
        str(videoPath),
        cv2.VideoWriter_fourcc('a','v','c','1'), 25, 
        size
    )
    
    for i in range(len(img_array)):
        out.write(img_array[i])

    out.release()

    return (videoPath, (width, height))

def getContours(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    fgMask = backSub.apply(gray)
    blur = cv2.GaussianBlur(fgMask, (11, 11), 0)
    canny = cv2.Canny(blur, 30, 150, 3)
    dilated = cv2.dilate(canny, (1, 1), iterations=0)
    (cnt, hierarchy) = cv2.findContours(dilated.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    return (dilated, cnt)

if __name__ == '__main__':
    createTimelapse()