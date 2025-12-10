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

#include "deClassFont.h"
#include "../math/deClassPoint.h"
#include "../resources/deClassResourceListener.h"
#include "../string/deClassUnicodeString.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"
#include "../../resourceloader/dedsResourceLoader.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUTF8Decoder.h>
#include <libdscript/exceptions.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontSize.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/systems/deGraphicSystem.h>



// Native Structure
/////////////////////

struct sFntNatDat{
	deFont::Ref font;
	deFontSize::Ref fontSize;
};



// Native Functions
/////////////////////

// public new(String filename, int size)
deClassFont::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsFont, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // filename
	p_AddParameter(init.clsInteger); // size
}
void deClassFont::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sFntNatDat * const nd = new (p_GetNativeData(myself)) sFntNatDat;
	const deScriptingDragonScript &ds = (static_cast<deClassFont*>(GetOwnerClass()))->GetDS();
	deFontManager &fontMgr = *ds.GetGameEngine()->GetFontManager();
	nd->font = nullptr;
	nd->fontSize = nullptr;
	nd.size = 0;
	nd.scale = 1.0f;
	
	// load font
	const char * const filename = rt->GetValue(0)->GetString();
	const int size = rt->GetValue(1)->GetInt();
	if(size < 1){
		DSTHROW(dueInvalidParam);
	}
	
	nd->font = fontMgr.LoadFont(filename, "/");
	nd->fontSize = nd->font->PrepareSize(size);
	nd.size = size;
	
	if(nd->fontSize){
		nd.scale = (float)size / (float)nd->fontSize->GetLineHeight();
		
	}else{
		nd.scale = (float)size / (float)nd->font->GetLineHeight();
	}
}

// public new(Font font, int size)
deClassFont::nfNewSize::nfNewSize(const sInitData &init) :
dsFunction(init.clsFont, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFont); // font
	p_AddParameter(init.clsInteger); // size
}
void deClassFont::nfNewSize::RunFunction(dsRunTime *rt, dsValue *myself){
	sFntNatDat * const nd = new (p_GetNativeData(myself)) sFntNatDat;
	deClassFont &clsFont = *(static_cast<deClassFont*>(GetOwnerClass()));
	nd->font = nullptr;
	nd->fontSize = nullptr;
	nd.size = 0;
	nd.scale = 1.0f;
	
	// store font
	deFont * font = clsFont.GetFont(rt->GetValue(0)->GetRealObject());
	if(!font){
		DSTHROW(dueNullPointer);
	}
	
	const int size = rt->GetValue(1)->GetInt();
	if(size < 1){
		DSTHROW(dueInvalidParam);
	}
	
	nd->font = font;
	font->AddReference();
	nd->fontSize = font->PrepareSize(size);
	nd.size = size;
	
	if(nd->fontSize){
		nd.scale = (float)size / (float)nd->fontSize->GetLineHeight();
		
	}else{
		nd.scale = (float)size / (float)nd->font->GetLineHeight();
	}
}

// static public func void loadAsynchron(String filename, ResourceListener listener)
deClassFont::nfLoadAsynchron::nfLoadAsynchron(const sInitData &init) :
dsFunction(init.clsFont, "loadAsynchron", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsString); // filename
	p_AddParameter(init.clsResourceListener); // listener
}
void deClassFont::nfLoadAsynchron::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = (static_cast<deClassFont*>(GetOwnerClass()))->GetDS();
	
	const char * const filename = rt->GetValue(0)->GetString();
	dsRealObject * const listener = rt->GetValue(1)->GetRealObject();
	if(!listener){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetResourceLoader()->AddRequest(filename, deResourceLoader::ertFont, listener);
}

// public void destructor()
deClassFont::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsFont, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassFont::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sFntNatDat*>(p_GetNativeData(myself))->~sFntNatDat();
}

