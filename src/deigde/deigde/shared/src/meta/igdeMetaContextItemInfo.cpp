/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeMetaContextItemInfo.h"


// Class igdeMetaContextItemInfo
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaContextItemInfo::igdeMetaContextItemInfo() = default;
igdeMetaContextItemInfo::~igdeMetaContextItemInfo() = default;


// Management
///////////////

void igdeMetaContextItemInfo::SetText(const decString &name){
	pText = name;
}

void igdeMetaContextItemInfo::SetDescription(const decString &description){
	pDescription = description;
}

void igdeMetaContextItemInfo::SetIcon(const igdeIcon::Ref &icon){
	pIcon = icon;
}

void igdeMetaContextItemInfo::Clear(){
	pText.Empty();
	pDescription.Empty();
	pIcon.Clear();
}

void igdeMetaContextItemInfo::SetAll(const char *text){
	pText = text;
	pDescription.Empty();
	pIcon.Clear();
}

void igdeMetaContextItemInfo::SetAll(const char *text, const char *description){
	pText = text;
	pDescription = description;
	pIcon.Clear();
}

void igdeMetaContextItemInfo::SetAll(const char *text, const char *description, const igdeIcon::Ref &icon){
	pText = text;
	pDescription = description;
	pIcon = icon;
}

void igdeMetaContextItemInfo::SetAll(const char *text, const igdeIcon::Ref &icon){
	pText = text;
	pDescription.Empty();
	pIcon = icon;
}
