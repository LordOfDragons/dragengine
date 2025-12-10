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

#include "deFont.h"
#include "deFontBuilder.h"
#include "deFontManager.h"
#include "../image/deImage.h"
#include "../image/deImageManager.h"
#include "../skin/deSkin.h"
#include "../skin/deSkinManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../errortracing/deErrorTrace.h"
#include "../../errortracing/deErrorTracePoint.h"
#include "../../errortracing/deErrorTraceValue.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/font/deBaseFontModule.h"



// Class deFontManager
////////////////////////

// Constructor, destructor
////////////////////////////

deFontManager::deFontManager(deEngine *engine) :
deFileResourceManager(engine, ertFont)
{
	SetLoggingName("font");
}

deFontManager::~deFontManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deFontManager::GetFontCount() const{
	return pFonts.GetCount();
}

deFont *deFontManager::GetRootFont() const{
	return (deFont*)pFonts.GetRoot();
}

deFont *deFontManager::GetFontWith(const char *filename) const{
	return GetFontWith(GetEngine()->GetVirtualFileSystem(), filename);
}

deFont *deFontManager::GetFontWith(deVirtualFileSystem *vfs, const char *filename) const{
	deFont * const font = (deFont*)pFonts.GetWithFilename(vfs, filename);
	return font && !font->GetOutdated() ? font : NULL;
}

deFont::Ref deFontManager::CreateFont(const char *filename, deFontBuilder &builder){
	return CreateFont(GetEngine()->GetVirtualFileSystem(), filename, builder);
}

deFont::Ref deFontManager::CreateFont(deVirtualFileSystem *vfs, const char *filename,
deFontBuilder &builder){
	if(!vfs || !filename){
		DETHROW(deeInvalidParam);
	}
	
	deFont::Ref font;
	
	try{
		// check if the animation with this filename already exists
		if(filename[0] != '\0'){
			deFont * const findFont = (deFont*)pFonts.GetWithFilename(vfs, filename);
			if(findFont && !findFont->GetOutdated()){
				DETHROW(deeInvalidParam);
			}
		}
		
		// create animation using the builder
		font.TakeOver(new deFont(this, vfs, filename, decDateTime::GetSystemTime()));
		builder.BuildFont(font);
		
		// check and init
		if(!font->Verify()){
			DETHROW(deeInvalidParam);
		}
		font->UpdateGlyphs();
		
		// load into systems
		GetGraphicSystem()->LoadFont(font);
		
		// add animation
		pFonts.Add(font);
		
	}catch(const deException &){
		LogErrorFormat("Creating font '%s' failed", filename);
		throw;
	}
	
	return font;
}

deFont::Ref deFontManager::LoadFont(const char *filename, const char *basePath){
	return LoadFont(GetEngine()->GetVirtualFileSystem(), filename, basePath);
}

