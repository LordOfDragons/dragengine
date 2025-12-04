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

#ifndef _IGDEEDITTAGS_H_
#define _IGDEEDITTAGS_H_

#include "../igdeListBoxReference.h"
#include "../igdeComboBoxFilterReference.h"
#include "../layout/igdeContainerFlow.h"
#include "../event/igdeAction.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeAction::Ref.h"

#include <dragengine/common/string/decStringSet.h>

class igdeUIHelper;

class decStringSet;


/**
 * \brief Tag list.
 */
class DE_DLL_EXPORT igdeEditTags : public igdeContainerFlow, igdeActionListener{
protected:
	/** \brief Action adding a tag. */
	class DE_DLL_EXPORT cActionAdd : public igdeAction{
	protected:
		igdeEditTags &pWidget;
		
	public:
		cActionAdd( igdeEditTags &widget );
		virtual void OnAction();
	};
	
	/** \brief Action removing selected tag. */
	class DE_DLL_EXPORT cActionRemove : public igdeAction{
	protected:
		igdeEditTags &pWidget;
		
	public:
		cActionRemove( igdeEditTags &widget );
		virtual void OnAction();
	};
	
	/** \brief Action removing all tags. */
	class DE_DLL_EXPORT cActionClear : public igdeAction{
	protected:
		igdeEditTags &pWidget;
		
	public:
		cActionClear( igdeEditTags &widget );
		virtual void OnAction();
	};
	
	
	
private:
	igdeComboBoxFilterReference pComboBox;
	igdeListBoxReference pListBox;
	
	decStringSet pTags;
	
	igdeAction::Ref pActionAdd;
	igdeAction::Ref pActionRemove;
	igdeAction::Ref pActionClear;
	
	igdeAction::Ref pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeEditTags( igdeUIHelper &helper, int rows );
	
	/** \brief Create panel. */
	igdeEditTags( igdeUIHelper &helper, int rows, igdeAction *action );
	
	
	
protected:
	/** \brief Clean up panel. */
	virtual ~igdeEditTags();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Tags. */
	inline const decStringSet &GetTags() const{ return pTags; }
	
	/** \brief Set tags. */
	void SetTags( const decStringSet &tags );
	
	
	
	/** \brief Selected tag. */
	const decString &GetSelectedTag() const;
	
	/** \brief Select tag. */
	void SelectTag( const decString &tag );
	
	/** \brief Update tag list. */
	void UpdateList();
	
	/** \brief Update used tag list in the combo box. */
	void UpdateUsedTagList( const decStringSet &usedTags );
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Action or NULL. */
	inline igdeAction *GetAction() const{ return pAction; }
	
	/** \brief Set action or NULL. */
	void SetAction( igdeAction *action );
	
	/**
	 * \brief Tags changed.
	 * 
	 * Called if user changed tags. Default implementation calls OnAction on action object if present.
	 */
	virtual void OnAction();
	
	/** \brief Action parameters changed. */
	virtual void OnParameterChanged( igdeAction *action );
	
	/** \brief Action has been destroyed. */
	virtual void OnDestroyed( igdeAction *action );
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionAdd() const{ return pActionAdd; }
	inline igdeAction *GetActionRemove() const{ return pActionRemove; }
	inline igdeAction *GetActionClear() const{ return pActionClear; }
	/*@}*/
	
	
	
	/** \name Widgets for use by actions. */
	/*@{*/
	/** \brief Combo box. */
	inline igdeComboBoxFilter &GetComboBox() const{ return pComboBox; }
	
	/** \brief List box. */
	inline igdeListBox &GetListBox() const{ return pListBox; }
	/*@}*/
	
	
	
private:
	void pCreateContent( igdeUIHelper &helper, int rows );
};

#endif
