ArdunioTHC
==========

Arduino-based Torch Height Controller for CNC Plasma torch systems

(see "Status.txt" for current project status.)

***

WARNING: CNC systems are inherently dangerous.  Use of this hardware and software is at your own risk.  I am not an expert at CNC systems, THC's or electrical design.  I'm just an embedded software guy who did the hardware design with help from some experienced EE's.

When working with a THC, you're messing with high voltage.  All of the project information is only for use with the acknowledgement and agreement that no statement of safety or appropriateness is offered for any of the information.  Additionally, you take full responsibility for all uses of this hardware, software and documentation.

***

Background
----------
Having recently built a Computer Numeric Control (CNC) driven Plasma torch cutting table, I wanted to add a torch height controller (THC).  I decided to make my own and open source it.

Both the CNC table build and the THC development couldn't have been done without the significant support I received from the folks on the Everlast Forums.  	www.everlastgenerators.com/forums
These are great forums for support with all things welding and plasma cutting.

I have built this THC specifically to work with the Everlast Power Plasma 50 plasma torch.  This is the unit I have and I've been very happy with it.

There are two very significant aspects that make the Everlast PP50 ideal for this project.  They are:
	- Pilot Arc start
	- full CNC interface

Pilot Arc Start - the PP50 offers a "pilot arc" start, this is in contrast to an HF (High Frequency) start.  The HF start is very noisy and has been reported to cause significant interference with computer systems.

Full CNC Interface - all control signals are available on a connector on the back of the PP50.  They are:
	- Torch On (input, dry contact)
	- Arc Good (output, dry contract)
	- Full torch voltage
	- Divided torch voltage

This hardware design uses the full arc voltage.  I've observed about 2 volts of noise on the voltage, no matter what you use (full voltage or divided voltage).  By using the full voltage and dividing it myself, the noise is kept to a significantly lower percentage of the signal.

I have not tested the THC with any other brand of plasma torch.  It should be a good starting point to make a THC to work with other torches that provide a CNC interface.

Hardware
--------
The system uses an Arduino Mega with a SainSmart 1602 LCD Keypad Shield.

I developed a custom shield with necessary THC functionality.  It provides:
	- isolated interface to CNC system
	- interface to Power Plasma 50 CNC interface
	- interface to 5 push buttons (mode, menu/select, cancel,up, down)
	- (optional) isolated RS-232 (TTL levels) for interface to THC

--

I will be updating all files as the project progresses.  There will be the following subdirectories:
	- "ArduinoSource", using Eclipse plugin from www.baeyens.it/eclipse/
	- "Documentation"
	- "HardwareDesign", Eagle schematic and PCB layout files
	- "WindowsSource", C# .Net application used to capture cutting data

License
-------
This is copyrighted software that is released under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 license

The unofficial summary is:

You can use it as it is for non-commercial purposes.  If you modify it, you must make the modifications public and attribute the original source to me.

(If you want to license it for commerical use, please contact me.)

