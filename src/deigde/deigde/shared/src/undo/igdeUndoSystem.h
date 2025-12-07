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
class DE_DLL_EXPORT igdeUndoSystem{
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
	igdeUndoSystem(igdeEditableEntity *editableEntity);
	
	/** \brief Clean up undo system. */
	~igdeUndoSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Editable entity. */
	inline igdeEditableEntity *GetEditableEntity() const{return pEditableEntity;}
	
	
	
	/** \brief Number of undo actions. */
	int GetCount() const;
	
	/** \brief Undo action at index. */
	igdeUndo *GetAt(int index) const;
	
	/** \brief Next undo action. */
	igdeUndo *GetTop() const;
	
	/**
	 * \brief Add undo.
	 * 
	 * Redos the undo before adding then sends UndoChanged notification to editable entity
	 * unless disabled. Disable only if you know what you are doing, for example preparing
	 * an undo action with the possibility to cancel before actually adding it
	 */
	void Add(igdeUndo *undo, bool runRedo = true);
	
	/**
	 * \brief Remove all undo actions.
	 * 
	 * Sends UndoChanged notification to editable entity if actions have been present.
	 */
	void RemoveAll();
	
	
	
	/** \brief Maximum number of undo actions. */
	inline int GetMaxUndoCount() const{return pMaxUndos;}
	
	/** \brief Set maximum number of undo actions. */
	void SetMaxUndoCount(int maxUndos);
	
	/** \brief Maximum memory to consume. */
	inline int GetMaxMemory() const{return pMaxMemory;}
	
	/** \brief Set maximum memory to consume. */
	void SetMaxMemory(int maxMemory);
	
	
	
	/** \brief Number of undo actions that can be undone. */
	int GetUndoableCount() const;
	
	/** \brief Number of undo actions that can be redone. */
	inline int GetRedoableCount() const{return pRedoCount;}
	
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
