//
//  BLEPeripheralTableCellView.swift
//  FindMeBLE
//
//  Created by Daniel Pink on 13/07/2015.
//  Copyright © 2015 Electronic Innovations. All rights reserved.
//

import Cocoa
import IOBluetooth

class BLEPeripheralTableCellView: NSView {
    
    var viewController: ViewController?
    var peripheral: CBPeripheral?
    
    @IBOutlet weak var name: NSTextField!
    @IBOutlet weak var rssi: NSTextField!
    @IBOutlet weak var alertButton: NSButton!
    
    @IBAction func soundTheAlarm(sender: AnyObject) {
        if let vc = viewController,
           let  p = peripheral {
            vc.soundTheAlarm(p)
        }
    }
}
