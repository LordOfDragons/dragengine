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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeGDProperty.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDProperty
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDProperty::igdeGDProperty( const char *name ) :
pName( name ),
pType( eptString ),
pMinimumValue( 0.0f ),
pMaximumValue( 1.0f ),
pPathPatternType( epptAll ),
pIdentifierUsage( false )
{
	if( pName.IsEmpty() ){
		DETHROW( deeInvalidParam );
	}
}

igdeGDProperty::igdeGDProperty( const igdeGDProperty &property ) :
pName( property.pName ),
pDescription( property.pDescription ),
pType( property.pType ),
pMinimumValue( property.pMinimumValue ),
pMaximumValue( property.pMaximumValue ),
pDefaultValue( property.pDefaultValue ),
pOptions( property.pOptions ),
pPathPatternType( property.pPathPatternType ),
pCustomPathPattern( property.pCustomPathPattern ),
pIdentifierGroup( property.pIdentifierGroup ),
pIdentifierUsage( property.pIdentifierUsage ){
}

igdeGDProperty::~igdeGDProperty(){
}



// Management
///////////////

void igdeGDProperty::SetName( const char *name ){
	pName = name;
}

void igdeGDProperty::SetDescription( const char *description ){
	pDescription = description;
}

void igdeGDProperty::SetType( ePropertyTypes type ){
	if( type < 0 || type >= CountTypes ){
		DETHROW( deeInvalidParam );
	}
	pType = type;
}

void igdeGDProperty::SetMinimumValue( float value ){
	pMinimumValue = value;
}

void igdeGDProperty::SetMaximumValue( float value ){
	pMaximumValue = value;
}

void igdeGDProperty::SetPathPatternType( ePathPatternTypes type ){
	if( type < 0 || type >= CountPatternTypes ){
		DETHROW( deeInvalidParam );
	}
	pPathPatternType = type;
}

void igdeGDProperty::SetIdentifierGroup( const char *group ){
	pIdentifierGroup = group;
}

void igdeGDProperty::SetIdentifierUsage( bool usage ){
	pIdentifierUsage = usage;
}

void igdeGDProperty::SetDefaultValue( const char *value ){
	pDefaultValue = value;
}



igdeGDProperty &igdeGDProperty::operator=( const igdeGDProperty &property ){
	pName = property.pName;
	pDescription = property.pDescription;
	pType = property.pType;
	pMinimumValue = property.pMinimumValue;
	pMaximumValue = property.pMaximumValue;
	pDefaultValue = property.pDefaultValue;
	pOptions = property.pOptions;
	pPathPatternType = property.pPathPatternType;
	pIdentifierGroup = property.pIdentifierGroup;
	pIdentifierUsage = property.pIdentifierUsage;
	pCustomPathPattern = property.pCustomPathPattern;
	return *this;
}
