Revised Library for Cytron MP3 Shield

Library for Cytron MP3 shield has been revised to allow the music to be played at the background while you can do 
other tasks in main loop. 

Library and example codes can be downloaded from http://cytron.com.my.

Limitations:
1. Tolerate with MP3 files up to 260kbps only. Higher the bitrate the MP3 file, the more frequent the program enters the
timerone interrupt and may cause the the program in main loop unstable. MP3 files with 200kbps or lower are preferable.
2. TimerOne interrupt cannot be used for other purposes while playing MP3 files.
3. Time critical programs in main loop might be affected if the bitrate of MP3 files are high(close to or exceeds 260kbps).

How to determine MP3 bitrate?

bitrate(kbps) = filesize(in kbit)/total time of MP3(in s)

For example: xxx.song with 5MB and total playtime 4 min

bitrate = 5 x 1024 x 8 / (4 x60) = 171 kbps

Other libraries needed:
1. SdFat
2. SPI
3. TimerOne

Functionalities:

1. mp3.Init(SS,ASD) - depends on your jumpers on MP3 shield board eg.SS - D3, ASD - D2 -> mp3.Init(3,2);
		    - returns true if successful
2. mp3.setVolume(byte volume) - set volume of file played ( range from 0 - 255) 0 - lowest, 255 - highest
	                      - default value is 220 after mp3.Init is successful
3. mp3.Pause() - pause the song played
4. mp3.Resume()- resume the song played
5. mp3.Mute() - mute the song
6. mp3.Unmute() - unmute the song
7. mp3.Next() - play next song (only available when playing all mp3 songs in a folder)
8. mp3.Previous() - play previous song (only available when playing all mp3 songs in a folder)
9. mp3.Stop() - stop any playing songs
10. mp3.On() - turn on amplifier (amplifier is turned on by default after mp3.Init is successful)
11. mp3.Off() - turn off amplifier
12. mp3.PlayTrack(const char* dirName,const char* track_name)
    - play song with track_name from directory called dirName
    e.g: play a song named xxx.mp3 stored in Favourites/Part I Folder -> mp3.PlayTrack("Favourites/Part I","xxx.mp3");
13. mp3.PlayTrack(const char* track_name)
    - play song with track_name from root directory
14. mp3.PlayTrack(const char* dirName,int track_no)
    - play song positioned at track_no from dirName directory
    e.g: play 5th song stored in Favourites Folder -> mp3.PlayTrack("Favourites",5);
15. mp3.PlayTrack(int track_no)
    - play song positioned at track_no from root directory
16. mp3.PlayFolder()
    - play all songs in root directory
17. mp3.PlayFolder(const char*dirName)
    - play all songs in dirName directory
18. mp3.PlayFolderStartFrom(int start_index)
    - play all songs in root directory starting from song with position start_index
19. mp3.PlayFolderStartFrom(const char* dirName, int start_index)
    - play all songs in dirName directory starting from song with position start_index
    e.g. play 5th song and onwards from xxx folder -> mp3.PlayFolderStartFrom("xxx",5);
20. mp3.PlayFolderStartFrom(const char* start_track)
    - play all songs from root directory starting with song named start_track
21. mp3.PlayFolderStartFrom(const char* dirName,const char* start_track)
    - play all songs from dirName directory starting with song named start_track
    e.g. play song named aaa.mp3 and onwards from bbb folder -> mp3.PlayTrackStartFrom("bbb","aaa.mp3");
22. mp3.lsFiles(const char*dirName)
    - list all mp3 files in dirName directory (requires Serial function)
23. mp3.isPlaying()
    - returns true if a song is played
24. mp3.isPause()
    - returns true if a song is paused
25. mp3.isMute()
    - returns true is a song is muted
26. mp3.getName()
    - returns a string of name of current song played
27. mp3.getVol()
    - returns byte of current volume
28. mp3.getCurrentDir()
    - returns a string of name of current directory where the song is played
29. mp3.getTrackNo()
    - returns the track number(integer) of the current song played

Note:
- This library supports mp3 filenames up to 80 characters and directory names up to 20 characters. 
**Don't make long names for directories and mp3 files, shorter the better :)
- Function 12- 15 are for playing single song
- Function 16- 21 are for playing songs continuously
- Function mp3.Next() and mp3.Previous() are only available if either one of functions 16 -21 are called
- mp3.lsFiles requires serial function. Initiate Serial function such as Serial.begin(9600) before begin
this function
