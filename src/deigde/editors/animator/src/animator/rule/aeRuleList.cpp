/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeRule.h"
#include "aeRuleList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class aeRule
/////////////////

// Constructor, destructor
////////////////////////////

aeRuleList::aeRuleList(){
}

aeRuleList::aeRuleList( const aeRuleList &copy ) :
pRules( copy.pRules ){
}

aeRuleList::~aeRuleList(){
}



// Management
///////////////

int aeRuleList::GetCount() const{
	return pRules.GetCount();
}

aeRule *aeRuleList::GetAt( int index ) const{
	return ( aeRule* )pRules.GetAt( index );
}

int aeRuleList::IndexOf( aeRule *rule ) const{
	return pRules.IndexOf( rule );
}

bool aeRuleList::Has( aeRule *rule ) const{
	return pRules.Has( rule );
}

void aeRuleList::Add( aeRule *rule ){
	if( Has( rule ) ){
		DETHROW( deeInvalidParam );
	}
	pRules.Add( rule );
}

void aeRuleList::Insert( aeRule *rule, int index ){
	if( Has( rule ) ){
		DETHROW( deeInvalidParam );
	}
	pRules.Insert( rule, index );
}

void aeRuleList::Move( aeRule *rule, int index ){
	pRules.Move( rule, index );
}

void aeRuleList::Remove( aeRule *rule ){
	pRules.Remove( rule );
}

void aeRuleList::RemoveAll(){
	pRules.RemoveAll();
}



// Operators
//////////////

aeRuleList &aeRuleList::operator=( const aeRuleList &list ){
	pRules = list.pRules;
	return *this;
}
