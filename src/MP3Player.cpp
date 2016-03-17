//Cytron Techonlogies Sdn. Bhd. 
//MP3 Shield

/*Thanks to the owner of SdFat library
https://code.google.com/p/sdfatlib/*/

#include "MP3Player.h"

SdFat sd;
SdFile myFile;
MP3Player mp3;

boolean MP3Player::Init(byte CS_uSD,byte ASD)
{
	PLAY = false;
	isPlayAll = false;
	name.reserve(80);
	name = "";
	vol = 0;
	counter=0;
	currentDir.reserve(20);
	currentDir = "";
	_isPause = false;
	_isMute = false;
	ls_flag = false;
	
	CS_SD=CS_uSD;
	AMP=ASD;	
	
	pinMode(CS,OUTPUT); //STA013CS
	digitalWrite(CS,LOW);   //deactivate sta013 spi input

	pinMode(DATREQ,INPUT);  //DATREQ

	pinMode(RESET,OUTPUT);  //reset
	digitalWrite(RESET,HIGH); 

	pinMode(AMP,INPUT); //shutdown the amp

	pinMode(CS_SD, OUTPUT);  //SPI uSD CS
	
	InitI2C();
	Reset_STA013(); 
	
		
#if DEBUG
	Serial.begin(9600);
	while (!Serial) ; // wait for serial port to connect. Needed for Leonardo only
#endif
	
	if (!sd.begin(CS_SD, SPI_FULL_SPEED)) 
	{
		#if DEBUG
		sd.initErrorHalt();
		#endif
		return false;
	}

	SPI.begin(); 

	boolean stat=Verify_STA013();
	if (stat==false) 
	{
		#if DEBUG
		Serial.println("STA013 not exist!");
		#endif
		return false;		
	}
	else
	{	
		#if DEBUG
		Serial.println("STA013 verified.."); 
		#endif
		if(!Setup_STA013())
		{
			return false;
		}
		else
		{
			delayMicroseconds(100000);
			setVolume(220); // set volume by default in case user forgets to set it.
			AMPON();
			return true;
		}
	}

}

void MP3Player::setVolume(byte volume)
{
  #define DLA 0x46
  #define DRA 0x48
  byte Vol;
  
  Vol=255-volume;
  
  I2C_Write(DLA, Vol);    //left channel volume
  I2C_Write(DRA, Vol);    //right channel volume
  
  vol = volume;

}
boolean MP3Player::Play(const char* SongName)
{  
  if(!PLAY)
  {
	k=65;
	if(!myFile.isOpen())
	{
		// open the file for read
		if (!myFile.open(SongName, O_READ)) 
		{
			#if DEBUG
			sd.errorHalt("open audio file failed");
			#endif 
			return false;
		}
	}
	else
		name = SongName;

	Run_STA013();
	delayMicroseconds(500000);
	Play_Pause(1);
  
	//AMPON();  

	#if DEBUG
	Serial.println("playing");
	#endif 
   
	filesize=myFile.fileSize();
	Timer1.initialize(30);// 30 us = can check data request and send a byte to STA013 at 1/30u = 33.33kHz
							// able to play a song up to 33.33 x 8 /1024 = 260 kbps
							// recommend play song at 200kbps or lower (such as 128 kbps)for stable performance
	PLAY=true; 
	Timer1.attachInterrupt(Callback);
  }

  return true;   
}

void MP3Player::Pause()
{
	if(PLAY)
		Play_Pause(0);
}

void MP3Player::Resume()
{
	if(PLAY)
		Play_Pause(1);
}

void MP3Player::Mute(void)
{
	_Mute(1);
}

void MP3Player::Unmute(void)
{
	_Mute(0);
}

void MP3Player::Next()
{
	if(isPlayAll){
		PLAY = false;
		delayMicroseconds(100000);
	}

}

void MP3Player::Previous()
{
	if(isPlayAll&&(counter-1)>0)
	{
		Stop();
		PlayTrack(currentDir.c_str(),(counter-1));//temp,currentDir.c_str()
		isPlayAll = true;
	}
}

void MP3Player::Stop()
{
	PLAY = false;
	isPlayAll = false;
	delayMicroseconds(100000);
}

void MP3Player::On()
{
	AMPON();
}

void MP3Player::Off()
{
	AMPSHUTDOWN();
}

void MP3Player::Callback()
{
  if(!mp3.mp3PlayCallback())
    mp3.Halt();
}

void MP3Player::Halt()
{
	Timer1.detachInterrupt();
	PLAY=false; 
	while(!myFile.close())
	{
	   #if DEBUG
	   Serial.println("close audio file");
	   #endif 
	}
	//AMPSHUTDOWN();
	#if DEBUG
    Serial.println("finish");
	#endif 
	delayMicroseconds(100000);
	name = "";
	  if(isPlayAll)
		PlayTrack("",0,"");
}

