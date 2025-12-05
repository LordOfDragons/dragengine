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

#ifndef _IGDEWPUNDOHISTORY_H_
#define _IGDEWPUNDOHISTORY_H_

#include "../igdeListBox.h"
#include "../event/igdeActionUndo.h"
#include "../event/igdeActionRedo.h"
#include "../event/igdeActionClearUndo.h"
#include "../layout/igdeContainerBox.h"

class igdeUndoSystem;



/**
 * \brief Undo history panel.
 */
class DE_DLL_EXPORT igdeWPUndoHistory : public igdeContainerBox{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWPUndoHistory> Ref;
	
	
private:
	igdeUndoSystem *pUndoSystem;
	igdeListBox::Ref pListUndo;
	igdeActionUndo::Ref pActionUndo;
	igdeActionRedo::Ref pActionRedo;
	igdeActionClearUndo::Ref pActionClear;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeWPUndoHistory( igdeEnvironment &environment );
	
	/** \brief Clean up panel. */
	virtual ~igdeWPUndoHistory();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Undo system or NULL. */
	inline igdeUndoSystem *GetUndoSystem() const{ return pUndoSystem; }
	
	/** \brief Set undo system or NULL. */
	void SetUndoSystem( igdeUndoSystem *undoSystem );
	
	
	
	/** \brief Update undo list. */
	void UpdateUndo();
	/*@}*/
};

#endif
