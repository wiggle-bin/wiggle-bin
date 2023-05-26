#!/bin/bash
DATE=$(date +"%Y-%m-%d_%H%M")
python3 /home/pi/wormAudio/worm_recorder.py --filename /home/[name]/wormAudio/recordings/$DATE.wav