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
#include <string.h>

#include "ceCConditionActorParameter.h"

#include <dragengine/common/exceptions.h>



// Class ceCConditionActorParameter
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceCConditionActorParameter::ceCConditionActorParameter() :
ceConversationCondition( ectActorParameter ),
pParameter( "Parameter" ),
pOperator( eopEqual ),
pTestValue( 0 ){
}

ceCConditionActorParameter::ceCConditionActorParameter( const ceCConditionActorParameter &condition ) :
ceConversationCondition( condition ),
pActor( condition.pActor ),
pParameter( condition.pParameter ),
pOperator( condition.pOperator ),
pTestValue( condition.pTestValue ),
pTestVariable( condition.pTestVariable ){
}

ceCConditionActorParameter::~ceCConditionActorParameter(){
}



// Management
///////////////

void ceCConditionActorParameter::SetActor( const char *id ){
	pActor = id;
}

void ceCConditionActorParameter::SetParameter( const char *name ){
	pParameter = name;
}

void ceCConditionActorParameter::SetOperator( eOperators aOperator ){
	pOperator = aOperator;
}

void ceCConditionActorParameter::SetTestValue( int value ){
	pTestValue = value;
}

void ceCConditionActorParameter::SetTestVariable( const char * variable ){
	pTestVariable = variable;
}



ceConversationCondition *ceCConditionActorParameter::CreateCopy() const{
	return new ceCConditionActorParameter( *this );
}
