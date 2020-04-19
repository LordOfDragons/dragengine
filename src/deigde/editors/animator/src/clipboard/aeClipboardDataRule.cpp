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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aeClipboardDataRule.h"
#include "../animator/rule/aeRule.h"

#include <dragengine/common/exceptions.h>



// Class aeClipboardDataRule
//////////////////////////////

const char * const aeClipboardDataRule::TYPE_NAME = "rule";

// Constructor, destructor
////////////////////////////

aeClipboardDataRule::aeClipboardDataRule( aeRule *rule ) :
igdeClipboardData( TYPE_NAME )
{
	aeRule *copyRule = NULL;
	
	try{
		copyRule = rule->CreateCopy();
		pRules.Add( copyRule );
		copyRule->FreeReference();
		
	}catch( const deException & ){
		if( copyRule ){
			copyRule->FreeReference();
		}
		throw;
	}
}

aeClipboardDataRule::aeClipboardDataRule( const aeRuleList &rules ) :
igdeClipboardData( TYPE_NAME )
{
	const int count = rules.GetCount();
	aeRule *rule = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			rule = rules.GetAt( i )->CreateCopy();
			pRules.Add( rule );
			rule->FreeReference();
			rule = NULL;
		}
		
	}catch( const deException & ){
		if( rule ){
			rule->FreeReference();
		}
		throw;
	}
}

aeClipboardDataRule::~aeClipboardDataRule(){
	pRules.RemoveAll();
}
