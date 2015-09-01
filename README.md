# Cytron_MP3Shield
Library and example code for Cytron MP3 shield

The product is located at: http://www.cytron.com.my/p-shield-mp3

The User's Manual: https://docs.google.com/document/d/1tB7A36YtpR3CYcPzxAEA511pYK5z5L1ypjZsPXZ3ctE/view
Please go through User's Manual to understand how to utilize the MP3.

The setting is done in STA013.cfg file (binary file) which is stored in microSD card or configfile.c which can be stored in flash/eeprom of microcontroller if the controller has large enough program memory. Both these files are located under "Configuration File" folder. 

The example sketch for Arduino MP3 shield is under MP3Player-> Example. Please move MP3Player and SdFat folders to Arduino->libraries, restart Arduino IDE and open under example.

About Library:

Library for Cytron MP3 shield has been revised to allow the music to be played at the background while the main program is running.

Limitations:<br/>
- Tolerate with MP3 files up to 260kbps only. MP3 files with 200kbps or lower are preferable.<br/>
- TimerOne interrupt cannot be used for other purposes while playing MP3 files.<br/>
- Time critical programs in main loop might be affected if the bitrate of MP3 files are high.<br/>

Other libraries needed:<br/>
1. SdFat<br/>
2. SPI<br/>
3. TimerOne<br/>

For more info, please refer to readme.txt in "extras" folder.<br/>
If you have any inquiry, please post in our <a href="http://forum.cytron.com.my/" target="_blank">technical forum</a>.
