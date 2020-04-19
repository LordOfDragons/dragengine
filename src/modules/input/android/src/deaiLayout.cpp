/* 
 * Drag[en]gine Android Input Module
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

#include "deaiLayout.h"

#include <dragengine/common/exceptions.h>



// Class deaiLayout
/////////////////////

// Constructor, destructor
////////////////////////////

deaiLayout::deaiLayout(){
}

deaiLayout::deaiLayout( const decPoint &position, const decPoint &size ) :
pPosition( position ),
pSize( decPoint().Largest( size ) ){
}

deaiLayout::deaiLayout( const deaiLayout &copy ) :
pPosition( copy.pPosition ),
pSize( copy.pSize ){
}

deaiLayout::~deaiLayout(){
}



// Management
///////////////

void deaiLayout::SetPosition( const decPoint &position ){
	pPosition = position;
}

void deaiLayout::SetSize( const decPoint &size ){
	pSize = decPoint().Largest( size );
}



// Operators
//////////////

bool deaiLayout::operator==( const deaiLayout &layout ) const{
	return pPosition == layout.pPosition && pSize == layout.pSize;
}

deaiLayout &deaiLayout::operator=( const deaiLayout &layout ){
	pPosition = layout.pPosition;
	pSize = layout.pSize;
	return *this;
}
