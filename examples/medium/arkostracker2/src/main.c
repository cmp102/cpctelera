//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2018 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#include <cpctelera.h>
#include "grenade.h"
#include "emptySong.h"
#include "sfx.h"


//Message structure to draw text easily on the screen
struct message_t{
	const char* str;
	u8 x;
	u8 y;
};

//Print helper to draw the messages
//We must indicate the foreground and de background colors
void printHelper(struct message_t* m, u8 fg, u8 bg){
	u8* pvmem = cpct_getScreenPtr(CPCT_VMEM_START, m->x, m->y);
	cpct_setDrawCharM1(fg, bg);
	cpct_drawStringM1(m->str, pvmem);
}


///////////////////////////////////////////////////////////
//
// All messages declarations
//
//////////////////////////////////////////////////////////
const struct message_t messageNowPlaying = {
	"Now Playing: ",
	2,
	0
};
const struct message_t messageNextMusic = {
	"[N] Next",
	2,
	12
};
const struct message_t messagePause = {
	"[P] Pause",
	2,
	22
};
const struct message_t messageToggleChannels = {
	"TOGGLE SFX CHANNELS:",
	2,
	68
};
const struct message_t messagesChannels[3] = {
	{
		"Ch0[Q]",
		43,
		58
	},
	{
		"Ch1[W]",
		43,
		68
	},
	{
		"Ch2[E]",
		43,
		78
	}
};
const struct message_t messageSFX = {
	"PLAY SFX: [1] [2] [3] [4] [5]",
	2,
	128
};
const struct message_t messageVolume = {
	"SFX VOLUME -[I] +[O]:",
	2,
	150
};


// Song structure to manage multiple songs lists with different names
struct song_t {
	struct message_t name;
	void* music;
};


///////////////////////////////////////////////////////////////
//
// Song list declaration (Harmless Grenade and an empty song)
//
//////////////////////////////////////////////////////////////
#define MAX_SONGS 2
const struct song_t songs[MAX_SONGS] = {
	{
		{
			"A Harmless Grenade",
			28,
			0
		},
		&grenade
	},
	{
		{
			"Empty             ", // Multiple spaces at de end to clear previous song text
			28,
			0
		},
		&emptySong
	}
};



// Structure and functions to guarantee no trigger an event foreach instant the key is pressed
// only the first instant where is pressed and no more until is released
struct keyLocker_t{
	u8 holding;
};

// Hold a key, if that key was pressed on the previuos instant returns False
// If it's first time sets it as holding and returns true
u8 kl_hold(struct keyLocker_t* kl){
	if(kl->holding){
		return 0;
	}
	kl->holding = 1;
	return 1;
}

// Releases the key
void kl_release(struct keyLocker_t* t){
	t->holding = 0;
}

// Checks a key and executes a handler only on the instant when the key was pressed
// by first time
void checkKey(struct keyLocker_t* kl, u16 key, void(*handler)(i8), i8 param){
	if(cpct_isKeyPressed(key)){
		if(kl_hold(kl)){
			handler(param);
		}
	}else{
		kl_release(kl);
	}

}


u8 actualSong = 0;           // Which song is playing
u8 musicPaused = 0;          // If music is paused
i8 volume = 0;               // The inverted volume of sfx 0(Maximum) 16(minimum)
u8 channels[3] = {0,0,0};    // Which channels are enabled


//Draws the volume var
void drawVolume(){
	u8 x = 45;
	u8 y = 150;
	u8 v = 16 - volume;   //Inverts the volume 16(Maximum) 0(Minimum)
	
	// Clears the area where the volume will be drawed
	u8* pvmem = cpct_getScreenPtr(CPCT_VMEM_START, x, y);
	cpct_drawSolidBox (pvmem, 0x00, 2*16, 8);
	
	// Draws a square for each volume point
	for(u8 i = 0; i<v; ++i){
		u8* pvmem = cpct_getScreenPtr(CPCT_VMEM_START, x, y);
		cpct_drawSolidBox (pvmem, 0xff, 1, 8);
		x+=2;
	}
}


// Adds points to the sfx volume
void volumeAdd(i8 n){
	volume += n;

	// Clamps volume between 0 and 16
	if(volume > 16){
		volume = 16;
	}else if(volume < 0){
		volume = 0;
	}

	//Redraws volume var
	drawVolume();
}



// Plays a sfx on the selected channels
void playSfx(i8 sfxID){
	for(u8 i = 0; i<3; ++i){
		if(channels[i]){
			/////////////////////////////////
			//
			// Function to play the sfx
			//
			// 1st param: id of the sfx to play (From 1 to the last sfx)
			// 2nd param: channel where play the sfx (From 0 to 2)
			// 3rd param: inverted volume of the sfx (0 Max, 16 Min)
			//
			////////////////////////////////
			cpct_akp2PlaySFX(sfxID, i, volume);
		}
	}
}



