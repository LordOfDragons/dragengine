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

#include "igdeSharedFontList.h"

#include <deigde/gui/resources/igdeFont.h>

#include <dragengine/common/exceptions.h>



// Internal Class
///////////////////

igdeSharedFontList::cFont::cFont(const igdeFont::sConfiguration &config, igdeFont *font) :
pConfig(config),
pFont(font){
}

igdeSharedFontList::cFont::~cFont(){
}



// Class igdeSharedFontList
/////////////////////////////

igdeSharedFontList::igdeSharedFontList(igdeEnvironment &environment) :
pEnvironment(environment){
}

igdeSharedFontList::~igdeSharedFontList(){
}



// Management
///////////////

const igdeFont::Ref &igdeSharedFontList::GetFontWith(const igdeFont::sConfiguration &configuration){
	const int count = pFonts.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		cFont * const cfont = static_cast<cFont*>(pFonts.GetAt(i));
		if(configuration == cfont->GetConfig()){
			return cfont->GetFont();
		}
	}
	
	const cFont::Ref cfont(cFont::Ref::New(configuration, igdeFont::Ref::New(pEnvironment, configuration)));
	pFonts.Add(cfont);
	
	return cfont->GetFont();
}
