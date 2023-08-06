from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
from crontab import CronTab
from datetime import datetime, timedelta
import os
import zipfile
from fastapi.responses import StreamingResponse, Response
from pathlib import Path
import glob
import io
from wiggler.light import pixels

BASE_FOLDER = Path.home() / 'WiggleR'
IMG_FOLDER = f"{BASE_FOLDER}/Pictures"
VID_FOLDER = f"{BASE_FOLDER}/Videos"
ZIP_FOLDER = f"{BASE_FOLDER}/Zip"

TAG_IMAGE = "Image"
TAG_ZIP = "Zip"
TAG_VIDEO = "Video"
TAG_PERIPHERAL = "Peripheral"

app = FastAPI()

tags_metadata = [
    {"name": TAG_IMAGE, "description": "View and delete images"},
    {"name": TAG_ZIP, "description": "Create and retrieve images in zip"},
    {"name": TAG_VIDEO, "description": "Create and view timelapse videos"},
    {"name": TAG_PERIPHERAL, "description": "Control actuators and sensors"},
]

app.mount("/image", StaticFiles(directory=IMG_FOLDER), name="image")
app.mount("/video", StaticFiles(directory=VID_FOLDER), name="video")
app.mount("/zip", StaticFiles(directory=ZIP_FOLDER), name="zip")


def list_files(folder, path, extension):
    out = []
    for fileName in sorted(os.listdir(folder)):
        name, ext = os.path.splitext(fileName)
        if ext == extension:
            out.append({
                "name": name,
                "path": path + fileName
            })
    return out


def zipfiles(filenames, name):
    zip_subdir = "./"
    zip_io = io.BytesIO()
    with zipfile.ZipFile(zip_io, mode='w', compression=zipfile.ZIP_DEFLATED) as temp_zip:
        for fpath in filenames:
            _, fname = os.path.split(fpath)
            zip_path = os.path.join(zip_subdir, fname)
            temp_zip.write(fpath, zip_path)
    return StreamingResponse(
        iter([zip_io.getvalue()]),
        media_type="application/x-zip-compressed",
        headers={"Content-Disposition": f"attachment; filename={name}.zip"}
    )


@app.get("/")
def read_root():
    return {"Hello": "Worm"}


@app.get("/images", tags=[TAG_IMAGE])
def list_images():
    return list_files(IMG_FOLDER, '/image/', '.jpg')


@app.delete("/images/{date}", tags=[TAG_IMAGE])
def delete_images(date: str):
    filePath = f"{IMG_FOLDER}/{date}*.jpg"
    os.system(f"rm {filePath}")
    return {"message": f"successfully deleted {filePath}"}


@app.get("/videos", tags=[TAG_VIDEO])
def videos():
    return list_files(VID_FOLDER, '/video/', '.mp4')


@app.delete("/videos/{date}", tags=[TAG_VIDEO])
def delete_videos(date: str):
    filePath = f"{VID_FOLDER}/{date}*.jpg"
    os.system(f"rm {filePath}")
    return {"message": f"successfully deleted {filePath}"}


@app.get("/zips", tags=[TAG_ZIP])
def zips():
    return list_files(ZIP_FOLDER, '/zip/', '.zip')


@app.delete("/zips/{date}", tags=[TAG_ZIP])
def delete_zip(date: str):
    filePath = f"{ZIP_FOLDER}/{date}*.jpg"
    os.system(f"rm {filePath}")
    return {"message": f"successfully deleted {filePath}"}


@app.get("/timelapse/yesterday", tags=[TAG_VIDEO])
def timelapse():
    yesterday = (datetime.now() - timedelta(1)).strftime('%Y-%m-%d')
    os.system(
        f'ffmpeg -framerate 30 -pattern_type glob -i "{IMG_FOLDER}/{yesterday}*.jpg" -s:v 1440x1080 -c:v libx264 -crf 17 -pix_fmt yuv420p {VID_FOLDER}/{yesterday}.mp4')
    return {
        "name": yesterday,
        "path": f'/video/{yesterday}.mp4'
    }


