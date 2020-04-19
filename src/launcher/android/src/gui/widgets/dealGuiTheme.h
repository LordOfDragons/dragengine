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

#ifndef _DEALGUITHEME_H_
#define _DEALGUITHEME_H_

#include "../../deObject.h"
#include "../../common/math/decMath.h"

class dealFont;
class dealImage;
class dealDisplay;


/**
 * \brief Gui theme parameters.
 */
class dealGuiTheme : public deObject{
private:
	decColor pWidgetBackgroundColor;
	decColor pWidgetTextColor;
	dealFont *pWidgetTextFont;
	int pWidgetTextFontSize;
	
	decColor pButtonBackgroundColor;
	decColor pButtonTextColor;
	dealImage *pButtonBackgroundImage;
	int pButtonBackgroundImageBorder[ 4 ];
	float pButtonBackgroundImageScale;
	decColor pButtonBackgroundImageColorize;
	
	decColor pButtonPressedBackgroundColor;
	decColor pButtonPressedTextColor;
	dealImage *pButtonPressedBackgroundImage;
	int pButtonPressedBackgroundImageBorder[ 4 ];
	float pButtonPressedBackgroundImageScale;
	decColor pButtonPressedBackgroundImageColorize;
	
	decColor pCheckBoxBackgroundColor;
	decColor pCheckBoxTextColor;
	dealImage *pCheckBoxImageUnchecked;
	dealImage *pCheckBoxImageChecked;
	float pCheckBoxImageScale;
	decColor pCheckBoxImageUncheckedColorize;
	decColor pCheckBoxImageCheckedColorize;
	
	decColor pOptionBoxBackgroundColor;
	decColor pOptionBoxTextColor;
	dealImage *pOptionBoxImageDeselected;
	dealImage *pOptionBoxImageSelected;
	float pOptionBoxImageScale;
	decColor pOptionBoxImageDeselectedColorize;
	decColor pOptionBoxImageSelectedColorize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gui theme. */
	dealGuiTheme( dealDisplay &display );
	
	/** \brief Create copy of gui theme. */
	dealGuiTheme( const dealGuiTheme &copy );
	
	/** \brief Clean up gui theme. */
	virtual ~dealGuiTheme();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Widget background color. */
	inline const decColor &GetWidgetBackgroundColor() const{ return pWidgetBackgroundColor; }
	
	/** \brief Set widget background color. */
	void SetWidgetBackgroundColor( const decColor &color );
	
	/** \brief Widget text color. */
	inline const decColor &GetWidgetTextColor() const{ return pWidgetTextColor; }
	
	/** \brief Set widget text color. */
	void SetWidgetTextColor( const decColor &color );
	
	/** \brief Widget text font. */
	inline dealFont *GetWidgetTextFont() const{ return pWidgetTextFont; }
	
	/** \brief Set widget text font. */
	void SetWidgetTextFont( dealFont *font );
	
	/** \brief Widget text font size. */
	inline int GetWidgetTextFontSize() const{ return pWidgetTextFontSize; }
	
	/** \brief Set widget text font size. */
	void SetWidgetTextFontSize( int size );
	
	
	
	/** \brief Button background color. */
	inline const decColor &GetButtonBackgroundColor() const{ return pButtonBackgroundColor; }
	
	/** \brief Set button background color. */
	void SetButtonBackgroundColor( const decColor &color );
	
	/** \brief Button text color. */
	inline const decColor &GetButtonTextColor() const{ return pButtonTextColor; }
	
	/** \brief Set button text color. */
	void SetButtonTextColor( const decColor &color );
	
	/** \brief Button background image or \em NULL if not used. */
	inline dealImage *GetButtonBackgroundImage() const{ return pButtonBackgroundImage; }
	
	/** \brief Set button background image or \em NULL if not used. */
	void SetButtonBackgroundImage( dealImage *image );
	
	/** \brief Button background image border. */
	int GetButtonBackgroundImageBorder( int border ) const;
	
	/** \brief Set button background image border. */
	void SetButtonBackgroundImageBorder( int border, int size );
	
	/** \brief Button background image scale. */
	inline float GetButtonBackgroundImageScale() const{ return pButtonBackgroundImageScale; }
	
	/** \brief Set button background image scale. */
	void SetButtonBackgroundImageScale( float scale );
	
	/** \brief Button background image colorize. */
	inline const decColor &GetButtonBackgroundImageColorize() const{ return pButtonBackgroundImageColorize; }
	
	/** \brief Set button background image colorize. */
	void SetButtonBackgroundImageColorize( const decColor &color );
	
	
	
	/** \brief Pressed buttno background color. */
	inline const decColor &GetButtonPressedBackgroundColor() const{ return pButtonPressedBackgroundColor; }
	
	/** \brief Set pressed button background color. */
	void SetButtonPressedBackgroundColor( const decColor &color );
	
	/** \brief Pressed button text color. */
	inline const decColor &GetButtonPressedTextColor() const{ return pButtonPressedTextColor; }
	