boolean MP3Player::mp3PlayCallback()
{
   if(filesize==0 &&PLAY==true)
   { 
     return false;
   }
   
   else if(filesize>0 &&PLAY==true)
   {
	if(k>=BUF_SIZE)
    {     //if buffer emptied refill buffer
        //word res=
		myFile.read(AudioBuf, BUF_SIZE);
              //reset counter
        k=0; 
    }
 
    if(STA013_DATREQ())       // if STA013 request data
    {
        
        STA013_CSH();   //enable STA013
        SPI.transfer(AudioBuf[k]); //send data  
        STA013_CSL();   //disable STA013      
        k++;
        filesize--;        //counting down
    }
	
	return true;
	
   }
   else
	 return false;
   
}

void MP3Player::PlayTrack(const char* dirName,int track_no,const char* track_name)
{	
	if(!PLAY)
	{
	 finishSearch = false;
	 int count=0;
	 //if(track_no>0)
		 //counter = track_no;
	
	if(!isPlayAll)
	{
		char temp[20];
		isPlayAll = true;
		sd.chdir();delayMicroseconds(100000);
		sd.chdir(dirName,true);
		sd.vwd()->rewind();
		sd.vwd()->getName(temp,20);
		currentDir = temp;
		counter = 0;
	}

	while(true)
	{
		if(myFile.openNext(myFile.cwd(), O_READ))
		{
			char FileName[80];
			myFile.getName(FileName,80);
			#if DEBUG
				//myFile.printName(&Serial);
				Serial.print(FileName);
				if (myFile.isDir()) 
				{
					// Indicate a directory.
					Serial.write('/');
				}
				Serial.println();
			#endif
			
			if (!(myFile.isDir()||(String(FileName).indexOf(".mp3")==-1&&String(FileName).indexOf(".MP3")==-1))) 
			{
				counter++;
				
				if(ls_flag)
				{
					Serial.print(counter);
					Serial.print(". ");
					Serial.print(FileName);
					Serial.print("     ");
					Serial.print(myFile.fileSize());
					Serial.println(" bytes");
					myFile.close();
					continue;
				}
				
				//Play(FileName);
				if(!String(track_name).equals(""))
				{
					//counter++;
					if(String(track_name).equals(FileName))
					{
						Play(FileName);
						//name = track_name;
						isPlayAll = false;
						break;
					}
				}
				
				else if(track_no==0)
				{
					//counter++;
					Play(FileName);
					break;
				}
				else if(track_no>0)
				{
					count++;
					if(count==track_no)
					{
						counter = track_no;
						Play(FileName);
						isPlayAll = false;
						break;
					}
				}
				

			}
	
		}

		else
		{
			isPlayAll = false;
			finishSearch = true;
			ls_flag = false;
			break;
		}
		//myFile.getFilename(FileName);
		myFile.close();
	}
	}
	
}
void MP3Player::PlayTrack(const char* track_name)
{	
	if(!String(track_name).equals(""))
		PlayTrack("/",-1,track_name);
}


void MP3Player::PlayTrack(int track_no)
{	
	if(track_no>0)
		PlayTrack("/",track_no,"");
}

void MP3Player::PlayTrack(const char* dirName,const char* track_name)
{	
	if(!String(track_name).equals(""))
		PlayTrack(dirName,-1,track_name);
}

void MP3Player::PlayTrack(const char* dirName,int track_no)
{
	if(track_no>0)
		PlayTrack(dirName,track_no,"");
}

void MP3Player::PlayFolder()
{
	PlayTrack("/",0,"");
}

void MP3Player::PlayFolderStartFrom(int start_index)
{
	PlayTrack(start_index);
	if(!finishSearch)
		isPlayAll = true;
}

void MP3Player::PlayFolder(const char* dirName)
{
	PlayTrack(dirName,0,"");
}

void MP3Player::PlayFolderStartFrom(const char* dirName,int start_index)
{
	PlayTrack(dirName,start_index);
	if(!finishSearch)
		isPlayAll = true;
}

void MP3Player::PlayFolderStartFrom(const char* start_track)
{
	PlayTrack(start_track);
	if(!finishSearch)
		isPlayAll = true;
}

void MP3Player::PlayFolderStartFrom(const char* dirName,const char* start_track)
{
	PlayTrack(dirName,start_track);
	if(!finishSearch)
		isPlayAll = true;
}

void MP3Player::lsFiles(const char* dirName)
{
	ls_flag = true;
	PlayFolder(dirName);
}

