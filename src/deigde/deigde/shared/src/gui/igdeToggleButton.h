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

#ifndef _IGDETOGGLEBUTTON_H_
#define _IGDETOGGLEBUTTON_H_

#include "igdeButton.h"


/**
 * \brief IGDE UI Toggle button with text and icon.
 */
class DE_DLL_EXPORT igdeToggleButton : public igdeButton{
private:
	bool pToggled;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create button. */
	igdeToggleButton( igdeEnvironment &environment, const char *text, igdeIcon *icon,
		eButtonStyle = ebsNormal );
	
	/** \brief Create button. */
	igdeToggleButton( igdeEnvironment &environment, const char *text, const char *description,
		igdeIcon *icon, eButtonStyle = ebsNormal );
	
	/** \brief Create button. */
	igdeToggleButton( igdeEnvironment &environment, igdeAction *action, eButtonStyle style = ebsNormal );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeToggleButton();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Button is toggled. */
	inline bool GetToggled() const{ return pToggled; }
	
	/** \brief Set if button is toggled. */
	void SetToggled( bool toogled );
	
	
	
	/**
	 * \brief Run button action.
	 * 
	 * Called if button is clicked by the user. Default implementation toggled button.
	 */
	virtual void OnAction();
	
	/** \brief Action parameters changed. */
	virtual void OnParameterChanged( igdeAction *action );
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
	
	/** \brief Toggled changed. */
	virtual void OnToggledChanged();
	/*@}*/
};

#endif
