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

#ifndef _DEFTCACHEDFONT_H_
#define _DEFTCACHEDFONT_H_

#include <dragengine/threading/deThreadSafeObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/threading/deMutex.h>

class deftModule;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Cached font.
 */
class deftCachedFont : public deThreadSafeObject{
public:
	using Ref = deTThreadSafeObjectReference<deftCachedFont>;
	
	
private:
	deftModule &pModule;
	deFont::Ref pFont;
	int pSdfLineHeight;
	deMutex pMutex;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create cached font. */
	deftCachedFont(deftModule &module, const deFont::Ref &font);
	
protected:
	/** Clean up cached font. */
	~deftCachedFont() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Font. */
	inline const deFont::Ref &GetFont() const{ return pFont; }
	
	/** SDF line height. */
	inline int GetSdfLineHeight() const{ return pSdfLineHeight; }
	
	/** Set SDF line height. */
	void SetSdfLineHeight(int lineHeight);
	
	/** Mutex. */
	inline deMutex &GetMutex(){ return pMutex; }
	
	
	/** Read from cache. Returns true if successful, false otherwise. */
	bool ReadFromCache(decBaseFileReader &reader);
	
	/** Write to cache. */
	void WriteToCache(decBaseFileWriter &writer) const;
	/*@}*/
};

#endif
