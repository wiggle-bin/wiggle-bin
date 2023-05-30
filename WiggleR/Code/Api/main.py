from fastapi import FastAPI, Response
from fastapi.staticfiles import StaticFiles
from fastapi import Header
from fastapi.middleware.cors import CORSMiddleware
from datetime import datetime
from datetime import timedelta
from Camera import diff, timelapse
import os
import base64
from pathlib import Path

origins = [
    "http://localhost:5173",
]

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/")
async def root():
    return {"message": "Hello Worm"}

app.mount("/image", StaticFiles(directory="./Camera/input/images"), name="image")
app.mount("/video", StaticFiles(directory="./Camera/output/video"), name="video")

out = []
for filename in sorted(os.listdir("./Camera/input/images")):
    name, ext = os.path.splitext(filename)
    if ext == '.jpg':
        out.append({
            "name": name,
            "path": "/image/" + filename
        })

@app.get("/images")
def images():
    return out

defaultStartTime = out[0]['name'] if len(out) < 16 else out[-15]['name']
defaultEndTime = out[-1]['name']

@app.get(
    "/diff/image/",  
    responses = {
        200: { "content": {"image/jpg": {}} }
    },
    response_class=Response
)
def get_image(
    beforeImage: str = defaultStartTime, 
    afterImage: str = defaultEndTime
):
    (score, img) = diff.diffImage(beforeImage, afterImage)
    headers = {"score": str(score)}
    return Response(content=img.tobytes(), media_type="image/jpg", headers=headers)

@app.get("/diff/")
async def root(
    beforeImage: str = defaultStartTime, 
    afterImage: str = defaultEndTime
):
    (score, img) = diff.diffImage(beforeImage, afterImage)
    return {
        "score": score,
        "image": base64.b64encode(img)
    }

@app.get("/video")
async def video_endpoint(
    range: str = Header(None),
    startTime: str = defaultStartTime, 
    endTime: str = defaultEndTime,
    showThresh: bool = True,
    showContours: bool = False
):
    (video_path, (width, height)) = timelapse.createTimelapse(
        startTime, 
        endTime, 
        showThresh, 
        showContours
    )
    CHUNK_SIZE = width*height
    start, end = range.replace("bytes=", "").split("-")
    start = int(start)
    end = int(end) if end else start + CHUNK_SIZE
    with open(video_path, "rb") as video:
        video.seek(start)
        data = video.read(end - start)
        filesize = str(video_path.stat().st_size)
        headers = {
            'Content-Range': f'bytes {str(start)}-{str(end)}/{filesize}',
            'Accept-Ranges': 'bytes'
        }
        return Response(data, status_code=206, headers=headers, media_type="video/mp4")