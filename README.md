# Audio amplituner

Audio amplituner with display, bass, middle, treble control. It has:

 - 3.2" OLED display with SSD1322 controler with 4bit gray scale. (check displays folder to see possible display states)
 - possibility to wake up to your favourite radio station at specified time.
 - two independent alarms which can work in five diffrent modes (everyday, one time, mon-fri, weekends, exact date)
 - six encoders with buttons used to control audio (volume front, volume back, treble, middle, bass, loudness)
 - four user buttons used to change settings, displays etc.
 - five audio inputs - bluetooth 5.0, 2x jack, radio, microphone
 - radio receiver with displaying RDS information 
 - possibility to save two favourite stations and then tune to them by pressing certain button
 - displaying current day of the week, day of the month, year, hour, minunte, second and possibility to change them
 - momentary button used as main power button with led controlled by PWM signal
 - displaying FFT of selected output signal
 - saving user settings in external EEPROM memory
 - using ultra low power stm32l4q5 


Soon to be done:

 - wireless communication with ESP8266 to control parametrs via smartphone, browser
 - playing audio files from USB disc using CS43L21 driver