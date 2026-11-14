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

#include "deftCachedFontSize.h"
#include "deftCaches.h"
#include "../deftModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>


/** Cache version. Increment each time the cache format changed. */
static const int vCacheVersion = 0;


// Class deftCachedFontSize
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deftCachedFontSize::deftCachedFontSize(deftModule &module, const deFont::Ref &font,
	const deFontSize::Ref &fontSize) :
pModule(module),
pFont(font),
pFontSize(fontSize){
}

deftCachedFontSize::~deftCachedFontSize() = default;


// Management
///////////////

bool deftCachedFontSize::ReadFromCache(decBaseFileReader &reader){
	const auto &filename = pFont->GetFilename();
	
	// valid cache checks
	if((int)reader.ReadVarUInt() != vCacheVersion){
		pModule.LogInfoFormat("Font[%s:%d]: Cache version changed. Cache discarded",
			filename.GetString(), pFontSize->GetLineHeight());
		return false;
	}
	
	const auto checkTime = (TIME_SYSTEM)reader.ReadUInt();
	if(pModule.GetGameEngine()->GetVirtualFileSystem()->
	GetFileModificationTime(decPath::CreatePathUnix(filename)) != checkTime){
		pModule.LogInfoFormat("Font[%s:%d]: Modification time changed. Cache discarded",
			filename.GetString(), pFontSize->GetLineHeight());
		return false;
	}
	
	if(!pModule.GetCaches()->ReadVerifyModuleConfig(reader)){
		pModule.LogInfoFormat("Font[%s]: Module configuration changed. Cache discarded", filename.GetString());
		return false;
	}
	
	// cached content
	try{
		const int glyphCount = (int)reader.ReadUShort();
		DEASSERT_TRUE(pFontSize->GetGlyphs().GetCount() == glyphCount)
		for(int i=0; i<glyphCount; i++){
			auto &glyph = pFontSize->GetGlyphAt(i);
			glyph.SetUnicode((int)reader.ReadUShort());
			glyph.SetX((int)reader.ReadUShort());
			glyph.SetY((int)reader.ReadUShort());
			glyph.SetZ((int)reader.ReadUShort());
			glyph.SetWidth((int)reader.ReadUShort());
			glyph.SetHeight((int)reader.ReadUShort());
			glyph.SetBearing((int)reader.ReadShort());
			glyph.SetBearingY((int)reader.ReadShort());
			glyph.SetAdvance((int)reader.ReadUShort());
		}
		
		pFontSize->SetSdfPixelRange(reader.ReadFloat());
		pFontSize->SetSdfDistanceRange(reader.ReadVector2());
		
		decPoint3 imageSize;
		imageSize.x = (int)reader.ReadUShort();
		imageSize.y = (int)reader.ReadUShort();
		imageSize.z = (int)reader.ReadUShort();
		const int imageComponentCount = (int)reader.ReadByte();
		const int imageBitCount = (int)reader.ReadByte();
		
		const auto image = pModule.GetGameEngine()->GetImageManager()->CreateImage(
			imageSize.x, imageSize.y, imageSize.z, imageComponentCount, imageBitCount);
		
		reader.Read(image->GetData(), imageSize.x * imageSize.y * imageSize.z
			* imageComponentCount * (imageBitCount / 8));
		image->NotifyImageDataChanged();
		pFontSize->SetImage(image);
		
	}catch(const deException &e){
		pModule.LogException(e);
		return false;
	}
	
	return true;
}

void deftCachedFontSize::WriteToCache(decBaseFileWriter &writer) const{
	const auto &filename = pFont->GetFilename();
	
	// valid cache checks
	writer.WriteVarUInt((uint32_t)vCacheVersion);
	writer.WriteUInt((uint32_t)pModule.GetGameEngine()->GetVirtualFileSystem()->
		GetFileModificationTime(decPath::CreatePathUnix(filename)));
	pModule.GetCaches()->WriteModuleConfig(writer);
	
	// cached content
	writer.WriteUShort((uint16_t)pFontSize->GetGlyphs().GetCount());
	pFontSize->GetGlyphs().Visit([&](const deFontGlyph &glyph){
		writer.WriteUShort((uint16_t)glyph.GetUnicode());
		writer.WriteUShort((uint16_t)glyph.GetX());
		writer.WriteUShort((uint16_t)glyph.GetY());
		writer.WriteUShort((uint16_t)glyph.GetZ());
		writer.WriteUShort((uint16_t)glyph.GetWidth());
		writer.WriteUShort((uint16_t)glyph.GetHeight());
		writer.WriteShort((int16_t)glyph.GetBearing());
		writer.WriteShort((int16_t)glyph.GetBearingY());
		writer.WriteUShort((uint16_t)glyph.GetAdvance());
	});
	
	writer.WriteFloat(pFontSize->GetSdfPixelRange());
	writer.WriteVector2(pFontSize->GetSdfDistanceRange());
	
	const auto &image = pFontSize->GetImage();
	writer.WriteUShort((uint16_t)image->GetWidth());
	writer.WriteUShort((uint16_t)image->GetHeight());
	writer.WriteUShort((uint16_t)image->GetDepth());
	writer.WriteByte((uint8_t)image->GetComponentCount());
	writer.WriteByte((uint8_t)image->GetBitCount());
	
	writer.Write(image->GetData(), image->GetWidth() * image->GetHeight() * image->GetDepth()
		* image->GetComponentCount() * (image->GetBitCount() / 8));
}
