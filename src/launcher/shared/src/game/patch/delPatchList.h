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

#ifndef _DELPATCHLIST_H_
#define _DELPATCHLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delPatch;

class decUuid;


/**
 * \brief Game patch list.
 */
class DE_DLL_EXPORT delPatchList{
private:
	decObjectList pPatches;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game patch list. */
	delPatchList();
	
	/** \brief Create copy of game patch list. */
	delPatchList( const delPatchList &list );
	
	/** \brief Clean up game patch list. */
	~delPatchList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of patches. */
	int GetCount() const;
	
	/** \brief Patch at index. */
	delPatch *GetAt( int index ) const;
	
	/** \brief Patch with identifier or nullptr if absent. */
	delPatch *GetWithID( const decUuid &id ) const;
	
	/** \brief Patch is present. */
	bool Has( delPatch *patch ) const;
	
	/** \brief Patch with identifier is present. */
	bool HasWithID( const decUuid &id ) const;
	
	/** \brief Index of patch or -1 if absent. */
	int IndexOf( delPatch *patch ) const;
	
	/** \brief Index of patch with identifier or -1 if absent. */
	int IndexOfWithID( const decUuid &id ) const;
	
	/** \brief Add patch. */
	void Add( delPatch *patch );
	
	/**
	 * \brief Add patches.
	 * \version 1.13
	 */
	void AddAll( const delPatchList &list );
	
	/** \brief Remove patch. */
	void Remove( delPatch *patch );
	
	/** \brief Remove patches. */
	void RemoveAll();
	/*@}*/
};

#endif
