/* 
 * Drag[en]gine IGDE
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

#include <stdlib.h>
#include <string.h>

#include "igdeTextureProperty.h"

#include <dragengine/common/exceptions.h>



// Class igdeTextureProperty
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTextureProperty::igdeTextureProperty( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	pName = name;
	pType = eptValue;
	pComponentCount = 1;
	pDefaultValue = 0.0f;
}

igdeTextureProperty::~igdeTextureProperty(){
}



// Management
///////////////

void igdeTextureProperty::SetType( ePropertyTypes type ){
	if( type < eptValue || type > eptImage ){
		DETHROW( deeInvalidParam );
	}
	
	pType = type;
}

void igdeTextureProperty::SetDescription( const char *description ){
	if( ! description ){
		DETHROW( deeInvalidParam );
	}
	
	pDescription = description;
}

void igdeTextureProperty::SetComponentCount( int count ){
	if( count < 1 || count > 4 ){
		DETHROW( deeInvalidParam );
	}
	
	pComponentCount = count;
}

void igdeTextureProperty::SetDefaultValue( float value ){
	pDefaultValue = value;
}

void igdeTextureProperty::SetDefaultColor( const decColor &color ){
	pDefaultColor = color;
}
