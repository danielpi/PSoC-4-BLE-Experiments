# PSoC 4 BLE Experiments

This set of projects is an attempt to use Cypress Semiconductors [100 projects in 100 days](http://www.cypress.com/?id=5509) as a basis for getting to know the [PSoC 4 BLE device](http://www.cypress.com/psoc4ble/) as well as Cocoa BLE programming. My plan is to try and re-create each project by myself (with the backup of being able to peruse a working project from [github](https://github.com/cypresssemiconductorco/PSoC-4-BLE/tree/master/100_Projects_in_100_Days). In addition I'll try to implement custom Mac or iOS projects that can interact with the dev board where appropriate.

Let's see how I go...

### Project 001 Find Me
The first project looks at how to make a device that can call out if it is lost. This is your typical location tile that you put on your keychain and when you press a button on the phone it makes a noise and flashes a light to make it easier to find. This functionality is built in to the BLE spec as the [Find Me Profile](https://developer.bluetooth.org/TechnologyOverview/Pages/FMP.aspx). 

The profile details the requirements for a locator and target devices. The main requirement is that the target has an Immediate Alert Service. 

One thing I found interesting is that the Immediate Alert Service has an alert level that is write only. So the locator can set the alert level but it cannot interrogate the target to see what the alert level is. Interesting choice. I would have thought that being able to confirm the current alert level would be important however I guess this makes the task of complying with the spec easier for the target which is likely to be a very low powered device.
- [Blog post](http://www.cypress.com/blog/100-projects-100-days/project-001-ble-find-me)
- [Github](https://github.com/cypresssemiconductorco/PSoC-4-BLE/tree/master/100_Projects_in_100_Days/Day001_Find_Me)