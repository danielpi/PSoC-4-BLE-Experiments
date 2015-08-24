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
    var advertisingData: [AdvertisingData] = []
    var rssiHistory: [RSSIRecord] = []
    var alarm: Bool = false
    init(peripheral: CBPeripheral) {
        self.peripheral = peripheral
    }
}

class ViewController: NSViewController {
    
    @IBOutlet weak var tableView: NSTableView!
    
    // let immediateAlertUUID = CBUUID(string: "0x1802")
    var bleManager = CBCentralManager(delegate: nil, queue: nil, options: nil)
    
    var discoveredPeripherals: [DiscoveredPeripheral] = []
    
    //var poller: NSTimer? = nil
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }

    override var representedObject: AnyObject? {
        didSet {
        // Update the view, if already loaded.
        }
    }
}

extension ViewController: NSTableViewDelegate, NSTableViewDataSource {
    func numberOfRowsInTableView(tableView: NSTableView) -> Int {
        if discoveredPeripherals.count < 1 {
            return 1
        } else {
            return discoveredPeripherals.count
        }
    }
    
    func tableView(tableView: NSTableView, heightOfRow row: Int) -> CGFloat {
        return 80
    }
    
    func tableView(tableView: NSTableView, viewForTableColumn tableColumn: NSTableColumn?, row: Int) -> NSView? {
        if discoveredPeripherals.count < 1 {
            let cell = tableView.makeViewWithIdentifier("EmptyTableCellView", owner: self)
            print(tableView)
            print(cell)
            return cell
        } else {
            let cell = tableView.makeViewWithIdentifier("BLEPeripheralTableVellView", owner: self) as! BLEPeripheralTableCellView
            cell.viewController = self
            
            return cell
        }
    }
}

