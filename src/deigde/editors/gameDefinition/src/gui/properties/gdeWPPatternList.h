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
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeActionReference.h>

class igdeUIHelper;
class igdeUndoSystem;
class igdeUndo;

class decStringSet;


/**
 * \brief Pattern list.
 * 
 * Subclass to implement creating undo actions.
 */
class gdeWPPatternList : public igdeContainerFlow{
private:
	igdeEditPathReference pEditPath;
	igdeListBoxReference pListBox;
	
	const decStringSet *pPatternList;
	igdeUndoSystem *pUndoSystem;
	
	igdeActionReference pActionAdd;
	igdeActionReference pActionRemove;
	igdeActionReference pActionClear;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPPatternList( igdeUIHelper &helper, int rows, const char *description );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPPatternList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Pattern list to edit. */
	inline const decStringSet *GetPatternList() const{ return pPatternList; }
	
	/** \brief Set pattern list to edit. */
	void SetPatternList( const decStringSet *patternList );
	
	/** \brief Undo system or NULL. */
	inline igdeUndoSystem *GetUndoSystem() const{ return pUndoSystem; }
	
	/** \brief Set undo system or NULL. */
	void SetUndoSystem( igdeUndoSystem *undoSystem );
	
	
	
	/** \brief Selected pattern. */
	const decString &GetSelectedPattern() const;
	
	/** \brief Update pattern list. */
	void UpdateList();
	
	/** \brief Select pattern. */
	void SelectPattern( const decString &pattern );
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionAdd() const{ return pActionAdd; }
	inline igdeAction *GetActionRemove() const{ return pActionRemove; }
	inline igdeAction *GetActionClear() const{ return pActionClear; }
	/*@}*/
	
	
	
	/** \name Subclass undo creation */
	/*@{*/
	virtual igdeUndo *UndoSet( const decStringSet &patterns ) = 0;
	/*@}*/
	
	
	
protected:
	/** \brief Edit path. */
	inline igdeEditPath *GetEditPath() const{ return pEditPath; }
	
	/** \brief List box. */
	inline igdeListBox *GetListBox() const{ return pListBox; }
};

#endif
