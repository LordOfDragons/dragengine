/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEUNDOSYSTEM_H_
#define _IGDEUNDOSYSTEM_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>


class igdeUndo;
class igdeEditableEntity;


/**
 * \brief Undo System.
 * 
 * Undo system storing a list of undo actions. Various actions can be undone and redone.
 * The list is capped using a maximum number of entries or a maximum number of memory
 * consumption. The default maximum number of entries is 100 and the maximum number of
 * memory consumption 0. The limits are ignored if the respective value is set to 0.
 */
class igdeUndoSystem{
private:
	igdeEditableEntity *pEditableEntity;
	
	decObjectOrderedSet pUndos;
	int pRedoCount;
	
	int pMaxUndos;
	int pMaxMemory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo system. */
	igdeUndoSystem( igdeEditableEntity *editableEntity );
	
	/** \brief Clean up undo system. */
	~igdeUndoSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Editable entity. */
	inline igdeEditableEntity *GetEditableEntity() const{ return pEditableEntity; }
	
	
	
	/** \brief Number of undo actions. */
	int GetCount() const;
	
	/** \brief Undo action at index. */
	igdeUndo *GetAt( int index ) const;
	
	/** \brief Next undo action. */
	igdeUndo *GetTop() const;
	
	/**
	 * \brief Add undo.
	 * 
	 * Redos the undo before adding then sends UndoChanged notification to editable entity
	 * unless disabled. Disable only if you know what you are doing, for example preparing
	 * an undo action with the possibility to cancel before actually adding it
	 */
	void Add( igdeUndo *undo, bool runRedo = true );
	
	/**
	 * \brief Remove all undo actions.
	 * 
	 * Sends UndoChanged notification to editable entity if actions have been present.
	 */
	void RemoveAll();
	
	
	
	/** \brief Maximum number of undo actions. */
	inline int GetMaxUndoCount() const{ return pMaxUndos; }
	
	/** \brief Set maximum number of undo actions. */
	void SetMaxUndoCount( int maxUndos );
	
	/** \brief Maximum memory to consume. */
	inline int GetMaxMemory() const{ return pMaxMemory; }
	
	/** \brief Set maximum memory to consume. */
	void SetMaxMemory( int maxMemory );
	
	
	
	/** \brief Number of undo actions that can be undone. */
	int GetUndoableCount() const;
	
	/** \brief Number of undo actions that can be redone. */
	inline int GetRedoableCount() const{ return pRedoCount; }
	
	/**
	 * \brief Undo top undo action if present.
	 * 
	 * Sends UndoChanged notification to editable entity if action has been undone.
	 */
	void Undo();
	
	/**
	 * \brief Redoe top undo action that can be redone if present.
	 * 
	 * Sends UndoChanged notification to editable entity if action has been redone.
	 */
	void Redo();
	
	/**
	 * \brief Remove all redoable undo actions.
	 * 
	 * Sends UndoChanged notification to editable entity if actions have been removed.
	 */
	void RemoveAllRedoable();
	/*@}*/
};

#endif