@app.get("/timelapse/date/{date}", tags=[TAG_VIDEO])
def timelapse(date: str):
    os.system(
        f'ffmpeg -framerate 30 -pattern_type glob -i "{IMG_FOLDER}/{date}*.jpg" -s:v 1440x1080 -c:v libx264 -crf 17 -pix_fmt yuv420p {VID_FOLDER}/{date}.mp4')
    return {
        "name": date,
        "path": f'/video/{date}.mp4'
    }


@app.get("/timelapse/stream/{date}", tags=[TAG_VIDEO])
async def video(date: str):
    def iterfile():
        with open(f'{VID_FOLDER}/{date}.mp4', mode="rb") as file_like:
            yield from file_like

    return StreamingResponse(iterfile(), media_type="video/mp4")


@app.get("/images/zip/stream/{date}", tags=[TAG_ZIP])
async def zip_stream(date: str):
    filenames = glob.glob(
        str(Path(__file__).parent / f"{IMG_FOLDER}/{date}*.jpg"))
    return zipfiles(filenames, date)


@app.get("/images/zip/yesterday", tags=[TAG_ZIP])
async def zip_yesterday():
    yesterday = (datetime.now() - timedelta(1)).strftime('%Y-%m-%d')
    os.system(
        f'zip -j {ZIP_FOLDER}/{yesterday} {IMG_FOLDER}/{yesterday}*.jpg')
    return {
        "name": yesterday,
        "path": f'/zip/{yesterday}.zip'
    }


@app.get("/images/zip/{date}", tags=[TAG_ZIP])
async def zip_based_on_date(date: str):
    os.system(f'zip -j {ZIP_FOLDER}/{date} {IMG_FOLDER}/{date}*.jpg')
    return {
        "name": date,
        "path": f'/zip/{date}.zip'
    }


@app.get("/camera/picture", tags=[TAG_PERIPHERAL])
def take_picture():
    now = datetime.now()
    fileName = now.strftime("%Y-%m-%d-%H-%M")
    filePath = f"{IMG_FOLDER}/{fileName}.jpg"
    os.system(
        f"libcamera-jpeg --width 1024 --height 768 --nopreview -t 1 -o {filePath}")
    return {"picture": f"image/{fileName}.jpg"}


@app.get("/light/on/{intensity}", tags=[TAG_PERIPHERAL])
async def light(intensity: float = 1):
    pixels.on(intensity)


@app.get("/light/off", tags=[TAG_PERIPHERAL])
async def lightOff():
    pixels.off()


@app.get("/schedule/images/zip", tags=[TAG_ZIP])
def schedule_timelapse():
    cron = CronTab(user=os.getlogin())
    cron.remove_all(comment='zip images')
    job = cron.new(
        command='curl localhost:8000/images/zip/yesterday', comment='zip images')
    job.every().day()
    cron.write()
    return {"message": "yesterdays images will be zipped"}


@app.get("/schedule/camera/picture/{minutes}", tags=[TAG_PERIPHERAL])
def schedule_picture(minutes: int = 1):
    cron = CronTab(user=os.getlogin())
    cron.remove_all(comment='take picture')
    job = cron.new(command='curl localhost:8000/camera/picture',
                   comment='take picture')
    job.minute.every(minutes)
    cron.write()
    return {"minutes": minutes}


@app.get("/schedule/timelapse", tags=[TAG_VIDEO])
def schedule_timelapse():
    cron = CronTab(user=os.getlogin())
    cron.remove_all(comment='take timelapse')
    job = cron.new(
        command='curl localhost:8000/timelapse/yesterday', comment='take timelapse')
    job.every().day()
    cron.write()
    return {"result": "yesterdays activity will be available every day"}
