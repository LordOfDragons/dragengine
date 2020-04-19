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

#include "igdeGDSky.h"
#include "igdeGDSkyManager.h"
#include "../igdeGDCategory.h"
#include "../igdeGameDefinition.h"
#include "../visitor/igdeGDVisitor.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deFileSearchVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class igdeGDSkyManager
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDSkyManager::igdeGDSkyManager(){
	pCategories.TakeOver( new igdeGDCategory( "Skies" ) );
}

igdeGDSkyManager::~igdeGDSkyManager(){
}



// Management
///////////////

void igdeGDSkyManager::AddSky( igdeGDSky *sky ){
	if( ! sky ){
		DETHROW( deeInvalidParam );
	}
	pSkyList.Add( sky );
}

void igdeGDSkyManager::RemoveSky( igdeGDSky *sky ){
	pSkyList.Remove( sky );
}

void igdeGDSkyManager::RemoveAllSkies(){
	pSkyList.RemoveAll();
}



void igdeGDSkyManager::SetDefaultPath( const char *path ){
	pDefaultPath = path;
}



void igdeGDSkyManager::VisitSkiesMatchingCategory( igdeGDVisitor &visitor, const igdeGDCategory *category ) const{
	const int count = pSkyList.GetCount();
	decPath pathCat;
	int i;
	
	if( category ){
		category->GetFullPath( pathCat );
	}
	const decString strPathCat( pathCat.GetPathUnix() );
	
	for( i=0; i<count; i++ ){
		igdeGDSky * const sky = ( igdeGDSky* )pSkyList.GetAt( i );
		if( sky->GetCategory() == strPathCat ){
			visitor.VisitSky( sky );
		}
	}
}

void igdeGDSkyManager::VisitMatchingFilter( igdeGDVisitor &visitor, const decString &filter ) const{
	if( filter.GetLength() < 2 ){
		return;
	}
	
	const decString realFilter( filter.GetLower() );
	const int count = pSkyList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGDSky * const sky = ( igdeGDSky* )pSkyList.GetAt( i );
		if( sky->GetName().GetLower().FindString( realFilter ) != -1
		|| sky->GetPath().GetLower().FindString( realFilter ) != -1 ){
			visitor.VisitSky( sky );
		}
	}
}



void igdeGDSkyManager::UpdateWith( const igdeGDSkyManager &manager ){
	const int count = manager.GetSkyList().GetCount();
	igdeGDSky *sky = NULL;
	igdeGDSky *skyCheck;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			sky = new igdeGDSky( *manager.GetSkyList().GetAt( i ) );
			skyCheck = pSkyList.GetWithPath( sky->GetPath() );
			if( skyCheck ){
				RemoveSky( skyCheck );
			}
			AddSky( sky );
			sky->FreeReference();
			sky = NULL;
		}
		
	}catch( const deException & ){
		if( sky ){
			sky->FreeReference();
		}
		throw;
	}
	
	pCategories->UpdateWith( manager.pCategories );
	pAutoFindPath = manager.pAutoFindPath;
	
	if( ! manager.pDefaultPath.IsEmpty() ){
		pDefaultPath = manager.pDefaultPath;
	}
}

void igdeGDSkyManager::UpdateWithFound( const igdeGDSkyManager &skyManager ){
	const int count = skyManager.GetSkyList().GetCount();
	deObjectReference refSky;
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDSky &foundSky = *skyManager.GetSkyList().GetAt( i );
		
		igdeGDSky * const skyCheck = pSkyList.GetWithPath( foundSky.GetPath() );
		if( skyCheck ){
			continue;
		}
		
		refSky.TakeOver( new igdeGDSky( foundSky ) );
		igdeGDSky * const sky = ( igdeGDSky* )( deObject* )refSky;
		
		igdeGDCategory * const autoCategory = pCategories->AutoCategorize( sky->GetPath() );
		if( autoCategory ){
			sky->SetCategory( autoCategory->GetFullPathString() );
		}
		
		AddSky( sky );
	}
}

class igdeGDSkyManagerFind : public deFileSearchVisitor{
private:
	igdeGDSkyManager &pOwner;
	const char * const pPattern;
	deObjectReference pSky;
	
public:
	igdeGDSkyManagerFind( igdeGDSkyManager &owner, const char *pattern ) :
	pOwner( owner ), pPattern( pattern ){
	}
	
	virtual bool VisitFile( const deVirtualFileSystem &vfs, const decPath &path ){
		if( ! path.GetLastComponent().MatchesPattern( pPattern ) ){
			return true;
		}
		
		const decString fullPath( path.GetPathUnix() );
		if( pOwner.GetSkyList().HasWithPath( fullPath ) ){
			return true;
		}
		
		decString genName( fullPath.GetReplaced( decPath::PathSeparator(), ' ' ) );
		const int index = genName.FindReverse( '.' );
		if( index != -1 ){
			genName = genName.GetLeft( index );
		}
		
		try{
			pSky.TakeOver( new igdeGDSky( fullPath, genName ) );
			( ( igdeGDSky* )( deObject* )pSky )->SetDescription( "Auto-Imported" );
			pOwner.AddSky( ( igdeGDSky* )( deObject* )pSky );
			
		}catch( const deException & ){
		}
		
		return true;
	}
	
	virtual bool VisitDirectory( const deVirtualFileSystem &vfs, const decPath &path ){
		vfs.SearchFiles( path, *this );
		return true;
	}
};

void igdeGDSkyManager::FindAndAddSkies( deVirtualFileSystem &vfs,
const decPath &directory, const char *pattern, bool recursive ){
	igdeGDSkyManagerFind process( *this, pattern );
	vfs.SearchFiles( directory, process );
}
