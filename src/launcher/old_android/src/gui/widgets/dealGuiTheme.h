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
	dealFont::Ref pWidgetTextFont;
	int pWidgetTextFontSize;
	
	decColor pButtonBackgroundColor;
	decColor pButtonTextColor;
	dealImage::Ref pButtonBackgroundImage;
	int pButtonBackgroundImageBorder[4];
	float pButtonBackgroundImageScale;
	decColor pButtonBackgroundImageColorize;
	
	decColor pButtonPressedBackgroundColor;
	decColor pButtonPressedTextColor;
	dealImage::Ref pButtonPressedBackgroundImage;
	int pButtonPressedBackgroundImageBorder[4];
	float pButtonPressedBackgroundImageScale;
	decColor pButtonPressedBackgroundImageColorize;
	
	decColor pCheckBoxBackgroundColor;
	decColor pCheckBoxTextColor;
	dealImage::Ref pCheckBoxImageUnchecked;
	dealImage::Ref pCheckBoxImageChecked;
	float pCheckBoxImageScale;
	decColor pCheckBoxImageUncheckedColorize;
	decColor pCheckBoxImageCheckedColorize;
	
	decColor pOptionBoxBackgroundColor;
	decColor pOptionBoxTextColor;
	dealImage::Ref pOptionBoxImageDeselected;
	dealImage::Ref pOptionBoxImageSelected;
	float pOptionBoxImageScale;
	decColor pOptionBoxImageDeselectedColorize;
	decColor pOptionBoxImageSelectedColorize;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dealGuiTheme> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gui theme. */
	dealGuiTheme(dealDisplay &display);
	
	/** \brief Create copy of gui theme. */
	dealGuiTheme(const dealGuiTheme &copy);
	
	/** \brief Clean up gui theme. */
	~dealGuiTheme() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Widget background color. */
	inline const decColor &GetWidgetBackgroundColor() const{ return pWidgetBackgroundColor; }
	
	/** \brief Set widget background color. */
	void SetWidgetBackgroundColor(const decColor &color);
	
	/** \brief Widget text color. */
	inline const decColor &GetWidgetTextColor() const{ return pWidgetTextColor; }
	
	/** \brief Set widget text color. */
	void SetWidgetTextColor(const decColor &color);
	
	/** \brief Widget text font. */
	inline dealFont *GetWidgetTextFont() const{ return pWidgetTextFont; }
	
	/** \brief Set widget text font. */
	void SetWidgetTextFont(dealFont::Ref font);
	
	/** \brief Widget text font size. */
	inline int GetWidgetTextFontSize() const{ return pWidgetTextFontSize; }
	
	/** \brief Set widget text font size. */
	void SetWidgetTextFontSize(int size);
	
	
	
	/** \brief Button background color. */
	inline const decColor &GetButtonBackgroundColor() const{ return pButtonBackgroundColor; }
	
	/** \brief Set button background color. */
	void SetButtonBackgroundColor(const decColor &color);
	
	/** \brief Button text color. */
	inline const decColor &GetButtonTextColor() const{ return pButtonTextColor; }
	
	/** \brief Set button text color. */
	void SetButtonTextColor(const decColor &color);
	
	/** \brief Button background image or \em NULL if not used. */
	inline dealImage *GetButtonBackgroundImage() const{ return pButtonBackgroundImage; }
	
	/** \brief Set button background image or \em NULL if not used. */
	void SetButtonBackgroundImage(dealImage::Ref image);
	
	/** \brief Button background image border. */
	int GetButtonBackgroundImageBorder(int border) const;
	
	/** \brief Set button background image border. */
	void SetButtonBackgroundImageBorder(int border, int size);
	
	/** \brief Button background image scale. */
	inline float GetButtonBackgroundImageScale() const{ return pButtonBackgroundImageScale; }
	
	/** \brief Set button background image scale. */
	void SetButtonBackgroundImageScale(float scale);
	
	/** \brief Button background image colorize. */
	inline const decColor &GetButtonBackgroundImageColorize() const{ return pButtonBackgroundImageColorize; }
	
	/** \brief Set button background image colorize. */
	void SetButtonBackgroundImageColorize(const decColor &color);
	
	
	
	/** \brief Pressed buttno background color. */
	inline const decColor &GetButtonPressedBackgroundColor() const{ return pButtonPressedBackgroundColor; }
	
	/** \brief Set pressed button background color. */
	void SetButtonPressedBackgroundColor(const decColor &color);
	
	/** \brief Pressed button text color. */
	inline const decColor &GetButtonPressedTextColor() const{ return pButtonPressedTextColor; }
	
	/** \brief Set pressed button text color. */
	void SetButtonPressedTextColor(const decColor &color);
	
	/** \brief Pressed button background image or \em NULL if not used. */
	inline dealImage *GetButtonPressedBackgroundImage() const{ return pButtonPressedBackgroundImage; }
	
	/** \brief Set pressed button background image or \em NULL if not used. */
	void SetButtonPressedBackgroundImage(dealImage::Ref image);
	
	/** \brief Pressed button background image border. */
	int GetButtonPressedBackgroundImageBorder(int border) const;
	
	/** \brief Set pressed button background image border. */
	void SetButtonPressedBackgroundImageBorder(int border, int size);
	
	/** \brief Pressed button background image scale. */
	inline float GetButtonPressedBackgroundImageScale() const{ return pButtonPressedBackgroundImageScale; }
	
	/** \brief Set pressed button background image scale. */
	void SetButtonPressedBackgroundImageScale(float scale);
	
	/** \brief Pressed button background image colorize. */
	inline const decColor &GetButtonPressedBackgroundImageColorize() const{ return pButtonPressedBackgroundImageColorize; }
	
	/** \brief Set pressed button background image colorize. */
	void SetButtonPressedBackgroundImageColorize(const decColor &color);
	
	
	
	/** \brief Check box background color. */
	inline const decColor &GetCheckBoxBackgroundColor() const{ return pCheckBoxBackgroundColor; }
	
	/** \brief Set check box background color. */
	void SetCheckBoxBackgroundColor(const decColor &color);
	
	/** \brief Check box text color. */
	inline const decColor &GetCheckBoxTextColor() const{ return pCheckBoxTextColor; }
	
	/** \brief Set check box text color. */
	void SetCheckBoxTextColor(const decColor &color);
	
	/** \brief Check box image unchecked. */
	inline dealImage *GetCheckBoxImageUnchecked() const{ return pCheckBoxImageUnchecked; }
	
	/** \brief Set check box image unchecked. */
	void SetCheckBoxImageUnchecked(dealImage::Ref image);
	
	/** \brief Check box image checked. */
	inline dealImage *GetCheckBoxImageChecked() const{ return pCheckBoxImageChecked; }
	
	/** \brief Set check box image checked. */
	void SetCheckBoxImageChecked(dealImage::Ref image);
	
	/** \brief Check box image scale. */
	inline float GetCheckBoxImageScale() const{ return pCheckBoxImageScale; }
	
	/** \brief Set check box image scale. */
	void SetCheckBoxImageScale(float scale);
	
	/** \brief Check box image unchecked colorize. */
	inline const decColor &GetCheckBoxImageUncheckedColorize() const{ return pCheckBoxImageUncheckedColorize; }
	
	/** \brief Set check box image unchecked colorize. */
	void SetCheckBoxImageUncheckedColorize(const decColor &color);
	
	/** \brief Check box image checked colorize. */
	inline const decColor &GetCheckBoxImageCheckedColorize() const{ return pCheckBoxImageCheckedColorize; }
	
	/** \brief Set check box image checked colorize. */
	void SetCheckBoxImageCheckedColorize(const decColor &color);
	
	
	
	/** \brief Option box background color. */
	inline const decColor &GetOptionBoxBackgroundColor() const{ return pOptionBoxBackgroundColor; }
	
	/** \brief Set option box background color. */
	void SetOptionBoxBackgroundColor(const decColor &color);
	
	/** \brief Option box text color. */
	inline const decColor &GetOptionBoxTextColor() const{ return pOptionBoxTextColor; }
	
	/** \brief Set option box text color. */
	void SetOptionBoxTextColor(const decColor &color);
	
	/** \brief Option box image deselected. */
	inline dealImage *GetOptionBoxImageDeselected() const{ return pOptionBoxImageDeselected; }
	
	/** \brief Set option box image deselected. */
	void SetOptionBoxImageDeselected(dealImage::Ref image);
	
	/** \brief Option box image selected. */
	inline dealImage *GetOptionBoxImageSelected() const{ return pOptionBoxImageSelected; }
	
	/** \brief Set option box image selected. */
	void SetOptionBoxImageSelected(dealImage::Ref image);
	
	/** \brief Option box image scale. */
	inline float GetOptionBoxImageScale() const{ return pOptionBoxImageScale; }
	
	/** \brief Set option box image scale. */
	void SetOptionBoxImageScale(float scale);
	
	/** \brief Option box image deselected colorize. */
	inline const decColor &GetOptionBoxImageDeselectedColorize() const{ return pOptionBoxImageDeselectedColorize; }
	
	/** \brief Set option box image deselected colorize. */
	void SetOptionBoxImageDeselectedColorize(const decColor &color);
	
	/** \brief Option box image selected colorize. */
	inline const decColor &GetOptionBoxImageSelectedColorize() const{ return pOptionBoxImageSelectedColorize; }
	
	/** \brief Set option box image selected colorize. */
	void SetOptionBoxImageSelectedColorize(const decColor &color);
	/*@}*/
};

#endif
