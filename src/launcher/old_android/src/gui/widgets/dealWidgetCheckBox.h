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
	dealWidgetCheckBox(dealDisplay &display, dealImage *image = NULL, const char *label = "");
	
	/** \brief Clean up widget. */
	~dealWidgetCheckBox() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set widget from gui theme. */
	void SetFromGuiTheme(const dealGuiTheme &guitheme) override;
	
	
	
	/** \brief Text color. */
	const decColor &GetTextColor() const;
	
	/** \brief Set text color. */
	void SetTextColor(const decColor &color);
	
	
	
	/** \brief Label or empty string if not displayed. */
	const decString &GetLabel() const;
	
	/** \brief Set label or empty string to not displayed. */
	void SetLabel(const char *label);
	
	/** \brief Image or \em NULL if not set. */
	dealImage *GetImage() const;
	
	/** \brief Set image or \em NULL if not set. */
	void SetImage(dealImage *image);
	
	/** \brief Bordered box image widget for unchecked state. */
	inline dealWidgetImage &GetBoxImageUnchecked() const{ return *pBoxImageUnchecked; }
	
	/** \brief Bordered box image widget for checked state. */
	inline dealWidgetImage &GetBoxImageChecked() const{ return *pBoxImageChecked; }
	
	
	
	/** \brief Button is checked. */
	inline bool GetChecked() const{ return pChecked; }
	
	/** \brief Set if button is checked. */
	void SetChecked(bool checked);
	
	
	
	/** \brief Enabled changed. */
	void OnEnabledChanged() override;
	
	/** \brief State changed. */
	virtual void OnStateChanged();
	
	
	
	/**
	 * \brief Key has been checked down.
	 * \param[in] keycode A value of AKEYCODE_*.
	 */
	void OnKeyPress(int keycode) override;
	
	/**
	 * \brief Key has been released.
	 * \param[in] keycode A value of AKEYCODE_*.
	 */
	void OnKeyRelease(int keycode) override;
	
	/**
	 * \brief Mouse button press / finger press.
	* \param[in] buttons Buttons checked while pressing screen. OR of values of AMOTION_EVENT_BUTTON_.
	 */
	void OnMousePress(int buttons, const decPoint &position) override;
	/*@}*/
	
	
	
private:
	void pBuildContent();
};

#endif
