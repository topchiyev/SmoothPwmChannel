# SmoothPwmChannel
An Arduino project to read RC PWM signal from one pin and put the smoothed PWM signal to other pin.

When I say **smoothed signal** I mean:
* If you switch a slider on your rc transmitter from top position directly to the bottom position, the signal changes suddenly from 1000us to 2000us PWM.
* And if you have a servo connected to the channel on the receiver, the servo will suddenly turn from min position to max position suddenly.
* But if you put an Arduino Nano with **SmoothPwmChannel** instaled in between the receiver and the servo. The signal will change in a much longer time and smoothly like: 1000.. 1200.. 1500.. 1800.. 2000us.

## Why
I wrote (mostly ported) this sketch to install it between my receiver and my **NAZA M v2** controller to smooth out the pitch movement of my *H3-3D* camera gimbal.
I Could not achieve this neither with the gimbal or flight controller settings (Because DJI did not add such a feature in their product) nor with my **RadioLink AT9** transmitter (Because it doesn't have a "Motor" menu like Futaba ([Whatch the method](http://futaba8fg.wikidot.com/channel-slow-down-using-motor-function)), and it is not OpenSource/Scriptable as FrSky Taranis is([Whatch the method](https://www.youtube.com/watch?v=htn64XRH558&feature=youtu.be))).

## What parts do you need
* Adruino board. I used Arduino Nano r3 clone because it is compact and because I easily found it in an electronic components store in our city Baku, Azerbaijan.
* Servo extension cable(s). I had one. I have cut 2 piceces 15 cm long with connectors from both sides of the cable. I have soldered these two cables to the corresponding pins on the Arduino board.
* Heatshrink tubing. To prevent shorting and secure the construction.

## Thanks
First of all thanks to Youtube user **diJMedia** for his video explanation of the algorithm: [Smooth Ramping Tilt Zenmuse H3-3D GCU with Open TX Lua script](https://www.youtube.com/watch?v=htn64XRH558&feature=youtu.be) and the Lua script he provided. I have ported the algorithm to JS for testing and finally to Arduino.

## Testing
In development process I wrote a script in JS to test the algorithm. [Here]() You can play with it. 