// Stops or plays the music
// Note: recives a dummy parameter to use this funcition as handler when we call checkKey
void toggleMusic(i8 param){
	if(musicPaused){
		printHelper(&messagePause, 1, 0); // Prints pause message with inverted fg and bg
		musicPaused = 0;
	}else{
		printHelper(&messagePause, 0, 1); // Prints pause message with normal fg and bg
		/////////////////////////////////
		//
		// Function to stop the music
		//
		////////////////////////////////
		cpct_akp2Stop();
		musicPaused = 1;
	}
}

// Plays the next music on the array
// Note: recives a dummy parameter to use this funcition as handler when we call checkKey
void nextMusic(i8 param){
	actualSong+=1;

	//If exceds max songs starts from song 0
	if(actualSong >= MAX_SONGS){
		actualSong = 0;
	}

	// Prints the song name
	printHelper(&songs[actualSong].name, 1, 0);

	/////////////////////////////////
	//
	// Function to sets the song to play
	//
	// 1st param: Pointer to the song
	//
	////////////////////////////////
	cpct_akp2Init(songs[actualSong].music);
}


// Enables or disables the indicated channel for sfx
void toggleSFXChannel(i8 ch){
	if(channels[ch]){
		channels[ch] =  0;
		printHelper(&messagesChannels[ch], 1, 0);
	}else{
		channels[ch] =  1;
		printHelper(&messagesChannels[ch], 0, 1);
	}
}



// De key lockers declarations
struct keyLocker_t klMusic;
struct keyLocker_t klSFX1;
struct keyLocker_t klSFX2;
struct keyLocker_t klSFX3;
struct keyLocker_t klSFX4;
struct keyLocker_t klSFX5;
struct keyLocker_t klChannel0;
struct keyLocker_t klChannel1;
struct keyLocker_t klChannel2;
struct keyLocker_t klVolumeUp;
struct keyLocker_t klVolumeDown;
struct keyLocker_t klNextMusic;


// Checks all keyboard events
void checkKeyboard(){
	cpct_scanKeyboard();
	checkKey(&klMusic, Key_P, toggleMusic, 0);
	checkKey(&klSFX1, Key_1, playSfx, 1);
	checkKey(&klSFX2, Key_2, playSfx, 2);
	checkKey(&klSFX3, Key_3, playSfx, 3);
	checkKey(&klSFX4, Key_4, playSfx, 4);
	checkKey(&klSFX5, Key_5, playSfx, 5);
	checkKey(&klChannel0, Key_Q, toggleSFXChannel, 0);
	checkKey(&klChannel1, Key_W, toggleSFXChannel, 1);
	checkKey(&klChannel2, Key_E, toggleSFXChannel, 2);
	checkKey(&klVolumeDown, Key_I, volumeAdd, 1);
	checkKey(&klVolumeUp, Key_O, volumeAdd, -1);
	checkKey(&klNextMusic, Key_N, nextMusic, 0);
}




void main(void) {
	cpct_disableFirmware();
	

	// Draws all screen text
	printHelper(&messagePause, 1, 0);
	printHelper(&messageNextMusic, 1, 0);
	printHelper(&messageToggleChannels, 1, 0);
	printHelper(&messagesChannels[0], 1, 0);
	printHelper(&messagesChannels[1], 1, 0);
	printHelper(&messagesChannels[2], 1, 0);
	printHelper(&messageSFX, 1, 0);
	printHelper(&messageVolume, 1, 0);
	printHelper(&messageNowPlaying, 1, 0);
	printHelper(&songs[actualSong].name, 1, 0);

	// Draws the volume var
	drawVolume();
	

	/////////////////////////////////
	//
	// Function to sets the song to play
	//
	// 1st param: Pointer to the song
	//
	////////////////////////////////
	cpct_akp2Init(songs[actualSong].music);
	
	/////////////////////////////////
	//
	// Function to sets the sfx data to play
	//
	// 1st param: Pointer to the sfx
	//
	////////////////////////////////
	cpct_akp2InitSFX(&sfx);


	while(1){
	   checkKeyboard();

	   //Halts and vsync to guarantee 50hz
	   cpct_waitHalts(2);
	   cpct_waitVSYNC();
	   if(!musicPaused){

			/////////////////////////////////
			//
			// Function to play a music frame
			//
			////////////////////////////////
			cpct_akp2Play();
	   }
   }

}

