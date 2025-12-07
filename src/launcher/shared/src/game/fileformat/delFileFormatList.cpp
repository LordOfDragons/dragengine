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

#include "delFileFormat.h"
#include "delFileFormatList.h"

#include <dragengine/common/exceptions.h>



// Class delFileFormatList
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delFileFormatList::delFileFormatList(){
}

delFileFormatList::~delFileFormatList(){
	RemoveAll();
}



// Management
///////////////

int delFileFormatList::GetCount() const{
	return pFormats.GetCount();
}

delFileFormat *delFileFormatList::GetAt(int index) const{
	return (delFileFormat*)pFormats.GetAt(index);
}

bool delFileFormatList::Has(delFileFormat *format) const{
	return pFormats.Has(format);
}

int delFileFormatList::IndexOf(delFileFormat *format) const{
	return pFormats.IndexOf(format);
}

void delFileFormatList::Add(delFileFormat *format){
	if(! format){
		DETHROW_INFO(deeNullPointer, "format");
	}
	
	pFormats.Add(format);
}

void delFileFormatList::Remove(delFileFormat *format){
	const int index = IndexOf (format);
	if(index == -1){
		DETHROW_INFO(deeInvalidParam, "format is absent");
	}
	
	pFormats.RemoveFrom(index);
}

void delFileFormatList::RemoveAll(){
	pFormats.RemoveAll();
}
