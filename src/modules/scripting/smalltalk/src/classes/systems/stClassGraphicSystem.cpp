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

#include "stClassGraphicSystem.h"
#include "../graphics/stClassCanvas.h"
#include "../stClassScripting.h"
#include "../../ScriptingSmalltalk.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deCmdLineArgs.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicModule.h>
#include <dragengine/common/exceptions.h>



// Class Struct
/////////////////

struct csGraSysClass : public csClassObject{
	OOP scripting;
};



// Class stClassGraphicSystem
////////////////////////

// Constructor, destructor
////////////////////////////

stClassGraphicSystem::stClassGraphicSystem(ScriptingSmalltalk &st) :
pST(st){
	gst_define_cfunc("DEGraphicSystem.getWindowWidth", (void*)ccGetWindowWidth);
	gst_define_cfunc("DEGraphicSystem.getWindowHeight", (void*)ccGetWindowHeight);
	gst_define_cfunc("DEGraphicSystem.setWindowSize", (void*)ccSetWindowSize);
	gst_define_cfunc("DEGraphicSystem.setWindowTitle", (void*)ccSetWindowTitle);
	gst_define_cfunc("DEGraphicSystem.getPrimaryCanvas", (void*)ccGetPrimaryCanvas);
}

stClassGraphicSystem::~stClassGraphicSystem(){
}



// Management
///////////////

void stClassGraphicSystem::SetUpLinks(){
	OOP oopClass = NULL;
	
	oopClass = gst_class_name_to_oop("DEGraphicSystem");
	if(!oopClass){
		DETHROW(deeInvalidParam); // GraphicSystem.st missing
	}
	
	csGraSysClass &csclass = *((csGraSysClass*)OOP_TO_OBJ(oopClass));
	csclass.scripting = pST.GetClassScripting()->GetSingleton();
}



// cCall Methods
//////////////////

int stClassGraphicSystem::ccGetWindowWidth(OOP self){
	const csGraSysClass &csclass = *((csGraSysClass*)OOP_TO_OBJ(self));
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP(csclass.scripting);
	const deGraphicSystem &grasys = *st.GetGameEngine()->GetGraphicSystem();
	
	return grasys.GetRenderWindow()->GetWidth();
}

int stClassGraphicSystem::ccGetWindowHeight(OOP self){
	const csGraSysClass &csclass = *((csGraSysClass*)OOP_TO_OBJ(self));
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP(csclass.scripting);
	const deGraphicSystem &grasys = *st.GetGameEngine()->GetGraphicSystem();
	
	return grasys.GetRenderWindow()->GetHeight();
}

void stClassGraphicSystem::ccSetWindowSize(OOP self, int width, int height){
	const csGraSysClass &csclass = *((csGraSysClass*)OOP_TO_OBJ(self));
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP(csclass.scripting);
	const deGraphicSystem &grasys = *st.GetGameEngine()->GetGraphicSystem();
	
	grasys.GetRenderWindow()->SetSize(width, height);
}

void stClassGraphicSystem::ccSetWindowTitle(OOP self, const char *title){
	const csGraSysClass &csclass = *((csGraSysClass*)OOP_TO_OBJ(self));
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP(csclass.scripting);
	const deGraphicSystem &grasys = *st.GetGameEngine()->GetGraphicSystem();
	
	grasys.GetRenderWindow()->SetTitle(title);
}

OOP stClassGraphicSystem::ccGetPrimaryCanvas(OOP self){
	const csGraSysClass &csclass = *((csGraSysClass*)OOP_TO_OBJ(self));
	const ScriptingSmalltalk &st = stClassScripting::GetSTFromOOP(csclass.scripting);
	const deGraphicSystem &grasys = *st.GetGameEngine()->GetGraphicSystem();
	
	try{
		return st.GetClassCanvas()->CanvasToOOP(grasys.GetRenderWindow()->GetCanvasView());
		
	}catch(const deException &e){
		e.PrintError();
		return st.GetNil();
	}
}
