from fastapi import FastAPI
from datetime import datetime
import os

app = FastAPI()

@app.get("/")
def read_root():
    return {"Hello": "Worm"}

@app.get("/images")
def images():
    out = []
    for filename in sorted(os.listdir("/images")):
        name, ext = os.path.splitext(filename)
        if ext == '.jpg':
            out.append({
                "name": name,
                "path": "/image/" + filename
            })
    return out

@app.get("/picture")
def images():
    now = datetime.now()
    dt_string = now.strftime("%M-%H-%Y-%m-%d")
    os.system(f"libcamera-jpeg --width 1024 --height 768 --nopreview -t 1 -o './images/{dt_string}.jpg'")
    return {"picture": "output.jpg"}
