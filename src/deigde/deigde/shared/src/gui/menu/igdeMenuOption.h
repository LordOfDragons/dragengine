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

#ifndef _IGDEMENUOPTION_H_
#define _IGDEMENUOPTION_H_

#include "igdeMenuCommand.h"



/**
 * \brief IGDE UI Option Menu Entry.
 * 
 * Menu entry changes "selected" state and calls OnAction() when user clicks menu
 * and the menu is enabled. Only one igdeMenuOption in the same group of options
 * is selected at all times.
 */
class DE_DLL_EXPORT igdeMenuOption : public igdeMenuCommand{
public:
	/** \brief Strong reference. */
	typedef deTObjectReference<igdeMenuOption> Ref;
	
	
private:
	bool pSelected;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create menu entry. */
	igdeMenuOption(igdeEnvironment &environment);
	
	/** \brief Create menu entry. */
	igdeMenuOption(igdeEnvironment &environment, igdeAction *action);
	
	
	
protected:
	/**
	 * \brief Clean up menu entry.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeMenuOption();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Menu entry is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if menu entry is selected. */
	void SetSelected(bool selected);
	
	
	
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
	/** \brief Selected changed. */
	virtual void OnSelectedChanged();
	/*@}*/
};

#endif
