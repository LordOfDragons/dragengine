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

#include "delGameProfile.h"
#include "delGameProfileList.h"

#include <dragengine/common/exceptions.h>



// Class delGameProfileList
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameProfileList::delGameProfileList(){
}

delGameProfileList::~delGameProfileList(){
	RemoveAll();
}



// Management
///////////////

int delGameProfileList::GetCount() const{
	return pProfiles.GetCount();
}

delGameProfile *delGameProfileList::GetAt( int index ) const{
	return ( delGameProfile* )pProfiles.GetAt( index );
}

delGameProfile *delGameProfileList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delGameProfile * const profile = ( delGameProfile* )pProfiles.GetAt( i );
		if( profile->GetName() == name ){
			return profile;
		}
	}
	
	return nullptr;
}

bool delGameProfileList::Has( delGameProfile *profile ) const{
	return pProfiles.Has( profile );
}

bool delGameProfileList::HasNamed( const char *name ) const{
	return GetNamed ( name );
}

int delGameProfileList::IndexOf( delGameProfile *profile ) const{
	return pProfiles.IndexOf( profile );
}

int delGameProfileList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( delGameProfile* )pProfiles.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void delGameProfileList::Add( delGameProfile *profile ){
	if( ! profile ){
		DETHROW_INFO( deeNullPointer, "profile" );
	}
	if( HasNamed ( profile->GetName() ) ){
		DETHROW_INFO( deeInvalidParam, "named profile is present" );
	}
	
	pProfiles.Add( profile );
}

void delGameProfileList::Remove( delGameProfile *profile ){
	const int index = IndexOf ( profile );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidParam, "profile is absent" );
	}
	
	pProfiles.RemoveFrom( index );
}

void delGameProfileList::RemoveAll(){
	pProfiles.RemoveAll();
}

void delGameProfileList::ValidateAll( delLauncher &launcher ){
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( delGameProfile* )pProfiles.GetAt( i ) )->Verify( launcher );
	}
}
