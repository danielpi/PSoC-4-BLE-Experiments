//
//  ViewController.swift
//  RGB_CapSense
//
//  Created by Daniel Pink on 17/09/2015.
//  Copyright © 2015 Daniel Pink. All rights reserved.
//

import Cocoa
import CoreBluetooth


class ViewController: NSViewController {

    @IBOutlet weak var redTextField: NSTextField!
    @IBOutlet weak var greenTextField: NSTextField!
    @IBOutlet weak var blueTextField: NSTextField!
    @IBOutlet weak var slider: NSSlider!
    @IBOutlet weak var colorWell: NSColorWell!

    
    @IBOutlet weak var connectButton: NSButton!
    @IBOutlet weak var deviceTable: NSTableView!
    
    var devices = [CBPeripheral]()
    
    let capSenseServiceUUID = CBUUID(string: "0xCAB5")
    let rgbServiceUUID = CBUUID(string: "0xCBBB")
    
    let capSenseSliderCharacteristicUUID = CBUUID(string: "0xCAA2")
    let rgbLEDCharacteristicUUID = CBUUID(string: "0xCBB1")
    //let eiVoltageMeasurementCharacteristic = CBUUID(string: "0x0000000000001000800000805f9b34fb")
    
    var bleManager = CBCentralManager(delegate: nil, queue: nil, options: nil)
    var connectedDevice: CBPeripheral?
    
    //var selectedPeripheral: DiscoveredPeripheral?
    //var discoveredPeripherals: [DiscoveredPeripheral] = []
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        let nib = NSNib(nibNamed: "DeviceCellView", bundle: NSBundle.mainBundle())
        deviceTable.registerNib(nib!, forIdentifier: "DeviceCellView")
        
        bleManager.delegate = self
        bleManager.scanForPeripheralsWithServices([capSenseServiceUUID,rgbServiceUUID], options: nil)
    }

    override var representedObject: AnyObject? {
        didSet {
        // Update the view, if already loaded.
        }
    }

    @IBAction func connectAction(sender: AnyObject) {
        print(devices.count)
        print(deviceTable.selectedRow)
        if (deviceTable.selectedRow >= 0) && (devices.count > 0) {
            let selected = devices[deviceTable.selectedRow]
            bleManager.connectPeripheral(selected, options: nil)
        }
    }

    @IBAction func colorChange(sender: AnyObject) {
        print(colorWell.color)
        if let device = connectedDevice {
            RateLimit.execute(name: "RefreshRGB", limit: 0.1) {
                writeColor(colorWell.color, forPeripheral: device)
            }
            
        }
    }
    
    @IBAction func tableViewAction(sender: AnyObject) {
        if (deviceTable.selectedRow >= 0) && (devices.count > 0) {
            let selected = devices[deviceTable.selectedRow]
            if selected != connectedDevice {
                print("Changing selection")
                bleManager.connectPeripheral(selected, options: nil)
            }
        }
    }
}

extension ViewController: NSTableViewDataSource {
    func numberOfRowsInTableView(tableView: NSTableView) -> Int {
        return devices.count < 1 ? 1 : devices.count
    }
}

extension ViewController: NSTableViewDelegate {
    func tableView(tableView: NSTableView, heightOfRow row: Int) -> CGFloat {
        return 40
    }
    
    func tableView(tableView: NSTableView, viewForTableColumn tableColumn: NSTableColumn?, row: Int) -> NSView? {
        
        let cell = tableView.makeViewWithIdentifier("DeviceCellView", owner: self) as! DeviceCellView
        if devices.count < 1 {
            cell.name.stringValue = "No Devices"
        } else {
            
            let item = devices[row]
            
            if let n = item.name {
                cell.name.stringValue = n
            } else {
                cell.name.stringValue = "Unknown"
            }
        }
        
        return cell
    }
}


extension ViewController: CBCentralManagerDelegate {
    func centralManagerDidUpdateState(central: CBCentralManager) {
        switch central.state {
        case .Unknown:
            print("CoreBluetooth BLE state is unknown")
        case .Resetting:
            print("CoreBluetooth BLE hardware is resetting")
        case .Unsupported:
            print("CoreBluetooth BLE hardware is unsupported on this platform")
        case .Unauthorized:
            print("CoreBluetooth BLE state is unauthorized")
        case .PoweredOff:
            print("CoreBluetooth BLE hardware is powered off")
        case .PoweredOn:
            print("CoreBluetooth BLE hardware is powered on and ready")
        }
    }
    
