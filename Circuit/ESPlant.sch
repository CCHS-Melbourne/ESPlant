EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:freetronics_schematic
LIBS:basic_esp_board-cache
LIBS:espkit-07-cache
LIBS:ESPlant
LIBS:ESPlant-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L GND #PWR?
U 1 1 56375AAB
P 750 700
F 0 "#PWR?" H 750 450 50  0001 C CNN
F 1 "GND" H 750 550 50  0000 C CNN
F 2 "" H 750 700 60  0000 C CNN
F 3 "" H 750 700 60  0000 C CNN
	1    750  700 
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 56375AC1
P 1450 700
F 0 "#PWR?" H 1450 550 50  0001 C CNN
F 1 "+3.3V" H 1450 840 50  0000 C CNN
F 2 "" H 1450 700 60  0000 C CNN
F 3 "" H 1450 700 60  0000 C CNN
	1    1450 700 
	0    1    1    0   
$EndComp
$Comp
L LED D?
U 1 1 56375ADB
P 950 700
F 0 "D?" H 950 800 50  0000 C CNN
F 1 "PWR_LED" H 950 600 50  0000 C CNN
F 2 "" H 950 700 60  0000 C CNN
F 3 "" H 950 700 60  0000 C CNN
	1    950  700 
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 56375B12
P 1300 700
F 0 "R?" V 1380 700 50  0000 C CNN
F 1 "1K" V 1300 700 50  0000 C CNN
F 2 "" V 1230 700 30  0000 C CNN
F 3 "" H 1300 700 30  0000 C CNN
	1    1300 700 
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 56375DAB
P 2000 700
F 0 "#PWR?" H 2000 450 50  0001 C CNN
F 1 "GND" H 2000 550 50  0000 C CNN
F 2 "" H 2000 700 60  0000 C CNN
F 3 "" H 2000 700 60  0000 C CNN
	1    2000 700 
	0    1    1    0   
$EndComp
$Comp
L SW_PUSH SW?
U 1 1 56375E10
P 2300 700
F 0 "SW?" H 2450 810 50  0000 C CNN
F 1 "PRGM" H 2300 620 50  0000 C CNN
F 2 "" H 2300 700 60  0000 C CNN
F 3 "" H 2300 700 60  0000 C CNN
	1    2300 700 
	1    0    0    -1  
$EndComp
Text GLabel 2600 700  2    60   Input ~ 0
GPIO0
$Comp
L +3.3V #PWR?
U 1 1 56375ED2
P 750 1050
F 0 "#PWR?" H 750 900 50  0001 C CNN
F 1 "+3.3V" H 750 1190 50  0000 C CNN
F 2 "" H 750 1050 60  0000 C CNN
F 3 "" H 750 1050 60  0000 C CNN
	1    750  1050
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 56375EE9
P 750 1300
F 0 "#PWR?" H 750 1050 50  0001 C CNN
F 1 "GND" H 750 1150 50  0000 C CNN
F 2 "" H 750 1300 60  0000 C CNN
F 3 "" H 750 1300 60  0000 C CNN
	1    750  1300
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 56375F00
P 900 1050
F 0 "R?" V 980 1050 50  0000 C CNN
F 1 "10K" V 900 1050 50  0000 C CNN
F 2 "" V 830 1050 30  0000 C CNN
F 3 "" H 900 1050 30  0000 C CNN
	1    900  1050
	0    1    1    0   
$EndComp
$Comp
L SW_PUSH SW?
U 1 1 56375F35
P 1050 1300
F 0 "SW?" H 1200 1410 50  0000 C CNN
F 1 "RESET" H 1050 1220 50  0000 C CNN
F 2 "" H 1050 1300 60  0000 C CNN
F 3 "" H 1050 1300 60  0000 C CNN
	1    1050 1300
	1    0    0    -1  
$EndComp
Text GLabel 1350 1300 2    60   Input ~ 0
RESET
Wire Wire Line
	1050 1050 1350 1050
Wire Wire Line
	1350 1050 1350 1300
