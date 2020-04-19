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

#include <stdlib.h>
#include <stdio.h>
#include "deSkinProperty.h"
#include "deSkinPropertyVisitor.h"
#include "deSkinPropertyValue.h"
#include "deSkinPropertyColor.h"
#include "deSkinPropertyImage.h"
#include "deSkinPropertyVideo.h"
#include "deSkinPropertyConstructed.h"
#include "deSkinPropertyMapped.h"
#include "../../../common/exceptions.h"



// Class deSkinPropertyVisitor
////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyVisitor::deSkinPropertyVisitor(){
}

deSkinPropertyVisitor::~deSkinPropertyVisitor(){
}



// Visiting
/////////////

void deSkinPropertyVisitor::VisitProperty( deSkinProperty &property ){
}

void deSkinPropertyVisitor::VisitValue( deSkinPropertyValue &property ){
	VisitProperty( property );
}

void deSkinPropertyVisitor::VisitColor( deSkinPropertyColor &property ){
	VisitProperty( property );
}

void deSkinPropertyVisitor::VisitImage( deSkinPropertyImage &property ){
	VisitProperty( property );
}

void deSkinPropertyVisitor::VisitVideo( deSkinPropertyVideo &property ){
	VisitProperty( property );
}

void deSkinPropertyVisitor::VisitConstructed( deSkinPropertyConstructed &property ){
	VisitProperty( property );
}

void deSkinPropertyVisitor::VisitMapped( deSkinPropertyMapped &property ){
	VisitProperty( property );
}
