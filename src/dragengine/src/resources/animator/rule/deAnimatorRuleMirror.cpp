/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "deAnimatorRuleMirror.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleMirror
///////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleMirror::deAnimatorRuleMirror() :
pMirrorAxis( emaX ),
pMatchNames( nullptr ),
pMatchNameCount( 0 ),
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( false ){
}

deAnimatorRuleMirror::~deAnimatorRuleMirror(){
	if( pMatchNames ){
		delete [] pMatchNames;
	}
}



// Management
///////////////

void deAnimatorRuleMirror::SetMirrorAxis( eMirrorAxis axis ){
	pMirrorAxis = axis;
}

void deAnimatorRuleMirror::SetMirrorBone( const char *boneName ){
	pMirrorBone = boneName;
}

const deAnimatorRuleMirror::sMatchName &deAnimatorRuleMirror::GetMatchNameAt( int index ) const{
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pMatchNameCount ){
		DETHROW_INFO( deeInvalidParam, "index >= matchNameCount" );
	}
	
	return pMatchNames[ index ];
}

void deAnimatorRuleMirror::AddMatchName( const char *first, const char *second, eMatchNameType type ){
	if( ! first ){
		DETHROW_INFO( deeNullPointer, "first" );
	}
	if( strlen( first ) == 0 ){
		DETHROW_INFO( deeInvalidParam, "first is empty string" );
	}
	if( ! second ){
		DETHROW_INFO( deeNullPointer, "second" );
	}
	if( strlen( second ) == 0 ){
		DETHROW_INFO( deeInvalidParam, "second is empty string" );
	}
	
	sMatchName * const newArray = new sMatchName[ pMatchNameCount + 1 ];
	if( pMatchNames ){
		int i;
		for( i=0; i<pMatchNameCount; i++ ){
			newArray[ i ] = pMatchNames[ i ];
		}
		delete [] pMatchNames;
	}
	
	pMatchNames = newArray;
	pMatchNames[ pMatchNameCount ].first = first;
	pMatchNames[ pMatchNameCount ].second = second;
	pMatchNames[ pMatchNameCount ].type = type;
	pMatchNameCount++;
}

void deAnimatorRuleMirror::RemoveAllMatchNames(){
	if( pMatchNames ){
		delete [] pMatchNames;
	}
	pMatchNames = nullptr;
	pMatchNameCount = 0;
}

void deAnimatorRuleMirror::SetEnablePosition( bool enable ){
	pEnablePosition = enable;
}

void deAnimatorRuleMirror::SetEnableOrientation( bool enable ){
	pEnableOrientation = enable;
}

void deAnimatorRuleMirror::SetEnableSize( bool enable ){
	pEnableSize = enable;
}



// Visiting
/////////////

void deAnimatorRuleMirror::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitMirror( *this );
}
