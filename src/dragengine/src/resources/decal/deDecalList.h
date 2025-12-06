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

#ifndef _DEDECALLIST_H_
#define _DEDECALLIST_H_

#include "../../deObject.h"

// predefintions
class deDecal;


/**
 * \brief Collision Decal List.
 
 * Stores a list of decals. This class is used to collect decals from collision
 * test into a list for later processing.
 */
class DE_DLL_EXPORT deDecalList : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deDecalList> Ref;
	
	
private:
	deDecal **pDecals;
	int pDecalCount;
	int pDecalSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new decal list object. */
	deDecalList();
	
	/** \brief Clean up decal list object. */
	virtual ~deDecalList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of decals. */
	inline int GetDecalCount() const{ return pDecalCount; }
	
	/** \brief Decal at the given index. */
	deDecal *GetDecalAt( int index ) const;
	
	/** \brief Determines if a decal exists. */
	bool HasDecal( deDecal *decal ) const;
	
	/** \brief Index of the decal or -1 if not found. */
	int IndexOfDecal( deDecal *decal ) const;
	
	/** \brief Adds a decal. */
	void AddDecal( deDecal *decal );
	
	/** \brief Removes all decals. */
	void RemoveAllDecals();
	/*@}*/
};

#endif
