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

#include "deftCaches.h"
#include "../deftModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingModule.h>
#include <dragengine/threading/deMutexGuard.h>


// Class deftCaches
/////////////////////

// Constructor, destructor
////////////////////////////

deftCaches::deftCaches(deftModule &module) :
pModule(module),
pHasSharedDir(true)
{
	auto vfs = &module.GetVFS();
	pCacheHelperFont = CacheHelperRef::New(vfs, decPath::CreatePathUnix("/cache/local/font"));
	pCacheHelperFontSize = CacheHelperRef::New(vfs, decPath::CreatePathUnix("/cache/local/fontsize"));
	pCacheHelperFontGlobal = CacheHelperRef::New(vfs, decPath::CreatePathUnix("/cache/global/font"));
	pCacheHelperFontSizeGlobal = CacheHelperRef::New(vfs, decPath::CreatePathUnix("/cache/global/fontsize"));
}

deftCaches::~deftCaches() = default;


// Management
///////////////

deftCachedFont::Ref deftCaches::LoadFont(const deFont::Ref &font){
	DEASSERT_NOTNULL(font)
	
	if(!font->GetVirtualFileSystem()){
		return {};
	}
	
	const deMutexGuard guard(pMutex);
	
	// find cached font matching font resource. if the font file changed we get a
	// font resource which does not match the loaded cache font resource
	auto cached = pFonts.FindOrDefault([&](const deftCachedFont &each){
		return each.GetFont() == font;
	});
	if(cached){
		return cached;
	}
	
	// drop cached font and font sizes with matching filenames if present. this happens if the
	// font file changed while the matching font and font size caches are loaded
	const auto &filename = font->GetFilename();
	const auto &vfs = font->GetVirtualFileSystem();
	
	pFonts.RemoveIf([&](const deftCachedFont &each){
		const auto &f = each.GetFont();
		return f->GetVirtualFileSystem() == vfs && f->GetFilename() == filename;
	});
	pFontSizes.RemoveIf([&](const deftCachedFontSize &each){
		const auto &f = each.GetFont();
		return f->GetVirtualFileSystem() == vfs && f->GetFilename() == filename;
	});
	
	// try loading font from cache if present. can potentially invalidate cache file
	const auto useCache = pUseCacheFont(font);
	auto reader = useCache.cache->Read(useCache.filename);
	if(reader){
		cached = deftCachedFont::Ref::New(pModule, font);
		if(cached->ReadFromCache(*reader)){
			pFonts.Add(cached);
			return cached;
		}
		
		// cache file is invalid so delete it
		useCache.cache->Delete(useCache.filename);
	}
	
	// no valid cache exists for this font
	return {};
}

void deftCaches::SaveFont(const deftCachedFont::Ref &cached){
	if(!cached->GetFont()->GetVirtualFileSystem()){
		return;
	}
	
	const deMutexGuard guard(pMutex);
	const auto &filename = cached->GetFont()->GetFilename();
	
	// drop cached font with the same filename if present
	const auto &vfs = cached->GetFont()->GetVirtualFileSystem();
	
	pFonts.RemoveIf([&](const deftCachedFont &each){
		const auto &f = each.GetFont();
		return f->GetVirtualFileSystem() == vfs && f->GetFilename() == filename;
	});
	
	// write cache file
	const auto useCache = pUseCacheFont(cached->GetFont());
	auto writer = useCache.cache->Write(useCache.filename);
	cached->WriteToCache(writer);
	
	// add to list of cached fonts
	pFonts.Add(cached);
}


deftCachedFontSize::Ref deftCaches::LoadFontSize(const deFont::Ref &font, const deFontSize::Ref &fontSize){
	DEASSERT_NOTNULL(font)
	DEASSERT_NOTNULL(fontSize)
	
	if(!font->GetVirtualFileSystem()){
		return {};
	}
	
	const deMutexGuard guard(pMutex);
	
	// find cached font size matching font size resource. if the font file changed we get a
	// font size resource which does not match the loaded cache font size resource
	auto cached = pFontSizes.FindOrDefault([&](const deftCachedFontSize &each){
		return each.GetFontSize() == fontSize;
	});
	if(cached){
		return cached;
	}
	
	// try loading font size from cache if present. can potentially invalidate cache file
	const auto useCache = pUseCacheFontSize(font);
	const auto id = pGetFontSizeCacheId(font, fontSize);
	auto reader = useCache.cache->Read(useCache.filename);
	if(reader){
		cached = deftCachedFontSize::Ref::New(pModule, font, fontSize);
		if(cached->ReadFromCache(*reader)){
			pFontSizes.Add(cached);
			return cached;
		}
		
		// cache file is invalid so delete it
		useCache.cache->Delete(useCache.filename);
	}

	// no valid cache exists for this font size
	return {};
}

