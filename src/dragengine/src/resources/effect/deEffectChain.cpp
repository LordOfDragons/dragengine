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

#include "deEffectChain.h"
#include "deEffectVisitor.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// class deEffectChain
////////////////////////

// constructor, destructor
////////////////////////////

deEffectChain::deEffectChain(){
}

deEffectChain::~deEffectChain(){
}



// Effect management
//////////////////////

const deEffect::Ref &deEffectChain::GetEffectAt(int index) const{
	return pEffects.GetAt(index);
}

bool deEffectChain::HasEffect(deEffect *effect) const{
	return pEffects.Has(effect);
}

int deEffectChain::IndexOfEffect(deEffect *effect) const{
	return pEffects.IndexOf(effect);
}

void deEffectChain::AddEffect(deEffect *effect){
	pEffects.Add(effect);
}

void deEffectChain::InsertEffect(deEffect *effect, int position){
	pEffects.Insert(effect, position);
}

void deEffectChain::RemoveEffect(deEffect *effect){
	pEffects.Remove(effect);
}

void deEffectChain::RemoveAllEffects(){
	pEffects.RemoveAll();
}

void deEffectChain::MoveEffect(deEffect *effect, int newPosition){
	DETHROW(deeInvalidParam);
}



// Visiting
/////////////

void deEffectChain::VisitEffects(deEffectVisitor &visitor){
	pEffects.Visit([&visitor](deEffect &effect){
		effect.Visit(visitor);
	});
}
