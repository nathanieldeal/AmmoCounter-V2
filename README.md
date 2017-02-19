# AmmoCounter V2
Repo for AmmoCounter V2 Arduino code

Learn more at http://ammocounter.com

### 2 Digit 7 Segment Common Anode 10 Pin Out

          A  B Ca1 Ca2 F
          |  |  |   |  |
     ---------    ---------
     |   A   |    |   A   |
    F|       |B  F|       |B
     |---G---|    |---G---|
    E|       |C  E|       |C
     |   D   |    |   D   |
     ---------    ---------
          |  |  |  |  |
          C  Dp E  D  G


### How to install:
Download the latest code [here](https://github.com/nathanieldeal/AmmoCounter-V2/archive/master.zip) and unzip.

Copy the contents of the "Libraries" folder content into the “Libraries” folder in the folder you installed your Arduino software.

Double click on the AmmoCounter_V2 > AmmoCounter_V2.ino to open the sketch in the Arduino IDE.

Upload the sketch to your Arduino.

### Version History

v2.0 - Addition of 10 pin 7 Segment LED, and 2 PNP transistor code, also remove shift register functions

v1.1 - Increment and decrement toggle switches, compatible with both IR beam and counter lever switches.

v1.0 - First installation of the AmmoCounter code, various revisions include IR beam, auto-reset, reset/clip toggle buttons.