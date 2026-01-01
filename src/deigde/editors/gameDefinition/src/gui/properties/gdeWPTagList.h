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
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/string/decStringSet.h>

class igdeUIHelper;
class igdeUndoSystem;


/**
 * \brief Tag list.
 * 
 * Subclass to implement creating undo actions.
 */
class gdeWPTagList : public igdeContainerFlow{
public:
	/** \brief Strong reference. */
	typedef deTObjectReference<gdeWPTagList> Ref;
	
private:
	igdeComboBoxFilter::Ref pComboBox;
	igdeListBox::Ref pListBox;
	
	const decStringSet *pTagList;
	igdeUndoSystem *pUndoSystem;
	
	igdeAction::Ref pActionAdd;
	igdeAction::Ref pActionRemove;
	igdeAction::Ref pActionClear;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPTagList(igdeUIHelper &helper, int rows, const char *description);
	
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
	void SetTagList(const decStringSet *tagList);
	
	/** \brief Undo system or nullptr. */
	inline igdeUndoSystem *GetUndoSystem() const{ return pUndoSystem; }
	
	/** \brief Set undo system or nullptr. */
	void SetUndoSystem(igdeUndoSystem *undoSystem);
	
	
	
	/** \brief Selected tag. */
	const decString &GetSelectedTag() const;
	
	/** \brief Update tag list. */
	void UpdateList();
	
	/** \brief Select tag. */
	void SelectTag(const decString &tag);
	
	/** \brief Update used tag list in the combo box. */
	void UpdateUsedTagList(const decStringSet &usedTags);
	
	
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionAdd() const{ return pActionAdd; }
	inline const igdeAction::Ref &GetActionRemove() const{ return pActionRemove; }
	inline const igdeAction::Ref &GetActionClear() const{ return pActionClear; }
	/*@}*/
	
	
	
	/** \name Subclass undo creation */
	/*@{*/
	virtual igdeUndo::Ref UndoSet(const decStringSet &tags) = 0;
	/*@}*/
	
	
	
protected:
	/** \brief Combo box. */
	inline const igdeComboBoxFilter::Ref &GetComboBox() const{ return pComboBox; }
	
	/** \brief List box. */
	inline const igdeListBox::Ref &GetListBox() const{ return pListBox; }
};

#endif