void deftCaches::SaveFontSize(const deftCachedFontSize::Ref &cached){
	if(!cached->GetFont()->GetVirtualFileSystem()){
		return;
	}
	
	const deMutexGuard guard(pMutex);
	const auto &filename = cached->GetFont()->GetFilename();
	const int lineHeight = cached->GetFontSize()->GetLineHeight();
	
	// drop cached font size with the same filename and line height if present
	const auto &vfs = cached->GetFont()->GetVirtualFileSystem();
	
	pFontSizes.RemoveIf([&](const deftCachedFontSize &each){
		const auto &f = each.GetFont();
		return f->GetVirtualFileSystem() == vfs && f->GetFilename() == filename
			&& each.GetFontSize()->GetLineHeight() == lineHeight;
	});
	
	// write cache file
	const auto useCache = pUseCacheFontSize(cached->GetFont());
	auto writer = useCache.cache->Write(useCache.filename);
	cached->WriteToCache(writer);
	
	// add to list of cached font sizes
	pFontSizes.Add(cached);
}


bool deftCaches::ReadVerifyModuleConfig(decBaseFileReader &reader) const{
	switch((int)reader.ReadVarUInt()){
	case 0:
		return pReadVerifyModuleConfigV0(reader);
		
	default:
		return false;
	}
}

void deftCaches::WriteModuleConfig(decBaseFileWriter &writer) const{
	writer.WriteVarUInt(0); // version
	
	uint8_t flags = 0;
	if(pModule.GetCreateSdfFont()){
		flags |= 0x1;
	}
	writer.WriteByte(flags);
	
	writer.WriteUShort((uint16_t)pModule.GetMaxImageSize());
	writer.WriteByte((uint16_t)pModule.GetLineHeightLimit());
	writer.WriteByte((uint16_t)pModule.GetGlyphSpacing());
}


void deftCaches::Clear(){
	const deMutexGuard guard(pMutex);
	pFonts.RemoveAll();
	pFontSizes.RemoveAll();
}


// Private Functions
//////////////////////

decString deftCaches::pGetFontSizeCacheId(const deFont &font, const deFontSize &fontSize) const{
	return decString::Formatted("{},{}", font.GetFilename(), fontSize.GetLineHeight());
}

bool deftCaches::pReadVerifyModuleConfigV0(decBaseFileReader &reader) const{
	const uint8_t flags = reader.ReadByte();
	if(((flags & 0x1) == 0x1) != pModule.GetCreateSdfFont()){
		return false;
	}
	
	if((int)reader.ReadUShort() != pModule.GetMaxImageSize()){
		return false;
	}
	if((int)reader.ReadByte() != pModule.GetLineHeightLimit()){
		return false;
	}
	if((int)reader.ReadByte() != pModule.GetGlyphSpacing()){
		return false;
	}
	
	return true;
}

bool deftCaches::pIsFilenameUnderSharedDir(const char *filename){
	if(!pHasSharedDir){
		return false;
	}
	
	if(pScriptSharedDir.IsEmpty()){
		auto module = pModule.GetGameEngine()->GetScriptingSystem()->GetActiveModule();
		if(!module){
			pHasSharedDir = false;
			return false;
		}
		
		auto sharedDataDir = module->GetVFSSharedDataDir();
		if(!sharedDataDir){
			pHasSharedDir = false;
			return false;
		}
		
		pScriptSharedCachePrefix.FormatSafe("sharedDataDir:{}:",
			module->GetLoadableModule().GetDirectoryName());
		
		pScriptSharedDir = sharedDataDir;
		if(!pScriptSharedDir.EndsWith("/")){
			pScriptSharedDir += "/";
		}
	}
	
	return decString(filename).BeginsWith(pScriptSharedDir);
}

deftCaches::UseCache deftCaches::pUseCacheFont(const deFont &font){
	const char * const filename = font.GetFilename();
	const auto &vfs = font.GetVirtualFileSystem();
	
	if(vfs == pModule.GetGameEngine()->GetVirtualFileSystem()){
		if(pIsFilenameUnderSharedDir(filename)){
			return UseCache{pCacheHelperFontGlobal, pScriptSharedCachePrefix + filename};
			
		}else{
			return UseCache{pCacheHelperFont, filename};
		}
		
	}else{
		const auto &id = vfs->GetIdentifier();
		if(id.BeginsWith("module:")){
			return UseCache{pCacheHelperFontGlobal, id + ":" + filename};
			
		}else{
			return UseCache{pCacheHelperFont, filename};
		}
	}
}

deftCaches::UseCache deftCaches::pUseCacheFontSize(const deFont &font){
	const char * const filename = font.GetFilename();
	const auto &vfs = font.GetVirtualFileSystem();
	
	if(vfs == pModule.GetGameEngine()->GetVirtualFileSystem()){
		if(pIsFilenameUnderSharedDir(filename)){
			return UseCache{pCacheHelperFontSizeGlobal, pScriptSharedCachePrefix + filename};
			
		}else{
			return UseCache{pCacheHelperFontSize, filename};
		}
		
	}else{
		const auto &id = vfs->GetIdentifier();
		if(id.BeginsWith("module:")){
			return UseCache{pCacheHelperFontSizeGlobal, id + ":" + filename};
			
		}else{
			return UseCache{pCacheHelperFontSize, filename};
		}
	}
}
