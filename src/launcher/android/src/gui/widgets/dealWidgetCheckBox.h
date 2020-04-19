/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALWIDGETCHECKBOX_H_
#define _DEALWIDGETCHECKBOX_H_

#include "dealWidgetLayoutFlow.h"
#include "../../common/string/decString.h"

class dealWidgetLabel;
class dealWidgetImage;
class dealImage;


/**
 * \brief Check box with text and image and optional box border image.
 */
class dealWidgetCheckBox : public dealWidgetLayoutFlow{
private:
	dealWidgetLabel *pLabel;
	dealWidgetImage *pImage;
	dealWidgetImage *pBoxImageUnchecked;
	dealWidgetImage *pBoxImageChecked;
	
	bool pChecked;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetCheckBox( dealDisplay &display, dealImage *image = NULL, const char *label = "" );
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetCheckBox();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set widget from gui theme. */
	virtual void SetFromGuiTheme( const dealGuiTheme &guitheme );
	
	
	
	/** \brief Text color. */
	const decColor &GetTextColor() const;
	
	/** \brief Set text color. */
	void SetTextColor( const decColor &color );
	
	
	
	/** \brief Label or empty string if not displayed. */
	const decString &GetLabel() const;
	
	/** \brief Set label or empty string to not displayed. */
	void SetLabel( const char *label );
	
	/** \brief Image or \em NULL if not set. */
	dealImage *GetImage() const;
	
	/** \brief Set image or \em NULL if not set. */
	void SetImage( dealImage *image );
	
	/** \brief Bordered box image widget for unchecked state. */
	inline dealWidgetImage &GetBoxImageUnchecked() const{ return *pBoxImageUnchecked; }
	
	/** \brief Bordered box image widget for checked state. */
	inline dealWidgetImage &GetBoxImageChecked() const{ return *pBoxImageChecked; }
	
	
	
	/** \brief Button is checked. */
	inline bool GetChecked() const{ return pChecked; }
	
	/** \brief Set if button is checked. */
	void SetChecked( bool checked );
	
	
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief State changed. */
	virtual void OnStateChanged();
	
	
	
	/**
	 * \brief Key has been checked down.
	 * \param[in] keycode A value of AKEYCODE_*.
	 */
	virtual void OnKeyPress( int keycode );
	
	/**
	 * \brief Key has been released.
	 * \param[in] keycode A value of AKEYCODE_*.
	 */
	virtual void OnKeyRelease( int keycode );
	
	/**
	 * \brief Mouse button press / finger press.
	* \param[in] buttons Buttons checked while pressing screen. OR of values of AMOTION_EVENT_BUTTON_.
	 */
	virtual void OnMousePress( int buttons, const decPoint &position );
	/*@}*/
	
	
	
private:
	void pBuildContent();
};

#endif
