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

#ifndef _IGDEETOGGLETAGS_H_
#define _IGDEETOGGLETAGS_H_

#include "../igdeListBoxReference.h"
#include "../layout/igdeContainerFlow.h"
#include "../event/igdeAction.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeActionReference.h"
#include "../resources/igdeIconReference.h"

#include <dragengine/common/string/decStringSet.h>

class igdeUIHelper;

class decStringSet;


/**
 * \brief Toggle tags list.
 * 
 * Shows a list of all tags allowing the user to enable/disable them by double clicking
 * or using the context menu.
 */
class DE_DLL_EXPORT igdeToggleTags : public igdeContainerFlow, igdeActionListener{
protected:
	/** \brief Action enable all tags. */
	class cActionEnableAll : public igdeAction{
	protected:
		igdeToggleTags &pWidget;
		
	public:
		cActionEnableAll( igdeToggleTags &widget );
		virtual void OnAction();
	};
	
	/** \brief Action disable all tags. */
	class cActionDisableAll : public igdeAction{
	protected:
		igdeToggleTags &pWidget;
		
	public:
		cActionDisableAll( igdeToggleTags &widget );
		virtual void OnAction();
	};
	
	
	
private:
	igdeIconReference pIconEnabled;
	igdeIconReference pIconDisabled;
	
	igdeListBoxReference pListBox;
	
	igdeActionReference pActionEnableAll;
	igdeActionReference pActionDisableAll;
	
	igdeActionReference pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeToggleTags( igdeUIHelper &helper, int rows );
	
	/** \brief Create panel. */
	igdeToggleTags( igdeUIHelper &helper, int rows, igdeAction *action );
	
	
	
protected:
	/** \brief Clean up panel. */
	virtual ~igdeToggleTags();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Enable tag icon. */
	inline igdeIcon *GetIconEnabled() const{ return pIconEnabled; }
	
	/** \brief Set enable tag icon. */
	void SetIconEnabled( igdeIcon *icon );
	
	/** \brief Disabled tag icon. */
	inline igdeIcon *GetIconDisabled() const{ return pIconDisabled; }
	
	/** \brief Set disabled tag icon. */
	void SetIconDisabled( igdeIcon *icon );
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Get set of all tags. */
	decStringSet GetTags() const;
	
	/**
	 * \brief Set from set of all tags.
	 * 
	 * Sets all tags disabled.
	 */
	void SetTags( const decStringSet &tags );
	
	/** \brief Clear. */
	void Clear();
	
	/** \brief Get set of enabled tags. */
	decStringSet GetEnabledTags() const;
	
	/** \brief Get set of disabled tags. */
	decStringSet GetDisabledTags() const;
	
	
	
	/** \brief Selected tag. */
	const decString &GetSelectedTag() const;
	
	/** \brief Select tag. */
	void SelectTag( const char *tag );
	
	/** \brief Enable all tags. */
	void EnableAllTags();
	
	/** \brief Disable all tags. */
	void DisableAllTags();
	
	/** \brief Enabled tags. */
	void EnableTags( const decStringSet &set );
	
	/** \brief Tag is enabled. */
	bool GetTagEnabled( const char *tag ) const;
	
	/** \brief Set tag enabled. */
	void SetTagEnabled( const char *tag, bool enabled );
	
	/** \brief Toggle tag enabled. */
	void ToggleTagEnabled( const char *tag );
	
	
	
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
	inline igdeAction *GetActionEnableAll() const{ return pActionEnableAll; }
	inline igdeAction *GetActionDisableAll() const{ return pActionDisableAll; }
	/*@}*/
	
	
	
	/** \name Widgets for use by actions. */
	/*@{*/
	/** \brief List box. */
	inline igdeListBox &GetListBox() const{ return pListBox; }
	
	/** \brief Toggle tag enabled. */
	void ToggleTagEnabled( int index );
	/*@}*/
	
	
	
private:
	void pCreateContent( igdeUIHelper &helper, int rows );
	void pUpdateAllIcons();
	void pUpdateIcon( int index );
};

#endif
