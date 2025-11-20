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
	/** \brief Strong reference. */
	typedef deTObjectReference<igdeButton> Ref;
	
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
	~igdeButton() override;
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
	void OnParameterChanged( igdeAction *action ) override;
	
	/** \brief Action has been destroyed. */
	void OnDestroyed( igdeAction *action ) override;
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
