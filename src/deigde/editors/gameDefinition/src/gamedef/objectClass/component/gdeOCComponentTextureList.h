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

#ifndef _GDEOCCOMPONENTTEXTURELIST_H_
#define _GDEOCCOMPONENTTEXTURELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCComponentTexture;



/**
 * \brief Object class component texture list.
 */
class gdeOCComponentTextureList{
private:
	decObjectOrderedSet pTextures;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class component texture list. */
	gdeOCComponentTextureList();
	
	/** \brief Create copy of object class component texture list. */
	gdeOCComponentTextureList(const gdeOCComponentTextureList &list);
	
	/** \brief Clean up object class component texture list. */
	~gdeOCComponentTextureList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class component texturees. */
	int GetCount() const;
	
	/** \brief Object class at position. */
	gdeOCComponentTexture *GetAt(int index) const;
	
	/** \brief Named object class component texture or \em NULL if absent. */
	gdeOCComponentTexture *GetNamed(const char *name) const;
	
	/** \brief Index of named object class component texture or -1 if absent. */
	int IndexOf(gdeOCComponentTexture *texture) const;
	
	/** \brief Index of named object class component texture or -1 if absent. */
	int IndexOfNamed(const char *name) const;
	
	/** \brief Object class is present. */
	bool Has(gdeOCComponentTexture *texture) const;
	
	/** \brief Named object class component texture is present. */
	bool HasNamed(const char *name) const;
	
	/** \brief Add object class component texture. */
	void Add(gdeOCComponentTexture *texture);
	
	/** \brief Remove object class component texture. */
	void Remove(gdeOCComponentTexture *texture);
	
	/** \brief Remove all object class component texturees. */
	void RemoveAll();
	
	/** \brief Set from another object class component texture list. */
	gdeOCComponentTextureList &operator=(const gdeOCComponentTextureList &list);
	/*@}*/
};

#endif
