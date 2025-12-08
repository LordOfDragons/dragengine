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

#include "dealDisplay.h"
#include "dealFont.h"
#include "dealImage.h"
#include "../dealLauncher.h"
#include "../common/exceptions.h"
#include "../common/math/decMath.h"
#include "../common/file/decMemoryFile.h"
#include "../common/file/decMemoryFileReader.h"
#include "../common/utils/decTgaImage.h"
#include "../common/xmlparser/decXmlDocument.h"
#include "../common/xmlparser/decXmlCharacterData.h"
#include "../common/xmlparser/decXmlElementTag.h"
#include "../common/xmlparser/decXmlAttValue.h"
#include "../common/xmlparser/decXmlVisitor.h"
#include "../common/xmlparser/decXmlParser.h"
#include "../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealFont
//////////////////////

// Constructors, destructors
//////////////////////////////

dealFont::dealFont(dealDisplay &display, const char *filename) :
pDisplay(display),

pImage(NULL),

pLineHeight(1),
pGlyphs(NULL),
pGlyphCount(0)
{
	try{
		pLoadXML(filename);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dealFont::~dealFont(){
	pCleanUp();
}



// Management
///////////////

const dealFont::sGlyph &dealFont::GetGlyphFor(int code) const{
	int i;
	
	for(i=0; i<pGlyphCount; i++){
		if(pGlyphs[i].code == code){
			return pGlyphs[i];
		}
	}
	
	return pDefaultGlyph;
}



// Private functions
//////////////////////

void dealFont::pCleanUp(){
	
	if(pGlyphs){
		delete [] pGlyphs;
	}
}

void dealFont::pLoadXML(const char *filename){
	decMemoryFile *memoryFileXML = NULL;
	decMemoryFileReader *reader = NULL;
	decXmlDocument::Ref xmldoc;
	
	try{
		memoryFileXML = new decMemoryFile("xml");
		pDisplay.GetLauncher().LoadAsset(filename, *memoryFileXML);
		
		decXmlParser parser(&pDisplay.GetLauncher().GetLogger());
		
		xmldoc.TakeOver(new decXmlDocument);
		
		reader = new decMemoryFileReader(memoryFileXML);
		parser.ParseXml(reader, xmldoc);
		
		xmldoc->StripComments();
		xmldoc->CleanCharData();
		
		decXmlElementTag * const root = xmldoc->GetRoot();
		if(!root || decString(root->GetName()) != "font"){
			DETHROW(deeInvalidParam);
		}
		
		pReadFont(*root);
		
		reader->FreeReference();
		reader = NULL;
		
		memoryFileXML->FreeReference();
		
	}catch(const deException &){
		if(reader){
			reader->FreeReference();
		}
		if(memoryFileXML){
			memoryFileXML->FreeReference();
		}
		throw;
	}
}

void dealFont::pReadFont(const decXmlElementTag &root){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(strcmp(tag->GetName(), "image") == 0){
			decXmlCharacterData * const cdata = tag->GetFirstData();
			if(!cdata){
				DETHROW(deeInvalidParam);
			}
			if(pImage){
				DETHROW(deeInvalidParam);
			}
			pImage.TakeOver(new dealImage(pDisplay, decString("fonts/") + cdata->GetData()));
			
		}else if(strcmp(tag->GetName(), "lineHeight") == 0){
			decXmlCharacterData * const cdata = tag->GetFirstData();
			if(!cdata){
				DETHROW(deeInvalidParam);
			}
			pLineHeight = decMath::max(decString(cdata->GetData()).ToFloat(), 0.0f);
			
		}else if(strcmp(tag->GetName(), "defaults") == 0){
			const decXmlAttValue *attribute = tag->FindAttribute("u");
			if(attribute){
				pDefaultGlyph.u = decString(attribute->GetValue()).ToInt();
			}
			
			attribute = tag->FindAttribute("v");
			if(attribute){
				pDefaultGlyph.v = decString(attribute->GetValue()).ToInt();
			}
			
			attribute = tag->FindAttribute("width");
			if(attribute){
				pDefaultGlyph.width = decString(attribute->GetValue()).ToInt();
			}
			
		}else if(strcmp(tag->GetName(), "glyph") == 0){
			const decXmlAttValue *attribute = tag->FindAttribute("code");
			if(!attribute){
				DETHROW(deeInvalidParam);
			}
			
			sGlyph &glyph = pGetGlyphForCreate(decString(attribute->GetValue()).ToInt());
			
			attribute = tag->FindAttribute("u");
			if(attribute){
				glyph.u = decString(attribute->GetValue()).ToInt();
			}
			
			attribute = tag->FindAttribute("v");
			if(attribute){
				glyph.v = decString(attribute->GetValue()).ToInt();
			}
			
			attribute = tag->FindAttribute("width");
			if(attribute){
				glyph.width = decString(attribute->GetValue()).ToInt();
			}
			
			attribute = tag->FindAttribute("bearing");
			if(attribute){
				glyph.bearing = decString(attribute->GetValue()).ToInt();
			}
			
			attribute = tag->FindAttribute("advance");
			if(attribute){
				glyph.advance = decString(attribute->GetValue()).ToInt();
			}
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
}

dealFont::sGlyph &dealFont::pGetGlyphForCreate(int code){
	int i;
	for(i=0; i<pGlyphCount; i++){
		if(pGlyphs[i].code == code){
			return pGlyphs[i];
		}
	}
	
	sGlyph * const newArray = new sGlyph[pGlyphCount + 1];
	if(pGlyphs){
		memcpy(newArray, pGlyphs, sizeof(sGlyph) * pGlyphCount);
		delete [] pGlyphs;
	}
	pGlyphs = newArray;
	pGlyphs[pGlyphCount].code = code;
	return pGlyphs[pGlyphCount++];
}
