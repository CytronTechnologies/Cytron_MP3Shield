//Cytron Technologies Sdn Bhd.
//MP3 Shield 

/*Thanks to the owner of SdFat library
https://code.google.com/p/sdfatlib/*/

#include <MP3Player.h>

MP3Player playerone(3, 2); 

void setup()
{
  //initialize the player: setup STA013 from STA013.cfg file in SD card, print status to through serial terminal
  playerone.Init(); 
  //set volume  
  playerone.Volume(240); 
  
  
}

void loop()
{
  //play a song
  playerone.Play("ok.mp3"); 
  
}

