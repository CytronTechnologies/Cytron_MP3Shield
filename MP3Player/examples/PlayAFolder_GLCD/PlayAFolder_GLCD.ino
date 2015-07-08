//Cytron Technologies Sdn Bhd.
//www.cytron.com.my
//MP3 Shield 

/*Thanks to SdFat library
https://code.google.com/p/sdfatlib/*/


//This Code needs: 
//1. Arduino Main board
//2. MP3 Shield
//3. Graphic LCD Shield (SHIELD-GLCD)

#include <MP3Player.h>
#include <LCD4884.h>
#include <TimerOne.h>


MP3Player player(10,8); 
byte vol=240;
char FileName[15] ; 
byte mute=0; 
byte play_pause=1; 


void setup()
{
  
  lcd.LCD_init();
  lcd.LCD_clear();  
  lcd.backlight(ON);//Turn on the backlight
  
  //lcd.LCD_draw_bmp_pixel(0,0, Cytron, 84,24);

  lcd.LCD_write_string(0, 1, "CYTRON.com.my", MENU_NORMAL );
  lcd.LCD_write_string(10, 3, "MP3 Shield", MENU_NORMAL );
  
  
  delay(1000); 
  
  //initialize the player: setup STA013 from STA013.cfg file in SD card, print status to through serial terminal
  player.Init(); 
  //set volume  
  player.Volume(vol); 
   
  Timer1.initialize(100000); // set timer to 100000 microseconds or 100 miliseconds
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
  
  lcd.LCD_clear(); 

  
  
  
  
    
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
      lcd.LCD_write_string(10, 3,"            ", MENU_HIGHLIGHT );  
      lcd.LCD_write_string(10, 3, FileName, MENU_HIGHLIGHT );  
      
      player.Play(FileName);
      
    }
 
  
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
  
  lcd.LCD_write_string(40, 0, "voL=", MENU_NORMAL ); 
 
  lcd.LCD_write_string(65, 0, string, MENU_NORMAL ); 
  
  
}
  

// joystick number
#define LEFT_KEY 0
#define CENTER_KEY 1
#define DOWN_KEY 2
#define RIGHT_KEY 3
#define UP_KEY 4

int  adc_key_val[5] ={50, 200, 400, 600, 800 };
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

/// ------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
  
  adc_key_in = analogRead(0);    // read the value from the sensor  
  key = get_key(adc_key_in);
  
  //Play Next Song when SelectKey pressed
  if(key==CENTER_KEY && oldkey!=CENTER_KEY)  
  {    
    player.PLAY=false;  //skip to next song
  }
  
  //Adjust Volume Up
  else if(key==UP_KEY)
  {
    
    if(vol<255)
    {
      vol++; 
    }
     player.Volume(vol); 
     Disp_vol(vol);      
    
  }
  //Adjust Volume Down
  else if(key==DOWN_KEY)
  {
    if(vol>200)
    {
      vol--; 
    }
     player.Volume(vol); 
     Disp_vol(vol);  
    
  }
  //Mute the Player
  else if(key==RIGHT_KEY && oldkey!=RIGHT_KEY)
  {
    mute^=1;
    player.Mute(mute);  
    if(mute==1)
    {
      lcd.LCD_write_string(0, 0, "MuTe ", MENU_NORMAL );  
    }
    else
    {
        if(play_pause==0)
        {
          lcd.LCD_write_string(0, 0, "PaUse", MENU_NORMAL ); 
        }
        else
        {
          lcd.LCD_write_string(0, 0, "PLAy ", MENU_NORMAL ); 
        }
     }
    
  }
    //Pause the Player
  else if(key==LEFT_KEY && oldkey!=LEFT_KEY)
  {
    play_pause^=1;
    player.Play_Pause(play_pause); 
    if(play_pause==0)
    {
      lcd.LCD_write_string(0, 0, "PaUse", MENU_NORMAL ); 
    }
    else
    {
      lcd.LCD_write_string(0, 0, "PLAy ", MENU_NORMAL ); 
    }
    
  }
  
  
  
  
  oldkey=key;
   
}
