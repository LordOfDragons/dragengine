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

#include "meByteArray.h"

#include <dragengine/common/exceptions.h>



// Class meByteArray
//////////////////////

// Constructor, destructor
////////////////////////////

meByteArray::meByteArray(int colons, int rows) :
pColons(colons),
pRows(rows)
{
	if(colons < 1 || rows < 1) {
		DETHROW(deeInvalidParam);
	}
	
	pValues.SetAll(colons * rows, 0);
}

meByteArray::meByteArray(const meByteArray &array) :
pColons(array.pColons),
pRows(array.pRows),
pValues(array.pValues){
}

meByteArray::~meByteArray() = default;



// Management
///////////////

int meByteArray::GetValueAt(int x, int y) const{
	if(x < 0 || x >= pColons || y < 0 || y >= pRows){
		DETHROW(deeInvalidParam);
	}
	return (int)pValues[y * pColons + x];
}

void meByteArray::SetValueAt(int x, int y, int value){
	if(x < 0 || x >= pColons || y < 0 || y >= pRows){
		DETHROW(deeInvalidParam);
	}
	pValues[y * pColons + x] = (unsigned char)value;
}

void meByteArray::SetAll(int value){
	pValues.SetRangeAt(0, pValues.GetCount(), (unsigned char)value);
}



// Operators
//////////////

bool meByteArray::operator==(const meByteArray &array) const{
	return pColons == array.pColons && pRows == array.pRows
		&& memcmp(pValues.GetArrayPointer(), array.pValues.GetArrayPointer(), pColons * pRows) == 0;
}

meByteArray &meByteArray::operator=(const meByteArray &array){
	if(array.pColons != pColons || array.pRows != pRows){
		DETHROW(deeInvalidParam);
	}
	pValues = array.pValues;
	return *this;
}
