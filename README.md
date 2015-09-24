# Cytron_MP3Shield
<strong>Arduino Library and example code for Cytron MP3 shield</strong>

The product is located at <a href="http://www.cytron.com.my/p-shield-mp3" target="_blank">here</a>.

The User's Manual can be referred  <a href="https://docs.google.com/document/d/1tB7A36YtpR3CYcPzxAEA511pYK5z5L1ypjZsPXZ3ctE/view" target="_blank">here</a>.
Please go through User's Manual to understand how to utilize the MP3 shield.

The setting is done in sta013.cfg file (binary file) which is stored in microSD card or configfile.c which can be stored in flash/eeprom of microcontroller if the controller has large enough program memory. Both these files are located under folders <strong>extras</strong> -> <strong>Configuration file</strong>. 

The example sketch for Arduino MP3 shield is under folders <strong>examples</strong>. Install the library, restart Arduino IDE and open under <strong>File</strong> -> <strong>Examples</strong>.

<strong>About Library:</strong><br/>
Library for Cytron MP3 shield has been revised to allow the music to be played at the background while the main program is running.

<strong>Limitations:</strong><br/>
- Tolerate with MP3 files up to 260kbps only. MP3 files with 200kbps or lower are preferable.<br/>
- TimerOne interrupt cannot be used for other purposes while playing MP3 files.<br/>
- Time critical programs in main loop might be affected if the bitrate of MP3 files are high.<br/>

<strong>Other libraries needed:</strong><br/>
1. Latest SdFat Library (can be downloaded from folder <strong>extras</strong>) <br/>
2. SPI<br/>
3. TimerOne<br/>

<strong>How to install:</strong><br/>
Download the library as .zip file and extract it to Arduino libraries directory. Copy latest version of SdFat library from <strong>extras</strong> folder and paste it to the Arduino libraries directory as well. 

For more info, please refer to readme.txt in <strong>extras</strong> folder.<br/>
If you have any inquiry, please post in our <a href="http://forum.cytron.com.my/" target="_blank">technical forum</a>.
