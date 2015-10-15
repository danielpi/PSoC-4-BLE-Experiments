//
//  ViewController.swift
//  BatteryMonitor02
//
//  Created by Daniel Pink on 9/10/2015.
//  Copyright © 2015 Daniel Pink. All rights reserved.
//

import Cocoa
import CoreBluetooth


class ViewController: NSViewController {

    @IBOutlet weak var deviceList: NSTableView!
    @IBOutlet weak var scanButton: NSButton!
    @IBOutlet weak var voltageDisplay: NSTextField!
    @IBOutlet weak var exportButton: NSButton!
    @IBOutlet weak var graphView: CPTGraphHostingView!
    
    var data = [Double]()
    var max = 0.0
    var min = 0.0
    
    var devices = [CBPeripheral]()
    let voltageMeasurementServiceUUID = CBUUID(string: "C8DAB479-738F-43CA-8368-0EBBB42D0D08")
    let voltageMeasurementCharacteristic = CBUUID(string: "9E5686A7-8308-4F80-845D-BE211D1F0E9F")
    
    var bleManager = CBCentralManager(delegate: nil, queue: nil, options: nil)
    var connectedDevice: CBPeripheral?
    
    let graph = CPTXYGraph(frame: CGRectZero)
    let theme = GraphTheme()
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        let nib = NSNib(nibNamed: "DeviceCellView", bundle: NSBundle.mainBundle())
        deviceList.registerNib(nib!, forIdentifier: "DeviceCellView")
        
        bleManager.delegate = self
        bleManager.scanForPeripheralsWithServices([voltageMeasurementServiceUUID], options: nil)
        
        
        // Setting up graph
        //let graph = CPTXYGraph(frame: CGRectZero)
        graph.applyTheme(theme)
        
        // Set a styled title
        graph.title = "Battery Voltage"
        
        // Setup Scatter Plot Space
        let plotSpace = graph.defaultPlotSpace as! CPTXYPlotSpace
        plotSpace.allowsUserInteraction = true
        plotSpace.delegate = self
        
        // Create a Plot that uses the data source method
        let plot = CPTScatterPlot()
        plot.identifier = "Scatter Plot"
        
        plot.delegate = self
        plot.dataSource = self
        
        graph.addPlot(plot)
        
        graph.paddingLeft = 60
        graph.paddingTop = 30
        graph.paddingRight = 30
        graph.paddingBottom = 30
        graph.plotAreaFrame!.masksToBorder = false
        
        // Auto scale the plot space to fit the plot data
        // Extend the y range by 10% for neatness
        plotSpace.scaleToFitPlots([plot])
        plotSpace.xRange = CPTPlotRange(location: 0, length: 10)
        plotSpace.yRange = CPTPlotRange(location: 0, length: 20)
        
        // This stops you from scrolling off into ranges that can't have values.
        plotSpace.globalXRange = CPTPlotRange(location: 0, length: 64000)
        plotSpace.globalYRange = CPTPlotRange(location: 0, length: 30)
        
        self.graphView.hostedGraph = graph
    }

    override var representedObject: AnyObject? {
        didSet {
        // Update the view, if already loaded.
        }
    }
    
    @IBAction func tableViewAction(sender: AnyObject) {
        if (deviceList.selectedRow >= 0) && (devices.count > 0) {
            let selected = devices[deviceList.selectedRow]
            if selected != connectedDevice {
                print("Changing selection")
                bleManager.connectPeripheral(selected, options: nil)
            }
        }
    }
    
    @IBAction func scanForPeripherals(sender: AnyObject) {
    }

    @IBAction func exportData(sender: AnyObject) {
    }
    
    
}

extension ViewController: CPTPlotDataSource, CPTPlotSpaceDelegate {
    //-(NSUInteger)numberOfRecordsForPlot:(nonnull CPTPlot *)plot;
    func numberOfRecordsForPlot(plot: CPTPlot) -> UInt {
        return UInt(data.count)
    }
    
    func numberForPlot(plot: CPTPlot, field fieldEnum: UInt, recordIndex idx: UInt) -> AnyObject? {
        
        switch fieldEnum {
        case 0:
            return idx
        case 1:
            return data[Int(idx)]
        default:
            return 0
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
            deviceList.reloadData()
        }
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
    
    func peripheral(peripheral: CBPeripheral, didDiscoverCharacteristicsForService service: CBService, error: NSError?) {
        if let characteristics = service.characteristics {
            for characteristic in characteristics {
                print("Discovered characteristic \(characteristic.UUID)")
                
                switch characteristic.UUID {
                case voltageMeasurementCharacteristic:
                    service.peripheral.setNotifyValue(true, forCharacteristic: characteristic)
                    service.peripheral.readValueForCharacteristic(characteristic)
                default:
                    print("No sure what to do with this characteristic \(characteristic)")
                }
            }
        }
    }
    
    func peripheral(peripheral: CBPeripheral, didUpdateValueForCharacteristic characteristic: CBCharacteristic, error: NSError?) {
        
        struct MultimeterFormater {
            static let formatter: NSNumberFormatter = {
                let formatter = NSNumberFormatter()
                formatter.maximumIntegerDigits = 2
                formatter.minimumIntegerDigits = 1
                formatter.maximumFractionDigits = 2
                formatter.minimumFractionDigits = 2
                return formatter
                }()
        }
        
        struct IntegerFormatter {
            static let formatter: NSNumberFormatter = {
                let formatter = NSNumberFormatter()
                formatter.hasThousandSeparators = false
                return formatter
                }()
        }
        
        print("Value for Characterisic \(characteristic.UUID)")
        switch characteristic.UUID {
        case voltageMeasurementCharacteristic:
            if let data = characteristic.value {
                var out: Float = 0
                data.getBytes(&out, length: 4)
                voltageDisplay.stringValue = MultimeterFormater.formatter.stringFromNumber(out)!
                self.data += [Double(out)]
                if Double(out) > max {
                    max = Double(out)
                }
                if Double(out) < min {
                    min = Double(out)
                }
                
                graphView.hostedGraph?.reloadData()
                
                let plotSpace = graph.defaultPlotSpace as! CPTXYPlotSpace
                // To rescale the plot to include everything in the data array use the following.
                //plotSpace.scaleToFitPlots([graph.plotAtIndex(0)!])
                
                // If you want a view that tracks the latest 20 values use the following.
                plotSpace.xRange = CPTPlotRange(location: self.data.count - 200, length: 200)
                plotSpace.yRange = CPTPlotRange(location: 0, length: max * 1.1)
                
            } else {
                voltageDisplay.stringValue = "NA"
            }
        default:
            print("Updated value was not handled")
        }
    }
}

