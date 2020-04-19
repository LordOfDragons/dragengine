/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeProperty.h"
#include "../filepattern/gdeFilePattern.h"

#include <dragengine/common/exceptions.h>



// Class gdeProperty
//////////////////////

// Constructor, destructor
////////////////////////////

gdeProperty::gdeProperty() :
pType( eptString ),
pMinimumValue( 0.0f ),
pMaximumValue( 1.0f ),
pPathPatternType( epptAll ),
pIdentifierUsage( false ){
}

gdeProperty::gdeProperty( const char *name ) :
pName( name ),
pType( eptString ),
pMinimumValue( 0.0f ),
pMaximumValue( 1.0f ),
pPathPatternType( epptAll ),
pIdentifierUsage( false ){
}

gdeProperty::gdeProperty( const gdeProperty &property ) :
pName( property.pName ),
pDescription( property.pDescription ),
pType( property.pType ),
pMinimumValue( property.pMinimumValue ),
pMaximumValue( property.pMaximumValue ),
pDefaultValue( property.pDefaultValue ),
pOptions( property.pOptions ),
pPathPatternType( property.pPathPatternType ),
pIdentifierGroup( property.pIdentifierGroup ),
pIdentifierUsage( property.pIdentifierUsage )
{
	const int count = property.pCustomPathPattern.GetCount();
	gdeFilePattern *filePattern = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			filePattern = new gdeFilePattern( *property.pCustomPathPattern.GetAt( i ) );
			pCustomPathPattern.Add( filePattern );
			filePattern->FreeReference();
			filePattern = NULL;
		}
		
	}catch( const deException & ){
		if( filePattern ){
			filePattern->FreeReference();
		}
		throw;
	}
}

gdeProperty::~gdeProperty(){
}



// Management
///////////////

void gdeProperty::SetName( const char *name ){
	pName = name;
}

void gdeProperty::SetDescription( const char *description ){
	pDescription = description;
}

void gdeProperty::SetType( ePropertyTypes type ){
	if( type < eptString || type > eptIdentifier ){
		DETHROW( deeInvalidParam );
	}
	pType = type;
}

void gdeProperty::SetMinimumValue( float value ){
	pMinimumValue = value;
}

void gdeProperty::SetMaximumValue( float value ){
	pMaximumValue = value;
}

void gdeProperty::SetPathPatternType( ePathPatternTypes type ){
	if( type < epptAll || type > epptCustom ){
		DETHROW( deeInvalidParam );
	}
	pPathPatternType = type;
}

void gdeProperty::SetIdentifierGroup( const char *group ){
	pIdentifierGroup = group;
}

void gdeProperty::SetIdentifierUsage( bool usage ){
	pIdentifierUsage = usage;
}

void gdeProperty::SetDefaultValue( const char *value ){
	pDefaultValue = value;
}



gdeProperty &gdeProperty::operator=( const gdeProperty &property ){
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
	
	const int count = property.pCustomPathPattern.GetCount();
	gdeFilePattern *filePattern = NULL;
	int i;
	
	pCustomPathPattern.RemoveAll();
	
	try{
		for( i=0; i<count; i++ ){
			filePattern = new gdeFilePattern( *property.pCustomPathPattern.GetAt( i ) );
			pCustomPathPattern.Add( filePattern );
			filePattern->FreeReference();
			filePattern = NULL;
		}
		
	}catch( const deException & ){
		if( filePattern ){
			filePattern->FreeReference();
		}
		throw;
	}
	
	return *this;
}