	/** \brief Set pressed button text color. */
	void SetButtonPressedTextColor( const decColor &color );
	
	/** \brief Pressed button background image or \em NULL if not used. */
	inline dealImage *GetButtonPressedBackgroundImage() const{ return pButtonPressedBackgroundImage; }
	
	/** \brief Set pressed button background image or \em NULL if not used. */
	void SetButtonPressedBackgroundImage( dealImage *image );
	
	/** \brief Pressed button background image border. */
	int GetButtonPressedBackgroundImageBorder( int border ) const;
	
	/** \brief Set pressed button background image border. */
	void SetButtonPressedBackgroundImageBorder( int border, int size );
	
	/** \brief Pressed button background image scale. */
	inline float GetButtonPressedBackgroundImageScale() const{ return pButtonPressedBackgroundImageScale; }
	
	/** \brief Set pressed button background image scale. */
	void SetButtonPressedBackgroundImageScale( float scale );
	
	/** \brief Pressed button background image colorize. */
	inline const decColor &GetButtonPressedBackgroundImageColorize() const{ return pButtonPressedBackgroundImageColorize; }
	
	/** \brief Set pressed button background image colorize. */
	void SetButtonPressedBackgroundImageColorize( const decColor &color );
	
	
	
	/** \brief Check box background color. */
	inline const decColor &GetCheckBoxBackgroundColor() const{ return pCheckBoxBackgroundColor; }
	
	/** \brief Set check box background color. */
	void SetCheckBoxBackgroundColor( const decColor &color );
	
	/** \brief Check box text color. */
	inline const decColor &GetCheckBoxTextColor() const{ return pCheckBoxTextColor; }
	
	/** \brief Set check box text color. */
	void SetCheckBoxTextColor( const decColor &color );
	
	/** \brief Check box image unchecked. */
	inline dealImage *GetCheckBoxImageUnchecked() const{ return pCheckBoxImageUnchecked; }
	
	/** \brief Set check box image unchecked. */
	void SetCheckBoxImageUnchecked( dealImage *image );
	
	/** \brief Check box image checked. */
	inline dealImage *GetCheckBoxImageChecked() const{ return pCheckBoxImageChecked; }
	
	/** \brief Set check box image checked. */
	void SetCheckBoxImageChecked( dealImage *image );
	
	/** \brief Check box image scale. */
	inline float GetCheckBoxImageScale() const{ return pCheckBoxImageScale; }
	
	/** \brief Set check box image scale. */
	void SetCheckBoxImageScale( float scale );
	
	/** \brief Check box image unchecked colorize. */
	inline const decColor &GetCheckBoxImageUncheckedColorize() const{ return pCheckBoxImageUncheckedColorize; }
	
	/** \brief Set check box image unchecked colorize. */
	void SetCheckBoxImageUncheckedColorize( const decColor &color );
	
	/** \brief Check box image checked colorize. */
	inline const decColor &GetCheckBoxImageCheckedColorize() const{ return pCheckBoxImageCheckedColorize; }
	
	/** \brief Set check box image checked colorize. */
	void SetCheckBoxImageCheckedColorize( const decColor &color );
	
	
	
	/** \brief Option box background color. */
	inline const decColor &GetOptionBoxBackgroundColor() const{ return pOptionBoxBackgroundColor; }
	
	/** \brief Set option box background color. */
	void SetOptionBoxBackgroundColor( const decColor &color );
	
	/** \brief Option box text color. */
	inline const decColor &GetOptionBoxTextColor() const{ return pOptionBoxTextColor; }
	
	/** \brief Set option box text color. */
	void SetOptionBoxTextColor( const decColor &color );
	
	/** \brief Option box image deselected. */
	inline dealImage *GetOptionBoxImageDeselected() const{ return pOptionBoxImageDeselected; }
	
	/** \brief Set option box image deselected. */
	void SetOptionBoxImageDeselected( dealImage *image );
	
	/** \brief Option box image selected. */
	inline dealImage *GetOptionBoxImageSelected() const{ return pOptionBoxImageSelected; }
	
	/** \brief Set option box image selected. */
	void SetOptionBoxImageSelected( dealImage *image );
	
	/** \brief Option box image scale. */
	inline float GetOptionBoxImageScale() const{ return pOptionBoxImageScale; }
	
	/** \brief Set option box image scale. */
	void SetOptionBoxImageScale( float scale );
	
	/** \brief Option box image deselected colorize. */
	inline const decColor &GetOptionBoxImageDeselectedColorize() const{ return pOptionBoxImageDeselectedColorize; }
	
	/** \brief Set option box image deselected colorize. */
	void SetOptionBoxImageDeselectedColorize( const decColor &color );
	
	/** \brief Option box image selected colorize. */
	inline const decColor &GetOptionBoxImageSelectedColorize() const{ return pOptionBoxImageSelectedColorize; }
	
	/** \brief Set option box image selected colorize. */
	void SetOptionBoxImageSelectedColorize( const decColor &color );
	/*@}*/
};

#endif