deFont::Ref deFontManager::LoadFont(deVirtualFileSystem *vfs, const char *filename,
const char *basePath){
	deFont::Ref font;
	decPath path;
	
	try{
		// locate file
		if(!FindFileForReading(path, *vfs, filename, basePath)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		const TIME_SYSTEM modificationTime = vfs->GetFileModificationTime(path);
		
		deFont *findFont = (deFont*)pFonts.GetWithFilename(vfs, path.GetPathUnix());
		
		if(findFont && findFont->GetModificationTime() != modificationTime){
			LogInfoFormat("Font '%s' (base path '%s') changed on VFS: Outdating and Reloading",
				filename, basePath ? basePath : "");
			findFont->MarkOutdated();
			findFont = NULL;
		}
		
		if(findFont){
			font = findFont;
			
		}else{
			// find the module able to handle this model file
			deBaseFontModule * const module = (deBaseFontModule*)GetModuleSystem()
				->GetModuleAbleToLoad(deModuleSystem::emtFont, path.GetPathUnix());
			
			// load the file with it
			font.TakeOver(new deFont(this, vfs, path.GetPathUnix(), modificationTime));
			font->SetAsynchron(false);
			
			module->LoadFont(OpenFileForReading(*vfs, path.GetPathUnix()), *font);
			
			// check and init
			pLoadFontSources(font);
			
			if(!font->Verify()){
				DETHROW(deeInvalidParam);
			}
			font->UpdateGlyphs();
			
			// load into systems
			GetGraphicSystem()->LoadFont(font);
			
			// add font
			pFonts.Add(font);
		}
		
	}catch(const deException &){
		LogErrorFormat("Loading font '%s' (base path '%s') failed",
			filename, basePath ? basePath : "");
		throw;
	}
	
	return font;
}

#define FONT_DEBUG "<debug_font>"
#define FONT_DEBUG_IMAGE "<debug_font_image>"
#include "../../debug/debugfont.c"

deFont::Ref deFontManager::LoadDebugFont(){
	deFont * const findFont = (deFont*)pFonts.GetWithFilename(
		GetEngine()->GetVirtualFileSystem(), FONT_DEBUG);
	if(findFont){
		if(!findFont->GetPeerGraphic()){
			GetGraphicSystem()->LoadFont(findFont);
		}
		return deFont::Ref(findFont);
	}
	
	struct sTempGlyph{
		int u, v, width;
	} tempGlyphs[224] = {
		{1, 1, 6}, /* glyph 32 */
		{8, 1, 6}, /* glyph 33 */
		{15, 1, 6}, /* glyph 34 */
		{22, 1, 6}, /* glyph 35 */
		{29, 1, 6}, /* glyph 36 */
		{36, 1, 6}, /* glyph 37 */
		{43, 1, 6}, /* glyph 38 */
		{50, 1, 6}, /* glyph 39 */
		{57, 1, 6}, /* glyph 40 */
		{64, 1, 6}, /* glyph 41 */
		{71, 1, 6}, /* glyph 42 */
		{78, 1, 6}, /* glyph 43 */
		{85, 1, 6}, /* glyph 44 */
		{92, 1, 6}, /* glyph 45 */
		{99, 1, 6}, /* glyph 46 */
		{106, 1, 6}, /* glyph 47 */
		{113, 1, 6}, /* glyph 48 */
		{120, 1, 6}, /* glyph 49 */
		{1, 12, 6}, /* glyph 50 */
		{8, 12, 6}, /* glyph 51 */
		{15, 12, 6}, /* glyph 52 */
		{22, 12, 6}, /* glyph 53 */
		{29, 12, 6}, /* glyph 54 */
		{36, 12, 6}, /* glyph 55 */
		{43, 12, 6}, /* glyph 56 */
		{50, 12, 6}, /* glyph 57 */
		{57, 12, 6}, /* glyph 58 */
		{64, 12, 6}, /* glyph 59 */
		{71, 12, 6}, /* glyph 60 */
		{78, 12, 6}, /* glyph 61 */
		{85, 12, 6}, /* glyph 62 */
		{92, 12, 6}, /* glyph 63 */
		{99, 12, 6}, /* glyph 64 */
		{106, 12, 6}, /* glyph 65 */
		{113, 12, 6}, /* glyph 66 */
		{120, 12, 6}, /* glyph 67 */
		{1, 23, 6}, /* glyph 68 */
		{8, 23, 6}, /* glyph 69 */
		{15, 23, 6}, /* glyph 70 */
		{22, 23, 6}, /* glyph 71 */
		{29, 23, 6}, /* glyph 72 */
		{36, 23, 6}, /* glyph 73 */
		{43, 23, 6}, /* glyph 74 */
		{50, 23, 6}, /* glyph 75 */
		{57, 23, 6}, /* glyph 76 */
		{64, 23, 6}, /* glyph 77 */
		{71, 23, 6}, /* glyph 78 */
		{78, 23, 6}, /* glyph 79 */
		{85, 23, 6}, /* glyph 80 */
		{92, 23, 6}, /* glyph 81 */
		{99, 23, 6}, /* glyph 82 */
		{106, 23, 6}, /* glyph 83 */
		{113, 23, 6}, /* glyph 84 */
		{120, 23, 6}, /* glyph 85 */
		{1, 34, 6}, /* glyph 86 */
		{8, 34, 6}, /* glyph 87 */
		{15, 34, 6}, /* glyph 88 */
		{22, 34, 6}, /* glyph 89 */
		{29, 34, 6}, /* glyph 90 */
		{36, 34, 6}, /* glyph 91 */
		{43, 34, 6}, /* glyph 92 */
		{50, 34, 6}, /* glyph 93 */
		{57, 34, 6}, /* glyph 94 */
		{64, 34, 6}, /* glyph 95 */
		{71, 34, 6}, /* glyph 96 */
		{78, 34, 6}, /* glyph 97 */
		{85, 34, 6}, /* glyph 98 */
		{92, 34, 6}, /* glyph 99 */
		{99, 34, 6}, /* glyph 100 */
		{106, 34, 6}, /* glyph 101 */
		{113, 34, 6}, /* glyph 102 */
		{120, 34, 6}, /* glyph 103 */
		{1, 45, 6}, /* glyph 104 */
		{8, 45, 6}, /* glyph 105 */
		{15, 45, 6}, /* glyph 106 */
		{22, 45, 6}, /* glyph 107 */
		{29, 45, 6}, /* glyph 108 */
		{36, 45, 6}, /* glyph 109 */
		{43, 45, 6}, /* glyph 110 */
		{50, 45, 6}, /* glyph 111 */
		{57, 45, 6}, /* glyph 112 */
		{64, 45, 6}, /* glyph 113 */
		{71, 45, 6}, /* glyph 114 */
		{78, 45, 6}, /* glyph 115 */
		{85, 45, 6}, /* glyph 116 */
		{92, 45, 6}, /* glyph 117 */
		{99, 45, 6}, /* glyph 118 */
		{106, 45, 6}, /* glyph 119 */
		{113, 45, 6}, /* glyph 120 */
		{120, 45, 6}, /* glyph 121 */
		{1, 56, 6}, /* glyph 122 */
		{8, 56, 6}, /* glyph 123 */
		{15, 56, 6}, /* glyph 124 */
		{22, 56, 6}, /* glyph 125 */
		{29, 56, 6}, /* glyph 126 */
		{36, 56, 6}, /* glyph 160 */
		{43, 56, 6}, /* glyph 161 */
		{50, 56, 6}, /* glyph 162 */
		{57, 56, 6}, /* glyph 163 */
		{64, 56, 6}, /* glyph 164 */
		{71, 56, 6}, /* glyph 165 */
		{78, 56, 6}, /* glyph 166 */
		{85, 56, 6}, /* glyph 167 */
		{92, 56, 6}, /* glyph 168 */
		{99, 56, 6}, /* glyph 169 */
		{106, 56, 6}, /* glyph 170 */
		{113, 56, 6}, /* glyph 171 */
		{120, 56, 6}, /* glyph 172 */
		{1, 67, 6}, /* glyph 173 */
		{8, 67, 6}, /* glyph 174 */
		{15, 67, 6}, /* glyph 175 */
		{22, 67, 6}, /* glyph 176 */
		{29, 67, 6}, /* glyph 177 */
		{36, 67, 6}, /* glyph 178 */
		{43, 67, 6}, /* glyph 179 */
		{50, 67, 6}, /* glyph 180 */
		{57, 67, 6}, /* glyph 181 */
		{64, 67, 6}, /* glyph 182 */
		{71, 67, 6}, /* glyph 183 */
		{78, 67, 6}, /* glyph 184 */
		{85, 67, 6}, /* glyph 185 */
		{92, 67, 6}, /* glyph 186 */
		{99, 67, 6}, /* glyph 187 */
		{106, 67, 6}, /* glyph 188 */
		{113, 67, 6}, /* glyph 189 */
		{120, 67, 6}, /* glyph 190 */
		{1, 78, 6}, /* glyph 191 */
		{8, 78, 6}, /* glyph 192 */
		{15, 78, 6}, /* glyph 193 */
		{22, 78, 6}, /* glyph 194 */
		{29, 78, 6}, /* glyph 195 */
		{36, 78, 6}, /* glyph 196 */
		{43, 78, 6}, /* glyph 197 */
		{50, 78, 6}, /* glyph 198 */
		{57, 78, 6}, /* glyph 199 */
		{64, 78, 6}, /* glyph 200 */
		{71, 78, 6}, /* glyph 201 */
		{78, 78, 6}, /* glyph 202 */
		{85, 78, 6}, /* glyph 203 */
		{92, 78, 6}, /* glyph 204 */
		{99, 78, 6}, /* glyph 205 */
		{106, 78, 6}, /* glyph 206 */
		{113, 78, 6}, /* glyph 207 */
		{120, 78, 6}, /* glyph 208 */
		{1, 89, 6}, /* glyph 209 */
		{8, 89, 6}, /* glyph 210 */
		{15, 89, 6}, /* glyph 211 */
		{22, 89, 6}, /* glyph 212 */
		{29, 89, 6}, /* glyph 213 */
		{36, 89, 6}, /* glyph 214 */
		{43, 89, 6}, /* glyph 215 */
		{50, 89, 6}, /* glyph 216 */
		{57, 89, 6}, /* glyph 217 */
		{64, 89, 6}, /* glyph 218 */
		{71, 89, 6}, /* glyph 219 */
		{78, 89, 6}, /* glyph 220 */
		{85, 89, 6}, /* glyph 221 */
		{92, 89, 6}, /* glyph 222 */
		{99, 89, 6}, /* glyph 223 */
		{106, 89, 6}, /* glyph 224 */
		{113, 89, 6}, /* glyph 225 */
		{120, 89, 6}, /* glyph 226 */
		{1, 100, 6}, /* glyph 227 */
		{8, 100, 6}, /* glyph 228 */
		{15, 100, 6}, /* glyph 229 */
		{22, 100, 6}, /* glyph 230 */
		{29, 100, 6}, /* glyph 231 */
		{36, 100, 6}, /* glyph 232 */
		{43, 100, 6}, /* glyph 233 */
		{50, 100, 6}, /* glyph 234 */
		{57, 100, 6}, /* glyph 235 */
		{64, 100, 6}, /* glyph 236 */
		{71, 100, 6}, /* glyph 237 */
		{78, 100, 6}, /* glyph 238 */
		{85, 100, 6}, /* glyph 239 */
		{92, 100, 6}, /* glyph 240 */
		{99, 100, 6}, /* glyph 241 */
		{106, 100, 6}, /* glyph 242 */
		{113, 100, 6}, /* glyph 243 */
		{120, 100, 6}, /* glyph 244 */
		{1, 111, 6}, /* glyph 245 */
		{8, 111, 6}, /* glyph 246 */
		{15, 111, 6}, /* glyph 247 */
		{22, 111, 6}, /* glyph 248 */
		{29, 111, 6}, /* glyph 249 */
		{36, 111, 6}, /* glyph 250 */
		{43, 111, 6}, /* glyph 251 */
		{50, 111, 6}, /* glyph 252 */
		{57, 111, 6}, /* glyph 253 */
		{64, 111, 6}, /* glyph 254 */
		{71, 111, 6}, /* glyph 255 */
	};
	
	deFont::Ref font;
	int i;
	
	try{
		const deImage::Ref image(GetImageManager()->CreateImage(
			gimp_debugfont.width, gimp_debugfont.height, 1, 4, 8));
		
		GIMP_DEBUGFONT_RUN_LENGTH_DECODE((unsigned char *)image->GetDataRGBA8(),
			gimp_debugfont.rle_pixel_data, gimp_debugfont.width * gimp_debugfont.height,
			gimp_debugfont.bytes_per_pixel);
		image->NotifyImageDataChanged();
		
		font.TakeOver(new deFont(this, GetEngine()->GetVirtualFileSystem(),
			FONT_DEBUG, decDateTime::GetSystemTime()));
		font->SetImage(image);
		font->SetImagePath(FONT_DEBUG_IMAGE);
		font->SetIsColorFont(false);
		font->SetLineHeight(10);
		
		font->SetGlyphCount(224);
		for(i=0; i<224; i++){
			deFontGlyph &glyph = font->GetGlyphAt(i);
			glyph.SetUnicode(32 + i);
			glyph.SetX(tempGlyphs[i].u);
			glyph.SetY(tempGlyphs[i].v);
			glyph.SetWidth(tempGlyphs[i].width);
			glyph.SetHeight(font->GetLineHeight());
			glyph.SetAdvance(tempGlyphs[i].width);
		}
		
		if(!font->Verify()){
			DETHROW(deeInvalidParam);
		}
		font->UpdateGlyphs();
		GetGraphicSystem()->LoadFont(font);
		
		pFonts.Add(font);
		
	}catch(const deException &){
		throw;
	}
	
	return font;
}



void deFontManager::ReleaseLeakingResources(){
	const int count = GetFontCount();
	if(count == 0){
		return;
	}
	
	LogWarnFormat("%i leaking fonts", count);
	
	deFont *font = (deFont*)pFonts.GetRoot();
	while(font){
		LogWarnFormat("- %s", font->GetFilename().GetString());
		font = (deFont*)font->GetLLManagerNext();
	}
	
	pFonts.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
}

void deFontManager::AddLoadedFont(deFont *font){
	DEASSERT_NOTNULL(font)
	
	pFonts.Add(font);
}

deFontSize *deFontManager::LoadFontSize(deFont &font, int size){
	if(!(font.GetScalable() || font.GetFixedSizes().Has(size))){
		return nullptr;
	}
	
	deBaseFontModule * const module = (deBaseFontModule*)GetModuleSystem()->
		GetModuleAbleToLoad(deModuleSystem::emtFont, font.GetFilename());
		
	try{
		DEASSERT_NOTNULL(module)
		deFontSize * const found = font.GetSizeWith(size);
		if(found){
			return found;
		}
		
		return font.AddSize(size, module->LoadFontSize(OpenFileForReading(
				*font.GetVirtualFileSystem(), font.GetFilename()),
			deFont::Ref(&font), size));
		
	}catch(const deException &e){
		LogErrorFormat("Load font size failed: %s", font.GetFilename().GetString());
		LogException(e);
		
		deErrorTrace &et = *GetEngine()->GetErrorTrace();
		et.AddAndSetIfEmpty(e.GetName(), nullptr, e.GetFile(), e.GetLine());
		
		deErrorTracePoint &etp = *et.AddPoint(nullptr, "deFontManager::LoadFontSize", __LINE__);
		
		deErrorTraceValue &etv = *etp.AddValue("font", "<deFont>");
		etv.AddSubValue("filename", font.GetFilename());
		etv.AddSubValueInt("size", size);
		if(module){
			etp.AddValue("module", module->GetLoadableModule().GetName());
		}
		throw;
	}
}

void deFontManager::RemoveResource(deResource *resource){
	pFonts.RemoveIfPresent(resource);
}



// Systems Support
////////////////////

void deFontManager::SystemGraphicLoad(){
	deGraphicSystem &graSys = *GetGraphicSystem();
	deFont *font = (deFont*)pFonts.GetRoot();
	
	while(font){
		if(!font->GetPeerGraphic()){
			graSys.LoadFont(font);
		}
		font = (deFont*)font->GetLLManagerNext();
	}
}

void deFontManager::SystemGraphicUnload(){
	deFont *font = (deFont*)pFonts.GetRoot();
	while(font){
		font->SetPeerGraphic(NULL);
		font = (deFont*)font->GetLLManagerNext();
	}
}



// Private Functions
//////////////////////

void deFontManager::pLoadFontSources(deFont *font){
	if(font->GetImagePath().IsEmpty()){
		return;
	}
	
	decPath basePath(decPath::CreatePathUnix(font->GetFilename()));
	basePath.RemoveLastComponent();
	
	font->SetImage(GetImageManager()->LoadImage(font->GetVirtualFileSystem(),
		font->GetImagePath(), basePath.GetPathUnix()));
}
