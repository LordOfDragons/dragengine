/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeGDSkin.h"
#include "igdeGDSkinManager.h"
#include "../igdeGDCategory.h"
#include "../visitor/igdeGDVisitor.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deFileSearchVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class igdeGDSkinManager
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDSkinManager::igdeGDSkinManager(){
	pCategories.TakeOver( new igdeGDCategory( "Skins" ) );
}

igdeGDSkinManager::~igdeGDSkinManager(){
}



// Management
///////////////

int igdeGDSkinManager::GetSkinCount() const{
	return pSkins.GetCount();
}

bool igdeGDSkinManager::HasSkin( igdeGDSkin *skin ) const{
	return pSkins.Has( skin );
}

bool igdeGDSkinManager::HasSkinWithPath( const char *path ) const{
	return IndexOfSkinWithPath( path ) != -1;
}

bool igdeGDSkinManager::HasSkinWithName( const char *name ) const{
	return IndexOfSkinWithName( name ) != -1;
}

bool igdeGDSkinManager::HasSkinWithPathOrName( const char *path, const char *name ) const{
	return IndexOfSkinWithPathOrName( path, name ) != -1;
}

int igdeGDSkinManager::IndexOfSkin( igdeGDSkin *skin ) const{
	return pSkins.IndexOf( skin );
}

int igdeGDSkinManager::IndexOfSkinWithPath( const char *path ) const{
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	const int count = pSkins.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDSkin* )pSkins.GetAt( i ) )->GetPath().Equals( path ) ){
			return i;
		}
	}
	
	return -1;
}

int igdeGDSkinManager::IndexOfSkinWithName( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	const int count = pSkins.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDSkin* )pSkins.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

int igdeGDSkinManager::IndexOfSkinWithPathOrName( const char *path, const char *name ) const{
	if( ! path || ! name ){
		DETHROW( deeInvalidParam );
	}
	const int count = pSkins.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDSkin * const skin = ( igdeGDSkin* )pSkins.GetAt( i );
		if( skin->GetPath().Equals( path ) || skin->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

igdeGDSkin *igdeGDSkinManager::GetSkinAt( int index ) const{
	return ( igdeGDSkin* )pSkins.GetAt( index );
}

igdeGDSkin *igdeGDSkinManager::GetSkinWithPath( const char *path ) const{
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	const int count = pSkins.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGDSkin * const skin = ( igdeGDSkin* )pSkins.GetAt( i );
		if( skin->GetPath().Equals( path ) ){
			return skin;
		}
	}
	
	return NULL;
}

igdeGDSkin *igdeGDSkinManager::GetSkinWithName( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	const int count = pSkins.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGDSkin * const skin = ( igdeGDSkin* )pSkins.GetAt( i );
		if( skin->GetName().Equals( name ) ){
			return skin;
		}
	}
	
	return NULL;
}

