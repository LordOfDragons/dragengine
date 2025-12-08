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
#include "deEffect.h"
#include "deEffectChain.h"
#include "deEffectVisitor.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// class deEffectChain
////////////////////////

// constructor, destructor
////////////////////////////

deEffectChain::deEffectChain(){
	pEffects = NULL;
	pEffectCount = 0;
	pEffectSize = 0;
}

deEffectChain::~deEffectChain(){
	RemoveAllEffects();
	if(pEffects) delete [] pEffects;
}



// Effect management
//////////////////////

deEffect *deEffectChain::GetEffectAt(int index) const{
	if(index < 0 || index >= pEffectCount) DETHROW(deeInvalidParam);
	return pEffects[index];
}

bool deEffectChain::HasEffect(deEffect::Ref effect) const{
	return IndexOfEffect(effect) != -1;
}

int deEffectChain::IndexOfEffect(deEffect::Ref effect) const{
	if(!effect) DETHROW(deeInvalidParam);
	int i;
	for(i=0; i<pEffectCount; i++){
		if(pEffects[i] == effect) return i;
	}
	return -1;
}

void deEffectChain::AddEffect(deEffect::Ref effect){
	InsertEffect(effect, pEffectCount);
}

void deEffectChain::InsertEffect(deEffect::Ref effect, int position){
	if(HasEffect(effect) || position < 0 || position > pEffectCount) DETHROW(deeInvalidParam);
	int i;
	if(pEffectCount == pEffectSize){
		int newSize = pEffectSize * 3 / 2 + 1;
		deEffect **newArray = new deEffect*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pEffects){
			for(i=0; i<pEffectSize; i++){
				newArray[i] = pEffects[i];
			}
			delete [] pEffects;
		}
		pEffects = newArray;
		pEffectSize = newSize;
	}
	for(i=pEffectCount; i>position; i--){
		pEffects[i] = pEffects[i - 1];
	}
	pEffects[position] = effect;
	pEffectCount++;
}

void deEffectChain::RemoveEffect(deEffect::Ref effect){
	int i, index = IndexOfEffect(effect);
	if(index == -1) DETHROW(deeInvalidParam);
	for(i=index; i<pEffectCount-1; i++){
		pEffects[i] = pEffects[i + 1];
	}
	pEffects[pEffectCount - 1] = NULL;
	pEffectCount--;
}

void deEffectChain::RemoveAllEffects(){
	while(pEffectCount > 0){
		pEffects[pEffectCount - 1]->FreeReference();
		pEffects[pEffectCount - 1] = NULL;
		pEffectCount--;
	}
}

void deEffectChain::MoveEffect(deEffect::Ref effect, int newPosition){
	DETHROW(deeInvalidParam);
}



// Visiting
/////////////

void deEffectChain::VisitEffects(deEffectVisitor &visitor){
	int i;
	for(i=0; i<pEffectCount; i++){
		pEffects[i]->Visit(visitor);
	}
}
