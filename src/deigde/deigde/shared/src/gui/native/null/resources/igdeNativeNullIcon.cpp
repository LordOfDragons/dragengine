/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifdef IGDE_TOOLKIT_NULL

#include "igdeNativeNullIcon.h"


// Class igdeNativeNullIcon
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeNullIcon::igdeNativeNullIcon(){
}

igdeNativeNullIcon::~igdeNativeNullIcon(){
}

void *igdeNativeNullIcon::CreateNativeIcon( deImage& ){
	return new igdeNativeNullIcon;
}

void *igdeNativeNullIcon::CreateNativeIconPNG( decBaseFileReader& ){
	return new igdeNativeNullIcon;
}

void *igdeNativeNullIcon::DuplicateNativeIcon( void* ){
	return new igdeNativeNullIcon;
}

void igdeNativeNullIcon::DestroyNativeIcon( void *native ){
	delete ( igdeNativeNullIcon* )native;
}



// Management
///////////////

decPoint igdeNativeNullIcon::GetSize( void* ){
	return decPoint( 32, 32 );
}

void igdeNativeNullIcon::Scale( const decPoint &, void* ){
}

void igdeNativeNullIcon::UpdatePixels( void*, deImage& ){
}

#endif
