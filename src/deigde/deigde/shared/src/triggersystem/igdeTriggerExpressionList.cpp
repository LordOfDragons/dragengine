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

#include "igdeTriggerExpression.h"
#include "igdeTriggerExpressionList.h"

#include <dragengine/common/exceptions.h>



// Class igdeTriggerExpressionList
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerExpressionList::igdeTriggerExpressionList(){
}

igdeTriggerExpressionList::~igdeTriggerExpressionList(){
}



// Management
///////////////

int igdeTriggerExpressionList::GetCount() const{
	return pExpressions.GetCount();
}

igdeTriggerExpression *igdeTriggerExpressionList::GetAt(int position) const{
	return (igdeTriggerExpression*)pExpressions.GetAt(position);
}

int igdeTriggerExpressionList::IndexOf(igdeTriggerExpression *expression) const{
	return pExpressions.IndexOf(expression);
}

bool igdeTriggerExpressionList::Has(igdeTriggerExpression *expression) const{
	return pExpressions.Has(expression);
}

void igdeTriggerExpressionList::Add(igdeTriggerExpression *expression){
	if(!expression){
		DETHROW(deeInvalidParam);
	}
	
	pExpressions.Add(expression);
}

void igdeTriggerExpressionList::AddIfAbsent(igdeTriggerExpression *expression){
	if(!expression){
		DETHROW(deeInvalidParam);
	}
	
	pExpressions.AddIfAbsent(expression);
}

void igdeTriggerExpressionList::Remove(igdeTriggerExpression *expression){
	if(!expression){
		DETHROW(deeInvalidParam);
	}
	
	pExpressions.Remove(expression);
}

void igdeTriggerExpressionList::RemoveIfPresent(igdeTriggerExpression *expression){
	if(!expression){
		DETHROW(deeInvalidParam);
	}
	
	pExpressions.RemoveIfPresent(expression);
}

void igdeTriggerExpressionList::RemoveAll(){
	pExpressions.RemoveAll();
}
