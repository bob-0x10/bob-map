# attendance-checker-node

node which collect packet, parsing and send to collector for attendance-checker

## How to use

1. Run ``dependencies.sh`` so your service ready. 
1. Connect the wireless LAN card and set it to monitor mode. 
	1. Check the interface name of the wireless LAN card connected with the ``iwconfig`` command.(ex. wlan0) 
	1. Change to monitor mode with this command. 
		```
		sudo ifconfig wlan0 down
		sudo iwconfig wlan0 mode monitor
		sudo ifconfig wlan0 up
		```
1. Enter the 'src' directory and run the ``sudo make`` command. 
1. Run ``bob0x10_core`` and enter the argument values ​​according to the syntax. 