// public func String getFilename()
deClassFont::nfGetFilename::nfGetFilename(const sInitData &init) :
dsFunction(init.clsFont, "getFilename", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassFont::nfGetFilename::RunFunction(dsRunTime *rt, dsValue *myself){
	const sFntNatDat &nd = *static_cast<sFntNatDat*>(p_GetNativeData(myself));
	rt->PushString(nd.font->GetFilename());
}

// public func int getSize()
deClassFont::nfGetSize::nfGetSize(const sInitData &init) :
dsFunction(init.clsFont, "getSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassFont::nfGetSize::RunFunction(dsRunTime *rt, dsValue *myself){
	const sFntNatDat &nd = *static_cast<sFntNatDat*>(p_GetNativeData(myself));
	rt->PushInt(nd.size);
}

// func float getBaseLine()
deClassFont::nfGetBaseLine::nfGetBaseLine(const sInitData &init) :
dsFunction(init.clsFont, "getBaseLine", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassFont::nfGetBaseLine::RunFunction(dsRunTime *rt, dsValue *myself){
	const sFntNatDat &nd = *static_cast<sFntNatDat*>(p_GetNativeData(myself));
	rt->PushFloat((float)nd.font->GetBaseLine() * nd.scale);
}



// public func Point getTextSize(String text)
deClassFont::nfGetTextSize::nfGetTextSize(const sInitData &init) :
dsFunction(init.clsFont, "getTextSize", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsPoint){
	p_AddParameter(init.clsString); // text
}
void deClassFont::nfGetTextSize::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = (static_cast<deClassFont*>(GetOwnerClass()))->GetDS();
	const sFntNatDat &nd = *static_cast<sFntNatDat*>(p_GetNativeData(myself));
	
	decUTF8Decoder utf8Decoder;
	utf8Decoder.SetString(rt->GetValue(0)->GetString());
	
	int textHeight = nd.size;
	float lineWidth = 0.0f;
	float textWidth = 0.0f;
	
	while(!utf8Decoder.HasReachedEnd()){
		const int character = utf8Decoder.DecodeNextCharacter();
		
		if(character == '\n'){
			textHeight += nd.size;
			lineWidth = 0.0f;
			
		}else if(character >= 0){
			const deFontGlyph &glyph = nd.font->GetGlyph(character, nd.fontSize);
			const float extendEnd = (float)(glyph.GetWidth()
				- glyph.GetAdvance() - glyph.GetBearing()) * nd.scale;
			lineWidth += (float)glyph.GetAdvance() * nd.scale;
			if(lineWidth + extendEnd > textWidth){
				textWidth = lineWidth + extendEnd;
			}
		}
	}
	
	ds.GetClassPoint()->PushPoint(rt, decPoint((int)ceilf(textWidth), textHeight));
}

// public func float getCharWidth(int char)
deClassFont::nfGetCharWidth::nfGetCharWidth(const sInitData &init) :
dsFunction(init.clsFont, "getCharWidth", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsInteger); // char
}
void deClassFont::nfGetCharWidth::RunFunction(dsRunTime *rt, dsValue *myself){
	const sFntNatDat &nd = *static_cast<sFntNatDat*>(p_GetNativeData(myself));
	const int character = rt->GetValue(0)->GetInt();
	
	if(character >= 0){
		rt->PushFloat((float)nd.font->GetGlyph(character, nd.fontSize).GetWidth() * nd.scale);
		
	}else{
		rt->PushFloat(0.0f);
	}
}

// public func float getCharAdvance(int char)
deClassFont::nfGetCharAdvance::nfGetCharAdvance(const sInitData &init) :
dsFunction(init.clsFont, "getCharAdvance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsInteger); // char
}
void deClassFont::nfGetCharAdvance::RunFunction(dsRunTime *rt, dsValue *myself){
	const sFntNatDat &nd = *static_cast<sFntNatDat*>(p_GetNativeData(myself));
	const int character = rt->GetValue(0)->GetInt();
	
	if(character >= 0){
		rt->PushFloat(nd.font->GetGlyph(character, nd.fontSize).GetAdvance() * nd.scale);
		
	}else{
		rt->PushFloat(0.0f);
	}
}

// public func float getCharBearing(int char)
deClassFont::nfGetCharBearing::nfGetCharBearing(const sInitData &init) :
dsFunction(init.clsFont, "getCharBearing", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsInteger); // char
}
void deClassFont::nfGetCharBearing::RunFunction(dsRunTime *rt, dsValue *myself){
	const sFntNatDat &nd = *static_cast<sFntNatDat*>(p_GetNativeData(myself));
	const int character = rt->GetValue(0)->GetInt();
	
	if(character >= 0){
		rt->PushFloat(nd.font->GetGlyph(character, nd.fontSize).GetBearing() * nd.scale);
		
	}else{
		rt->PushFloat(0.0f);
	}
}

