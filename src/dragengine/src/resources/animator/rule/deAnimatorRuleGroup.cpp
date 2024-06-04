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

#include "deAnimatorRuleGroup.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleGroup
//////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleGroup::deAnimatorRuleGroup() :
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( true ),
pEnableVertexPositionSet( true ),
pUseCurrentState( false ),
pApplicationType( deAnimatorRuleGroup::eatAll ){
}

deAnimatorRuleGroup::~deAnimatorRuleGroup(){
	RemoveAllRules();
}



// Management
///////////////

int deAnimatorRuleGroup::GetRuleCount() const{
	return pRules.GetCount();
}

deAnimatorRule *deAnimatorRuleGroup::GetRuleAt( int index ) const{
	return ( deAnimatorRule* )pRules.GetAt( index );
}

int deAnimatorRuleGroup::IndexOfRule( deAnimatorRule *rule ) const{
	return pRules.IndexOf( rule );
}

bool deAnimatorRuleGroup::HasRule( deAnimatorRule *rule ) const{
	return pRules.Has( rule );
}

void deAnimatorRuleGroup::AddRule( deAnimatorRule *rule ){
	DEASSERT_NOTNULL( rule )
	pRules.Add( rule );
}

void deAnimatorRuleGroup::RemoveRule( deAnimatorRule *rule ){
	pRules.Remove( rule );
}

void deAnimatorRuleGroup::RemoveAllRules(){
	pRules.RemoveAll();
}



void deAnimatorRuleGroup::SetEnablePosition( bool enabled ){
	pEnablePosition = enabled;
}

void deAnimatorRuleGroup::SetEnableOrientation( bool enabled ){
	pEnableOrientation = enabled;
}

void deAnimatorRuleGroup::SetEnableSize( bool enabled ){
	pEnableSize = enabled;
}

void deAnimatorRuleGroup::SetEnableVertexPositionSet( bool enabled ){
	pEnableVertexPositionSet = enabled;
}



void deAnimatorRuleGroup::SetUseCurrentState( bool useCurrentState ){
	pUseCurrentState = useCurrentState;
}

void deAnimatorRuleGroup::SetApplicationType( deAnimatorRuleGroup::eApplicationTypes type ){
	pApplicationType = type;
}



// Visiting
/////////////

void deAnimatorRuleGroup::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitGroup( *this );
}
