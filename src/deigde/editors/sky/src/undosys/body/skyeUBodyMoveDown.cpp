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
#include <string.h>
#include <stdlib.h>

#include "skyeUBodyMoveDown.h"
#include "../../sky/body/skyeBody.h"
#include "../../sky/layer/skyeLayer.h"

#include <dragengine/common/exceptions.h>



// Class skyeUBodyMoveDown
//////////////////////////

// Constructor, destructor
////////////////////////////

skyeUBodyMoveDown::skyeUBodyMoveDown(skyeBody *body) :

pIndex(0)
{
	if(!body){
		DETHROW(deeInvalidParam);
	}
	
	const skyeLayer * const layer = body->GetLayer();
	if(!layer){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Move Body Down");
	
	pIndex = layer->GetBodies().IndexOf(body);
	if(pIndex == -1 || pIndex == layer->GetBodies().GetCount() - 1){
		DETHROW(deeInvalidParam);
	}
	
	pBody = body;
}

skyeUBodyMoveDown::~skyeUBodyMoveDown(){
}



// Management
///////////////

void skyeUBodyMoveDown::Undo(){
	pBody->GetLayer()->MoveBodyTo(pBody, pIndex);
}

void skyeUBodyMoveDown::Redo(){
	pBody->GetLayer()->MoveBodyTo(pBody, pIndex + 1);
}
