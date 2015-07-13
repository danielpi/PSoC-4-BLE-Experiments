//
//  BLEPeripheralTableCellView.swift
//  FindMeBLE
//
//  Created by Daniel Pink on 13/07/2015.
//  Copyright © 2015 Electronic Innovations. All rights reserved.
//

import Cocoa

class BLEPeripheralTableCellView: NSView {

    @IBOutlet weak var name: NSTextField!
    @IBOutlet weak var rssi: NSTextField!
    @IBOutlet weak var connectButton: NSButton!
    @IBOutlet weak var alertButton: NSButton!
    
}
