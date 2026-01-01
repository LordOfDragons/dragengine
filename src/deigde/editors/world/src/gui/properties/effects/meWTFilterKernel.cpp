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

#if 0

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "meWTFilterKernel.h"
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class meWTFilterKernel
///////////////////////////

// Constructor, destructor
////////////////////////////

meWTFilterKernel::meWTFilterKernel(const char *name, int rows, int cols, float scale){
	if(!name || !name[0] || rows < 1 || cols < 1) DETHROW(deeInvalidParam);
	if((rows % 2) == 0 || (cols % 2) == 0) DETHROW(deeInvalidParam);
	int i, size;
	
	pName = nullptr;
	pKernel = nullptr;
	pKernelRows = rows;
	pKernelCols = cols;
	pScale = decMath::max(scale, 0.0f);
	
	try{
		pName = new char[strlen(name) + 1];		strcpy(pName, name);
		
		pKernel = new float[rows * cols]		size = rows * cols;
		for(i=0; i<size; i++) pKernel[i] = 0.0f;
		pKernel[((rows - 1) / 2) * cols + (cols - 1) / 2] = 1.0f;
		
	}catch(const deException &){
		if(pKernel) delete [] pKernel;
		if(pName) delete [] pName;
		throw;
	}
}

meWTFilterKernel::~meWTFilterKernel(){
	if(pKernel) delete [] pKernel;
	if(pName) delete [] pName;
}



// Management
///////////////

float meWTFilterKernel::GetKernelValueAt(int row, int col) const{
	if(row < 0 || row >= pKernelRows) DETHROW(deeInvalidParam);
	if(col < 0 || col >= pKernelCols) DETHROW(deeInvalidParam);
	return pKernel[row * pKernelCols + col];
}

void meWTFilterKernel::SetKernelValueAt(int row, int col, float value){
	if(row < 0 || row >= pKernelRows) DETHROW(deeInvalidParam);
	if(col < 0 || col >= pKernelCols) DETHROW(deeInvalidParam);
	pKernel[row * pKernelCols + col] = value;
}

void meWTFilterKernel::SetScale(float scale){
	pScale = scale;
}

#endif
