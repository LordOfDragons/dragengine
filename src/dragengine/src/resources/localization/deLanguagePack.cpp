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

#include "deLanguagePack.h"
#include "deLanguagePackEntry.h"
#include "deLanguagePackManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"



// Class deLanguagePack
/////////////////////////

// Constructor, destructor
////////////////////////////

deLanguagePack::deLanguagePack( deLanguagePackManager *manager, deVirtualFileSystem *vfs,
	const char *filename, TIME_SYSTEM modificationTime ) :
deFileResource( manager, vfs, filename, modificationTime ),
pEntries( NULL ),
pEntryCount( 0 )
{
	pMissingText.SetFromUTF8( "< Missing Text >" );
}

deLanguagePack::~deLanguagePack(){
	if( pEntries ){
		delete [] pEntries;
	}
}



// Management
///////////////

void deLanguagePack::SetName( const decUnicodeString &name ){
	pName = name;
}

void deLanguagePack::SetDescription( const decUnicodeString &description ){
	pDescription = description;
}

void deLanguagePack::SetMissingText( const decUnicodeString &missingText ){
	pMissingText = missingText;
}



void deLanguagePack::SetEntryCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pEntries ){
		delete [] pEntries;
		pEntries = NULL;
		pEntryCount = 0;
	}
	
	if( count > 0 ){
		pEntries = new deLanguagePackEntry[ count ];
		pEntryCount = count;
	}
}

const deLanguagePackEntry &deLanguagePack::GetEntryAt( int index ) const{
	if( index < 0 || index >= pEntryCount ){
		DETHROW( deeInvalidParam );
	}
	return pEntries[ index ];
}

deLanguagePackEntry &deLanguagePack::GetEntryAt( int index ){
	if( index < 0 || index >= pEntryCount ){
		DETHROW( deeInvalidParam );
	}
	return pEntries[ index ];
}

int deLanguagePack::IndexOfEntryNamed( const char *name ) const{
	int i;
	
	for( i=0; i<pEntryCount; i++ ){
		if( pEntries[ i ].GetName() == name ){
			return i;
		}
	}
	
	return -1;
}



const decUnicodeString &deLanguagePack::Translate( const char *name ) const{
	return Translate( name, pMissingText );
}

const decUnicodeString &deLanguagePack::Translate( const char *name,
const decUnicodeString &defaultValue ) const{
	void *pointer;
	
	if( pLookupTable.GetAt( name, &pointer ) ){
		return ( ( const deLanguagePackEntry* )pointer )->GetText();
		
	}else{
		return defaultValue;
	}
}



bool deLanguagePack::Verify() const{
	int i, j;
	
	for( i=0; i<pEntryCount; i++ ){
		const decString &name = pEntries[ i ].GetName();
		
		if( name.IsEmpty() ){
			GetEngine()->GetLogger()->LogErrorFormat( "Dragengine",
				"deLanguagePack::Verify(%s): Entry has empty name (index %d)",
					GetFilename().GetString(), i );
			return false;
		}
		
		for( j=0; j<pEntryCount; j++ ){
			if( j != i && pEntries[ j ].GetName() == name ){
				GetEngine()->GetLogger()->LogErrorFormat( "Dragengine",
					"deLanguagePack::Verify(%s): Duplicate name '%s' (index %d and %d)",
						GetFilename().GetString(), name.GetString(), i, j );
				return false;
			}
		}
	}
	
	return true;
}

void deLanguagePack::BuildLookupTable(){
	pLookupTable.RemoveAll();
	
	int i;
	for( i=0; i<pEntryCount; i++ ){
		pLookupTable.SetAt( pEntries[ i ].GetName(), pEntries + i );
	}
}
