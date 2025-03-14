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

#ifndef _DEALWIDGETOPTIONBOX_H_
#define _DEALWIDGETOPTIONBOX_H_

#include "dealWidgetLayoutFlow.h"
#include "../../common/string/decString.h"

class dealWidgetLabel;
class dealWidgetImage;
class dealImage;
class dealWidgetOptionGroup;



/**
 * \brief Option box with text and image and optional box border image.
 */
class dealWidgetOptionBox : public dealWidgetLayoutFlow{
private:
	dealWidgetLabel *pLabel;
	dealWidgetImage *pImage;
	dealWidgetImage *pBoxImageDeselected;
	dealWidgetImage *pBoxImageSelected;
	
	dealWidgetOptionGroup *pGroup;
	bool pSelected;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetOptionBox( dealDisplay &display, dealImage *image = NULL, const char *label = "" );
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetOptionBox();
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
	
	/** \brief Bordered box image widget for deselected state. */
	inline dealWidgetImage &GetBoxImageDeselected() const{ return *pBoxImageDeselected; }
	
	/** \brief Bordered box image widget for selected state. */
	inline dealWidgetImage &GetBoxImageSelected() const{ return *pBoxImageSelected; }
	
	
	
	/** \brief Group or \em NULL if not assigned to one. */
	inline dealWidgetOptionGroup *GetGroup() const{ return pGroup; }
	
	/** \brief Set group or \em NULL if not assigned to one. */
	void SetGroup( dealWidgetOptionGroup *group );
	
	/** \brief Button is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if button is selected. */
	void SetSelected( bool selected );
	
	
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief State changed. */
	virtual void OnStateChanged();
	
	
	
	/**
	 * \brief Key has been pressed down.
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
	* \param[in] buttons Buttons pressed while pressing screen. OR of values of AMOTION_EVENT_BUTTON_.
	 */
	virtual void OnMousePress( int buttons, const decPoint &position );
	/*@}*/
	
	
	
private:
	void pBuildContent();
};

#endif
