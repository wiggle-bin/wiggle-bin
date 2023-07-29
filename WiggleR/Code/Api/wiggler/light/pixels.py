import board
import neopixel

def on(brightness = 0.1):
    pixels = neopixel.NeoPixel(board.D10, 24, brightness=brightness)
    pixels.fill((255, 0, 0))

def off():
    pixels = neopixel.NeoPixel(board.D10, 24)
    pixels.show()