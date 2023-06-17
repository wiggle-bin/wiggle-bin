import cv2
import numpy as np
import glob
from pathlib import Path
import os

backSub = cv2.createBackgroundSubtractorMOG2() # better on small particles 
# backSub = cv2.createBackgroundSubtractorKNN() 

def createTimelapse(
    startTime: str = "2023-05-11_1949", 
    endTime: str = "2023-05-11_1956",
    showThresh: bool = True,
    showContours: bool = False
):
    videoPath = Path(__file__).parent / f"output/video/{startTime}-{endTime}.mp4"

    if os.path.exists(videoPath):
        os.remove(videoPath)

    allImages = sorted(glob.glob(str(Path(__file__).parent / f"input/images/*.jpg")))
    
    startIndex = [idx for idx, s in enumerate(allImages) if startTime in s][0]
    endIndex = [idx for idx, s in enumerate(allImages) if endTime in s][0]

    images = allImages[startIndex:endIndex + 1]
    dates = [os.path.splitext(os.path.basename(path))[0] for path in images]

    img_array = []
    totalContourArea = []
    contoursAmount = []
    contourAreas = []
    contourLocations = []

    for filename in images:
        img = cv2.imread(filename)
        img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        ret, th = cv2.threshold(img_gray, 75, 255, cv2.THRESH_TOZERO_INV)
        height, width, layers = img.shape
        size = (width,height)

        (dilated, cnt) = getContours(img)

        # Extract data
        filteredContours = tuple(filter(checkArea, cnt))
        totalContourArea.append(getTotalContourArea(filteredContours))
        contoursAmount.append(len(filteredContours))
        contourAreas.append(getContourAreas(filteredContours))

        circles = getCircles(filteredContours)
        contourLocations.append(circles)

        if showThresh and showContours:
            cv2.drawContours(th, cnt, -1, (255, 0, 0), 1)
            img_array.append(th)
        elif showContours:
            cv2.drawContours(img_gray, cnt, -1, (255, 0, 0), 1)

            for i in circles:
                color = (255, 0, 0)
                cv2.circle(img_gray, (int(i[0]), int(i[1])), int(i[2]), color, 1)

            img_array.append(img_gray)
        elif showThresh:
            img_array.append(th)
        else:
            img_array.append(img_gray)

    out = cv2.VideoWriter(
        str(videoPath),
        cv2.VideoWriter_fourcc('a','v','c','1'), 25, 
        size,
        isColor = False
    )
    
    for i in range(len(img_array)):
        out.write(img_array[i])

    out.release()

    print(contourLocations[1][1])

    return (videoPath, (width, height), dates, (contourAreas, totalContourArea, contoursAmount, contourLocations))

def getContours(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    fgMask = backSub.apply(gray)
    blur = cv2.GaussianBlur(fgMask, (11, 11), 0)
    canny = cv2.Canny(blur, 30, 150, 3)
    dilated = cv2.dilate(canny, (1, 1), iterations=0)
    (cnt, hierarchy) = cv2.findContours(dilated.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    return (dilated, cnt)

def checkArea(cnt):
    area = cv2.contourArea(cnt)
    if area > 1:
        return True
    else: 
        return False
    
def getCircles(contours):
    circles = []

    for i, c in enumerate(contours):
        contours_poly = cv2.approxPolyDP(c, 3, True)
        xy, radius = cv2.minEnclosingCircle(contours_poly)
        circles.append((xy[0], xy[1], radius))

    return circles

def getTotalContourArea(cnt):
    totalCntArea = 0
    for item in cnt: 
        area = cv2.contourArea(item)
        totalCntArea += area
    return totalCntArea

def getContourAreas(cnt):
    areas = []
    for item in cnt: 
        area = cv2.contourArea(item)
        areas.append(area)
    return areas

def getContourCentroid(cnt):
    xy = []
    for item in cnt: 
        M = cv2.moments(item)
        cx = int(M['m10']/M['m00'])
        cy = int(M['m01']/M['m00'])
        xy.append((cx, cy))
    return xy

def getContourRectCenter(cnt):
    xy = []
    for item in cnt: 
        x,y,w,h = cv2.boundingRect(item)
        center = (x,y)
        xy.append(center)
    return xy

if __name__ == '__main__':
    createTimelapse()