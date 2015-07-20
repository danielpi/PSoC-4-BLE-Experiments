## Find Me BLE for Mac OS X

A little project to interact with an embedded BLE device that is programmed with the Find Me Profile.

So what does this program need to do?
- Search for nearby devices that are advertising that they are in Find Me mode.
- Connect to a selected component
- Adjust the alert level
- Maybe display the signal strength

### User story
The user is a budding Bluetooth hardware engineer who has a stack of BLE devices lying around his workshop. So many in fact that it can be difficult to locate any individual board. As a result he has this program open on his computer to passively keep track of which devices are in range. 

scan every 30 seconds?
Connected peripherals?

RSSI values are located in two places. When a device is connected you can request it and then access it from a property on the CBPeripheral object. During discovery the RSSI value and the advertisment data is included in the advertising packet. I want to be able to capture and store all of this data over time. 

I would like one object for every peripheral that we have ever been in contact with. I would like to be able to access an array with the timestamp, name, RSSI and advertising data for every packet of comms that we have recieved.

Looks like you can't read the alertLevel characteristic. Does that make any sense???

- Display some UI if the Computer is not able to use the BLE hardware
- Display a row for every peripheral that we come across that has the immediateAlert Service.
- Display the name and ID for each advertised peripheral
- Display the RSSI for each packet we receive
    - Average the RSSI values
- Display the time since we last received a packet, if it is more than 1min
- Click on a row to attempt a connection
- Display connection status
- Check up on the RSSI every second
- Click once to start the alert
- Alert off after 10sec
- Click a second time to start alert at higher level
- Alert off after 1min
- Allow for alert to be cancelled


peripherals can be connected or not, in alarm or not, 3 different alarm levels