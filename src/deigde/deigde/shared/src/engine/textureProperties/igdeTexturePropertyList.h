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

#ifndef _IGDETEXTUREPROPERTYLIST_H_
#define _IGDETEXTUREPROPERTYLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class igdeTextureProperty;



/**
 * \brief Texture Property List.
 */
class DE_DLL_EXPORT igdeTexturePropertyList{
private:
	decPointerList pProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create texture property list. */
	igdeTexturePropertyList();
	
	/** \brief Clean up texture property list. */
	~igdeTexturePropertyList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of texture properties. */
	int GetCount() const;
	
	/** \brief Texture property at the given position. */
	const igdeTextureProperty *GetAt( int index ) const;
	
	/** \brief Texture property with the given name or NULL if not found. */
	const igdeTextureProperty *GetNamed( const char *name ) const;
	
	/** \brief Index of the given texture property or -1 if not found. */
	int IndexOf( const igdeTextureProperty *texture ) const;
	
	/** \brief Index of the texture property with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Determines if a texture property exists. */
	bool Has( const igdeTextureProperty *texture ) const;
	
	/** \brief Determines if a texture property with the given name exists. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Adds a texture property. */
	void Add( igdeTextureProperty *texture );
	
	/** \brief Removes a texture property. */
	void Remove( const igdeTextureProperty *texture );
	
	/** \brief Removes all texture properties. */
	void RemoveAll();
	/*@}*/
};

#endif
