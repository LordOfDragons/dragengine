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

#include "deftCachedFont.h"
#include "deftCaches.h"
#include "../deftModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontGlyph.h>


/** Cache version. Increment each time the cache format changed. */
static const int vCacheVersion = 0;


// Class deftCachedFont
/////////////////////////

// Constructor, destructor
////////////////////////////

deftCachedFont::deftCachedFont(deftModule &module, const deFont::Ref &font) :
pModule(module),
pFont(font),
pSdfLineHeight(0){
}

deftCachedFont::~deftCachedFont() = default;


// Management
///////////////

void deftCachedFont::SetSdfLineHeight(int lineHeight){
	DEASSERT_TRUE(lineHeight >= 0)
	DEASSERT_TRUE(lineHeight <= 255)
	
	pSdfLineHeight = lineHeight;
}


bool deftCachedFont::ReadFromCache(decBaseFileReader &reader){
	const auto &filename = pFont->GetFilename();
	
	// valid cache checks
	if((int)reader.ReadVarUInt() != vCacheVersion){
		pModule.LogInfoFormat("Font[%s]: Cache version changed. Cache discarded", filename.GetString());
		return false;
	}
	
	const auto checkTime = (TIME_SYSTEM)reader.ReadUInt();
	if(pModule.GetGameEngine()->GetVirtualFileSystem()->
	GetFileModificationTime(decPath::CreatePathUnix(filename)) != checkTime){
		pModule.LogInfoFormat("Font[%s]: Modification time changed. Cache discarded", filename.GetString());
		return false;
	}
	
	if(!pModule.GetCaches()->ReadVerifyModuleConfig(reader)){
		pModule.LogInfoFormat("Font[%s]: Module configuration changed. Cache discarded", filename.GetString());
		return false;
	}
	
	// cached content
	try{
		const uint8_t flags = reader.ReadByte();
		pFont->SetIsColorFont((flags & 0x1) == 0x1);
		pFont->SetScalable((flags & 0x2) == 0x2);
		pFont->SetIsSdf((flags & 0x4) == 0x4);
		
		pFont->SetLineHeight((int)reader.ReadUInt());
		pFont->SetBaseLine((int)reader.ReadUInt());
		
		const int fixedSizeCount = (int)reader.ReadUInt();
		for(int i=0; i<fixedSizeCount; i++){
			pFont->GetFixedSizes().Add((int)reader.ReadUInt());
		}
		
		const int glyphCount = (int)reader.ReadUInt();
		pFont->SetGlyphCount(glyphCount);
		for(int i=0; i<glyphCount; i++){
			pFont->GetGlyphAt(i).SetUnicode((int)reader.ReadUInt());
		}
		
		pSdfLineHeight = (int)reader.ReadByte();
		
	}catch(const deException &e){
		pModule.LogException(e);
		
		pFont->SetGlyphCount(0);
		pFont->GetFixedSizes().RemoveAll();
		return false;
	}
	
	return true;
}

void deftCachedFont::WriteToCache(decBaseFileWriter &writer) const{
	const auto &filename = pFont->GetFilename();
	
	// valid cache checks
	writer.WriteVarUInt((uint32_t)vCacheVersion);
	writer.WriteUInt((uint32_t)pModule.GetGameEngine()->GetVirtualFileSystem()->
		GetFileModificationTime(decPath::CreatePathUnix(filename)));
	pModule.GetCaches()->WriteModuleConfig(writer);
	
	// cached content
	uint8_t flags = 0;
	if(pFont->GetIsColorFont()){
		flags |= 0x1;
	}
	if(pFont->GetScalable()){
		flags |= 0x2;
	}
	if(pFont->GetIsSdf()){
		flags |= 0x4;
	}
	writer.WriteByte(flags);
	
	writer.WriteUInt((uint32_t)pFont->GetLineHeight());
	writer.WriteUInt((uint32_t)pFont->GetBaseLine());
	
	writer.WriteUInt((uint32_t)pFont->GetFixedSizes().GetCount());
	pFont->GetFixedSizes().Visit([&](int size){
		writer.WriteUInt((uint32_t)size);
	});
	
	const int glyphCount = pFont->GetGlyphCount();
	writer.WriteUInt((uint32_t)glyphCount);
	for(int i=0; i<glyphCount; i++){
		writer.WriteUInt((uint32_t)pFont->GetGlyphAt(i).GetUnicode());
	}
	
	writer.WriteByte((uint8_t)pSdfLineHeight);
}
