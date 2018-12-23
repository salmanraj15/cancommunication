# CAN bus communication

This is a project done in Automotive Software Engineering practical classes used to show CAN bus comminication which exchanges data such as vehicle speed, light control, door lock control etc.

Task 4 is an AUTOSAR application task done using DSpace System Desk 4. This task is to create AUTOSAR Architecture with 2 components AutoLight Adapter and Exterior light manager.
AutoLight Adapter has 3 inputs: 
- Switch on Auto
- Switch on LowBeam
- Light intensity Sensor
2 outputs: 
- Low beam
- Day time running lights

Exterior light manager has 3 inputs:
- The outputs of Autolight Adapter are the first two inputs to exterior light manager.
- Another input is left unconnected (To check if car is switched on)
3 outputs of Exterior light manager:
- Activate Low beam
- Activate Low beam indicator
- Activate day time running light

Task: Check if car is switched on. if the car is switched off then do not do anything, else if the car is switched on check for switch input. If the switch is on Auto, if the light intensity is above 100, activate day time running lights else activate low beam with low beam indicator. If the switch is on low beam, activate low beam along with low beam indicator regardless of the light intensity value and make sure that low beam and day time running lights should not be switched on at the same time.


Task 5 is based on running Blackbox tests on ECUs (manual and automated test cases). Task: Seat Belt Manager System.


## Author/s

| FROM |TO | Firstname | Lastname | 

|:-----------|:-----------| :----------- | :------------| 

| 2015-10-01 | 2016-03-18| Salman | Raj | 

##### Note: Readme is yet to be updated
