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

#ifndef _IGDETOGGLEBUTTON_H_
#define _IGDETOGGLEBUTTON_H_

#include "igdeButton.h"


/**
 * \brief IGDE UI Toggle button with text and icon.
 */
class DE_DLL_EXPORT igdeToggleButton : public igdeButton{

public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<igdeToggleButton>;
	
	
private:
	bool pToggled;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create button. */
	igdeToggleButton(igdeEnvironment &environment, const char *text, igdeIcon *icon,
		eButtonStyle = ebsNormal);
	
	/** \brief Create button. */
	igdeToggleButton(igdeEnvironment &environment, const char *text, const char *description,
		igdeIcon *icon, eButtonStyle = ebsNormal);
	
	/** \brief Create button. */
	igdeToggleButton(igdeEnvironment &environment, igdeAction *action, eButtonStyle style = ebsNormal);
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	~igdeToggleButton() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Button is toggled. */
	inline bool GetToggled() const{ return pToggled; }
	
	/** \brief Set if button is toggled. */
	void SetToggled(bool toogled);
	
	
	
	/**
	 * \brief Run button action.
	 * 
	 * Called if button is clicked by the user. Default implementation toggled button.
	 */
	void OnAction() override;
	
	/** \brief Action parameters changed. */
	void OnParameterChanged(igdeAction *action) override;
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
	void CreateNativeWidget() override;
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void DestroyNativeWidget() override;
	
	
	
protected:
	/** \brief Style changed. */
	void OnStyleChanged() override;
	
	/** \brief Text changed. */
	void OnTextChanged() override;
	
	/** \brief Description changed. */
	void OnDescriptionChanged() override;
	
	/** \brief Icon changed. */
	void OnIconChanged() override;
	
	/** \brief Enabled changed. */
	void OnEnabledChanged() override;
	
	/** \brief Default changed. */
	void OnDefaultChanged() override;
	
	/** \brief Toggled changed. */
	virtual void OnToggledChanged();
	
	/** \brief Active language changed. */
	void OnLanguageChanged() override;
	/*@}*/
};

#endif
