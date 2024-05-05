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

#include "gdeSky.h"
#include "gdeSkyList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeSky
/////////////////

// Constructor, destructor
////////////////////////////

gdeSkyList::gdeSkyList(){
}

gdeSkyList::gdeSkyList( const gdeSkyList &list ) :
pSkys( list.pSkys ){
}

gdeSkyList::~gdeSkyList(){
}



// Management
///////////////

int gdeSkyList::GetCount() const{
	return pSkys.GetCount();
}

gdeSky *gdeSkyList::GetAt( int index ) const{
	return ( gdeSky* )pSkys.GetAt( index );
}

gdeSky *gdeSkyList::GetWithPath( const char *path ) const{
	const int count = pSkys.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeSky * const sky = ( gdeSky* )pSkys.GetAt( i );
		
		if( sky->GetPath() == path ){
			return sky;
		}
	}
	
	return NULL;
}

int gdeSkyList::IndexOf( gdeSky *sky ) const{
	return pSkys.IndexOf( sky );
}

int gdeSkyList::IndexOfWithPath( const char *path ) const{
	const int count = pSkys.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeSky* )pSkys.GetAt( i ) )->GetPath() == path ){
			return i;
		}
	}
	
	return -1;
}

bool gdeSkyList::Has( gdeSky *sky ) const{
	return pSkys.Has( sky );
}

bool gdeSkyList::HasWithPath( const char *path ) const{
	const int count = pSkys.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeSky* )pSkys.GetAt( i ) )->GetPath() == path ){
			return true;
		}
	}
	
	return false;
}

void gdeSkyList::Add( gdeSky *sky ){
	if( ! sky || HasWithPath( sky->GetPath() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pSkys.Add( sky );
}

void gdeSkyList::Remove( gdeSky *sky ){
	pSkys.Remove( sky );
}

void gdeSkyList::RemoveAll(){
	pSkys.RemoveAll();
}



gdeSkyList &gdeSkyList::operator=( const gdeSkyList &list ){
	pSkys = list.pSkys;
	return *this;
}
