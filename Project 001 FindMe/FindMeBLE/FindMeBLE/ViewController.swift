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
        print("The alarm")
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
        print("\(central)")
    }
    
    func centralManager(central: CBCentralManager, didDiscoverPeripheral peripheral: CBPeripheral, advertisementData: [String : AnyObject], RSSI: NSNumber) {
        
        let ad = AdvertisingData(timestamp: NSDate(), rssi: RSSI.integerValue, data: advertisementData)
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
        print("Did connect \(peripheral)")
    }
}


