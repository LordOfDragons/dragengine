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

#ifndef _DEFTCACHES_H_
#define _DEFTCACHES_H_

#include "deftCachedFont.h"
#include "deftCachedFontSize.h"

#include <dragengine/deTUniqueReference.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/threading/deMutex.h>

class deftModule;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * Disk caches.
 */
class deftCaches{
public:
	using Ref = deTUniqueReference<deftCaches>;
	using CacheHelperRef = deTUniqueReference<deCacheHelper>;
	
	
private:
	deftModule &pModule;
	deMutex pMutex;
	decTThreadSafeObjectOrderedSet<deftCachedFont> pFonts;
	decTThreadSafeObjectOrderedSet<deftCachedFontSize> pFontSizes;
	CacheHelperRef pCacheHelperFont, pCacheHelperFontSize;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create caches. */
	deftCaches(deftModule &module);
	
	/** Clean up caches. */
	~deftCaches();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deftModule &GetModule() const{ return pModule; }
	
	/** Mutex guarding access to the cache helpers. */
	inline deMutex &GetMutex(){ return pMutex; }
	
	
	/** Load font from cache. Returns nullptr if not cached or cache is invalid. */
	deftCachedFont::Ref LoadFont(const deFont::Ref &font);
	
	/** Write font to cache. */
	void SaveFont(const deftCachedFont::Ref &cached);
	
	
	/** Load font size from cache. Returns nullptr if not cached or cache is invalid. */
	deftCachedFontSize::Ref LoadFontSize(const deFont::Ref &font, const deFontSize::Ref &fontSize);
	
	/** Write font size to cache. */
	void SaveFontSize(const deftCachedFontSize::Ref &cached);
	
	
	/** Read module configuration from cache file and verify against current module configuration. */
	bool ReadVerifyModuleConfig(decBaseFileReader &reader) const;
	
	/** Write module configuration to cache file. */
	void WriteModuleConfig(decBaseFileWriter &writer) const;
	/*@}*/
	
	
private:
	decString pGetFontSizeCacheId(const deFont &font, const deFontSize &fontSize) const;
	bool pReadVerifyModuleConfigV0(decBaseFileReader &reader) const;
};

#endif
