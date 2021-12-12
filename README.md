This code is specifically written for Arduino Leonardo and Arduino Micro boards to make USB ride control panels
(fancy keyboards) to use with games like No Limits 2.

How does controller interact with the computer?
--------------------------------------------------------------------------------------------
The computer sees the Arduino Leonardo and Micro boards as generic USB keyboards. When a button on the controller is pressed,
the board sends a key-press to the computer via the USB connection.

How do I change the key mapping?
--------------------------------------------------------------------------------------------
The code uses the number keys by default, but they can be easily changed to just about any key on the keyboard. Around line
25 of the code, there is a group of “#define” statements with the heading “//key mappings.” Each key used is listed here. The
number next to the name is the keyboard key number. You can change any of these numbers to suit your needs.

How do the lights work?
--------------------------------------------------------------------------------------------
Since the controllers act like regular USB keyboards, the data only travels one way, so the lights are controlled internally
by the Ardunio code. For example, when the dispatch button is pressed, the light(s) goes off, along with the A,B,C lights,
for a number of seconds. Then, they turn back on. This is all done from inside the Arduino without and information from the
computer it is connected to.

The lighting states are as follows:
  Dispatch Light(s)
    Blinking: when not e-stopped, key on, and all function buttons/switches are in their default/off states,
    Off: when e-stopped, any function operator is turned on, or when the key switch is locked,
    Timed-Off: after dispatch is activated

  A,B,C Lights
    Blinking: when the function is in its off state,
    On Solid: when the function is in its on state,
    Off: when e-stopped or the key switch is locked,
    Timed-Off: after dispatch is pressed,

  E-Stop Light
    On Sold: when e-stop is off,
    Blinking: when e-stop is on
  
  Power Light
    On: when key switch is unlocked,
    Off: when key switch is locked
    
  Reset Light
    On: when e-stopped and the estop button is in the default position,
    Off: when key switch is locked
    
Inputs and Outputs
--------------------------------------------------------------------------------------------
The code is designed to have the following inputs:
(each input can be configured for dual-button enabling [where two buttons have to be pressed together]
using a using a series circuit)

  1 Dispatch,
  3 (A,B,C) momentary function push buttons,
  3 (A,B,C) two-way (on/off) function selector switches,
  1 E-Stop momentary push button,
  1 E-Stop switch,
  1 Lock/Unlock two-way (on/off) keyed or non-keyed selector switch,

The code is designed to have the following outputs:
(each output can be wired to multiple lights/relays using a parallel circuit)
  1 Dispatch light,
  3 Function button lights, one assigned for each function button,
  1 E-stop light,
  1 Power light,
  1 Reset light
