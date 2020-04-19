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

#include "deNetworkValue.h"
#include "deNetworkValueInteger.h"
#include "deNetworkValueFloat.h"
#include "deNetworkValueString.h"
#include "deNetworkValueData.h"
#include "deNetworkValuePoint2.h"
#include "deNetworkValuePoint3.h"
#include "deNetworkValueVector2.h"
#include "deNetworkValueVector3.h"
#include "deNetworkValueQuaternion.h"
#include "deNetworkValueVisitor.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deNetworkValueVisitor
////////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkValueVisitor::deNetworkValueVisitor(){
}

deNetworkValueVisitor::~deNetworkValueVisitor(){
}



// Visiting
/////////////

void deNetworkValueVisitor::VisitValue( deNetworkValue *value ){
}

void deNetworkValueVisitor::VisitInteger( deNetworkValueInteger *value ){
	VisitValue( value );
}

void deNetworkValueVisitor::VisitFloat( deNetworkValueFloat *value ){
	VisitValue( value );
}

void deNetworkValueVisitor::VisitString( deNetworkValueString *value ){
	VisitValue( value );
}

void deNetworkValueVisitor::VisitData( deNetworkValueData *value ){
	VisitValue( value );
}

void deNetworkValueVisitor::VisitPoint2( deNetworkValuePoint2 *value ){
	VisitValue( value );
}

void deNetworkValueVisitor::VisitPoint3( deNetworkValuePoint3 *value ){
	VisitValue( value );
}

void deNetworkValueVisitor::VisitVector2( deNetworkValueVector2 *value ){
	VisitValue( value );
}

void deNetworkValueVisitor::VisitVector3( deNetworkValueVector3 *value ){
	VisitValue( value );
}

void deNetworkValueVisitor::VisitQuaternion( deNetworkValueQuaternion *value ){
	VisitValue( value );
}
