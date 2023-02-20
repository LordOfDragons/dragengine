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

#ifndef _IGDEEDITTAGS_H_
#define _IGDEEDITTAGS_H_

#include "../igdeListBoxReference.h"
#include "../igdeComboBoxFilterReference.h"
#include "../layout/igdeContainerFlow.h"
#include "../event/igdeAction.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeActionReference.h"

#include <dragengine/common/string/decStringSet.h>

class igdeUIHelper;

class decStringSet;


/**
 * \brief Tag list.
 */
class DE_DLL_EXPORT igdeEditTags : public igdeContainerFlow, igdeActionListener{
protected:
	/** \brief Action adding a tag. */
	class cActionAdd : public igdeAction{
	protected:
		igdeEditTags &pWidget;
		
	public:
		cActionAdd( igdeEditTags &widget );
		virtual void OnAction();
	};
	
	/** \brief Action removing selected tag. */
	class cActionRemove : public igdeAction{
	protected:
		igdeEditTags &pWidget;
		
	public:
		cActionRemove( igdeEditTags &widget );
		virtual void OnAction();
	};
	
	/** \brief Action removing all tags. */
	class cActionClear : public igdeAction{
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
	
	igdeActionReference pActionAdd;
	igdeActionReference pActionRemove;
	igdeActionReference pActionClear;
	
	igdeActionReference pAction;
	
	
	
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
