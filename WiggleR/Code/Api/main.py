from fastapi import FastAPI, Response
from fastapi.staticfiles import StaticFiles
from datetime import datetime
from datetime import timedelta
from Camera import diff
import os

app = FastAPI()

@app.get("/")
async def root():
    return {"message": "Hello World"}

now = datetime.now()
yesterday = now - timedelta(days = 1)

app.mount("/image", StaticFiles(directory="./Camera/input/images"), name="image")

@app.get("/images")
def images():
    out = []
    for filename in os.listdir("./Camera/input/images"):
        out.append({
            "name": filename.split(".")[0],
            "path": "/static/" + filename
        })
    return out

@app.get(
    "/diff/image/",  
    responses = {
        200: { "content": {"image/jpg": {}} }
    },
    response_class=Response
)
def get_image(beforeDate: str = yesterday, afterDate: str = now):
    (score, img) = diff.diffImage()
    return Response(content=img.tobytes(), media_type="image/jpg")

@app.get("/diff/")
async def root(beforeDate: str = yesterday, afterDate: str = now):
    (score, _) = diff.diffImage()
    return {
        "score": score
    }