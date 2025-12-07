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

#ifndef _IGDEACTIONUNDO_H_
#define _IGDEACTIONUNDO_H_

#include <stddef.h>

#include "igdeAction.h"


class igdeUndoSystem;
class igdeEnvironment;


/**
 * \brief IGDE UI Action Undo.
 * 
 * Undo action. Set Undo System to use or NULL if not available.
 */
class DE_DLL_EXPORT igdeActionUndo : public igdeAction{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeActionUndo> Ref;
	
	
private:
	igdeEnvironment &pEnvironment;
	igdeUndoSystem *pUndoSystem;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	igdeActionUndo(igdeEnvironment &environment, igdeUndoSystem *undoSystem = NULL);
	
	
	
protected:
	/**
	 * \brief Clean up action.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeActionUndo();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Undo system or NULL. */
	inline igdeUndoSystem *GetRedoSystem() const{ return pUndoSystem; }
	
	/** \brief Set undo system or NULL. */
	void SetUndoSystem(igdeUndoSystem *undoSystem);
	
	
	
	/**
	 * \brief Run action.
	 * 
	 * Called if user interacts with a UI element triggering actions.
	 */
	virtual void OnAction();
	
	/**
	 * \brief Request update of action parameters if required.
	 * 
	 * Called for example by UI elements to update action parameters.
	 * Default implementation does nothing.
	 */
	virtual void Update();
	/*@}*/
};

#endif
