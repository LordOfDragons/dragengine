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



#include <time.h>
#include <stdio.h>
#include <string.h>

#include "deClassSystem.h"
#include "../../deScriptingDragonScript.h"

#include <libdscript/exceptions.h>



// Native functions
/////////////////////


// public static func void print(string text)
deClassSystem::nfPrint::nfPrint(const sInitData &init) : dsFunction(init.clsSys, "print",
DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsStr); // text
}
void deClassSystem::nfPrint::RunFunction(dsRunTime *RT, dsValue *This){
	deScriptingDragonScript &ds = *(((deClassSystem*)GetOwnerClass())->GetDS());
	
	ds.LogInfo(RT->GetValue(0)->GetString());
}




// Class deClassSystem
////////////////////////

// Constructors, destructor
/////////////////////////////

deClassSystem::deClassSystem(deScriptingDragonScript *ds) :
dsClass("System", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE){
	pDS = ds;
	
	GetParserInfo()->SetBase("Object");
	p_SetNativeDataSize(0);
	CalcMemberOffsets();
}

deClassSystem::~deClassSystem(){
}



// Management
///////////////

void deClassSystem::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsSys = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsInt = engine->GetClassInt();
	
	AddFunction(new nfPrint(init));
}
