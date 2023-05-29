from fastapi import FastAPI, Response
from fastapi.staticfiles import StaticFiles
from fastapi import Header
from datetime import datetime
from datetime import timedelta
from Camera import diff
import os
import base64
from pathlib import Path

app = FastAPI()

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
            "path": "/static/" + filename
        })

@app.get("/images")
def images():
    return out

defaultBeforeImage = out[0]['name'] if len(out) < 16 else out[-15]['name']
defaultAfterImage = out[-1]['name']

@app.get(
    "/diff/image/",  
    responses = {
        200: { "content": {"image/jpg": {}} }
    },
    response_class=Response
)
def get_image(
    beforeImage: str = defaultBeforeImage, 
    afterImage: str = defaultAfterImage
):
    (score, img) = diff.diffImage(beforeImage, afterImage)
    headers = {"score": str(score)}
    return Response(content=img.tobytes(), media_type="image/jpg", headers=headers)

@app.get("/diff/")
async def root(
    beforeImage: str = defaultBeforeImage, 
    afterImage: str = defaultAfterImage
):
    (score, img) = diff.diffImage(beforeImage, afterImage)
    return {
        "score": score,
        "image": base64.b64encode(img)
    }

CHUNK_SIZE = 1024*768
video_path = Path(__file__).parent / f"Camera/output/video/test.mov"

@app.get("/video")
async def video_endpoint(range: str = Header(None)):
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
        return Response(data, status_code=206, headers=headers, media_type="video/mov")