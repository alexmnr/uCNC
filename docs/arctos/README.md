# Disclaimer

This guide is a work in progress and is an experimental alternative to the GRBL controller for the Arctos.
It solves a few problems:
* The Arcots GRBL fork as currently documented isn't set up for the limit switches.
* The Arctos GRBL fork software's 'homing' system is unstable and has had numerous reports of crashes and unstable behaviour.

This is an early draft and has only been bench tested with the Mega and motors, but has not (yet) been fully realized
on a fully built Arctos.  I am hoping to remedy that when I've finished construction of the rest of the bot.

Your mileage may vary and if you decide to use it at this stage, you should be prepared to help with the debugging and shakedown of the settings and raise a PR to this repo to help fix them.

# Flashing uCNC for your Arctos robot arm:
Flashing uCNC firmware to an Arduino Mega is a process that involves uploading new software to the microcontroller on the board. In order to do this, you’ll need a few things:

    This repository checked out to your computer
    An Arduino MEGA 2560
    A USB cable to connect the board to your computer
    The GRBL firmware (which can be downloaded from this link). It has to be unzipped and zipped the grbl folder only.
    The Arduino IDE software (which can be downloaded from the official Arduino website)

Once you have these things, you can follow the steps below to flash the uCNC firmware to your Arduino:

Step 1: Install the Arduino IDE Software Download the Arduino IDE software and install it on your computer. Once the installation is complete, open the Arduino IDE.

Step 2: Connect the Arduino Board Connect your Arduino board to your computer using the USB cable.

Step 3: Select the Arduino Board Type In the Arduino IDE, go to the “Tools” menu and select “Board.” Choose the type of Arduino board you are using (such as “Arduino Uno” or “Arduino Nano”).

Step 4: Select the Serial Port In the same “Tools” menu, select “Port” and choose the serial port that your Arduino board is connected to. If you’re not sure which port to select, you can check the “Device Manager” on your computer to see which port the Arduino board is using.

Step 5: Configure uCNC for the Arctos robot.  Copy the settings .h files to the 'uCNC' directory.
```
cp docs/arctos/boardmap_reset.h uCNC/
cp docs/arctos/boardmap_overrides.h uCNC/
cp docs/arctos/cnc_hal_reset.h uCNC/
cp docs/arctos/cnc_hal_overrides.h uCNC/
```

There is also a 'ucnc_build.json' you can use with the uCNC [configuration tool](https://paciente8159.github.io/uCNC-config-builder/) to alter your set-up.

Step 6: Open the uCNC.ino file in the Arduino IDE

Step 7: Upload the sketch (->) Arrow Button.

Step 8: Modify the settings as below in the Serial console:

uCNC settings:

```
$0 = 33.334 (Step pulse time, microseconds)
$1 = 255 (Step idle delay, milliseconds)
$2 = 0 (Step pulse invert, mask)
$3 = 0 (Step direction invert, mask)
$4 = 0 (Invert step enable pin, boolean)
$5 = 255 (Invert limit pins, boolean)
$6 = 0 (Invert probe pin, boolean)
$10 = 0 (Status report options, mask)
$11 = 0.010 (Junction deviation, millimeters)
$12 = 0.002 (Arc tolerance, millimeters)
$13 = 0 (Report in inches, boolean)
$20 = 0 (Soft limits enable, boolean)
$21 = 1 (Hard limits enable, boolean)
$22 = 1 (Homing cycle enable, boolean)
$23 = 0 (Homing direction invert, mask)
$24 = 400.000 (Homing locate feed rate, mm/min)
$25 = 400.000 (Homing search seek rate, mm/min)
$26 = 250 (Homing switch debounce delay, milliseconds)
$27 = 5.0 (Homing switch pull-off distance, millimeters)
$30 = 0 (Maximum spindle speed, RPM)
$31 = 5 (Minimum spindle speed, RPM)
$32 = 0 (Laser-mode enable, boolean)
$81 = 0 (Tool - unused)
$100 = 120.000 (X-axis travel resolution, step/mm)
$101 = 576.000 (Y-axis travel resolution, step/mm)
$102 = 450.000 (Z-axis travel resolution, step/mm)
$103 = 120.000 (A-axis travel resolution, step/mm)
$104 = 120.000 (B-axis travel resolution, step/mm)
$105 = 120.000 (C-azis travel resolution, step/mm)
$110 = 7000.000 (X-axis maximum rate, mm/min)
$111 = 900.000 (Y-axis maximum rate, mm/min)
$112 = 9000.000 (Z-axis maximum rate, mm/min)
$113 = 9000.000 (A-axis maximum rate mm/min)
$114 = 9000.000 (B-axis maximum rate mm/min)
$115 = 9000.000 (C-axis maximum rate mm/min)
$120 = 50.000 (X-axis acceleration, mm/sec^2)
$121 = 150.000 (Y-axis acceleration, mm/sec^2)
$122 = 150.000 (Z-axis acceleration, mm/sec^2)
$123 = 150.000 (A-axis acceleration, mm/sec^2)
$124 = 150.000 (B-axis acceleration, mm/sec^2)
$125 = 150.000 (C-axis acceleration, mm/sec^2)
$130 = 150.000 (X-axis maximum travel, millimeters)
$131 = 200.000 (Y-axis maximum travel, millimeters)
$132 = 150.000 (Z-axis maximum travel, millimeters)
$133 = 800.000 (A-axis maximum travel, millimeters)
$134 = 800.000 (B-axis maximum travel, millimeters)
$135 = 800.000 (C-axis maximum travel, millimeters)
```

