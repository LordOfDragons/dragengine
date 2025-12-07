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

#include "igdeClipboard.h"
#include "igdeClipboardData.h"

#include <dragengine/common/exceptions.h>



// Class igdeClipboard
////////////////////////

// Constructor, destructor
////////////////////////////

igdeClipboard::igdeClipboard(){
}

igdeClipboard::~igdeClipboard(){
}



// Management
///////////////

int igdeClipboard::GetCount() const{
	return pData.GetCount();
}

bool igdeClipboard::HasWithTypeName(const char *typeName) const{
	const int count = pData.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((igdeClipboardData*)pData.GetAt(i))->GetTypeName() == typeName){
			return true;
		}
	}
	
	return false;
}

igdeClipboardData *igdeClipboard::GetAt(int index) const{
	return (igdeClipboardData*)pData.GetAt(index);
}

igdeClipboardData *igdeClipboard::GetWithTypeName(const char *typeName) const{
	const int count = pData.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		igdeClipboardData * const data = (igdeClipboardData*)pData.GetAt(i);
		if(data->GetTypeName() == typeName){
			return data;
		}
	}
	
	return NULL;
}

void igdeClipboard::Set(igdeClipboardData *data){
	if(!data){
		DETHROW(deeInvalidParam);
	}
	
	Clear(data->GetTypeName());
	pData.Add(data);
}

void igdeClipboard::Clear(const char *typeName){
	igdeClipboardData * const data = GetWithTypeName(typeName);
	if(data){
		pData.Remove(data);
	}
}

void igdeClipboard::ClearAll(){
	pData.RemoveAll();
}
