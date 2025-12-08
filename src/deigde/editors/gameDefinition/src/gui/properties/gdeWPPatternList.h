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
	igdeEditPath::Ref pEditPath;
	igdeListBox::Ref pListBox;
	
	const decStringSet *pPatternList;
	igdeUndoSystem *pUndoSystem;
	
	igdeAction::Ref pActionAdd;
	igdeAction::Ref pActionRemove;
	igdeAction::Ref pActionClear;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPPatternList(igdeUIHelper &helper, int rows, const char *description);
	
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
	void SetPatternList(const decStringSet *patternList);
	
	/** \brief Undo system or NULL. */
	inline igdeUndoSystem *GetUndoSystem() const{ return pUndoSystem; }
	
	/** \brief Set undo system or NULL. */
	void SetUndoSystem(igdeUndoSystem *undoSystem);
	
	
	
	/** \brief Selected pattern. */
	const decString &GetSelectedPattern() const;
	
	/** \brief Update pattern list. */
	void UpdateList();
	
	/** \brief Select pattern. */
	void SelectPattern(const decString &pattern);
	
	
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionAdd() const{ return pActionAdd; }
	inline const igdeAction::Ref &GetActionRemove() const{ return pActionRemove; }
	inline const igdeAction::Ref &GetActionClear() const{ return pActionClear; }
	/*@}*/
	
	
	
	/** \name Subclass undo creation */
	/*@{*/
	virtual igdeUndo *UndoSet(const decStringSet &patterns) = 0;
	/*@}*/
	
	
	
protected:
	/** \brief Edit path. */
	inline const igdeEditPath::Ref &GetEditPath() const{ return pEditPath; }
	
	/** \brief List box. */
	inline const igdeListBox::Ref &GetListBox() const{ return pListBox; }
};

#endif
