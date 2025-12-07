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

// includes
#include <stdio.h>
#include <stdlib.h>
#include "deEffectFilterKernel.h"
#include "deEffectVisitor.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicEffect.h"



// class deEffectFilterKernel
///////////////////////////////

// constructor, destructor
////////////////////////////

deEffectFilterKernel::deEffectFilterKernel(deEffectManager *manager) :
deEffect(manager),
pKernel(NULL),
pKernelRows(0),
pKernelCols(0),
pScale(1.0f)
{
	SetKernelSize(1, 1);
}

deEffectFilterKernel::~deEffectFilterKernel(){
	if(pKernel){
		delete [] pKernel;
	}
}



// Management
///////////////

void deEffectFilterKernel::SetKernelSize(int rows, int cols){
	if(rows < 1 || cols < 1){
		DETHROW(deeInvalidParam);
	}
	if((rows % 2) == 0 || (cols % 2) == 0){
		DETHROW(deeInvalidParam);
	}
	
	if(rows == pKernelRows && cols == pKernelCols){
		return;
	}
	
	int i, count = rows * cols;
	float *newKernel = new float[count];
	if(! newKernel) DETHROW(deeOutOfMemory);
	if(pKernel) delete [] pKernel;
	pKernel = newKernel;
	pKernelRows = rows;
	pKernelCols = cols;
	for(i=0; i<count; i++){
		pKernel[i] = 0.0f;
	}
	pKernel[((rows - 1) / 2) * cols + (cols - 1) / 2] = 1.0f;
	
	deBaseGraphicEffect * const graEffect = GetPeerGraphic();
	if(graEffect){
		graEffect->FilterKernelChanged();
	}
}

float deEffectFilterKernel::GetKernelValueAt(int row, int col) const{
	if(row < 0 || row >= pKernelRows){
		DETHROW(deeInvalidParam);
	}
	if(col < 0 || col >= pKernelCols){
		DETHROW(deeInvalidParam);
	}
	
	return pKernel[row * pKernelCols + col];
}

void deEffectFilterKernel::SetKernelValueAt(int row, int col, float value){
	if(row < 0 || row >= pKernelRows){
		DETHROW(deeInvalidParam);
	}
	if(col < 0 || col >= pKernelCols){
		DETHROW(deeInvalidParam);
	}
	
	pKernel[row * pKernelCols + col] = value;
	
	deBaseGraphicEffect * const graEffect = GetPeerGraphic();
	if(graEffect){
		graEffect->FilterKernelChanged();
	}
}

void deEffectFilterKernel::SetScale(float scale){
	scale = decMath::max(scale, 0.0f);
	if(fabsf(scale - pScale) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pScale = scale;
	
	deBaseGraphicEffect * const graEffect = GetPeerGraphic();
	if(graEffect){
		graEffect->FilterKernelChanged();
	}
}



// visiting
/////////////

void deEffectFilterKernel::Visit(deEffectVisitor &visitor){
	visitor.VisitFilterKernel(*this);
}
