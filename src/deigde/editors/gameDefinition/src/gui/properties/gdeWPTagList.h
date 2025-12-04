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

#ifndef _GDEWPTAGLIST_H_
#define _GDEWPTAGLIST_H_

#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeAction.h>

class igdeUIHelper;
class igdeUndoSystem;
class igdeUndo;

class decStringSet;


/**
 * \brief Tag list.
 * 
 * Subclass to implement creating undo actions.
 */
class gdeWPTagList : public igdeContainerFlow{
private:
	igdeComboBoxFilterReference pComboBox;
	igdeListBoxReference pListBox;
	
	const decStringSet *pTagList;
	igdeUndoSystem *pUndoSystem;
	
	igdeAction::Ref pActionAdd;
	igdeAction::Ref pActionRemove;
	igdeAction::Ref pActionClear;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPTagList( igdeUIHelper &helper, int rows, const char *description );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPTagList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Tag list to edit. */
	inline const decStringSet *GetTagList() const{ return pTagList; }
	
	/** \brief Set tag list to edit. */
	void SetTagList( const decStringSet *tagList );
	
	/** \brief Undo system or NULL. */
	inline igdeUndoSystem *GetUndoSystem() const{ return pUndoSystem; }
	
	/** \brief Set undo system or NULL. */
	void SetUndoSystem( igdeUndoSystem *undoSystem );
	
	
	
	/** \brief Selected tag. */
	const decString &GetSelectedTag() const;
	
	/** \brief Update tag list. */
	void UpdateList();
	
	/** \brief Select tag. */
	void SelectTag( const decString &tag );
	
	/** \brief Update used tag list in the combo box. */
	void UpdateUsedTagList( const decStringSet &usedTags );
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionAdd() const{ return pActionAdd; }
	inline igdeAction *GetActionRemove() const{ return pActionRemove; }
	inline igdeAction *GetActionClear() const{ return pActionClear; }
	/*@}*/
	
	
	
	/** \name Subclass undo creation */
	/*@{*/
	virtual igdeUndo *UndoSet( const decStringSet &tags ) = 0;
	/*@}*/
	
	
	
protected:
	/** \brief Combo box. */
	inline igdeComboBoxFilter *GetComboBox() const{ return pComboBox; }
	
	/** \brief List box. */
	inline igdeListBox *GetListBox() const{ return pListBox; }
};

#endif
