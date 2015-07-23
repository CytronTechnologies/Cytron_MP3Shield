//Cytron Technologies Sdn Bhd.
//www.cytron.com.my
//MP3 Shield 

/*Thanks to the owner of SdFat library
https://code.google.com/p/sdfatlib/*/

/* Reminder: Set the ASD Pin Selector S1 on board  to D2 when using LCD KEYPAD shield*/

//This Code needs: 
//1. Arduino Main board
//2. MP3 Shield
//3. LCD keypad Shield

#include <SdFat.h>
#include <MP3Player.h>
#include <LiquidCrystal.h> 

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

byte vol = 220;  
String currentDir;
String track_no;
int i = 0;
int track = 0; 

//right, up, down, left, select
#define RightKey 0
#define UpKey 1
#define DownKey 2
#define LeftKey 3
#define SelectKey 4
#define NUM_KEYS 5

unsigned int adc_key_val[5] ={100, 550, 700, 800, 900 };
byte key=-1;
byte oldkey=-1;
long timeout = 0;
long timeForDisp = 0;
boolean disp_flag = true;

// Convert ADC value to key number
byte get_key(unsigned int input)
{
  for (byte k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {
      return k;
    }
  }
  
  return -1;
}

void Disp_vol(byte val)
{
  lcd.clear();lcd.home();
  lcd.print("Vol: "); 
  lcd.print(val); 
}


void setup()
{
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("CYTRON TECH.");
  lcd.setCursor(0, 1);
  lcd.print("MP3 Shield");
  
   //initialize the player: setup STA013 from STA013.cfg file in SD card, print status to through serial terminal
  if(!mp3.Init(3,2))
  {
    lcd.clear();
    lcd.print("Init failed");
    while(1);
  }  
  //set volume  
  mp3.setVolume(vol); 

  delay(1000);  
  lcd.clear();lcd.home();

  //Play songs from a folder named "Favourites"
  mp3.PlayFolder("Favourites");
  timeout = millis();
}

void loop()
{
  //To do if no key are pressed
  if(!mp3.isPlaying())// if mp3 stops playing
  {
    lcd.clear();lcd.setCursor(0,0);
    lcd.print("Stop Playing at");
    lcd.setCursor(0,1);
    lcd.print("Track "+String(mp3.getTrackNo()));
    mp3.Off();
  }
  
  else if(mp3.isPlaying())
  {  
     if(track!=mp3.getTrackNo())
     {
      currentDir ="";
      track_no ="";
      
      track = mp3.getTrackNo();
      String temp = mp3.getCurrentDir();
      currentDir = temp;
      for(int j=0;j<(16-temp.length());j++)
       currentDir+=" ";
      temp ="";
      temp = String(track);
      track_no = temp;
      for(int j=0;j<(10-temp.length());j++)
       track_no+=" ";
      i = 0;
      
     }

     if(disp_flag)
     {
       if((i%15)<5){
        lcd.setCursor(0,0);lcd.print("Now Playing...  ");
       }
       else if((i%15)>=5&&(i%15)<10){
        lcd.setCursor(0,0);lcd.print(currentDir);
       }
       else{
        lcd.setCursor(0,0);lcd.print("Track "+track_no);
       }
       scrollToLeft(i,1,mp3.getName());
     }
  }
    
  key = get_key(analogRead(A0));
  
  //Play Next Song when SelectKey pressed
  if(key==SelectKey&&oldkey!=SelectKey)  
    mp3.Next();// skip to next song
  
  //Adjust Volume Up
  else if(key==UpKey)
  {
    disp_flag = false;
    timeForDisp = millis();
    if(vol<255)
      vol++; 
      
     mp3.setVolume(vol); 
     Disp_vol(vol); 
     delay(200);      
  }
  
  //Adjust Volume Down
  else if(key==DownKey)
  {
     disp_flag = false;
     timeForDisp = millis();
     
     if(vol>0)
      vol--; 
      
     mp3.setVolume(vol); 
     Disp_vol(vol);
     delay(200);  
  }
  
  //Mute the Player
  else if(key==RightKey&&oldkey!=RightKey)
  {
    mp3.isMute()==true?mp3.Unmute():mp3.Mute();
    disp_flag = false;
    timeForDisp = millis();
    lcd.clear();lcd.home();
    mp3.isMute()==true?lcd.print("Mute"):lcd.print("Unmute"); 
    delay(200);
  }
  
  //Pause the Player
  else if(key==LeftKey&&oldkey!=LeftKey)
  {
    mp3.isPause()==false?mp3.Pause():mp3.Resume();
    disp_flag = false;
    timeForDisp = millis();
    lcd.clear();lcd.home();
    mp3.isPause()==true?lcd.print("Pause"):lcd.print("Resume");
    delay(200);
  }

  oldkey = key;
  
  if(millis()-timeout>1000)
  {
    i++;timeout = millis();
  }

  if(millis()-timeForDisp>2000)
    disp_flag = true;
  
}

void scrollToLeft(int index,int line,String dat)
{
  lcd.setCursor(0,line);
  dat = dat + "            ";
  index = index%dat.length();
  if(index+16>dat.length())
  {
    lcd.print(dat.substring(index,dat.length())
                  +dat.substring(0,index+16-dat.length()));
    }
  else
    lcd.print(dat.substring(index,index+16));
}

