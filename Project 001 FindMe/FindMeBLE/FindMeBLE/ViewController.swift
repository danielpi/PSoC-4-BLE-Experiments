//
//  ViewController.swift
//  FindMeBLE
//
//  Created by Daniel Pink on 9/07/2015.
//  Copyright © 2015 Electronic Innovations. All rights reserved.
//

import Cocoa
import IOBluetooth

struct AdvertisingData {
    let timestamp: NSDate
    let rssi: Int
    let data: [String : AnyObject]
}

class DiscoveredPeripheral {
    let peripheral: CBPeripheral
    var advertisingData: [AdvertisingData] = []
    init(peripheral: CBPeripheral) {
        self.peripheral = peripheral
    }
}

class ViewController: NSViewController {
    
    @IBOutlet weak var tableView: NSTableView!
    
    let immediateAlertUUID = CBUUID(string:"0x1802")
    let alertLevelUUID = CBUUID(string: "0x2A06")
    
    var bleManager = CBCentralManager(delegate: nil, queue: nil, options: nil)
    
    var discoveredPeripherals: [DiscoveredPeripheral] = []
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        let nib = NSNib(nibNamed: "BLEPeripheralTableCellView", bundle: NSBundle.mainBundle())
        tableView.registerNib(nib!, forIdentifier: "BLEPeripheralTableCellView")
        let emptyCell = NSNib(nibNamed: "EmptyTableCellView", bundle: NSBundle.mainBundle())
        tableView.registerNib(emptyCell, forIdentifier: "EmptyTableCellView")
        
        bleManager.delegate = self
        bleManager.scanForPeripheralsWithServices([immediateAlertUUID], options: nil)
    }

    override var representedObject: AnyObject? {
        didSet {
        // Update the view, if already loaded.
            //CBCentralManager *manager;
            //CBPeripheral *peripheral;
            
            
        }
    }
    
    func soundTheAlarm(peripheral: CBPeripheral) {
        print("Sound the alarm button was pressed.")
        bleManager.connectPeripheral(peripheral, options: nil)
    }

}

extension ViewController:  NSTableViewDelegate, NSTableViewDataSource {
    
    func numberOfRowsInTableView(tableView: NSTableView) -> Int {
        if discoveredPeripherals.count < 1 {
            return 1
        }
        
        return discoveredPeripherals.count
    }
    
    func tableView(tableView: NSTableView, heightOfRow row: Int) -> CGFloat {
        return 80
    }
    
    func tableView(tableView: NSTableView, viewForTableColumn tableColumn: NSTableColumn?, row: Int) -> NSView? {
        
        if discoveredPeripherals.count < 1 {
            let cell = tableView.makeViewWithIdentifier("EmptyTableCellView", owner: self)
            return cell
        } else {
            let cell = tableView.makeViewWithIdentifier("BLEPeripheralTableCellView", owner: self) as! BLEPeripheralTableCellView
            cell.viewController = self
            
            let item = discoveredPeripherals[row]
            
            cell.peripheral = item.peripheral
            
            if let n = item.peripheral.name {
                cell.name.stringValue = n
            } else {
                cell.name.stringValue = "Unknown"
            }
            
            if let recentData = item.advertisingData.last {
                cell.rssi.stringValue = "RSSI: \(recentData.rssi)db"
            } else {
                cell.rssi.stringValue = ""
            }
            
            
            return cell
        }
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
        // This should be handled in the UI so that the user is aware of what is going on.
    }
    
    func centralManager(central: CBCentralManager, didDiscoverPeripheral peripheral: CBPeripheral, advertisementData: [String : AnyObject], RSSI: NSNumber) {
        
        let ad = AdvertisingData(timestamp: NSDate(), rssi: RSSI.integerValue, data: advertisementData)
        let localName = advertisementData[CBAdvertisementDataLocalNameKey]
        
        let matching = discoveredPeripherals.filter() { $0.peripheral == peripheral }
        
        switch matching.count {
        case 0:
            let a = DiscoveredPeripheral(peripheral: peripheral)
            a.advertisingData.append(ad)
            discoveredPeripherals.append(a)
        case 1:
            let a = matching[0]
            a.advertisingData.append(ad)
        default:
            fatalError("discoveredPeripherals has duplicate records")
        }
        
        tableView.reloadData()
    }
    
    func centralManager(central: CBCentralManager, didConnectPeripheral peripheral: CBPeripheral) {
        peripheral.delegate = self
        
        if let name = peripheral.name {
            print("Connected to peripheral named \(name)")
        } else {
            print("Connected to peripheral with ID \(peripheral.identifier)")
        }
        peripheral.discoverServices(nil)
        peripheral.readRSSI()
    }
    
    func centralManager(central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: NSError?) {
        peripheral.delegate = nil
        
        if let name = peripheral.name {
            print("Connected to peripheral named \(name)")
        } else {
            print("Connected to peripheral with ID \(peripheral.identifier)")
        }
    }
    
    func centralManager(central: CBCentralManager, didFailToConnectPeripheral peripheral: CBPeripheral, error: NSError?) {
        print("Did fail to connect \(peripheral)")
    }
    
    
}

extension ViewController: CBPeripheralDelegate {
    func peripheral(peripheral: CBPeripheral, didDiscoverServices error: NSError?) {
        if let services = peripheral.services {
            for service in services {
                print("  Discovered service ID \(service.UUID)")
                peripheral.discoverCharacteristics(nil, forService: service)
            }
            
        }
    }
    
    func peripheral(peripheral: CBPeripheral, didDiscoverIncludedServicesForService service: CBService, error: NSError?) {
        print("discovered service: \(service)")
        
    }
    
    func peripheral(peripheral: CBPeripheral, didDiscoverCharacteristicsForService service: CBService, error: NSError?) {
        if let characteristics = service.characteristics {
            
            for characteristic in characteristics {
                print("    Characteristic \(characteristic.UUID) from service \(characteristic.service) found")
                
                if characteristic.UUID == alertLevelUUID {
                    print("Characteristic is for alertLevel")
                    service.peripheral.setNotifyValue(true, forCharacteristic: characteristic)
                    service.peripheral.readValueForCharacteristic(characteristic)
                    var alertLevel = 0
                    service.peripheral.writeValue(NSData(bytes: &alertLevel, length: 1), forCharacteristic: characteristic, type: CBCharacteristicWriteType.WithResponse)
                }
                
                peripheral.discoverDescriptorsForCharacteristic(characteristic)
                
                characteristic.isNotifying ? print("    \(characteristic.UUID) is Notifying") : print("    \(characteristic.UUID) is NOT Notifying")
                print("      \(characteristic.properties)")
            }
        }
    }
    
    func peripheral(peripheral: CBPeripheral, didDiscoverDescriptorsForCharacteristic characteristic: CBCharacteristic, error: NSError?) {
        if let descriptors = characteristic.descriptors {
            for descriptor in descriptors {
                print("        \(descriptor)")
            }
        }
    }
    
    func peripheral(peripheral: CBPeripheral, didUpdateValueForCharacteristic characteristic: CBCharacteristic, error: NSError?) {
        if let data = characteristic.value {
            print("\(characteristic) updated its value to \(data)")
        } else {
            print("\(characteristic) updated its value to nil")
        }
    }
    
    func peripheral(peripheral: CBPeripheral, didUpdateValueForDescriptor descriptor: CBDescriptor, error: NSError?) {
         print("\(peripheral)")
    }
    
    func peripheralDidUpdateRSSI(peripheral: CBPeripheral, error: NSError?) {
        print("\(peripheral.RSSI)")
    }
    
}

