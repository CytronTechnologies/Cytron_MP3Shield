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

#include <MP3Player.h>
#include <LCD4Bit_mod.h> 
#include <TimerOne.h>

LCD4Bit_mod lcd = LCD4Bit_mod(2);   
MP3Player player(3,2); 

char FileName[15] ; 
byte vol=240; 
byte mute=0; 
byte play_pause=1; 

void setup()
{
  
  lcd.init();
  lcd.clear();
  lcd.printIn("  CYTRON TECH.");
  lcd.cursorTo(2, 0);
  lcd.printIn(" MP3 Shield");
  
   //initialize the player: setup STA013 from STA013.cfg file in SD card, print status to through serial terminal
  player.Init(); 
  //set volume  
  player.Volume(vol); 
  
  
  delay(1000);  
  lcd.clear();
  lcd.cursorTo(1, 0);
  

  
  Timer1.initialize(100000); // set timer to 100000 microseconds or 100 miliseconds
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

  
  
}

void loop()
{
  //change volume working directory & current working directory
  //Play songs from a folder named "Songs"
  sd.chdir("Song2", true); 
  sd.vwd()->rewind();
   
   while (myFile.openNext(myFile.cwd(), O_READ))
   {
      myFile.getFilename(FileName); 
     
     //print song name to LCD
      lcd.clear();
      lcd.cursorTo(1, 0);
      lcd.printIn(FileName);     
      
      player.Play(FileName);
      
    }
 
  
}

//right, up, down, left, select
#define RightKey 0
#define UpKey 1
#define DownKey 2
#define LeftKey 3
#define SelectKey 4
int  adc_key_val[5] ={30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;

// Convert ADC value to key number
int get_key(unsigned int input)
{
  int k;

  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {
      return k;
    }
  }

  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed

  return k;
}




void Disp_vol(byte val)
{
  char string[4]; 
  
  string[0]=val/100 +'0'; 
  val%=100; 
  string[1]=val/10 +'0'; 
  val%=10; 
  string[2]= val+'0';   
  string[3]=0;   
  
  lcd.cursorTo(2, 0);
  lcd.printIn("Vol="); 
 
  lcd.printIn(string); 
  
  
}
  
  
/// ------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
  
  adc_key_in = analogRead(0);    // read the value from the sensor  
  key = get_key(adc_key_in);
  
  //Play Next Song when SelectKey pressed
  if(key==SelectKey && oldkey!=SelectKey)  
  {    
    player.PLAY=false;  //skip to next song
  }
  
  //Adjust Volume Up
  else if(key==UpKey)
  {
    
    if(vol<255)
    {
      vol++; 
    }
     player.Volume(vol); 
     Disp_vol(vol);      
    
  }
  //Adjust Volume Down
  else if(key==DownKey)
  {
    if(vol>200)
    {
      vol--; 
    }
     player.Volume(vol); 
     Disp_vol(vol);  
    
  }
  //Mute the Player
  else if(key==RightKey && oldkey!=RightKey)
  {
    mute^=1;
    player.Mute(mute); 
    lcd.cursorTo(2, 10);
    if(mute==1)
    {
      lcd.printIn("Mute"); 
    }
    else
    {
      lcd.printIn("    "); 
    }
    
  }
    //Pause the Player
  else if(key==LeftKey && oldkey!=LeftKey)
  {
    play_pause^=1;
    player.Play_Pause(play_pause); 
    lcd.cursorTo(2, 10);
    if(play_pause==0)
    {
      lcd.printIn("II  "); 
    }
    else
    {
      lcd.printIn("    "); 
    }
    
  }
  
  
  
  
  oldkey=key;
   
}

