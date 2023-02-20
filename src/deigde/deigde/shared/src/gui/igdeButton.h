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

#ifndef _IGDEBUTTON_H_
#define _IGDEBUTTON_H_

#include "igdeWidget.h"
#include "event/igdeActionListener.h"
#include "event/igdeActionReference.h"
#include "resources/igdeIconReference.h"

#include <dragengine/common/string/decString.h>


/**
 * \brief IGDE UI Button with text and icon.
 */
class DE_DLL_EXPORT igdeButton : public igdeWidget, igdeActionListener{
public:
	/** \brief Button style. */
	enum eButtonStyle{
		/** \brief Normal button. */
		ebsNormal,
		
		/** \brief ToolBar button. */
		ebsToolBar
	};
	
	
	
private:
	eButtonStyle pStyle;
	decString pText;
	decString pDescription;
	igdeIconReference pIcon;
	bool pEnabled;
	bool pDefault;
	igdeActionReference pAction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create button. */
	igdeButton( igdeEnvironment &environment, const char *text, igdeIcon *icon,
		eButtonStyle = ebsNormal );
	
	/** \brief Create button. */
	igdeButton( igdeEnvironment &environment, const char *text, const char *description,
		igdeIcon *icon, eButtonStyle = ebsNormal );
	
	/** \brief Create button. */
	igdeButton( igdeEnvironment &environment, igdeAction *action, eButtonStyle style = ebsNormal );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeButton();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Button style. */
	inline eButtonStyle GetStyle() const{ return pStyle; }
	
	/** \brief Set button style. */
	void SetStyle( eButtonStyle style );
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const char *text );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Icon or NULL. */
	inline igdeIcon *GetIcon() const{ return pIcon; }
	
	/** \brief Set icon or NULL. */
	void SetIcon( igdeIcon *icon );
	
	/** \brief Button is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if button is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Widget is the default widget if used in dialogs. */
	inline bool GetDefault() const{ return pDefault; }
	
	/** \brief Set if if widget is the default widget if used in dialogs. */
	void SetDefault( bool isdefault );
	
	/** \brief Action or NULL. */
	inline igdeAction *GetAction() const{ return pAction; }
	
	/** \brief Set action or NULL. */
	void SetAction( igdeAction *action );
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/**
	 * \brief Run button action.
	 * 
	 * Called if button is clicked by the user. Default implementation calls OnAction
	 * on set action object if present.
	 */
	virtual void OnAction();
	
	/** \brief Action parameters changed. */
	virtual void OnParameterChanged( igdeAction *action );
	
	/** \brief Action has been destroyed. */
	virtual void OnDestroyed( igdeAction *action );
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	
	
	
protected:
	/** \brief Style changed. */
	virtual void OnStyleChanged();
	
	/** \brief Text changed. */
	virtual void OnTextChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Icon changed. */
	virtual void OnIconChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Default changed. */
	virtual void OnDefaultChanged();
	/*@}*/
};

#endif
