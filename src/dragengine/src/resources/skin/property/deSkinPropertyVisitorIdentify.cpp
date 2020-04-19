/* 
 * Drag[en]gine Game Engine
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

#include <stdio.h>
#include <stdlib.h>
#include "deSkinProperty.h"
#include "deSkinPropertyValue.h"
#include "deSkinPropertyColor.h"
#include "deSkinPropertyImage.h"
#include "deSkinPropertyVideo.h"
#include "deSkinPropertyConstructed.h"
#include "deSkinPropertyMapped.h"
#include "deSkinPropertyVisitorIdentify.h"
#include "../../../common/exceptions.h"



// Class deSkinPropertyVisitorIdentify
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyVisitorIdentify::deSkinPropertyVisitorIdentify(){
	Reset();
}

deSkinPropertyVisitorIdentify::~deSkinPropertyVisitorIdentify(){
}



// Management
///////////////

deSkinPropertyValue &deSkinPropertyVisitorIdentify::CastToValue() const{
	if( pType != eptValue ){
		DETHROW( deeInvalidParam );
	}
	
	return *( ( deSkinPropertyValue* )pProperty );
}

deSkinPropertyColor &deSkinPropertyVisitorIdentify::CastToColor() const{
	if( pType != eptColor ){
		DETHROW( deeInvalidParam );
	}
	
	return *( ( deSkinPropertyColor* )pProperty );
}

deSkinPropertyImage &deSkinPropertyVisitorIdentify::CastToImage() const{
	if( pType != eptImage ){
		DETHROW( deeInvalidParam );
	}
	
	return *( ( deSkinPropertyImage* )pProperty );
}

deSkinPropertyVideo &deSkinPropertyVisitorIdentify::CastToVideo() const{
	if( pType != eptVideo ){
		DETHROW( deeInvalidParam );
	}
	
	return *( ( deSkinPropertyVideo* )pProperty );
}

deSkinPropertyConstructed &deSkinPropertyVisitorIdentify::CastToConstructed() const{
	if( pType != eptConstructed ){
		DETHROW( deeInvalidParam );
	}
	
	return *( ( deSkinPropertyConstructed* )pProperty );
}

deSkinPropertyMapped &deSkinPropertyVisitorIdentify::CastToMapped() const{
	if( pType != eptMapped ){
		DETHROW( deeInvalidParam );
	}
	
	return *( ( deSkinPropertyMapped* )pProperty );
}

void deSkinPropertyVisitorIdentify::Reset(){
	pProperty = NULL;
	pType = eptUnknown;
}



// Visiting
/////////////

void deSkinPropertyVisitorIdentify::VisitProperty( deSkinProperty &property ){
	pProperty = &property;
	pType = eptUnknown;
}

void deSkinPropertyVisitorIdentify::VisitValue( deSkinPropertyValue &property ){
	pProperty = &property;
	pType = eptValue;
}

void deSkinPropertyVisitorIdentify::VisitColor( deSkinPropertyColor &property ){
	pProperty = &property;
	pType = eptColor;
}

void deSkinPropertyVisitorIdentify::VisitImage( deSkinPropertyImage &property ){
	pProperty = &property;
	pType = eptImage;
}

void deSkinPropertyVisitorIdentify::VisitVideo( deSkinPropertyVideo &property ){
	pProperty = &property;
	pType = eptVideo;
}

void deSkinPropertyVisitorIdentify::VisitConstructed( deSkinPropertyConstructed &property ){
	pProperty = &property;
	pType = eptConstructed;
}

void deSkinPropertyVisitorIdentify::VisitMapped( deSkinPropertyMapped &property ){
	pProperty = &property;
	pType = eptMapped;
}
