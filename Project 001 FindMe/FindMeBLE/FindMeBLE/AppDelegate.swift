//
//  AppDelegate.swift
//  FindMeBLE
//
//  Created by Daniel Pink on 9/07/2015.
//  Copyright © 2015 Electronic Innovations. All rights reserved.
//

import Cocoa
import IOBluetooth

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {
    let immediateAlertUUID = CBUUID(string:"0x1802")
    var bleManager = CBCentralManager(delegate: nil, queue: nil, options: nil) //CBCentralManager(delegate: self, queue: dispatch_get_main_queue())
    
    func applicationDidFinishLaunching(aNotification: NSNotification) {
        // Insert code here to initialize your application
        bleManager.delegate = self
        
        bleManager.scanForPeripheralsWithServices([immediateAlertUUID], options: nil)
    }

    func applicationWillTerminate(aNotification: NSNotification) {
        // Insert code here to tear down your application
    }
    
    func centralManagerDidUpdateState(central: CBCentralManager) {
        print("\(central)")
    }

    
}

extension AppDelegate: CBCentralManagerDelegate {
    
    
    func centralManager(central: CBCentralManager, didDiscoverPeripheral peripheral: CBPeripheral, advertisementData: [String : AnyObject], RSSI: NSNumber) {
        print("\(peripheral) RSSI:\(RSSI) Data:\(advertisementData)")
    }
    
    // Methods for interogating the bluetooth manager
    //func retrieveConnectedPeripheralsWithServices(_ serviceUUIDs: [CBUUID]) -> [CBPeripheral]
    func retrieveConnectedPeripherals() -> [CBPeripheral] {
        return bleManager.retrieveConnectedPeripheralsWithServices([immediateAlertUUID])
    }
}