boolean MP3Player::isPlaying(void)
{
	return PLAY;
}

boolean MP3Player::isPause(void)
{
	return _isPause;
}

boolean MP3Player::isMute(void)
{
	return _isMute;
}

String MP3Player::getName(void)
{
	return name;
}
byte MP3Player::getVol(void)
{
	return vol;
}
String MP3Player::getCurrentDir(void)
{
	return currentDir;
}
int MP3Player::getTrackNo(void)
{
	return counter;
}

boolean MP3Player::Setup_STA013(void)
{ 
  byte buf[2];
  
  if (!myFile.open("sta013.cfg", O_READ)) 
  {
    #if DEBUG
	sd.errorHalt("cfg file error");
	#endif 
	return false;
  }
  
  #if DEBUG
  Serial.println("setting STA013 from cfgfile");  
  #endif 
  
  
  while (myFile.available()) 
  {
    buf[0]=byte (myFile.read());
    buf[1]=byte (myFile.read()); 
    I2C_Write(buf[0], buf[1]);
    // Serial.write(buf[0]);
    // Serial.write(buf[1]);
  }  
  
  while(!myFile.close())
  {
	  #if DEBUG
	  Serial.println("close cfg file..");
	  #endif
  }
  
    
  #if DEBUG
  Serial.println("Setup STA013 Register Done..");     
  #endif 
  
  return true;
}


void MP3Player::Run_STA013(void)
{      //leave idle and start decode
    delayMicroseconds(500000);
    I2C_Write(114, 1);      //start STA013
}

// input 1,0 for play or stop play
void MP3Player::Play_Pause(byte play)
{

    //delay(500);
    I2C_Write(19, play);       //play
	_isPause = (play==0?true:false);
}
void MP3Player::_Mute(byte mute)
{
	
	I2C_Write(0x14, mute);       //play
	_isMute = (mute==0?false:true);
	
}

void MP3Player::Reset_STA013(void)
{
    STA013_RESET(LOW);
    delayMicroseconds(500000);
    STA013_RESET(HIGH);
    delayMicroseconds(500000);
}

//check the presence of STA013
boolean MP3Player::Verify_STA013(void)
{
    byte data;
    data=I2C_Read(0x01);

    if( data==0xAC)
        return true;
    else
        return false;
}

void MP3Player::InitI2C(void)
{
  pinMode(A4,INPUT); //SDA
  pinMode(A5,INPUT); //SCL
  
}


void MP3Player::i2c_start(void)
{
  SDA_H();             // i2c start bit sequence
  i2c_dly();
  SCL_H();
  i2c_dly();
  SDA_L(); 
  i2c_dly();
  SCL_L(); 
  i2c_dly();
}

void MP3Player::i2c_stop(void)
{
  SDA_L();             // i2c stop bit sequence
  i2c_dly();
  SCL_H(); 
  i2c_dly();
  SDA_H(); 
  i2c_dly();
}

byte MP3Player::i2c_rx(byte ack)
{
  byte x, d=0;
  SDA_H(); 
  for(x=0; x<8; x++) 
  {
    d <<= 1;
    do {
      SCL_H();
    }
    while(SCL_IN()==0);    // wait for any SCL clock stretching
    i2c_dly();
    if(SDA_IN()) d |= 1;
    SCL_L(); 
  }
  if(ack) 
  {
    SDA_L(); 
  }
  else 
  {
    SDA_H(); 
  }
  SCL_H()
  i2c_dly();             // send (N)ACK bit
  SCL_L(); 
  SDA_H(); 
  return d;
}


byte MP3Player::i2c_tx(byte d)
{
  byte x;
  byte b;
  for(x=8; x; x--)
  {
    if(d&0x80)
    { 
      SDA_H(); 
    }
    else 
    {
      SDA_L();
    }
    SCL_H(); 
    d <<= 1;
    SCL_L(); 
  }
  SDA_H();
  SCL_H(); 
  i2c_dly();
  b = SDA_IN();          // possible ACK bit
  SCL_L(); 
  return b;
}

void MP3Player::I2C_Write(byte Address, byte data)
{

    i2c_start();
    if(i2c_tx(0x86)) while(1);
    if(i2c_tx(Address)) while(1);
    if(i2c_tx(data)) while(1);
    i2c_stop();

}

byte MP3Player::I2C_Read(byte Address)
{
    byte data=0;

    i2c_start();
    if(i2c_tx(0x86)) while(1);
    if(i2c_tx(Address)) while(1);
    i2c_start();
    if(i2c_tx(0x87)) while(1);
    data=i2c_rx(0);
    i2c_stop();

    return(data); 
}









