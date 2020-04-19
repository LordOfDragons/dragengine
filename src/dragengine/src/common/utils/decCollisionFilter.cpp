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

#include "decCollisionFilter.h"



// Class decCollisionFilter
/////////////////////////////

// Constructor, destructor
////////////////////////////

decCollisionFilter::decCollisionFilter(){
	pFilter.FillMask();
}

decCollisionFilter::decCollisionFilter( const decLayerMask &mask ) :
pCategory( mask ), pFilter( mask ){
}

decCollisionFilter::decCollisionFilter( const decLayerMask &category, const decLayerMask &filter ) :
pCategory( category ), pFilter( filter ){
}

decCollisionFilter::decCollisionFilter( const decCollisionFilter &other ) :
pCategory( other.pCategory ), pFilter( other.pFilter ){
}

decCollisionFilter::~decCollisionFilter(){
}



// Management
///////////////

bool decCollisionFilter::Collides( const decCollisionFilter &other ) const{
	return pFilter.Matches( other.pCategory ) && other.pFilter.Matches( pCategory );
}

bool decCollisionFilter::CollidesNot( const decCollisionFilter &other ) const{
	return pFilter.MatchesNot( other.pCategory ) || other.pFilter.MatchesNot( pCategory );
}

bool decCollisionFilter::CanCollide() const{
	return pCategory.IsNotEmpty() && pFilter.IsNotEmpty();
}

bool decCollisionFilter::CanNotCollide() const{
	return pCategory.IsEmpty() || pFilter.IsEmpty();
}



// Operators
//////////////

bool decCollisionFilter::operator==( const decCollisionFilter &other ) const{
	return pCategory == other.pCategory && pFilter == other.pFilter;
}

bool decCollisionFilter::operator!=( const decCollisionFilter &other ) const{
	return pCategory != other.pCategory || pFilter != other.pFilter;
}

decCollisionFilter &decCollisionFilter::operator=( const decCollisionFilter &other ){
	pCategory = other.pCategory;
	pFilter = other.pFilter;
	return *this;
}
