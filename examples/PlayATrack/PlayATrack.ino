//Cytron Technologies Sdn Bhd.
//MP3 Shield 

//Thanks to the owner of SdFat library
//https://code.google.com/p/sdfatlib/

#include <SdFat.h>
#include <MP3Player.h>

String inputString = ""; // a string to hold incoming data
const char* Folder ="Favourites"; //folder to play
const char* default_song = "Taio Cruz - Hangover ft. Flo Rida.mp3";//default song
int max_no = 26; //total number of tracks in Folder
int vol = 220;

void setup()
{
  //initialize the player: setup STA013 from STA013.cfg file in SD card
  pinMode(A0,INPUT);
  randomSeed(analogRead(A0));
  
  Serial.begin(9600);
  while(!Serial); // for arduino leonardo only
  
  if(!mp3.Init(3,2))
  {
    Serial.println("Init failed");
    while(1);
  }
    
  mp3.setVolume(vol);
  
  mp3.PlayTrack(Folder,default_song);
  Serial.println("Currently playing...");
  Serial.print("Track "+String(mp3.getTrackNo()));
  Serial.println(": "+mp3.getName());
}
  
void loop()
{
  serialEvent(); //call the function
  //if default song finishes, play a random track from a folder
  if(!mp3.isPlaying())
  {
    mp3.PlayTrack(Folder,random(1,max_no));
    Serial.println("Currently playing...");
    Serial.print("Track "+String(mp3.getTrackNo()));
    Serial.println(": "+mp3.getName());
  }
}

void serialEvent() {

  boolean isNum = true;
  inputString ="";
  
  if(Serial.available())
  {
    while (Serial.available()) 
    {
      char inChar = (char)Serial.read();
      if (!isDigit(inChar)) isNum = false;
      inputString += inChar;
      delay(10);
    }

    if(isNum)
    {
      if(mp3.isPlaying()) mp3.Stop();
      mp3.PlayTrack(Folder,inputString.toInt());
      Serial.println("Currently playing...");
      Serial.print("Track "+String(mp3.getTrackNo()));
      Serial.println(": "+mp3.getName());
    }
    else
    {
      if(inputString.equals("a"))
        mp3.setVolume(--vol);  
      else if(inputString.equals("d"))
        mp3.setVolume(++vol); 
      else if(inputString.equals("s"))
        mp3.Stop();
    }
  }

}
