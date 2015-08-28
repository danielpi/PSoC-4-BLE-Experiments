//
//  ViewController.swift
//  HeartRateSensor
//
//  Created by Daniel Pink on 24/08/2015.
//  Copyright © 2015 Daniel Pink. All rights reserved.
//

import Cocoa
import IOBluetooth

struct AdvertisingData {
    let timestamp: NSDate
    let data: [String : AnyObject]
}

struct RSSIRecord {
    let timestamp: NSDate
    let rssi: Int
}

class DiscoveredPeripheral {
    let peripheral: CBPeripheral
    let title: String // Must be unique
    var advertisingData: [AdvertisingData] = []
    var rssiHistory: [RSSIRecord] = []
    var alarm: Bool = false
    init(peripheral: CBPeripheral, title: String) {
        self.peripheral = peripheral
        self.title = title
    }
}

class ViewController: NSViewController {
    
    @IBOutlet weak var peripheralSelectorMenu: NSPopUpButton!
    @IBOutlet weak var heartRateLabel: NSTextField!
    @IBOutlet weak var heartRateUnitsLabel: NSTextField!
    
    
    let heartRateUUID = CBUUID(string: "0x180D")
    var bleManager = CBCentralManager(delegate: nil, queue: nil, options: nil)
    
    var selectedPeripheral: DiscoveredPeripheral?
    var discoveredPeripherals: [DiscoveredPeripheral] = []
    
    //var poller: NSTimer? = nil
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view
        self.updatePeripheralSelector()
        
        bleManager.delegate = self
        bleManager.scanForPeripheralsWithServices([heartRateUUID], options: nil)
    }

    override var representedObject: AnyObject? {
        didSet {
        // Update the view, if already loaded.
        }
    }
    
    @IBAction func peripheralSelectorAction(sender: AnyObject) {
        guard let selectedMenu = peripheralSelectorMenu.selectedItem else {
            peripheralSelectorMenu.selectItemAtIndex(0)
            return
        }
        
        let matched = discoveredPeripherals.filter{ $0.title == selectedMenu.title }
        if let selected = matched.first {
            peripheralSelectorMenu.selectItemWithTitle(selected.title)
        }
    }
    
    func updatePeripheralSelector() {
        // Is there a selection or not?
        // Are there any devices available?

        peripheralSelectorMenu.removeAllItems()
        if discoveredPeripherals.count < 1 {
            peripheralSelectorMenu.addItemWithTitle("No Devices in Range")
        } else {
            peripheralSelectorMenu.addItemWithTitle("Please select a device")
            for peripheral in discoveredPeripherals {
                peripheralSelectorMenu.addItemWithTitle(peripheral.title)
            }
        }
        if let selected = selectedPeripheral {
            peripheralSelectorMenu.selectItemWithTitle(selected.title)
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
    }
    
    func centralManager(central: CBCentralManager, didDiscoverPeripheral peripheral: CBPeripheral, advertisementData: [String : AnyObject], RSSI: NSNumber) {
        let timestamp = NSDate()
        let ad = AdvertisingData(timestamp: timestamp, data: advertisementData)
        let rssiRecord = RSSIRecord(timestamp: timestamp, rssi: RSSI.integerValue)
        //let localName = advertisementData[CBAdvertisementDataLocalNameKey]
        
        let matching = discoveredPeripherals.filter() { $0.peripheral == peripheral }
        
        switch matching.count {
        case 0:
            // peripheral may not have a name. Make one up
            // peripheral may have the same name as an already discovered peripheral. Make one up
            var title: String = "Unknown Device"
            if let peripheralTitle = peripheral.name {
                title = peripheralTitle
            }
            
            let clashingTitles = discoveredPeripherals.filter { $0.title == title }
            if clashingTitles.count > 0 {
                title = "\(title) (\(clashingTitles.count + 1))"
            }
            
            let a = DiscoveredPeripheral(peripheral: peripheral, title: title)
            a.advertisingData.append(ad)
            a.rssiHistory.append(rssiRecord)
            discoveredPeripherals.append(a)
            print(a.peripheral.identifier.hashValue)
        case 1:
            let a = matching[0]
            a.advertisingData.append(ad)
            a.rssiHistory.append(rssiRecord)
        default:
            fatalError("discoveredPeripherals has duplicate records")
        }
        updatePeripheralSelector()
    }
    
    func centralManager(central: CBCentralManager, didConnectPeripheral peripheral: CBPeripheral) {
        peripheral.delegate = self
        peripheral.discoverServices(nil)
    }
    
    func centralManager(central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: NSError?) {
        peripheral.delegate = nil
    }
    
    func centralManager(central: CBCentralManager, didFailToConnectPeripheral peripheral: CBPeripheral, error: NSError?) {
        print("Failed to connect to a peripheral")
    }
}

extension ViewController: CBPeripheralDelegate {
    func peripheral(peripheral: CBPeripheral, didDiscoverServices error: NSError?) {
        if let services = peripheral.services {
            for service in services {
                peripheral.discoverCharacteristics(nil, forService: service)
            }
        }
    }
    
    func peripheral(peripheral: CBPeripheral, didDiscoverCharacteristicsForService service: CBService, error: NSError?) {
        if let characteristics = service.characteristics {
            
            for characteristic in characteristics {
                
                if characteristic.UUID == heartRateUUID {
                    service.peripheral.setNotifyValue(true, forCharacteristic: characteristic)
                    service.peripheral.readValueForCharacteristic(characteristic)
                    var alertLevel = 2
                    service.peripheral.writeValue(NSData(bytes: &alertLevel, length: 1), forCharacteristic: characteristic, type: CBCharacteristicWriteType.WithResponse)
                }
                
                peripheral.discoverDescriptorsForCharacteristic(characteristic)
                
                characteristic.isNotifying ? print("    \(characteristic.UUID) is Notifying") : print("    \(characteristic.UUID) is NOT Notifying")
                print("      \(characteristic.properties)")
            }
        }
    }
}