Connection ~ 1350 1300
$Comp
L GND #PWR?
U 1 1 56375FE4
P 2000 1000
F 0 "#PWR?" H 2000 750 50  0001 C CNN
F 1 "GND" H 2000 850 50  0000 C CNN
F 2 "" H 2000 1000 60  0000 C CNN
F 3 "" H 2000 1000 60  0000 C CNN
	1    2000 1000
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 56375FFD
P 2150 1000
F 0 "C?" H 2175 1100 50  0000 L CNN
F 1 "1uf" H 2175 900 50  0000 L CNN
F 2 "" H 2188 850 30  0000 C CNN
F 3 "" H 2150 1000 60  0000 C CNN
	1    2150 1000
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 563760A3
P 2150 1200
F 0 "C?" H 2175 1300 50  0000 L CNN
F 1 "0.1uf" H 2175 1100 50  0000 L CNN
F 2 "" H 2188 1050 30  0000 C CNN
F 3 "" H 2150 1200 60  0000 C CNN
	1    2150 1200
	0    1    1    0   
$EndComp
Wire Wire Line
	2000 1200 2000 1000
Wire Wire Line
	2300 1200 2300 1000
Connection ~ 2000 1000
Connection ~ 2300 1000
$Comp
L +3.3V #PWR?
U 1 1 56376117
P 2300 1000
F 0 "#PWR?" H 2300 850 50  0001 C CNN
F 1 "+3.3V" H 2300 1140 50  0000 C CNN
F 2 "" H 2300 1000 60  0000 C CNN
F 3 "" H 2300 1000 60  0000 C CNN
	1    2300 1000
	0    1    1    0   
$EndComp
$Comp
L ESP-07 P?
U 1 1 563762A8
P 1700 2050
F 0 "P?" H 1700 2500 50  0000 C CNN
F 1 "ESP-07" V 1700 2250 40  0000 C CNN
F 2 "" H 1700 2250 60  0000 C CNN
F 3 "" H 1700 2250 60  0000 C CNN
	1    1700 2050
	1    0    0    -1  
$EndComp
Text GLabel 1300 1700 0    60   Input ~ 0
RESET
Text GLabel 2450 2100 2    60   Input ~ 0
GPIO0
Text GLabel 950  1800 0    60   Input ~ 0
ADC
$Comp
L +3.3V #PWR?
U 1 1 563767DA
P 1300 1900
F 0 "#PWR?" H 1300 1750 50  0001 C CNN
F 1 "+3.3V" V 1300 2150 50  0000 C CNN
F 2 "" H 1300 1900 60  0000 C CNN
F 3 "" H 1300 1900 60  0000 C CNN
	1    1300 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	950  1800 1300 1800
$Comp
L +3.3V #PWR?
U 1 1 5637689F
P 1300 2400
F 0 "#PWR?" H 1300 2250 50  0001 C CNN
F 1 "+3.3V" V 1300 2650 50  0000 C CNN
F 2 "" H 1300 2400 60  0000 C CNN
F 3 "" H 1300 2400 60  0000 C CNN
	1    1300 2400
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5637692A
P 2100 2400
F 0 "#PWR?" H 2100 2150 50  0001 C CNN
F 1 "GND" V 2100 2200 50  0000 C CNN
F 2 "" H 2100 2400 60  0000 C CNN
F 3 "" H 2100 2400 60  0000 C CNN
	1    2100 2400
	0    -1   -1   0   
$EndComp
Text GLabel 2300 1700 2    60   Input ~ 0
TX
Text GLabel 2100 2200 2    60   Input ~ 0
GPIO2
Text GLabel 2450 2300 2    60   Input ~ 0
GPIO15
Wire Wire Line
	2450 2300 2100 2300
Wire Wire Line
	2450 2100 2100 2100
Text GLabel 2100 2000 2    60   Input ~ 0
GPIO5
Text GLabel 2450 1900 2    60   Input ~ 0
GPIO4
Wire Wire Line
	2100 1900 2450 1900
Text GLabel 2100 1800 2    60   Input ~ 0
RX
Wire Wire Line
	2100 1700 2300 1700
Text GLabel 1300 2300 0    60   Input ~ 0
GPIO13
Text GLabel 900  2200 0    60   Input ~ 0
GPIO12
Text GLabel 1300 2100 0    60   Input ~ 0
GPIO14
Text GLabel 900  2000 0    60   Input ~ 0
GPIO16
Wire Wire Line
	900  2000 1300 2000
Wire Wire Line
	1300 2200 900  2200
