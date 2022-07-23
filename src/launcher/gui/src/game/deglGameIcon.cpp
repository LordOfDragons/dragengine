/* 
 * Drag[en]gine GUI Launcher
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deglGameIcon.h"

#include <dragengine/common/exceptions.h>


// Class deglGameIcon
///////////////////////

// Constructors and Destructors
/////////////////////////////////

deglGameIcon::deglGameIcon( int size, const char *path ) :
delGameIcon( size, path ){
}

deglGameIcon::~deglGameIcon(){
}



// Management
///////////////

deglSharedIcon::Ref deglGameIcon::GetScaledFoxIcon( int size ) const{
	if( ! pFoxIcon ){
		return nullptr;
	}
	
	if( GetSize() == size ){
		return pFoxIcon;
	}
	
	const int quality = 2;
		// quality 0: fast but ugly scale
		// quality 1: slower box filtered scale
		// quality 2: slow gamma corrected scale
	
	deglSharedIcon::Ref icon( deglSharedIcon::Ref::New(
		new deglSharedIcon( new FXPNGIcon( FXApp::instance() ) ) ) );
	
	FXMemoryStream stream( FXStreamLoad, ( FXuchar* )GetContent()->GetPointer(), GetContent()->GetLength() );
	stream.position( 0 );
	
	FXIcon &iconScaled = *icon->GetIcon();
	iconScaled.loadPixels( stream );
	iconScaled.scale( size, size, quality );
	iconScaled.create();
	
	return icon;
}



// Protected Functions
////////////////////////

void deglGameIcon::OnContentChanged(){
	pCreateFoxIcon();
}



// Private Functions
//////////////////////

void deglGameIcon::pCreateFoxIcon(){
	pFoxIcon = nullptr;
	
	if( ! GetContent() ){
		return;
	}
	
	if( GetPath().EndsWith( ".png" ) ){
		pCreatePNGFoxIcon();
		
	}else{
		// TODO create a placeholder icon instead
	}
}

void deglGameIcon::pCreatePNGFoxIcon(){
	try{
		FXMemoryStream stream( FXStreamLoad, ( FXuchar* )GetContent()->GetPointer(), GetContent()->GetLength() );
		stream.position( 0 );
		pFoxIcon.TakeOver( new deglSharedIcon( new FXPNGIcon( FXApp::instance() ) ) );
		FXIcon &icon = *pFoxIcon->GetIcon();
		icon.loadPixels( stream );
		icon.create();
		
	}catch( const deException & ){
		pFoxIcon = nullptr;
	}
}
