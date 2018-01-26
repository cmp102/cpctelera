//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2017 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
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
//-------------------------------------------------------------------------------

//#####################################################################
//### MODULE: Sprites
//### SUBMODULE: flipping
//#####################################################################
//### This module contains routines to flip sprites or draw them 
//### directly flipped to screen vide memory
//#####################################################################
//

#ifndef CPCT_SPRITEFLIPPING_H
#define CPCT_SPRITEFLIPPING_H

// Draw Sprite Flipped Functions
extern void cpct_drawSpriteVFlip (void *sprite, void* memory, u8 width, u8 height) __z88dk_callee;

// Sprite flipping functions
extern void cpct_hflipSpriteM0   (u8 width, u8 height, void* sprite) __z88dk_callee;
extern void cpct_hflipSpriteM1   (u8 width, u8 height, void* sprite) __z88dk_callee;
extern void cpct_hflipSpriteM2   (u8 width, u8 height, void* sprite) __z88dk_callee;

// Sprite flipping functions (ROM-friendly versions)
extern void cpct_hflipSpriteM0_r (void* sprite, u8 width, u8 height) __z88dk_callee;
extern void cpct_hflipSpriteM1_r (void* sprite, u8 width, u8 height) __z88dk_callee;
extern void cpct_hflipSpriteM2_r (void* sprite, u8 width, u8 height) __z88dk_callee;

// Masked Sprite flipping functions
extern void cpct_hflipSpriteMaskedM0(u8 width, u8 height, void* sprite) __z88dk_callee;
extern void cpct_hflipSpriteMaskedM1(u8 width, u8 height, void* sprite) __z88dk_callee;
extern void cpct_hflipSpriteMaskedM2(u8 width, u8 height, void* sprite) __z88dk_callee;

#endif