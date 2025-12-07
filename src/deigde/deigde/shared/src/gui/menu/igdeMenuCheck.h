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

#ifndef _IGDEMENUCHECK_H_
#define _IGDEMENUCHECK_H_

#include "igdeMenuCommand.h"



/**
 * \brief IGDE UI Check Menu Entry.
 * 
 * Menu entry changes "checked" state and calls OnAction() when user clicks menu
 * and the menu is enabled.
 */
class DE_DLL_EXPORT igdeMenuCheck : public igdeMenuCommand{
public:
	/** \brief Strong reference. */
	typedef deTObjectReference<igdeMenuCheck> Ref;
	
	
private:
	bool pChecked;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create menu entry. */
	igdeMenuCheck(igdeEnvironment &environment);
	
	/** \brief Create menu entry. */
	igdeMenuCheck(igdeEnvironment &environment, igdeAction *action);
	
	
	
protected:
	/**
	 * \brief Clean up menu entry.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeMenuCheck();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Menu entry is checked. */
	inline bool GetChecked() const{ return pChecked; }
	
	/** \brief Set if menu entry is checked. */
	void SetChecked(bool checked);
	
	
	
	/** \brief Action parameters changed. */
	virtual void OnParameterChanged(igdeAction *action);
	/*@}*/
	
	
	
	/**
	 * \text IGDE Internal Use Only
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
	/*@}*/
};

#endif
