import cv2
import numpy as np
import glob
from pathlib import Path

def createTimelapse(
    startTime: str = "2023-05-11_1949", 
    endTime: str = "2023-05-11_1956"
):
    videoPath = Path(__file__).parent / f"output/video/{startTime}-{endTime}.mp4"

    allImages = sorted(glob.glob(str(Path(__file__).parent / f"input/images/*.jpg")))

    startIndex = [idx for idx, s in enumerate(allImages) if startTime in s][0]
    endIndex = [idx for idx, s in enumerate(allImages) if endTime in s][0]

    images = allImages[startIndex:endIndex + 1]

    img_array = []
    for filename in images:
        img = cv2.imread(filename)
        height, width, layers = img.shape
        size = (width,height)
        img_array.append(img)
    
    out = cv2.VideoWriter(
        str(videoPath),
        cv2.VideoWriter_fourcc('a','v','c','1'), 25, 
        size
    )
    
    for i in range(len(img_array)):
        out.write(img_array[i])

    out.release()

    return videoPath

if __name__ == '__main__':
    createTimelapse()