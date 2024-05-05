/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
