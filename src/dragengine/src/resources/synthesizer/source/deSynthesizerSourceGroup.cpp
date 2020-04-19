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

#include "deSynthesizerSourceGroup.h"
#include "deSynthesizerSourceVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deSynthesizerSourceGroup
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerSourceGroup::deSynthesizerSourceGroup() :
pApplicationType( deSynthesizerSourceGroup::eatAll ){
}

deSynthesizerSourceGroup::~deSynthesizerSourceGroup(){
	RemoveAllSources();
}



// Management
///////////////

int deSynthesizerSourceGroup::GetSourceCount() const{
	return pSources.GetCount();
}

deSynthesizerSource *deSynthesizerSourceGroup::GetSourceAt( int index ) const{
	return ( deSynthesizerSource* )pSources.GetAt( index );
}

int deSynthesizerSourceGroup::IndexOfSource( deSynthesizerSource *source ) const{
	return pSources.IndexOf( source );
}

bool deSynthesizerSourceGroup::HasSource( deSynthesizerSource *source ) const{
	return pSources.Has( source );
}

void deSynthesizerSourceGroup::AddSource( deSynthesizerSource *source ){
	if( ! source ){
		DETHROW( deeInvalidParam );
	}
	pSources.Add( source );
}

void deSynthesizerSourceGroup::RemoveSource( deSynthesizerSource *source ){
	pSources.Remove( source );
}

void deSynthesizerSourceGroup::RemoveAllSources(){
	pSources.RemoveAll();
}



void deSynthesizerSourceGroup::SetApplicationType( deSynthesizerSourceGroup::eApplicationTypes type ){
	if( type < deSynthesizerSourceGroup::eatAll || type > deSynthesizerSourceGroup::eatSolo ){
		DETHROW( deeInvalidParam );
	}
	pApplicationType = type;
}



// Visiting
/////////////

void deSynthesizerSourceGroup::Visit( deSynthesizerSourceVisitor &visitor ){
	visitor.VisitGroup( *this );
}
