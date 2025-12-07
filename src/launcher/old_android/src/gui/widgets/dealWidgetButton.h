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

#ifndef _DEALWIDGETBUTTON_H_
#define _DEALWIDGETBUTTON_H_

#include "dealWidgetLayoutFlow.h"
#include "../../common/string/decString.h"

class dealWidgetLabel;
class dealWidgetImage;
class dealWidgetBorderImage;
class dealImage;


/**
 * \brief Button with text, image and optional background border image.
 */
class dealWidgetButton : public dealWidgetLayoutFlow{
private:
	decColor pBgColorNormal;
	decColor pTextColorNormal;
	decColor pBgColorPressed;
	decColor pTextColorPressed;
	
	dealWidgetLabel *pLabel;
	dealWidgetImage *pImage;
	dealWidgetBorderImage *pBgImageNormal;
	dealWidgetBorderImage *pBgImagePressed;
	
	bool pPressed;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetButton(dealDisplay &display, dealImage *image = NULL, const char *label = "");
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetButton();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set widget from gui theme. */
	virtual void SetFromGuiTheme(const dealGuiTheme &guitheme);
	
	
	
	/** \brief Background color normal state. */
	inline const decColor &GetBgColorNormal() const{ return pBgColorNormal; }
	
	/** \brief Set background color normal state. */
	void SetBgColorNormal(const decColor &color);
	
	/** \brief Text color normal. */
	inline const decColor &GetTextColorNormal() const{ return pTextColorNormal; }
	
	/** \brief Set text color normal. */
	void SetTextColorNormal(const decColor &color);
	
	/** \brief Background image colorize normal state. */
	const decColor &GetBgImageColorizeNormal() const;
	
	/** \brief Set background image colorize normal state. */
	void SetBgImageColorizeNormal(const decColor &color);
	
	/** \brief Background color pressed state. */
	inline const decColor &GetBgColorPressed() const{ return pBgColorPressed; }
	
	/** \brief Set background color pressed state. */
	void SetBgColorPressed(const decColor &color);
	
	/** \brief Text color pressed. */
	inline const decColor &GetTextColorPressed() const{ return pTextColorPressed; }
	
	/** \brief Set text color pressed. */
	void SetTextColorPressed(const decColor &color);
	
	/** \brief Background image colorize pressed state. */
	const decColor &GetBgImageColorizePressed() const;
	
	/** \brief Set background image colorize pressed state. */
	void SetBgImageColorizePressed(const decColor &color);
	
	
	
	/** \brief Label or empty string if not displayed. */
	const decString &GetLabel() const;
	
	/** \brief Set label or empty string to not displayed. */
	void SetLabel(const char *label);
	
	/** \brief Image or \em NULL if not set. */
	dealImage *GetImage() const;
	
	/** \brief Set image or \em NULL if not set. */
	void SetImage(dealImage *image);
	
	/** \brief Bordered background image widget for normal state. */
	inline dealWidgetBorderImage &GetBgImageNormal() const{ return *pBgImageNormal; }
	
	/** \brief Bordered background image widget for pressed state. */
	inline dealWidgetBorderImage &GetBgImagePressed() const{ return *pBgImagePressed; }
	
	
	
	/** \brief Button is pressed. */
	inline bool GetPressed() const{ return pPressed; }
	
	/** \brief Set if button is pressed. */
	void SetPressed(bool pressed);
	
	
	
	/** \brief Render background. */
	virtual void RenderBackground(const sRenderContext &context);
	
	
	
	/** \brief Size changed. */
	virtual void OnSizeChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Button state changed. */
	virtual void OnStateChanged();
	
	/** \brief Run button action. */
	virtual void OnAction();
	
	
	
	/**
	 * \brief Key has been pressed down.
	 * \param[in] keycode A value of AKEYCODE_*.
	 */
	virtual void OnKeyPress(int keycode);
	
	/**
	 * \brief Key has been released.
	 * \param[in] keycode A value of AKEYCODE_*.
	 */
	virtual void OnKeyRelease(int keycode);
	
	/**
	 * \brief Mouse button press / finger press.
	* \param[in] buttons Buttons pressed while pressing screen. OR of values of AMOTION_EVENT_BUTTON_.
	 */
	virtual void OnMousePress(int buttons, const decPoint &position);
	
	/**
	 * \brief Mouse button release / finger release.
	 * \param[in] buttons Buttons pressed while releasing screen. OR of values of AMOTION_EVENT_BUTTON_.
	 */
	virtual void OnMouseRelease(int buttons, const decPoint &position);
	
	/**
	 * \brief Mouse/finger moved while pressing display.
	 * \param[in] buttons Buttons pressed while moving. OR of values of AMOTION_EVENT_BUTTON_.
	 */
	virtual void OnMouseMove(int buttons, const decPoint &position);
	/*@}*/
	
	
	
protected:
	virtual void pUpdateContentPressedState();
	
private:
	void pBuildContent();
};

#endif