void igdeGDSkinManager::AddSkin( igdeGDSkin *skin ){
	if( ! skin || HasSkinWithPathOrName( skin->GetPath(), skin->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	pSkins.Add( skin );
}

void igdeGDSkinManager::RemoveSkin( igdeGDSkin *skin ){
	pSkins.Remove( skin );
}

void igdeGDSkinManager::RemoveAllSkins(){
	pSkins.RemoveAll();
}



void igdeGDSkinManager::SetDefaultSkinPath( const char *defaultSkinPath ){
	pDefaultSkinPath = defaultSkinPath;
}



void igdeGDSkinManager::VisitSkinsMatchingCategory( igdeGDVisitor &visitor, const igdeGDCategory *category ) const{
	const int count = pSkins.GetCount();
	decPath pathCat;
	int i;
	
	if( category ){
		category->GetFullPath( pathCat );
	}
	const decString strPathCat( pathCat.GetPathUnix() );
	
	for( i=0; i<count; i++ ){
		igdeGDSkin * const skin = ( igdeGDSkin* )pSkins.GetAt( i );
		if( skin->GetCategory() == strPathCat ){
			visitor.VisitSkin( skin );
		}
	}
}

void igdeGDSkinManager::VisitMatchingFilter( igdeGDVisitor &visitor, const decString &filter ) const{
	if( filter.GetLength() < 2 ){
		return;
	}
	
	const decString realFilter( filter.GetLower() );
	const int count = pSkins.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGDSkin * const skin = ( igdeGDSkin* )pSkins.GetAt( i );
		if( skin->GetName().GetLower().FindString( realFilter ) != -1
		|| skin->GetPath().GetLower().FindString( realFilter ) != -1 ){
			visitor.VisitSkin( skin );
		}
	}
}



void igdeGDSkinManager::UpdateWith( const igdeGDSkinManager &skinManager ){
	const int count = skinManager.GetSkinCount();
	deObjectReference skin;
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDSkin &otherSkin = *skinManager.GetSkinAt( i );
		
		igdeGDSkin *skinCheck = GetSkinWithPath( otherSkin.GetPath() );
		if( ! skinCheck ){
			skinCheck = GetSkinWithName( otherSkin.GetName() );
		}
		
		skin.TakeOver( new igdeGDSkin( otherSkin ) );
		if( skinCheck ){
			RemoveSkin( skinCheck );
		}
		AddSkin( ( igdeGDSkin* )( deObject* )skin );
	}
	
	pCategories->UpdateWith( skinManager.pCategories );
	pAutoFindPath = skinManager.pAutoFindPath;
	
	if( ! skinManager.pDefaultSkinPath.IsEmpty() ){
		pDefaultSkinPath = skinManager.pDefaultSkinPath;
	}
}

void igdeGDSkinManager::UpdateWithFound( const igdeGDSkinManager &skinManager ){
	const int count = skinManager.GetSkinCount();
	deObjectReference refSkin;
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDSkin &foundSkin = *skinManager.GetSkinAt( i );
		
		igdeGDSkin * const skinCheck = GetSkinWithPath( foundSkin.GetPath() );
		if( skinCheck ){
			continue;
		}
		
		refSkin.TakeOver( new igdeGDSkin( foundSkin ) );
		igdeGDSkin * const skin = ( igdeGDSkin* )( deObject* )refSkin;
		
		igdeGDCategory * const autoCategory = pCategories->AutoCategorize( skin->GetPath() );
		if( autoCategory ){
			skin->SetCategory( autoCategory->GetFullPathString() );
		}
		
		AddSkin( skin );
	}
}

class igdeGDSkinManagerFind : public deFileSearchVisitor{
private:
	igdeGDSkinManager &pOwner;
	const char * const pPattern;
	deObjectReference pSkin;
	
public:
	igdeGDSkinManagerFind( igdeGDSkinManager &owner, const char *pattern ) :
	pOwner( owner ), pPattern( pattern ){
	}
	
	virtual bool VisitFile( const deVirtualFileSystem &, const decPath &path ){
		if( ! path.GetLastComponent().MatchesPattern( pPattern ) ){
			return true;
		}
		
		const decString fullPath( path.GetPathUnix() );
		if( pOwner.HasSkinWithPath( fullPath ) ){
			return true;
		}
		
		decString genName( fullPath.GetReplaced( decPath::PathSeparator(), ' ' ) );
		const int index = genName.FindReverse( '.' );
		if( index != -1 ){
			genName = genName.GetLeft( index );
		}
		
		try{
			pSkin.TakeOver( new igdeGDSkin( fullPath, genName ) );
			( ( igdeGDSkin* )( deObject* )pSkin )->SetDescription( "Auto-Imported" );
			pOwner.AddSkin( ( igdeGDSkin* )( deObject* )pSkin );
			
		}catch( const deException & ){
		}
		
		return true;
	}
	
	virtual bool VisitDirectory( const deVirtualFileSystem &vfs, const decPath &path ){
		vfs.SearchFiles( path, *this );
		return true;
	}
};

void igdeGDSkinManager::FindAndAddSkins( deVirtualFileSystem &vfs, const decPath &directory,
const char *pattern, bool recursive ){
	igdeGDSkinManagerFind process( *this, pattern );
	vfs.SearchFiles( directory, process );
}
