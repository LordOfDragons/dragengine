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

#ifndef _DELGAMEICON_H_
#define _DELGAMEICON_H_

#include <dragengine/deObject.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/string/decString.h>


/**
 * \brief Game icon.
 * 
 * Format of content depends on path extension.
 */
class DE_DLL_EXPORT delGameIcon : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delGameIcon> Ref;
	
	
private:
	int pSize;
	decString pPath;
	decMemoryFile::Ref pContent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create icon. */
	delGameIcon(int size, const char *path);
	
protected:
	/** \brief Clean up icon. */
	virtual ~delGameIcon();
	
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Icon size in pixel. */
	inline const int GetSize() const{ return pSize; }
	
	/** \brief Path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Content or nullptr if not loaded. */
	inline const decMemoryFile::Ref &GetContent() const{ return pContent; }
	
	/** \brief Set content or nullptr if not loaded. */
	void SetContent(decMemoryFile *content);
	/*@}*/
	
	
	
protected:
	/**
	 * \brief Content changed due to call by SetContent().
	 * 
	 * Default implementation does nothing. For subclass use.
	 */
	virtual void OnContentChanged();
};

#endif
