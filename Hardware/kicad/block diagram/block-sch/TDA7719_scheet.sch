EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 9
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 6550 3650 2    50   Input ~ 0
AUDIO_IN_RB
Text GLabel 6550 4000 2    50   Input ~ 0
AUDIO_IN_LB
Text GLabel 6550 3900 2    50   Input ~ 0
AUDIO_IN_LF
Text GLabel 6550 3550 2    50   Input ~ 0
AUDIO_IN_RF
Text GLabel 5550 2550 0    50   Input ~ 0
I2C_SCL
Text GLabel 5550 2650 0    50   Input ~ 0
I2C_SDA
Wire Wire Line
	5550 2650 5600 2650
Wire Wire Line
	5600 2650 5600 2750
Wire Wire Line
	5550 2550 5700 2550
Wire Wire Line
	5700 2550 5700 2750
$Comp
L Device:C C74
U 1 1 61E76921
P 6400 2450
F 0 "C74" H 6450 2350 50  0000 L CNN
F 1 "100nF" H 6450 2550 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 6438 2300 50  0001 C CNN
F 3 "~" H 6400 2450 50  0001 C CNN
	1    6400 2450
	-1   0    0    1   
$EndComp
$Comp
L Device:CP C75
U 1 1 61E77788
P 6700 2450
F 0 "C75" H 6900 2550 50  0000 R CNN
F 1 "4,7uF" H 6950 2350 50  0000 R CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 6738 2300 50  0001 C CNN
F 3 "~" H 6700 2450 50  0001 C CNN
	1    6700 2450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C65
U 1 1 62161F5F
P 5000 3000
F 0 "C65" V 4950 2800 50  0000 L CNN
F 1 "100nF" V 5050 2700 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5038 2850 50  0001 C CNN
F 3 "~" H 5000 3000 50  0001 C CNN
	1    5000 3000
	0    1    1    0   
$EndComp
$Comp
L Device:C C59
U 1 1 62161F60
P 4450 3150
F 0 "C59" V 4400 3200 50  0000 L CNN
F 1 "100nF" V 4500 3200 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 4488 3000 50  0001 C CNN
F 3 "~" H 4450 3150 50  0001 C CNN
	1    4450 3150
	0    1    1    0   
$EndComp
$Comp
L Device:C C66
U 1 1 62161F61
P 5000 3300
F 0 "C66" V 4950 3100 50  0000 L CNN
F 1 "220nF" V 5050 3000 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5038 3150 50  0001 C CNN
F 3 "~" H 5000 3300 50  0001 C CNN
	1    5000 3300
	0    1    1    0   
$EndComp
$Comp
L Device:C C67
U 1 1 62161F62
P 5000 3600
F 0 "C67" V 4950 3400 50  0000 L CNN
F 1 "100nF" V 5050 3300 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5038 3450 50  0001 C CNN
F 3 "~" H 5000 3600 50  0001 C CNN
	1    5000 3600
	0    1    1    0   
$EndComp
$Comp
L Device:C C61
U 1 1 62161F63
P 4450 3750
F 0 "C61" V 4400 3800 50  0000 L CNN
F 1 "100nF" V 4500 3800 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 4488 3600 50  0001 C CNN
F 3 "~" H 4450 3750 50  0001 C CNN
	1    4450 3750
	0    1    1    0   
$EndComp
$Comp
L Device:C C71
U 1 1 62161F64
P 5000 4800
F 0 "C71" V 4950 4600 50  0000 L CNN
F 1 "100nF" V 5050 4500 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5038 4650 50  0001 C CNN
F 3 "~" H 5000 4800 50  0001 C CNN
	1    5000 4800
	0    1    1    0   
$EndComp
$Comp
L Device:C C60
U 1 1 62161F65
P 4450 3450
F 0 "C60" V 4400 3500 50  0000 L CNN
F 1 "220nF" V 4500 3500 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 4488 3300 50  0001 C CNN
F 3 "~" H 4450 3450 50  0001 C CNN
	1    4450 3450
	0    1    1    0   
$EndComp
$Comp
L Device:C C70
U 1 1 61F07C71
P 5000 4500
F 0 "C70" V 4950 4300 50  0000 L CNN
F 1 "100nF" V 5050 4200 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5038 4350 50  0001 C CNN
F 3 "~" H 5000 4500 50  0001 C CNN
	1    5000 4500
	0    1    1    0   
$EndComp
$Comp
L Device:C C69
U 1 1 62161F67
P 5000 4200
F 0 "C69" V 4950 4000 50  0000 L CNN
F 1 "100nF" V 5050 3900 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5038 4050 50  0001 C CNN
F 3 "~" H 5000 4200 50  0001 C CNN
	1    5000 4200
	0    1    1    0   
$EndComp
$Comp
L Device:C C68
U 1 1 62161F68
P 5000 3900
F 0 "C68" V 4950 3700 50  0000 L CNN
F 1 "100nF" V 5050 3600 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5038 3750 50  0001 C CNN
F 3 "~" H 5000 3900 50  0001 C CNN
	1    5000 3900
	0    1    1    0   
$EndComp
$Comp
L Device:C C62
U 1 1 62161F69
P 4450 4050
F 0 "C62" V 4400 4100 50  0000 L CNN
F 1 "220nF" V 4500 4100 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 4488 3900 50  0001 C CNN
F 3 "~" H 4450 4050 50  0001 C CNN
	1    4450 4050
	0    1    1    0   
$EndComp
$Comp
L Device:C C63
U 1 1 61F0BD3E
P 4450 4350
F 0 "C63" V 4400 4400 50  0000 L CNN
F 1 "100nF" V 4500 4400 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 4488 4200 50  0001 C CNN
F 3 "~" H 4450 4350 50  0001 C CNN
	1    4450 4350
	0    1    1    0   
