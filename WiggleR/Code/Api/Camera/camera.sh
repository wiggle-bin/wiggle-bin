#!/bin/bash
DATE=$(date +"%Y-%m-%d_%H%M")
libcamera-jpeg --width 1024 --height 768 --nopreview -t 1 -o "/home/[name]/Desktop/timelapse/images/$DATE.jpg"
