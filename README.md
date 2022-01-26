# BrushlessDriveWheels

This project is to hack a brushless drive wheels unit. The wheels, ESC and controller are sold in the set for wheelchair-like UGV, so user can control the speed and direction of the vehicle by using the controller or joystick that comes with. The supplier from China doesn't provide a detail to modify the controller, but in our application, we focus on an unmanned ground vehicle with autonomuos drive, so our goal for this project is to hack the communication between the ESC and the joystick, then pass our own command from other input (e.g. Arduino, Pi, or RC transmitter).

The tested UGV is look like this image below

![](images/iso.jpg)

![](images/top.jpg)

Normally, a general wiring of this unit is as shown here...

![](images/commonwiring.JPG)

## Hack New Batch ESC

For some reason, the ESC supplier has changed something on their ESC and joystick on a new batch.

![](images/new_batch_esc_SN.jpeg)

![](images/new_batch_joystick.jpeg)

From hacking, similar with old batch method, we observed some changes as listed below