$EndComp
$Comp
L Device:C C64
U 1 1 61F0BFF5
P 4450 4650
F 0 "C64" V 4400 4700 50  0000 L CNN
F 1 "100nF" V 4500 4700 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 4488 4500 50  0001 C CNN
F 3 "~" H 4450 4650 50  0001 C CNN
	1    4450 4650
	0    1    1    0   
$EndComp
Text GLabel 4300 3000 0    50   Input ~ 0
Bluetooth_IN_L
Text GLabel 4300 3150 0    50   Input ~ 0
Bluetooth_IN_R
Text GLabel 4300 3300 0    50   Input ~ 0
Bluetooth_IN_G
Wire Wire Line
	4300 3300 4850 3300
Wire Wire Line
	4850 3000 4300 3000
$Comp
L Device:CP C73
U 1 1 61F6A11C
P 6250 5350
F 0 "C73" H 6500 5450 50  0000 R CNN
F 1 "10uF" H 6550 5350 50  0000 R CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 6288 5200 50  0001 C CNN
F 3 "~" H 6250 5350 50  0001 C CNN
	1    6250 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C72
U 1 1 62161F6D
P 5750 5350
F 0 "C72" H 5600 5450 50  0000 R CNN
F 1 "4,7uF" H 5600 5350 50  0000 R CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 5788 5200 50  0001 C CNN
F 3 "~" H 5750 5350 50  0001 C CNN
	1    5750 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 5050 5750 5200
Wire Wire Line
	5850 5050 5850 5150
Wire Wire Line
	5850 5150 6250 5150
Wire Wire Line
	6250 5150 6250 5200
Wire Wire Line
	5750 5500 6000 5500
$Comp
L power:+12V #PWR010
U 1 1 61FE31FC
P 6400 2300
F 0 "#PWR010" H 6400 2150 50  0001 C CNN
F 1 "+12V" H 6415 2473 50  0000 C CNN
F 2 "" H 6400 2300 50  0001 C CNN
F 3 "" H 6400 2300 50  0001 C CNN
	1    6400 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR011
U 1 1 61FE44C9
P 6700 2600
F 0 "#PWR011" H 6700 2350 50  0001 C CNN
F 1 "GNDA" V 6705 2472 50  0000 R CNN
F 2 "" H 6700 2600 50  0001 C CNN
F 3 "" H 6700 2600 50  0001 C CNN
	1    6700 2600
	1    0    0    -1  
$EndComp
Connection ~ 6700 2600
Wire Wire Line
	6300 2600 6300 2750
Wire Wire Line
	6300 2600 6400 2600
Wire Wire Line
	6100 2750 6100 2300
Connection ~ 6400 2600
Connection ~ 6400 2300
Wire Wire Line
	6400 2600 6700 2600
Wire Wire Line
	6400 2300 6700 2300
Wire Wire Line
	6100 2300 6400 2300
$Comp
L power:GNDA #PWR09
U 1 1 62097793
P 6000 5500
F 0 "#PWR09" H 6000 5250 50  0001 C CNN
F 1 "GNDA" V 6005 5372 50  0000 R CNN
F 2 "" H 6000 5500 50  0001 C CNN
F 3 "" H 6000 5500 50  0001 C CNN
	1    6000 5500
	1    0    0    -1  
$EndComp
Connection ~ 6000 5500
Wire Wire Line
	6000 5500 6250 5500
Text GLabel 4300 3450 0    50   Input ~ 0
Jack_1_IN_G
Text GLabel 4300 3600 0    50   Input ~ 0
Jack_1_IN_L
Text GLabel 4300 3750 0    50   Input ~ 0
Jack_1_IN_R
Text GLabel 4300 3900 0    50   Input ~ 0
Jack_2_IN_L
Text GLabel 4300 4050 0    50   Input ~ 0
Jack_2_IN_G
Text GLabel 4300 4200 0    50   Input ~ 0
Jack_2_IN_R
Text GLabel 4300 4350 0    50   Input ~ 0
Radio_L
Text GLabel 4300 4500 0    50   Input ~ 0
Radio_R
Text GLabel 4300 4650 0    50   Input ~ 0
Microphone_L
Text GLabel 4300 4800 0    50   Input ~ 0
Microphone_R
Wire Wire Line
	4300 3600 4850 3600
Wire Wire Line
	4300 3900 4850 3900
Wire Wire Line
	4300 4200 4850 4200
Wire Wire Line
	4850 4500 4300 4500
Wire Wire Line
	4850 4800 4300 4800
Text Notes 3150 2250 0    197  ~ 39
Preamp
Wire Wire Line
	4600 3150 5150 3150
Wire Wire Line
	4600 3450 5150 3450
Wire Wire Line
	4600 4650 5150 4650
Wire Wire Line
	4600 4350 5150 4350
Wire Wire Line
	4600 4050 5150 4050
Wire Wire Line
	4600 3750 5150 3750
$Comp
L block-sch-rescue:Ultra_kombajn_audio-cache_TDA7719-Ultra~kombajn~audio-cache U5
U 1 1 621334AD
P 5650 3600
AR Path="/621334AD" Ref="U5"  Part="1" 
AR Path="/61C9D382/621334AD" Ref="U5"  Part="1" 
F 0 "U5" H 5300 4400 50  0000 C CNN
F 1 "TDA7719" H 6800 2200 50  0000 C CNN
F 2 "lib-amplituner:TDA7719" H 7150 3600 50  0001 C CNN
F 3 "" H 7150 3600 50  0001 C CNN
	1    5650 3600
	1    0    0    -1  
$EndComp
$EndSCHEMATC