// public func float getMaxWidth()
deClassFont::nfGetMaxWidth::nfGetMaxWidth(const sInitData &init) :
dsFunction(init.clsFont, "getMaxWidth", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassFont::nfGetMaxWidth::RunFunction(dsRunTime *rt, dsValue *myself){
	const sFntNatDat &nd = *static_cast<sFntNatDat*>(p_GetNativeData(myself));
	rt->PushFloat((float)(nd.fontSize ? nd.fontSize->GetFontWidth() : nd.font->GetFontWidth()) * nd.scale);
}

// public func bool hasGlyph(int character)
deClassFont::nfHasGlyph::nfHasGlyph(const sInitData &init) :
dsFunction(init.clsFont, "hasGlyph", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsInteger); // char
}
void deClassFont::nfHasGlyph::RunFunction(dsRunTime *rt, dsValue *myself){
	const sFntNatDat &nd = *static_cast<sFntNatDat*>(p_GetNativeData(myself));
	const int character = rt->GetValue(0)->GetInt();
	rt->PushBool(character >= 0 && nd.font->HasGlyph(character));
}



// public func int hashCode()
deClassFont::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsFont, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassFont::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const sFntNatDat &nd = *static_cast<sFntNatDat*>(p_GetNativeData(myself));
	rt->PushInt((int)(intptr_t)nd.font);
}

// public func bool equals( Object obj )
deClassFont::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsFont, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}
void deClassFont::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const sFntNatDat &nd = *static_cast<sFntNatDat*>(p_GetNativeData(myself));
	deClassFont * const clsFont = static_cast<deClassFont*>(GetOwnerClass());
	
	dsValue * const object = rt->GetValue(0);
	
	if(object->GetType()->GetPrimitiveType() != DSPT_OBJECT || !object->GetRealObject()
	|| object->GetRealObject()->GetType() != clsFont){
		rt->PushBool(false);
		
	}else{
		const sFntNatDat &other = *static_cast<sFntNatDat*>(p_GetNativeData(object));
		rt->PushBool(nd.font == other.font && nd.size == other.size);
	}
}



// Class deClassFont
//////////////////////

// Constructor, destructor
////////////////////////////

deClassFont::deClassFont(deScriptingDragonScript &ds) :
dsClass("Font", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds){
	GetParserInfo()->SetParent(DENS_GUI);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sFntNatDat));
}

deClassFont::~deClassFont(){
}



// Management
///////////////

void deClassFont::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsFont = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsByte = engine->GetClassByte();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	
	init.clsPoint = pDS.GetClassPoint();
	init.clsUnicodeString = pDS.GetClassUnicodeString();
	init.clsResourceListener = pDS.GetClassResourceListener();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfNewSize(init));
	AddFunction(new nfLoadAsynchron(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetFilename(init));
	AddFunction(new nfGetSize(init));
	AddFunction(new nfGetBaseLine(init));
	
	AddFunction(new nfGetTextSize(init));
	AddFunction(new nfGetCharWidth(init));
	AddFunction(new nfGetCharAdvance(init));
	AddFunction(new nfGetCharBearing(init));
	AddFunction(new nfGetMaxWidth(init));
	AddFunction(new nfHasGlyph(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

void deClassFont::PushFont(dsRunTime *rt, deFont *font, int size){
	if(!rt || size < 1){
		DSTHROW(dueInvalidParam);
	}
	
	if(!font){
		rt->PushObject(nullptr, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	sFntNatDat &nd = *static_cast<sFntNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.font = font;
	font->AddReference();
	nd.fontSize = font->PrepareSize(size);
	nd.size = size;
	
	if(nd.fontSize){
		nd.fontSize->AddReference();
		nd.scale = (float)size / (float)nd.fontSize->GetLineHeight();
		
	}else{
		nd.scale = (float)size / (float)font->GetLineHeight();
	}
}

deFont *deClassFont::GetFont(dsRealObject *object) const{
	return object ? static_cast<sFntNatDat*>(p_GetNativeData(object->GetBuffer()))->font : nullptr;
}

int deClassFont::GetFontSize(dsRealObject *object) const{
	return object ? static_cast<sFntNatDat*>(p_GetNativeData(object->GetBuffer()))->size : 0;
}
