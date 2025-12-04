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

#ifndef _IGDEETOGGLETAGS_H_
#define _IGDEETOGGLETAGS_H_

#include "../igdeListBox.h"
#include "../layout/igdeContainerFlow.h"
#include "../event/igdeAction.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeAction.h"
#include "../resources/igdeIcon.h"

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
	class DE_DLL_EXPORT cActionEnableAll : public igdeAction{
	protected:
		igdeToggleTags &pWidget;
		
	public:
		cActionEnableAll( igdeToggleTags &widget );
		virtual void OnAction();
	};
	
	/** \brief Action disable all tags. */
	class DE_DLL_EXPORT cActionDisableAll : public igdeAction{
	protected:
		igdeToggleTags &pWidget;
		
	public:
		cActionDisableAll( igdeToggleTags &widget );
		virtual void OnAction();
	};
	
	
	
private:
	igdeIcon::Ref pIconEnabled;
	igdeIcon::Ref pIconDisabled;
	
	igdeListBox::Ref pListBox;
	
	igdeAction::Ref pActionEnableAll;
	igdeAction::Ref pActionDisableAll;
	
	igdeAction::Ref pAction;
	
	
	
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
