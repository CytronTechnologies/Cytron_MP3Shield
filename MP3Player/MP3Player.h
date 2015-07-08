//Cytron Techonlogies Sdn. Bhd. 
//MP3 Shield

/*Thanks to the owner of SdFat library
https://code.google.com/p/sdfatlib/*/

#ifndef MP3Player_h
#define MP3Player_h

#include "Arduino.h"
#include "SdFat.h"
#include "SPI.h"


//Arduino Leonardo
#if defined (__AVR_ATmega32U4__)
	#define Serial Serial1
#else 
	#define Serial Serial
#endif


//definitions


#define SDA A4
#define SCL A5
#define DATREQ A2
#define CS A1
#define RESET A3

#define AMPSHUTDOWN()  pinMode(AMP,INPUT); 
#define AMPON()      pinMode(AMP,OUTPUT); digitalWrite(AMP,LOW); 
#define STA013_DATREQ() digitalRead(DATREQ)
#define STA013_CSH() digitalWrite(CS,HIGH); 
#define STA013_CSL() digitalWrite(CS,LOW); 
#define STA013_RESET(val) digitalWrite(RESET,val);

#define BUF_SIZE 512


#define SDA_H() pinMode(SDA,INPUT);
#define SDA_L() pinMode(SDA,OUTPUT); digitalWrite(SDA,LOW);
#define SCL_H() pinMode(SCL,INPUT); 
#define SCL_L() pinMode(SCL,OUTPUT); digitalWrite(SCL,LOW); 
#define SDA_IN() digitalRead(SDA) 
#define SCL_IN() digitalRead(SCL)
#define i2c_dly() delayMicroseconds(10); 

extern SdFat sd;
extern SdFile myFile;

class MP3Player
{
	public:	
	byte CS_SD;
	byte AMP;
	boolean PLAY; 
	
	
	MP3Player(byte CS_uSD,byte ASD) ; 
	void Init();
	void Volume(byte volume);
	void Play(const char* SongName); 
	void Mute(byte mute); 
	void Play_Pause(byte play); 
	
	
	
	
	private:
	void Setup_STA013(void);
	void Run_STA013(void); 
	
	void Reset_STA013(void);
	byte Verify_STA013(void);
	
	void InitI2C(void);
	void i2c_start(void);
	void i2c_stop(void);
	byte i2c_rx(byte ack); 
	byte i2c_tx(byte d); 
	void I2C_Write(byte Address, byte data); 
	byte I2C_Read(byte Address); 
	
}; 



#endif