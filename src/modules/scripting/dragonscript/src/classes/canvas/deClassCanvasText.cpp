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

#include <new>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libdscript/libdscript.h>

#include "deClassCanvasText.h"
#include "deClassCanvas.h"
#include "../graphics/deClassColor.h"
#include "../graphics/deClassFont.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasManager.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sCTextNatDat{
	deCanvasText::Ref canvas;
};



// Native Functions
/////////////////////

// public func new()
deClassCanvasText::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCText,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCanvasText::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCTextNatDat * const nd = new (p_GetNativeData(myself)) sCTextNatDat;
	const deScriptingDragonScript &ds = ((deClassCanvasText*)GetOwnerClass())->GetDS();
	
	// super call
	deClassCanvas * const baseClass = (deClassCanvas*)GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create canvas
	nd->canvas = ds.GetGameEngine()->GetCanvasManager()->CreateCanvasText();
	baseClass->AssignCanvas(myself->GetRealObject(), nd->canvas);
}

// public func destructor()
deClassCanvasText::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCText,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCanvasText::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sCTextNatDat*>(p_GetNativeData(myself))->~sCTextNatDat();
}



// Management all types
/////////////////////////

// public func Font getFont()
deClassCanvasText::nfGetFont::nfGetFont(const sInitData &init) : dsFunction(init.clsCText,
"getFont", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFont){
}
void deClassCanvasText::nfGetFont::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCTextNatDat &nd = *static_cast<sCTextNatDat*>(p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasText*)GetOwnerClass())->GetDS();
	
	ds.GetClassFont()->PushFont(rt, nd.canvas->GetFont(), (int)(nd.canvas->GetFontSize() + 0.5f));
}

// public func void setFont( Font font )
deClassCanvasText::nfSetFont::nfSetFont(const sInitData &init) : dsFunction(init.clsCText,
"setFont", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFont); // font
}
void deClassCanvasText::nfSetFont::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCTextNatDat &nd = *static_cast<sCTextNatDat*>(p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasText*)GetOwnerClass())->GetDS();
	
	dsRealObject * const objFont = rt->GetValue(0)->GetRealObject();
	deFont * const font = ds.GetClassFont()->GetFont(objFont);
	const int fontSize = ds.GetClassFont()->GetFontSize(objFont);
	
	nd.canvas->SetFont(font);
	nd.canvas->SetFontSize((float)fontSize);
}

// public func String getText()
deClassCanvasText::nfGetText::nfGetText(const sInitData &init) : dsFunction(init.clsCText,
"getText", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr){
}
void deClassCanvasText::nfGetText::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCTextNatDat &nd = *static_cast<sCTextNatDat*>(p_GetNativeData(myself));
	rt->PushString(nd.canvas->GetText());
}

// public func void setText( String text )
deClassCanvasText::nfSetText::nfSetText(const sInitData &init) : dsFunction(init.clsCText,
"setText", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsStr); // text
}
void deClassCanvasText::nfSetText::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCTextNatDat &nd = *static_cast<sCTextNatDat*>(p_GetNativeData(myself));
	nd.canvas->SetText(rt->GetValue(0)->GetString());
}

// public func Color getColor()
deClassCanvasText::nfGetColor::nfGetColor(const sInitData &init) : dsFunction(init.clsCText,
"getColor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsColor){
}
void deClassCanvasText::nfGetColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCTextNatDat &nd = *static_cast<sCTextNatDat*>(p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasText*)GetOwnerClass())->GetDS();
	
	ds.GetClassColor()->PushColor(rt, nd.canvas->GetColor());
}

// public func void setColor( Color color )
deClassCanvasText::nfSetColor::nfSetColor(const sInitData &init) : dsFunction(init.clsCText,
"setColor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsColor); // color
}
void deClassCanvasText::nfSetColor::RunFunction(dsRunTime *rt, dsValue *myself){
	const sCTextNatDat &nd = *static_cast<sCTextNatDat*>(p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassCanvasText*)GetOwnerClass())->GetDS();
	
	const decColor &color = ds.GetClassColor()->GetColor(rt->GetValue(0)->GetRealObject());
	nd.canvas->SetColor(color);
}



// public func int hashCode()
deClassCanvasText::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsCText, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassCanvasText::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deCanvasText * const canvas = static_cast<sCTextNatDat*>(p_GetNativeData(myself))->canvas;
	// hash code = memory location
	rt->PushInt((int)(intptr_t)canvas);
}

// public func bool equals( Object obj )
deClassCanvasText::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsCText, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassCanvasText::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deCanvasText * const canvas = static_cast<sCTextNatDat*>(p_GetNativeData(myself))->canvas;
	deClassCanvasText * const clsCText = (deClassCanvasText*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsCText)){
		rt->PushBool(false);
		
	}else{
		deCanvasText * const otherCanvas = static_cast<sCTextNatDat*>(p_GetNativeData(obj))->canvas;
		rt->PushBool(canvas == otherCanvas);
	}
}



// Class deClassCanvasText
////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassCanvasText::deClassCanvasText(deScriptingDragonScript &ds) :
dsClass("CanvasText", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Canvas");
	
	p_SetNativeDataSize(sizeof(sCTextNatDat));
}

deClassCanvasText::~deClassCanvasText(){
}



// Management
///////////////

void deClassCanvasText::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsCText = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsFont = pDS.GetClassFont();
	init.clsColor = pDS.GetClassColor();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetFont(init));
	AddFunction(new nfSetFont(init));
	AddFunction(new nfGetText(init));
	AddFunction(new nfSetText(init));
	AddFunction(new nfGetColor(init));
	AddFunction(new nfSetColor(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}



deCanvasText *deClassCanvasText::GetCanvas(dsRealObject *myself) const {
	if(!myself){
		return NULL;
	}
	
	return static_cast<sCTextNatDat*>(p_GetNativeData(myself->GetBuffer()))->canvas;
}

void deClassCanvasText::PushCanvas(dsRunTime *rt, deCanvasText *canvas){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!canvas){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassCanvas * const baseClass = (deClassCanvas*)GetBaseClass();
	rt->CreateObjectNakedOnStack(this);
	sCTextNatDat &nd = *static_cast<sCTextNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.canvas = NULL;
	
	try{
		baseClass->CallBaseClassConstructor(rt, rt->GetValue(0), baseClass->GetFirstConstructor(), 0);
		
		canvas->AddReference();
		nd.canvas = canvas;
		
		baseClass->AssignCanvas(rt->GetValue(0)->GetRealObject(), canvas);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
