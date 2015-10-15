//
//  MyTheme.swift
//  RealTimeExample
//
//  Created by Daniel Pink on 10/10/2015.
//  Copyright © 2015 Daniel Pink. All rights reserved.
//

import Cocoa

//
//  EIExperimentalTheme.m
//  CorePlotExperiments
//
//  Created by Daniel Pink on 10/01/13.
//  Copyright (c) 2013 Electronic Innovations. All rights reserved.
//

class GraphTheme: CPTTheme {
    let backgroundColor =       CPTColor(componentRed: 0.87, green: 0.87, blue: 0.87, alpha: 1.00)
    let plotBackgroundColor =   CPTColor(componentRed: 1.00, green: 1.00, blue: 1.00, alpha: 1.00)
    let blueColor =             CPTColor(componentRed: 0.02, green: 0.49, blue: 0.62, alpha: 1.00)
    let redColor =              CPTColor(componentRed: 1.00, green: 0.14, blue: 0.00, alpha: 1.00)
    let greenColor =            CPTColor(componentRed: 0.00, green: 0.74, blue: 0.22, alpha: 1.00)
    let orangeColor =           CPTColor(componentRed: 1.00, green: 0.51, blue: 0.00, alpha: 1.00)
    
    //let orderedColors = [blueColor, redColor, greenColor, orangeColor]
    
    var majorGridLineStyle: CPTLineStyle {
        let lineStyle = CPTMutableLineStyle()
        lineStyle.lineColor = CPTColor(genericGray: 0.25)
        lineStyle.lineWidth = 0.2
        return lineStyle
    }
    var minorGridLineStyle: CPTLineStyle {
        let lineStyle = CPTMutableLineStyle()
        lineStyle.lineColor = CPTColor(genericGray: 0.125)
        lineStyle.lineWidth = 0.1
        return lineStyle
    }
    
    override func applyThemeToBackground(graph: CPTGraph?) {
        if let g = graph {
            g.fill = CPTFill(color: backgroundColor)
        }
    }
    
    override func applyThemeToPlotArea(plotAreaFrame: CPTPlotAreaFrame?) {
        if let p = plotAreaFrame,
              pa = plotAreaFrame?.plotArea {
            p.fill = CPTFill(color: backgroundColor)
            pa.fill = CPTFill(color: plotBackgroundColor)
        }
    }
    
    override func applyThemeToAxisSet(axisSet: CPTAxisSet?) {
        if let axSet = axisSet {
            let majorLineStyle = CPTMutableLineStyle()
            majorLineStyle.lineCap = CGLineCap.Butt
            majorLineStyle.lineColor = CPTColor(genericGray: 0.5)
            majorLineStyle.lineWidth = 1.0
            
            let minorLineStyle = CPTMutableLineStyle()
            minorLineStyle.lineCap = CGLineCap.Butt
            minorLineStyle.lineColor = CPTColor.blackColor()
            minorLineStyle.lineWidth = 1.0
            
            
            let blackTextStyle = CPTMutableTextStyle()
            blackTextStyle.color = CPTColor(genericGray: 0.4)
            blackTextStyle.fontSize = 14.0
            
            let minorTickBlackTextStyle = CPTMutableTextStyle()
            minorTickBlackTextStyle.color = CPTColor.blackColor()
            minorTickBlackTextStyle.fontSize = 12.0
            
            let x = axSet.axes![0] as! CPTXYAxis
            x.labelingPolicy = .Automatic
            //x.majorIntervalLength = 100
            x.tickDirection = .None
            x.minorTicksPerInterval = 4
            x.majorTickLineStyle = majorLineStyle
            x.minorTickLineStyle = minorLineStyle
            x.axisLineStyle = majorLineStyle
            x.majorTickLength = 7.0
            x.minorTickLength = 5.0
            x.labelTextStyle = blackTextStyle
            x.minorTickLabelTextStyle = blackTextStyle
            x.titleTextStyle = blackTextStyle
            x.majorGridLineStyle = majorGridLineStyle
            x.minorGridLineStyle = minorGridLineStyle
            x.orthogonalPosition = 0.0 // Draws the x axis and labels at y = 0
            x.axisConstraints = CPTConstraints(lowerOffset: 1) // Fixes the axis (with labels) to the bottom of the plot
            
            
            let y = axSet.axes![1] as! CPTXYAxis
            y.labelingPolicy = .Automatic
            //y.majorIntervalLength = 5
            y.tickDirection = .None
            y.minorTicksPerInterval = 1
            y.majorTickLineStyle = majorLineStyle
            y.minorTickLineStyle = minorLineStyle
            y.axisLineStyle = majorLineStyle
            y.majorTickLength = 7.0
            y.minorTickLength = 5.0
            y.labelTextStyle = blackTextStyle
            y.minorTickLabelTextStyle = blackTextStyle
            y.titleTextStyle = blackTextStyle
            y.majorGridLineStyle = majorGridLineStyle
            y.minorGridLineStyle = minorGridLineStyle
            y.orthogonalPosition = 0.0
            y.axisConstraints = CPTConstraints(lowerOffset: 1) // Fixes the axis (with labels) to the left of the plot
        }
    }
}
