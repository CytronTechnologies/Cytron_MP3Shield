//Cytron Techonlogies Sdn. Bhd. 
//MP3 Shield

/*Thanks to the owner of SdFat library
https://code.google.com/p/sdfatlib/*/

#include "Arduino.h"
#include "MP3Player.h"


byte AudioBuf[512];
long filesize;
SdFat sd;
SdFile myFile;

MP3Player::MP3Player(byte CS_uSD,byte ASD) 	//, char ctrl)
{   
	CS_SD=CS_uSD;
	AMP=ASD;	
  
}
void MP3Player::Init()
{

	pinMode(CS,OUTPUT); //STA013CS
	digitalWrite(CS,LOW);   //deactivate sta013 spi input

	pinMode(DATREQ,INPUT);  //DATREQ

	pinMode(RESET,OUTPUT);  //reset
	digitalWrite(RESET,HIGH); 

	pinMode(AMP,INPUT); //shutdown the amp

	pinMode(CS_SD, OUTPUT);  //SPI uSD CS
	
	InitI2C();
	Reset_STA013(); 

	if (!sd.begin(CS_SD, SPI_FULL_SPEED)) sd.initErrorHalt();

	SPI.begin(); 

	Serial.begin(9600);
	while (!Serial) ; // wait for serial port to connect. Needed for Leonardo only
	
	
	
	byte stat=Verify_STA013();
	if (stat==0) 
	{
		Serial.println("STA013 not exist!"); 
	}
	else
	{
		Serial.println("STA013 verified.."); 
		Setup_STA013();  		
		delay(100);
	}

}

void MP3Player::Volume(byte volume)
{
  #define DLA 0x46
  #define DRA 0x48
  byte vol;
  
  vol=255-volume;
  
  I2C_Write(DLA, vol);    //left channel volume
  I2C_Write(DRA, vol);    //right channel volume

}
void MP3Player::Play(const char* SongName)
{  
  byte dat;
  word k=513;
  
  PLAY=true; 
  if(!myFile.isOpen	())
  {
   // open the file for read
	if (!myFile.open(SongName, O_READ)) 
	{
	sd.errorHalt("open audio file failed");
	}
  }
  
   
  Run_STA013();   
  Play_Pause(1); 
  
  AMPON(); 
  Serial.println("playing");
   
   
   filesize=myFile.fileSize();
   
   while(filesize>0 &&PLAY==true)
   { 
     
     if(k>=BUF_SIZE)
     {     //if buffer emptied refill buffer
        word res=myFile.read(AudioBuf, BUF_SIZE);
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
   }
   PLAY=false; 
   
   while(!myFile.close())
   {Serial.println("close audio file");}
   
   AMPSHUTDOWN();  
}

void MP3Player::Setup_STA013(void)
{ 
  byte buf[2];

  if (!myFile.open("sta013.cfg", O_READ)) 
  {
    sd.errorHalt("cfg file error");
  }
  Serial.println("setting STA013 from cfgfile");  
     
  while (myFile.available()) 
  {
    buf[0]=byte (myFile.read());
    buf[1]=byte (myFile.read()); 
    I2C_Write(buf[0], buf[1]);
    // Serial.write(buf[0]);
    // Serial.write(buf[1]);
  }  

  while(!myFile.close())
  {Serial.println("close cfg file..");}

  Serial.println("Setup STA013 Register Done..");     
      
}


void MP3Player::Run_STA013(void)
{      //leave idle and start decode
    delay(500);
    I2C_Write(114, 1);      //start STA013
}

// input 1,0 for play or stop play
void MP3Player::Play_Pause(byte play)
{

    delay(500);
    I2C_Write(19, play);       //play
}
void MP3Player::Mute(byte mute)
{
	
	I2C_Write(0x14, mute);       //play
}

void MP3Player::Reset_STA013(void)
{
    STA013_RESET(LOW);
    delay(500);
    STA013_RESET(HIGH);
    delay(500); 
}

//check the presence of STA013
byte MP3Player::Verify_STA013(void)
{
    byte data;
    data=I2C_Read(0x01);

    if( data==0xAC)
        return(1);
    else
        return (0);
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









