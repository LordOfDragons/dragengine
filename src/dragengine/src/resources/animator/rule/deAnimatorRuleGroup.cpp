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
