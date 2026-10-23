# Laser-harp

An 8-string laser harp. Break a laser beam, and it plays a note. it shows up as a usb midi device, so it works with any daw or synth on your laptop.

## how it works
 
8 red lasers on the base shine straight up at 8 phototransistors on a bar on top of them. when your hand breaks a beam, that string's signal flips and the esp32 sends a midi note over usb. a neopixel under each string lights up while you play it.
 

## frame idea
 
a wood or 3d-printed arch holds the bar, hollow curved side pillars carry the cable, and a chunky base case holds the base pcb. none of this is built yet.

## images


<img width="1641" height="838" alt="kicad-base-screenshot" src="https://github.com/user-attachments/assets/bcc91e41-92f5-4ae7-bbf7-6923c333500a" />
<img width="2080" height="424" alt="pcb-bar" src="https://github.com/user-attachments/assets/42f37f9e-0e6b-4a79-b19b-52dc498522a5" />
<img width="2080" height="904" alt="pcb-base" src="https://github.com/user-attachments/assets/7d35e4e3-a07e-4f0f-afe8-47b3c7512b38" />
