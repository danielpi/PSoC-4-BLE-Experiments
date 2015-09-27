# THIS FILE IS AUTOMATICALLY GENERATED
# Project: Y:\repos\PSoC-4-BLE-Experiments\Project 003 Custom Profile CapSense RGB LED\PSoC\CapSenseRGB\CapSenseRGB.cydsn\CapSenseRGB.cyprj
# Date: Sun, 27 Sep 2015 07:19:14 GMT
#set_units -time ns
create_clock -name {Slider_SampleClk(FFB)} -period 5312.5 -waveform {0 2656.25} [list [get_pins {ClockBlock/ff_div_5}]]
create_clock -name {Slider_SenseClk(FFB)} -period 5312.5 -waveform {0 2656.25} [list [get_pins {ClockBlock/ff_div_4}]]
create_clock -name {CyRouted1} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyWCO} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/wco}]]
create_clock -name {CyLFCLK} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyECO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/eco}]]
create_clock -name {CyIMO} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFCLK} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySYSCLK} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {Slider_SampleClk} -source [get_pins {ClockBlock/hfclk}] -edges {1 255 511} [list]
create_generated_clock -name {Slider_SenseClk} -source [get_pins {ClockBlock/hfclk}] -edges {1 255 511} [list]
create_generated_clock -name {PRS_CLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 49 97} [list [get_pins {ClockBlock/udb_div_0}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for Y:\repos\PSoC-4-BLE-Experiments\Project 003 Custom Profile CapSense RGB LED\PSoC\CapSenseRGB\CapSenseRGB.cydsn\TopDesign\TopDesign.cysch
# Project: Y:\repos\PSoC-4-BLE-Experiments\Project 003 Custom Profile CapSense RGB LED\PSoC\CapSenseRGB\CapSenseRGB.cydsn\CapSenseRGB.cyprj
# Date: Sun, 27 Sep 2015 07:19:11 GMT
