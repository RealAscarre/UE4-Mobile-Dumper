# UE4-Mobile-Dumper
About Dump offsets and functions for android games made using Unreal Engine

# What it does exactly?
1. It regenerates and dumps all strings from the games using the Global Names pointer (GNames/FNames).
2. It dumps all objects present in the game using the Global Names (GNames/FNames) & Global Object(GUObjectArray).
3. It dumps the offsets and function address using Global Names (GNames/FNames) and Global Object(GUObjectArray) or using Global World (UWorld/GWorld).
4. It also dumps the Actors present as entity in the game to check the integrity of the Global World pointer.

# What to do with the outputs?
Its upto you how you want to use the dump files. For general knowledge, the output files are used as reference to reverse engineer the game to access non-accessable options/ unfair advantages.

# How to build this source?
This was made using Android Studio and can be easily rebuild. 
1. Change the offsets required, most games use the offsets provided for dumping and you just need to update the Global pointers but in case it doesn't dump correctly consider making somee changes to the pre initialized offsets in 'Offsets.h' file
2. Build.
3. Import the Lib file to correct arch folder and put dex file as the incremental dex file compared to the other ones.
4. Open the dex file and go to the dumper main activity (In this case its 'DeleteActivity.smali') and copy the smali code to start the next activity.
5. Paste the line you copied from step 4 to the mainactivity of the game.
6. Bypass the Sign verification of the game (if there is any)
7. Sign the whole mod apk.
8. Copy the user permission from mod to game manifest file (Add read write to storage permission, some games doesnt use them but the dumper needs it).
9. Install and go to the app settings and allow overlay permission and storage permission. 
10. Open game.

# Does it work with AIDE?
Currently NO, it doesn't. I will update in future but till then just make a new project in AIDE and copy all files then build.

# Failed to get Base Address of games?
1. Check for the game package name and library used for main data storage.
2. If game changes packagename then use your brain and change the get packagename func manually.

# What games does it work on?
It will work on any game built using Unreal Engine no matter the version used. 
This source or dumper is not intended to be any game specific, its for educational purposes, so people can learn.

# Want to dump a PC Game?
GO here for PC Dumper - https://github.com/RealAscarre/UE4-Dumper
