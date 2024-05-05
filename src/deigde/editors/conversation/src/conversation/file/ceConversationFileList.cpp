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

#include "ceConversationFile.h"
#include "ceConversationFileList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceConversationFile
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationFileList::ceConversationFileList(){
}

ceConversationFileList::~ceConversationFileList(){
}



// Management
///////////////

int ceConversationFileList::GetCount() const{
	return pFiles.GetCount();
}

ceConversationFile *ceConversationFileList::GetAt( int index ) const{
	return ( ceConversationFile* )pFiles.GetAt( index );
}

ceConversationFile *ceConversationFileList::GetWithID( const char *id ) const{
	const int count = pFiles.GetCount();
	ceConversationFile *file;
	int i;
	
	for( i=0; i<count; i++ ){
		file = ( ceConversationFile* )pFiles.GetAt( i );
		
		if( file->GetID().Equals( id ) ){
			return file;
		}
	}
	
	return NULL;
}

int ceConversationFileList::IndexOf( ceConversationFile *file ) const{
	return pFiles.IndexOf( file );
}

int ceConversationFileList::IndexOfWithID( const char *id ) const{
	const int count = pFiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceConversationFile* )pFiles.GetAt( i ) )->GetID().Equals( id ) ){
			return i;
		}
	}
	
	return -1;
}

bool ceConversationFileList::Has( ceConversationFile *file ) const{
	return pFiles.Has( file );
}

bool ceConversationFileList::HasWithID( const char *id ) const{
	const int count = pFiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceConversationFile* )pFiles.GetAt( i ) )->GetID().Equals( id ) ){
			return true;
		}
	}
	
	return false;
}

void ceConversationFileList::Add( ceConversationFile *file ){
	if( ! file || HasWithID( file->GetID().GetString() ) ) DETHROW( deeInvalidParam );
	
	pFiles.Add( file );
}

void ceConversationFileList::Remove( ceConversationFile *file ){
	pFiles.Remove( file );
}

void ceConversationFileList::RemoveAll(){
	pFiles.RemoveAll();
}



ceConversationFileList &ceConversationFileList::operator=( const ceConversationFileList &list ){
	pFiles = list.pFiles;
	return *this;
}

ceConversationFileList &ceConversationFileList::operator+=( const ceConversationFileList &list ){
	pFiles += list.pFiles;
	return *this;
}
