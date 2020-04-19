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

#include "deModuleParameter.h"
#include "../../common/exceptions.h"



// class deModuleParameter
////////////////////////////

// Constructor, destructor
////////////////////////////

deModuleParameter::deModuleParameter() :
pType( eptBoolean ),
pMinValue( 0.0f ),
pMaxValue( 1.0f ),
pValueStepSize( 0.1f ),
pSelectionEntries( NULL ),
pSelectionEntryCount( 0 ),
pCategory( ecBasic ){
}

deModuleParameter::deModuleParameter( const deModuleParameter &parameter ) :
pType( parameter.pType ),
pName( parameter.pName ),
pDescription( parameter.pDescription ),
pMinValue( parameter.pMinValue ),
pMaxValue( parameter.pMaxValue ),
pValueStepSize( parameter.pValueStepSize ),
pSelectionEntries( NULL ),
pSelectionEntryCount( 0 ),
pCategory( parameter.pCategory ),
pDisplayName( parameter.pDisplayName )
{
	if( parameter.pSelectionEntryCount > 0 ){
		pSelectionEntries = new SelectionEntry[ parameter.pSelectionEntryCount ];
		for( pSelectionEntryCount=0; pSelectionEntryCount<parameter.pSelectionEntryCount; pSelectionEntryCount++ ){
			pSelectionEntries[ pSelectionEntryCount ] = parameter.pSelectionEntries[ pSelectionEntryCount ];
		}
	}
}

deModuleParameter::~deModuleParameter(){
	if( pSelectionEntries ){
		delete [] pSelectionEntries;
	}
}



// Management
///////////////

void deModuleParameter::SetType( eParameterTypes type ){
	pType = type;
}

void deModuleParameter::SetName( const char *name ){
	pName = name;
}

void deModuleParameter::SetDescription( const char *description ){
	pDescription = description;
}

void deModuleParameter::SetMinimumValue( float minValue ){
	pMinValue = minValue;
}

void deModuleParameter::SetMaximumValue( float maxValue ){
	pMaxValue = maxValue;
}

void deModuleParameter::SetValueStepSize( float valueStepSize ){
	pValueStepSize = valueStepSize;
}

const deModuleParameter::SelectionEntry &deModuleParameter::GetSelectionEntryAt( int index ) const{
	if( index < 0 || index >= pSelectionEntryCount ){
		DETHROW( deeInvalidParam );
	}
	return pSelectionEntries[ index ];
}

int deModuleParameter::IndexOfSelectionEntryWithValue( const char *value ) const{
	int i;
	for( i=0; i<pSelectionEntryCount; i++ ){
		if( pSelectionEntries[ i ].value == value ){
			return i;
		}
	}
	return -1;
}

void deModuleParameter::AddSelectionEntry( const deModuleParameter::SelectionEntry &entry ){
	if( IndexOfSelectionEntryWithValue( entry.value ) != -1 ){
		DETHROW( deeInvalidParam );
	}
	
	SelectionEntry * const newArray = new SelectionEntry[ pSelectionEntryCount + 1 ];
	if( pSelectionEntries ){
		int i;
		for( i=0; i<pSelectionEntryCount; i++ ){
			newArray[ i ] = pSelectionEntries[ i ];
		}
		delete [] pSelectionEntries;
	}
	pSelectionEntries = newArray;
	pSelectionEntries[ pSelectionEntryCount++ ] = entry;
}

void deModuleParameter::AddSelectionEntries( const deModuleParameter::SelectionEntry *entries, int entryCount ){
	if( entryCount == 0 ){
		return;
	}
	if( ! entries ){
		DETHROW_INFO( deeNullPointer, "entries" );
	}
	if( entryCount < 1 ){
		DETHROW_INFO( deeInvalidParam, "entryCount < 0" );
	}
	
	int i, j;
	for( i=0; i<entryCount; i++ ){
		if( IndexOfSelectionEntryWithValue( entries[ i ].value ) != -1 ){
			DETHROW_INFO( deeInvalidParam, "an entries value is present" );
		}
		for( j=i+1; j<entryCount; j++ ){
			if( entries[ i ].value == entries[ j ].value ){
				DETHROW_INFO( deeInvalidParam, "two entries values are equal" );
			}
		}
	}
	
	SelectionEntry * const newArray = new SelectionEntry[ pSelectionEntryCount + entryCount ];
	if( pSelectionEntries ){
		for( i=0; i<pSelectionEntryCount; i++ ){
			newArray[ i ] = pSelectionEntries[ i ];
		}
		delete [] pSelectionEntries;
	}
	pSelectionEntries = newArray;
	for( i=0; i<entryCount; i++ ){
		pSelectionEntries[ pSelectionEntryCount++ ] = entries[ i ];
	}
}

void deModuleParameter::RemoveAllSelectionEntries(){
	if( pSelectionEntries ){
		delete [] pSelectionEntries;
	}
	pSelectionEntries = NULL;
	pSelectionEntryCount = 0;
}

void deModuleParameter::SetCategory( eCategory category ){
	pCategory = category;
}

void deModuleParameter::SetDisplayName( const char *displayName ){
	pDisplayName = displayName;
}

void deModuleParameter::Reset(){
	pType = eptBoolean;
	pName.Empty();
	pDescription.Empty();
	pMinValue = 0.0f;
	pMaxValue = 1.0f;
	pValueStepSize = 0.1f;
	RemoveAllSelectionEntries();
	pCategory = ecBasic;
	pDisplayName.Empty();
}



// Operators
//////////////

deModuleParameter &deModuleParameter::operator=( const deModuleParameter &parameter ){
	pType = parameter.pType;
	pName = parameter.pName;
	pDescription = parameter.pDescription;
	pMinValue = parameter.pMinValue;
	pMaxValue = parameter.pMaxValue;
	pValueStepSize = parameter.pValueStepSize;
	pCategory = parameter.pCategory;
	pDisplayName = parameter.pDisplayName;
	
	RemoveAllSelectionEntries();
	if( parameter.pSelectionEntryCount > 0 ){
		pSelectionEntries = new SelectionEntry[ parameter.pSelectionEntryCount ];
		for( pSelectionEntryCount=0; pSelectionEntryCount<parameter.pSelectionEntryCount; pSelectionEntryCount++ ){
			pSelectionEntries[ pSelectionEntryCount ] = parameter.pSelectionEntries[ pSelectionEntryCount ];
		}
	}
	
	return *this;
}
