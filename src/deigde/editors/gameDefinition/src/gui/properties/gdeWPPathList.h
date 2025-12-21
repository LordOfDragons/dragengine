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

#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/string/decStringList.h>

class igdeUIHelper;
class igdeUndoSystem;


/**
 * \brief Path list.
 * 
 * Subclass to implement creating undo actions.
 */
class gdeWPPathList : public igdeContainerFlow{
private:
	igdeEditPath::Ref pEditPath;
	igdeListBox::Ref pListBox;
	
	const decStringList *pPathList;
	igdeUndoSystem *pUndoSystem;
	
	igdeAction::Ref pActionAppend;
	igdeAction::Ref pActionInsert;
	igdeAction::Ref pActionRemove;
	igdeAction::Ref pActionClear;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPPathList(igdeUIHelper &helper, int rows, const char *description);
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPPathList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Path list to edit. */
	inline const decStringList *GetPathList() const{ return pPathList; }
	
	/** \brief Set path list to edit. */
	void SetPathList(const decStringList *pathList);
	
	/** \brief Undo system or nullptr. */
	inline igdeUndoSystem *GetUndoSystem() const{ return pUndoSystem; }
	
	/** \brief Set undo system or nullptr. */
	void SetUndoSystem(igdeUndoSystem *undoSystem);
	
	
	
	/** \brief Selected path. */
	const decString &GetSelectedPath() const;
	
	/** \brief Update path list. */
	void UpdateList();
	
	/** \brief Select path. */
	void SelectPath(const decString &path);
	
	
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionAppend() const{ return pActionAppend; }
	inline const igdeAction::Ref &GetActionInsert() const{ return pActionInsert; }
	inline const igdeAction::Ref &GetActionRemove() const{ return pActionRemove; }
	inline const igdeAction::Ref &GetActionClear() const{ return pActionClear; }
	/*@}*/
	
	
	
	/** \name Subclass undo creation */
	/*@{*/
	virtual igdeUndo::Ref UndoSet(const decStringList &paths) = 0;
	/*@}*/
	
	
	
protected:
	/** \brief Edit path. */
	inline const igdeEditPath::Ref &GetEditPath() const{ return pEditPath; }
	
	/** \brief List box. */
	inline const igdeListBox::Ref &GetListBox() const{ return pListBox; }
};

#endif
