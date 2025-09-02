# SIMON Game Clone

This repo holds the Arduino .ino file used to program the Uno for the SIMON game. 

---
### PIN Definitions:
```
Red LED Anode    -> 13  
Green LED Anode  -> 12  
Blue LED Anode   -> 11  
Yellow LED Anode -> 10

Red Button Positive   -> 7
Green Button Positive -> 6
Blue Button Positive  -> 5
Green Button Positive -> 4

Buzzer Positive -> 3
```

## Wiring Diagram
I use a 220 ohm resistor on each anode side of the LED to Vcc  
I use a 1k ohm pull-up resistor on each switch to pull the pins pin high. The button will go negative when depressed so all button presses are treated as LOW for press.

![wiring diagram](https://github.com/jdodson3106/CCAMD_Course/blob/master/simon_clone/Arduino%20Writing%20Diagram.png)
