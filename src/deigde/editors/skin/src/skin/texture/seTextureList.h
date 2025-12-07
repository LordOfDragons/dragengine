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

#ifndef _SETEXTURELIST_H_
#define _SETEXTURELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seTexture;



/**
 * @brief Texture List.
 */
class seTextureList{
private:
	decObjectOrderedSet pTextures;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new texture list. */
	seTextureList();
	/** Cleans up the texture list. */
	~seTextureList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of textures. */
	int GetCount() const;
	/** Retrieves the texture at the given position. */
	seTexture *GetAt(int index) const;
	/** Retrieves the texture with the given name or NULL if not found. */
	seTexture *GetNamed(const char *name) const;
	/** Retrieves the index of the given texture or -1 if not found. */
	int IndexOf(seTexture *texture) const;
	/** Retrieves the index of the texture with the given name or -1 if not found. */
	int IndexOfNamed(const char *name) const;
	/** Determines if a texture exists. */
	bool Has(seTexture *texture) const;
	/** Determines if a texture with the given name exists. */
	bool HasNamed(const char *name) const;
	/** Adds a texture. */
	void Add(seTexture *texture);
	/** Inserts a new texture. */
	void InsertAt(seTexture *texture, int index);
	/** Moves a texture to a new position. */
	void MoveTo(seTexture *texture, int index);
	/** Removes a texture. */
	void Remove(seTexture *texture);
	/** Removes all textures. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	seTextureList &operator=(const seTextureList &list);
	/*@}*/
};

#endif
