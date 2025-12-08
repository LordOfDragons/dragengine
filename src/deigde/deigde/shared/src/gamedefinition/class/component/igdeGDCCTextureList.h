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

#ifndef _IGDEGDCCTEXTURELIST_H_
#define _IGDEGDCCTEXTURELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCCTexture;



/**
 * \brief Game Definition Class Component Texture List.
 */
class DE_DLL_EXPORT igdeGDCCTextureList{
private:
	decObjectOrderedSet pTextures;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCCTextureList();
	
	/** \brief Create copy of a game definition class component texture list. */
	igdeGDCCTextureList(const igdeGDCCTextureList &list);
	
	/** \brief Clean up list. */
	~igdeGDCCTextureList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of textures. */
	int GetCount() const;
	
	/** \brief Texture by index. */
	igdeGDCCTexture *GetAt(int index) const;
	
	/** \brief Texture by name or NULL if not found. */
	igdeGDCCTexture *GetNamed(const char *name) const;
	
	/** \brief Index of an texture or -1 if not found. */
	int IndexOf(igdeGDCCTexture *texture) const;
	
	/** \brief Index of a named texture or -1 if not found. */
	int IndexOfNamed(const char *name) const;
	
	/** \brief Determines if an texture exists. */
	bool Has(igdeGDCCTexture *texture) const;
	
	/** \brief Determines if an named texture exists. */
	bool HasNamed(const char *name) const;
	
	/** \brief Adds an texture. */
	void Add(igdeGDCCTexture *texture);
	
	/** \brief Inserts a new texture. */
	void InsertAt(igdeGDCCTexture *texture, int index);
	
	/** \brief Moves an texture to a new position. */
	void MoveTo(igdeGDCCTexture *texture, int index);
	
	/** \brief Removes an texture. */
	void Remove(igdeGDCCTexture *texture);
	
	/** \brief Removes all textures. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom(const igdeGDCCTextureList &list);
	
	/** \brief Set list from another list. */
	igdeGDCCTextureList &operator=(const igdeGDCCTextureList &list);
	/*@}*/
};

#endif