$Comp
L +3.3V #PWR?
U 1 1 563771C9
P 3800 750
F 0 "#PWR?" H 3800 600 50  0001 C CNN
F 1 "+3.3V" H 3800 890 50  0000 C CNN
F 2 "" H 3800 750 60  0000 C CNN
F 3 "" H 3800 750 60  0000 C CNN
	1    3800 750 
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 563771EF
P 3200 750
F 0 "#PWR?" H 3200 500 50  0001 C CNN
F 1 "GND" H 3200 600 50  0000 C CNN
F 2 "" H 3200 750 60  0000 C CNN
F 3 "" H 3200 750 60  0000 C CNN
	1    3200 750 
	0    1    1    0   
$EndComp
$Comp
L BATTERY BAT?
U 1 1 5637756B
P 3500 750
F 0 "BAT?" H 3500 950 50  0000 C CNN
F 1 "BATTERY" H 3500 560 50  0000 C CNN
F 2 "" H 3500 750 60  0000 C CNN
F 3 "" H 3500 750 60  0000 C CNN
	1    3500 750 
	-1   0    0    1   
$EndComp
$Comp
L USB_5PIN_SHELL P?
U 1 1 56377743
P 3700 1550
F 0 "P?" H 3625 1800 60  0000 C CNN
F 1 "USB_5PIN_SHELL" H 3550 1200 60  0001 L CNN
F 2 "FT:USB-MICRO_TH_4PEGS" H 3550 1150 31  0001 L CNN
F 3 "" H 3700 1550 60  0000 C CNN
	1    3700 1550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 56377AB6
P 3900 1800
F 0 "#PWR?" H 3900 1550 50  0001 C CNN
F 1 "GND" H 3900 1650 50  0000 C CNN
F 2 "" H 3900 1800 60  0000 C CNN
F 3 "" H 3900 1800 60  0000 C CNN
	1    3900 1800
	0    -1   -1   0   
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 56377CE7
P 800 2750
F 0 "#PWR?" H 800 2600 50  0001 C CNN
F 1 "+3.3V" H 800 2890 50  0000 C CNN
F 2 "" H 800 2750 60  0000 C CNN
F 3 "" H 800 2750 60  0000 C CNN
	1    800  2750
	0    -1   -1   0   
$EndComp
$Comp
L LED D?
U 1 1 56377FAE
P 1000 2750
F 0 "D?" H 1000 2850 50  0000 C CNN
F 1 "D13_LED" H 1000 2650 50  0000 C CNN
F 2 "" H 1000 2750 60  0000 C CNN
F 3 "" H 1000 2750 60  0000 C CNN
	1    1000 2750
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 563781EC
P 1350 2750
F 0 "R?" V 1430 2750 50  0000 C CNN
F 1 "1K" V 1350 2750 50  0000 C CNN
F 2 "" V 1280 2750 30  0000 C CNN
F 3 "" H 1350 2750 30  0000 C CNN
	1    1350 2750
	0    1    1    0   
$EndComp
$Comp
L 2N7002 Q?
U 1 1 563783DC
P 1700 2850
F 0 "Q?" V 1630 2980 50  0000 R CNN
F 1 "2N7002" V 1950 3000 50  0000 R CNN
F 2 "" H 1900 2950 29  0000 C CNN
F 3 "" H 1700 2850 60  0000 C CNN
	1    1700 2850
	0    -1   -1   0   
$EndComp
Text GLabel 1700 3050 3    60   Input ~ 0
GPIO13
$Comp
L GND #PWR?
U 1 1 56378615
P 1900 2750
F 0 "#PWR?" H 1900 2500 50  0001 C CNN
F 1 "GND" V 1900 2550 50  0000 C CNN
F 2 "" H 1900 2750 60  0000 C CNN
F 3 "" H 1900 2750 60  0000 C CNN
	1    1900 2750
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR?
U 1 1 56378962
P 3900 1400
F 0 "#PWR?" H 3900 1250 50  0001 C CNN
F 1 "+5V" H 3900 1540 50  0000 C CNN
F 2 "" H 3900 1400 60  0000 C CNN
F 3 "" H 3900 1400 60  0000 C CNN
	1    3900 1400
	0    1    1    0   
$EndComp
NoConn ~ 3900 1700
$EndSCHEMATC
