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

#include "deAnimatorRuleMirror.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/collection/decTList.h"



// Class deAnimatorRuleMirror
///////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleMirror::deAnimatorRuleMirror() :
pMirrorAxis(emaX),
pEnablePosition(true),
pEnableOrientation(true),
pEnableSize(false),
pEnableVertexPositionSet(true){
}

deAnimatorRuleMirror::~deAnimatorRuleMirror(){
}



// Management
///////////////

void deAnimatorRuleMirror::SetMirrorAxis(eMirrorAxis axis){
	pMirrorAxis = axis;
}

void deAnimatorRuleMirror::SetMirrorBone(const char *boneName){
	pMirrorBone = boneName;
}

const deAnimatorRuleMirror::sMatchName &deAnimatorRuleMirror::GetMatchNameAt(int index) const{
	return pMatchNames.GetAt(index);
}

void deAnimatorRuleMirror::AddMatchName(const char *first, const char *second, eMatchNameType type){
	DEASSERT_NOTNULL(first)
	DEASSERT_TRUE(strlen(first) > 0)
	DEASSERT_NOTNULL(second)
	DEASSERT_TRUE(strlen(second) > 0)
	
	pMatchNames.Add({first, second, type});
}

void deAnimatorRuleMirror::RemoveAllMatchNames(){
	pMatchNames.RemoveAll();
}

void deAnimatorRuleMirror::SetEnablePosition(bool enable){
	pEnablePosition = enable;
}

void deAnimatorRuleMirror::SetEnableOrientation(bool enable){
	pEnableOrientation = enable;
}

void deAnimatorRuleMirror::SetEnableSize(bool enable){
	pEnableSize = enable;
}

void deAnimatorRuleMirror::SetEnableVertexPositionSet(bool enabled){
	pEnableVertexPositionSet = enabled;
}



// Visiting
/////////////

void deAnimatorRuleMirror::Visit(deAnimatorRuleVisitor &visitor){
	visitor.VisitMirror(*this);
}
