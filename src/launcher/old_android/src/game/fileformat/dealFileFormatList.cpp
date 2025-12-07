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

#include "dealFileFormat.h"
#include "dealFileFormatList.h"

#include "../../common/exceptions.h"



// Class dealFileFormatList
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealFileFormatList::dealFileFormatList(){
}

dealFileFormatList::~dealFileFormatList(){
	RemoveAllFormats();
}



// Management
///////////////

int dealFileFormatList::GetFormatCount() const{
	return pFormats.GetCount();
}

dealFileFormat *dealFileFormatList::GetFormatAt(int index) const{
	return (dealFileFormat*)pFormats.GetAt(index);
}

bool dealFileFormatList::HasFormat(dealFileFormat *format) const{
	return pFormats.Has(format);
}

int dealFileFormatList::IndexOfFormat(dealFileFormat *format) const{
	return pFormats.IndexOf(format);
}

void dealFileFormatList::AddFormat(dealFileFormat *format){
	if(!format){
		DETHROW(deeInvalidParam);
	}
	pFormats.Add(format);
}

void dealFileFormatList::RemoveFormat(dealFileFormat *format){
	const int index = IndexOfFormat(format);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	pFormats.RemoveFrom(index);
}

void dealFileFormatList::RemoveAllFormats(){
	pFormats.RemoveAll();
}
