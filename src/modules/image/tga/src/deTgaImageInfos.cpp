/* 
 * Drag[en]gine Targa Image Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deTgaImageInfos.h"


// class deTgaImageInfo
//////////////////////////

// constructor, destructor
deTgaImageInfo::deTgaImageInfo(){
	SetIsCompressed( false );
}
deTgaImageInfo::~deTgaImageInfo(){ }

// management
int deTgaImageInfo::GetWidth(){
	return header.width;
}
int deTgaImageInfo::GetHeight(){
	return header.height;
}
int deTgaImageInfo::GetDepth(){
	return 1;
}
int deTgaImageInfo::GetComponentCount(){
	return header.bpp >> 3;
}
int deTgaImageInfo::GetBitCount(){
	return 8;
}
