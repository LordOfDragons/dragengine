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

#ifndef _MEDECALSELECTION_H_
#define _MEDECALSELECTION_H_

#include "meDecalList.h"



/**
 * @brief Decal Selection.
 */
class meDecalSelection{
private:
	meDecalList pSelection;
	meDecal *pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	meDecalSelection();
	/** Cleans up the list. */
	~meDecalSelection();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the list of selected decals. */
	inline const meDecalList &GetSelected() const{ return pSelection; }
	/** Adds a decals to the selection if not included already. */
	void Add( meDecal *decal );
	/** Removes a decals from the selection if included. */
	void Remove( meDecal *decal );
	/** Removes all decals from the selection. */
	void RemoveAll();
	
	/** Retrieves the active decals or NULL if there is none. */
	inline meDecal *GetActive() const{ return pActive; }
	/** Determines if there is an active decals. */
	bool HasActive() const;
	/** Sets the active decals or NULL if there is none. */
	void SetActive( meDecal *decal );
	/** Activate the first decals in the selection. */
	void ActivateNext();
	
	/** Resets the selection removing all decals and setting the active one to NULL. */
	void Reset();
};

#endif
