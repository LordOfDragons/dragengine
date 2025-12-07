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

#include "stClassScripting.h"
#include "../ScriptingSmalltalk.h"

#include <dragengine/common/exceptions.h>



// Class Struct
/////////////////

struct csScripting : public csObject{
	ScriptingSmalltalk *st;
};



// Class stClassScripting
///////////////////////////

// Constructor, destructor
////////////////////////////

stClassScripting::stClassScripting(ScriptingSmalltalk *st) :
pST(st),
pSingleton(NULL){
}

stClassScripting::~stClassScripting(){
	FreeSingleton();
}



// Management
///////////////

ScriptingSmalltalk &stClassScripting::GetSTFromOOP(OOP object){
	return *((csScripting*)OOP_TO_OBJ(object))->st;
}

void stClassScripting::CreateSingleton(){
	OOP oopClass = NULL;
	
	oopClass = gst_class_name_to_oop("DEScripting");
	if(!oopClass){
		DETHROW(deeInvalidParam);
	}
	
	pSingleton = pST->CreateNewObject(oopClass, sizeof(csScripting) - sizeof(csObject));
	if(!pSingleton || pSingleton == pST->GetNil()){
		DETHROW(deeOutOfMemory);
	}
	
	gst_register_oop(pSingleton);
	
	csScripting &csobj = *((csScripting*)OOP_TO_OBJ(pSingleton));
	csobj.st = pST;
}

void stClassScripting::FreeSingleton(){
	if(pSingleton){
		gst_unregister_oop(pSingleton);
		pSingleton = NULL;
	}
}