    func centralManager(central: CBCentralManager, didDiscoverPeripheral peripheral: CBPeripheral, advertisementData: [String : AnyObject], RSSI: NSNumber) {
        print("Discovered peripheral: \(peripheral)")
        
        if !devices.contains(peripheral) {
            devices.append(peripheral)
            deviceTable.reloadData()
        }
        
        //bleManager.connectPeripheral(peripheral, options: nil)
        //print("What is going on???: \(bleManager)")
    }
    
    func centralManager(central: CBCentralManager, didConnectPeripheral peripheral: CBPeripheral) {
        print("Connected to peripheral: \(peripheral)")
        connectedDevice = peripheral
        peripheral.delegate = self
        peripheral.discoverServices(nil)
    }
    func centralManager(central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: NSError?) {
        print("Disconnected from peripheral: \(peripheral)")
        peripheral.delegate = nil
        connectedDevice = nil
    }
    
    func centralManager(central: CBCentralManager, didFailToConnectPeripheral peripheral: CBPeripheral, error: NSError?) {
        print("Failed to connect to peripheral: \(peripheral)")
    }
}


extension ViewController: CBPeripheralDelegate {
    func peripheral(peripheral: CBPeripheral, didDiscoverServices error: NSError?) {
        if let services = peripheral.services {
            for service in services {
                print("Discovered service ID \(service.UUID)")
                peripheral.discoverCharacteristics(nil, forService: service)
            }
        }
    }
    
    func writeColor(color: NSColor, forPeripheral peripheral: CBPeripheral) {
        var red: UInt8 = UInt8(color.redComponent * 0xFF)
        var green: UInt8 = UInt8(color.greenComponent * 0xFF)
        var blue: UInt8 = UInt8(color.blueComponent * 0xFF)
        var alpha: UInt8 = UInt8(color.alphaComponent * 0xFF)
        
        let data = NSMutableData(bytes: &red, length: sizeof(UInt8))
        data.appendBytes(&green, length: sizeof(UInt8))
        data.appendBytes(&blue, length: sizeof(UInt8))
        data.appendBytes(&alpha, length: sizeof(UInt8))
        
        if let services = peripheral.services {
            for service in services {
                if let characteristics = service.characteristics {
                    for characteristic in characteristics {
                        if characteristic.UUID == rgbLEDCharacteristicUUID {
                            peripheral.writeValue(data, forCharacteristic: characteristic, type: .WithoutResponse)
                        }
                    }
                }
            }
        }
    }
    
    func peripheral(peripheral: CBPeripheral, didDiscoverCharacteristicsForService service: CBService, error: NSError?) {
        if let characteristics = service.characteristics {
            for characteristic in characteristics {
                print("Discovered characteristic \(characteristic)")
                
                print("\(capSenseSliderCharacteristicUUID) \(characteristic.UUID)")
                if characteristic.UUID == capSenseSliderCharacteristicUUID {
                    service.peripheral.setNotifyValue(true, forCharacteristic: characteristic)
                    service.peripheral.readValueForCharacteristic(characteristic)
                }
                if characteristic.UUID == rgbLEDCharacteristicUUID {
                    let color = colorWell.color
                    writeColor(colorWell.color, forPeripheral: peripheral)
                    
                    var red: UInt8 = UInt8(color.redComponent * 0xFF)
                    var green: UInt8 = UInt8(color.greenComponent * 0xFF)
                    var blue: UInt8 = UInt8(color.blueComponent * 0xFF)
                    var alpha: UInt8 = UInt8(color.alphaComponent * 0xFF)
                    
                    let data = NSMutableData(bytes: &red, length: sizeof(UInt8))
                    data.appendBytes(&green, length: sizeof(UInt8))
                    data.appendBytes(&blue, length: sizeof(UInt8))
                    data.appendBytes(&alpha, length: sizeof(UInt8))
                    print(data)
                    
                    //service.peripheral.writeValue(data, forCharacteristic: characteristic, type: .WithoutResponse)
                    
                }
            }
        }
    }
    
    func peripheral(peripheral: CBPeripheral, didUpdateValueForCharacteristic characteristic: CBCharacteristic, error: NSError?) {
        if characteristic.UUID == capSenseSliderCharacteristicUUID {
            if let data = characteristic.value {
                var out: NSInteger = 0
                data.getBytes(&out, length: sizeof(NSInteger))
                slider.integerValue = out
            } else {
                slider.integerValue = 0
            }
        }
    }
}