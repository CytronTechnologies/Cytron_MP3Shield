//Cytron Technologies Sdn Bhd.
//MP3 Shield 

//Thanks to the owner of SdFat library
//https://code.google.com/p/sdfatlib/

#include <SdFat.h>
#include <MP3Player.h>

String inputString = ""; // a string to hold incoming data
String Folder ="";
int vol = 220;

void setup()
{
  //initialize the player: setup STA013 from STA013.cfg file in SD card
   
  Serial.begin(9600);
  while(!Serial); // for arduino leonardo only
  
  if(!mp3.Init(3,2))
  {
    Serial.println("Init failed");
    while(1);
  }
    
  mp3.setVolume(vol);
 
  Serial.println("Please type the name of the folder to play:");
  while(Serial.available()<=0); //wait for input
  while (Serial.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    Folder+= inChar;
    delay(10);
  }

  Serial.println("List MP3 files in folder "+Folder+" :");
  
  mp3.lsFiles(Folder.c_str());  //list files in serial monitor

  Serial.println("");
  Serial.println("Send key 'a' to volume down");
  Serial.println("Send key 'd' to volume up");
  Serial.println("Send key 'w' to pause or resume");
  Serial.println("Send key 's' to mute or unmute");
  Serial.println("Send a number to play song positioned at that number");
  Serial.println("ENJOY!!! :D");
}
  
void loop()
{
  serialEvent(); //call the function
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
      mp3.PlayTrack(Folder.c_str(),inputString.toInt()); // play a track from a folder
                                                         // inputString  = track no.
    }
    else
    {
      if(inputString.equals("a"))
        mp3.setVolume(--vol);  // volume down
      else if(inputString.equals("d"))
        mp3.setVolume(++vol); // volume up
      else if(inputString.equals("w"))
        mp3.isPause()==true?mp3.Resume():mp3.Pause(); // pause/resume
      else if(inputString.equals("s"))
        mp3.isMute()==true?mp3.Unmute():mp3.Mute(); // mute/unmute
    }
  }

}




