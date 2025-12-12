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

#include "igdeFont.h"
#include "../native/toolkit.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Struct igdeFont.sConfiguration
///////////////////////////////////

igdeFont::sConfiguration::sConfiguration() :
size(10.0f),
bold(false),
italic(false),
underline(false),
strikeThrough(false){
}

igdeFont::sConfiguration::sConfiguration(const igdeFont::sConfiguration &config) :
name(config.name),
size(config.size),
bold(config.bold),
italic(config.italic),
underline(config.underline),
strikeThrough(config.strikeThrough){
}

igdeFont::sConfiguration &igdeFont::sConfiguration::operator=(const igdeFont::sConfiguration &config){
	name = config.name;
	size = config.size;
	bold = config.bold;
	italic = config.italic;
	underline = config.underline;
	strikeThrough = config.strikeThrough;
	return *this;
}

bool igdeFont::sConfiguration::operator==(const sConfiguration &config) const{
	return name == config.name
		&& fabsf(size - config.size) <= 0.01f
		&& bold == config.bold
		&& italic == config.italic
		&& underline == config.underline
		&& strikeThrough == config.strikeThrough;
}



// Class igdeFont
///////////////////

// Constructor, destructor
////////////////////////////

igdeFont::igdeFont(igdeEnvironment &environment, const sConfiguration &config) :
pEnvironment(environment),
pNativeFont(nullptr),
pName(config.name),
pSize(config.size),
pBold(config.bold),
pItalic(config.italic),
pUnderline(config.underline),
pStrikeThrough(config.strikeThrough)
{
	if(config.size <= 0.0f){
		DETHROW(deeInvalidParam);
	}
	
	pNativeFont = igdeNativeFont::CreateNativeFont(*this);
}

igdeFont::~igdeFont(){
	if(pNativeFont){
		((igdeNativeFont*)pNativeFont)->DestroyNativeFont();
	}
}



// Management
///////////////

void igdeFont::GetConfig(sConfiguration &config) const{
	config.name = pName;
	config.size = pSize;
	config.bold = pBold;
	config.italic = pItalic;
	config.underline = pUnderline;
	config.strikeThrough = pStrikeThrough;
}

decPoint igdeFont::TextSize(const char *text) const{
	DEASSERT_NOTNULL(text)
	return ((igdeNativeFont*)pNativeFont)->TextSize(text);
}

deFont *igdeFont::GetEngineFont(){
	if(!pEngineFont){
		pEngineFont = static_cast<igdeNativeFont*>(pNativeFont)->CreateEngineFont();
	}
	return pEngineFont;
}
