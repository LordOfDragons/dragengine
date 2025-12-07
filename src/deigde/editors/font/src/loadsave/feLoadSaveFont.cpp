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

#include "feLoadSaveFont.h"
#include "../font/feFont.h"
#include "../font/glyph/feFontGlyph.h"
#include "../font/image/feFontImage.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/font/deFontBuilder.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/systems/modules/font/deBaseFontModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/exceptions.h>



// Class feLoadSaveFont
/////////////////////////

// Constructor, destructor
////////////////////////////

feLoadSaveFont::feLoadSaveFont(deBaseFontModule *module) :
pModule(module)
{
	if(!module){
		DETHROW(deeInvalidParam);
	}
	
	const deLoadableModule &loadableModule = module->GetLoadableModule();
	const decStringList &patternList = loadableModule.GetPatternList();
	const int patternCount = patternList.GetCount();
	int i;
	
	pName = loadableModule.GetName();
	for(i=0; i<patternCount; i++){
		if(i > 0){
			pPattern.AppendCharacter(',');
		}
		pPattern.AppendCharacter('*');
		pPattern.Append(patternList.GetAt(i));
	}
}

feLoadSaveFont::~feLoadSaveFont(){
	pCleanUp();
}



// Management
///////////////

void feLoadSaveFont::SetName(const char *name){
	pName = name;
}

void feLoadSaveFont::SetPattern(const char *pattern){
	pPattern = pattern;
}



// Loading and saving
///////////////////////

class cDirectFontLoader : public deFontBuilder{
private:
	deBaseFontModule *pModule;
	decBaseFileReader *pFile;
	decString pVirtualPath;
	deEngine *pEngine;
	
public:
	cDirectFontLoader(deEngine *engine, const char *virtualPath, decBaseFileReader *file, deBaseFontModule *module){
		pModule = module;
		pFile = file;
		pEngine = engine;
		pVirtualPath = virtualPath;
	}
	virtual ~cDirectFontLoader(){}
	
	virtual void BuildFont(deFont *font){
		deImageManager &imageMgr = *pEngine->GetImageManager();
		deImage *image = NULL;
		decPath basePath;
		
		pModule->LoadFont(*pFile, *font);
		
		try{
			basePath.SetFromUnix(pVirtualPath.GetString());
			basePath.RemoveLastComponent();
			
			if(strlen(font->GetImagePath()) > 0){
				image = imageMgr.LoadImage(font->GetImagePath(), basePath.GetPathUnix());
				font->SetImage(image);
				image->FreeReference();
				image = NULL;
			}
			
		}catch(const deException &){
			if(image) image->FreeReference();
			throw;
		}
	}
};

void feLoadSaveFont::LoadFont(const char *virtualPath, feFont *font, decBaseFileReader *file){
	if(!font || !file) DETHROW(deeInvalidParam);
	
	feFontImage &fontImage = *font->GetFontImage();
	deEngine *engine = pModule->GetGameEngine();
	feFontGlyph *glyph = NULL;
	deFont *engFont = NULL;
	
	cDirectFontLoader directFontLoader(engine, virtualPath, file, pModule);
	
	// protect this area
	try{
		// try to load the font
		engFont = engine->GetFontManager()->CreateFont("", directFontLoader);
		
		// store font information
		font->SetFilePath(virtualPath);
		font->SetLineHeight(engFont->GetLineHeight());
		font->SetBaseLine(engFont->GetBaseLine());
		font->SetColorFont(engFont->GetIsColorFont());
		
		// store font image information
		fontImage.SetFilename(engFont->GetImagePath(), true);
		
		// store glyph information
		const int glyphCount = engFont->GetGlyphCount();
		int i;
		
		// TODO engFont->GetUndefinedGlyph()
		
		for(i=0; i<glyphCount; i++){
			const deFontGlyph &engGlyph = engFont->GetGlyphAt(i);
			
			glyph = new feFontGlyph;
			glyph->SetCode(engGlyph.GetUnicode());
			glyph->SetU(engGlyph.GetX());
			glyph->SetV(engGlyph.GetY());
			// engGlyph.GetZ()
			glyph->SetWidth(engGlyph.GetWidth());
			glyph->SetHeight(engGlyph.GetHeight());
			glyph->SetBearing(engGlyph.GetBearing());
			glyph->SetBearingY(engGlyph.GetBearingY());
			glyph->SetAdvance(engGlyph.GetAdvance());
			
			font->AddGlyph(glyph);
			glyph = NULL;
		}
		
		// time to release the font resource
		engFont->FreeReference();
		
	}catch(const deException &e){
		e.PrintError();
		if(glyph){
			glyph->FreeReference();
		}
		if(engFont){
			engFont->FreeReference();
		}
		throw;
	}
}

void feLoadSaveFont::SaveFont(const char *virtualPath, feFont *font, decBaseFileWriter *file){
	if(!font || !file) DETHROW(deeInvalidParam);
	
	deEngine &engine = *pModule->GetGameEngine();
	deImageManager &imgmgr = *engine.GetImageManager();
	
	// this is now another little hack. usually we would have to build a temporary
	// font to save but the font is already existing if nothing is wrong. we rebuild
	// the font if not done already
	font->Rebuild();
	
	// if nothing went wrong we have now a font to save
	if(!font->GetEngineFont()) DETHROW(deeInvalidParam);
	
	pModule->SaveFont(*file, *font->GetEngineFont());
	font->SetFilePath(virtualPath);
	
	// save the fonr image if required
	feFontImage &fontImage = *font->GetFontImage();
	
	if(fontImage.GetChanged() && fontImage.GetEngineImage()){
		decPath path;
		
		path.SetFromUnix(fontImage.GetFilename());
		if(path.IsRelative()){
			path.SetFromUnix(font->GetDirectoryPath());
			path.AddUnixPath(fontImage.GetFilename());
		}
		
		imgmgr.SaveImage(fontImage.GetEngineImage(), path.GetPathUnix());
		
		fontImage.SetSaved(true);
		fontImage.SetChanged(false);
	}
	
}



// Private functions
//////////////////////

void feLoadSaveFont::pCleanUp(){
}
