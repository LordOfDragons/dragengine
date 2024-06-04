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

// include only once
#ifndef _MEOBJECTTEXTURELIST_H_
#define _MEOBJECTTEXTURELIST_H_

// includes
#include <dragengine/common/collection/decObjectOrderedSet.h>

// predefinitions
class meObjectTexture;



/**
 * @brief Object Texture List.
 * List of object textures.
 */
class meObjectTextureList{
private:
	decObjectOrderedSet pTextures;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meObjectTextureList();
	/** Cleans up the list. */
	~meObjectTextureList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of textures. */
	int GetTextureCount() const;
	/** Retrieves the texture at the given position. */
	meObjectTexture *GetTextureAt( int position ) const;
	/** Retrieves the index of the given texture or -1 if not found. */
	int IndexOfTexture( meObjectTexture *texture ) const;
	/** Determines if the given texture exists. */
	bool HasTexture( meObjectTexture *texture );
	/** Adds an texture throwing an exception if already existing. */
	void AddTexture( meObjectTexture *texture );
	/** Adds an texture if not existing already. */
	void AddTextureIfAbsent( meObjectTexture *texture );
	/** Removes an texture throwing an exception if not existing. */
	void RemoveTexture( meObjectTexture *texture );
	/** Removes an texture if existing. */
	void RemoveTextureIfPresent( meObjectTexture *texture );
	/** Removes all textures. */
	void RemoveAllTextures();
	/*@}*/
};

// end of include only once
#endif
