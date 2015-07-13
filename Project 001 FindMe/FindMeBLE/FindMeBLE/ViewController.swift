//
//  ViewController.swift
//  FindMeBLE
//
//  Created by Daniel Pink on 9/07/2015.
//  Copyright © 2015 Electronic Innovations. All rights reserved.
//

import Cocoa
import IOBluetooth


class ViewController: NSViewController {
    
    @IBOutlet weak var tableView: NSTableView!
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        let nib = NSNib(nibNamed: "BLEPeripheralTableCellView", bundle: NSBundle.mainBundle())
        tableView.registerNib(nib!, forIdentifier: "BLEPeripheralTableCellView")
    }

    override var representedObject: AnyObject? {
        didSet {
        // Update the view, if already loaded.
            //CBCentralManager *manager;
            //CBPeripheral *peripheral;
            
            
        }
    }

}

extension ViewController:  NSTableViewDelegate, NSTableViewDataSource {
    
    func numberOfRowsInTableView(tableView: NSTableView) -> Int {
        let appDelegate = NSApplication.sharedApplication().delegate as! AppDelegate // If we don't have an appdelegate we should crash.
        
        let peripherals = appDelegate.retrieveConnectedPeripherals()
        
        if peripherals.count < 1 {
            return 1
        }
        
        return peripherals.count
    }
    
    func tableView(tableView: NSTableView, heightOfRow row: Int) -> CGFloat {
        return 80
    }
    
    func tableView(tableView: NSTableView, viewForTableColumn tableColumn: NSTableColumn?, row: Int) -> NSView? {
        let appDelegate = NSApplication.sharedApplication().delegate as! AppDelegate // If we don't have an appdelegate we should crash.
        
        let peripherals = appDelegate.retrieveConnectedPeripherals()
        
        let cell = tableView.makeViewWithIdentifier("BLEPeripheralTableCellView", owner: self) as! BLEPeripheralTableCellView
        
        print("\(cell)")
        print("\(peripherals)")
        
        if peripherals.count < 1 {
            cell.name.stringValue = "None"
            cell.rssi.stringValue = "RSSI: --db"
            cell.connectButton.stringValue = "Connect"
            cell.connectButton.enabled = false
            cell.alertButton.hidden = true
        } else {
            let item: CBPeripheral = peripherals[row]
            
            if let n = item.name {
                cell.name.stringValue = n
            } else {
                cell.name.stringValue = "Unknown"
            }
            
            cell.connectButton.stringValue = "Connect"
            cell.connectButton.enabled = true
        }
        
        return cell
    }
}


