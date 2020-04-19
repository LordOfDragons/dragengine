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
#include "deNetworkValueVisitorIdentify.h"
#include "../../../common/exceptions.h"



// Class deNetworkValueVisitorIdentify
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkValueVisitorIdentify::deNetworkValueVisitorIdentify(){
	Reset();
}

deNetworkValueVisitorIdentify::~deNetworkValueVisitorIdentify(){
}



// Management
///////////////

deNetworkValueInteger *deNetworkValueVisitorIdentify::CastToInteger() const{
	if( pType != envtInteger ){
		DETHROW( deeInvalidParam );
	}
	return ( deNetworkValueInteger* )pValue;
}

deNetworkValueFloat *deNetworkValueVisitorIdentify::CastToFloat() const{
	if( pType != envtFloat ){
		DETHROW( deeInvalidParam );
	}
	return ( deNetworkValueFloat* )pValue;
}

deNetworkValueString *deNetworkValueVisitorIdentify::CastToString() const{
	if( pType != envtString ){
		DETHROW( deeInvalidParam );
	}
	return ( deNetworkValueString* )pValue;
}

deNetworkValueData *deNetworkValueVisitorIdentify::CastToData() const{
	if( pType != envtData ){
		DETHROW( deeInvalidParam );
	}
	return ( deNetworkValueData* )pValue;
}

deNetworkValuePoint2 *deNetworkValueVisitorIdentify::CastToPoint2() const{
	if( pType != envtPoint2 ){
		DETHROW( deeInvalidParam );
	}
	return ( deNetworkValuePoint2* )pValue;
}

deNetworkValuePoint3 *deNetworkValueVisitorIdentify::CastToPoint3() const{
	if( pType != envtPoint3 ){
		DETHROW( deeInvalidParam );
	}
	return ( deNetworkValuePoint3* )pValue;
}

deNetworkValueVector2 *deNetworkValueVisitorIdentify::CastToVector2() const{
	if( pType != envtVector2 ){
		DETHROW( deeInvalidParam );
	}
	return ( deNetworkValueVector2* )pValue;
}

deNetworkValueVector3 *deNetworkValueVisitorIdentify::CastToVector3() const{
	if( pType != envtVector3 ){
		DETHROW( deeInvalidParam );
	}
	return ( deNetworkValueVector3* )pValue;
}

deNetworkValueQuaternion *deNetworkValueVisitorIdentify::CastToQuaternion() const{
	if( pType != envtQuaternion ){
		DETHROW( deeInvalidParam );
	}
	return ( deNetworkValueQuaternion* )pValue;
}

void deNetworkValueVisitorIdentify::Reset(){
	pValue = NULL;
	pType = envtUnknown;
}



// Visiting
/////////////

void deNetworkValueVisitorIdentify::VisitValue( deNetworkValue *value ){
	pValue = value;
	pType = envtUnknown;
}

void deNetworkValueVisitorIdentify::VisitInteger( deNetworkValueInteger *value ){
	pValue = value;
	pType = envtInteger;
}

void deNetworkValueVisitorIdentify::VisitFloat( deNetworkValueFloat *value ){
	pValue = value;
	pType = envtFloat;
}

void deNetworkValueVisitorIdentify::VisitString( deNetworkValueString *value ){
	pValue = value;
	pType = envtString;
}

void deNetworkValueVisitorIdentify::VisitData( deNetworkValueData *value ){
	pValue = value;
	pType = envtData;
}

void deNetworkValueVisitorIdentify::VisitPoint2( deNetworkValuePoint2 *value ){
	pValue = value;
	pType = envtPoint2;
}

void deNetworkValueVisitorIdentify::VisitPoint3( deNetworkValuePoint3 *value ){
	pValue = value;
	pType = envtPoint3;
}

void deNetworkValueVisitorIdentify::VisitVector2( deNetworkValueVector2 *value ){
	pValue = value;
	pType = envtVector2;
}

void deNetworkValueVisitorIdentify::VisitVector3( deNetworkValueVector3 *value ){
	pValue = value;
	pType = envtVector3;
}

void deNetworkValueVisitorIdentify::VisitQuaternion( deNetworkValueQuaternion *value ){
	pValue = value;
	pType = envtQuaternion;
}
