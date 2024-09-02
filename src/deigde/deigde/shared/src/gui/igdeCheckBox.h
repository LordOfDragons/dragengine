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

#ifndef _IGDECHECKBOX_H_
#define _IGDECHECKBOX_H_

#include "igdeButton.h"


/**
 * \brief IGDE UI CheckBox with text and icon.
 */
class DE_DLL_EXPORT igdeCheckBox : public igdeButton{
private:
	bool pChecked;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create checkbox. */
	igdeCheckBox( igdeEnvironment &environment, const char *text, igdeIcon *icon,
		eButtonStyle = ebsNormal );
	
	/** \brief Create checkbox. */
	igdeCheckBox( igdeEnvironment &environment, const char *text, const char *description,
		igdeIcon *icon, eButtonStyle = ebsNormal );
	
	/** \brief Create checkbox. */
	igdeCheckBox( igdeEnvironment &environment, igdeAction *action, eButtonStyle style = ebsNormal );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeCheckBox();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Menu entry is checked. */
	inline bool GetChecked() const{ return pChecked; }
	
	/** \brief Set if menu entry is checked. */
	void SetChecked( bool checked );
	
	
	
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
	/** \brief Checked changed. */
	virtual void OnCheckedChanged();
	
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
	/*@}*/
};

#endif
