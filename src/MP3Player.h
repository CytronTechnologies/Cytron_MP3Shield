//Cytron Techonlogies Sdn. Bhd. 
//MP3 Shield

/*Thanks to the owner of SdFat library
https://code.google.com/p/sdfatlib/*/

#ifndef MP3Player_h
#define MP3Player_h

//#include "Arduino.h"
#include <SPI.h>
#include <SdFat.h>
#include "TimerOne.h"

#define DEBUG 0

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

#define BUF_SIZE 64

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
	
	boolean Init(byte CS_uSD,byte ASD);
	void setVolume(byte volume); 
	void Pause(void);
	void Resume(void);
	void Mute(void);
	void Unmute(void);
	void Next(void);
	void Previous(void);	
	void Stop(void);
	void On(void);
	void Off(void);
	void PlayTrack(const char* dirName,const char* track_name);
	void PlayTrack(const char* dirName,int track_no);
	void PlayTrack(const char* track_name);
	void PlayTrack(int track_no);
	void PlayFolder();
	void PlayFolderStartFrom(int start_index);
	void PlayFolder(const char* dirName);
	void PlayFolderStartFrom(const char* dirName,int start_index);
	void PlayFolderStartFrom(const char* start_track);
	void PlayFolderStartFrom(const char* dirName,const char* start_track);
	void lsFiles(const char* dirName);
	boolean isPlaying(void);
	boolean isPause(void);
	boolean isMute(void);
	String getName(void);
	byte getVol(void);
	String getCurrentDir(void);
	int getTrackNo(void);

	private:
	boolean PLAY;
	boolean _isMute;
	boolean _isPause;
	String name;
	String currentDir;
	byte vol;
	int counter;
	//char FileName[80];
	byte CS_SD;
	byte AMP;
	boolean isPlayAll;
	byte AudioBuf[64];
	long filesize;
	word k;
	boolean finishSearch;
	boolean ls_flag;
	
	boolean Setup_STA013(void);
	void Run_STA013(void); 
	void Reset_STA013(void);
	boolean Verify_STA013(void);
	boolean mp3PlayCallback();
	static void Callback();	
	boolean Play(const char* SongName);
	void Halt();
	void PlayTrack(const char* dirName,int track_no,const char* track_name);
	void Play_Pause(byte play); 
	void _Mute(byte mute); 
	
	void InitI2C(void);
	void i2c_start(void);
	void i2c_stop(void);
	byte i2c_rx(byte ack); 
	byte i2c_tx(byte d); 
	void I2C_Write(byte Address, byte data); 
	byte I2C_Read(byte Address); 

	
}; 

extern MP3Player mp3;

#endif
