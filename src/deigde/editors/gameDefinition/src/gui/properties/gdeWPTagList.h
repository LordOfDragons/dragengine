/* 
 * Drag[en]gine IGDE Game Definition Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _GDEWPTAGLIST_H_
#define _GDEWPTAGLIST_H_

#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeActionReference.h>

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
	
	igdeActionReference pActionAdd;
	igdeActionReference pActionRemove;
	igdeActionReference pActionClear;
	
	
	
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
