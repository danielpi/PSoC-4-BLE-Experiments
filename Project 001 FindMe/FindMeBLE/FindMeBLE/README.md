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