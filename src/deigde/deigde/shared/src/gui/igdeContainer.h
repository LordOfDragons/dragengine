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

#ifndef _IGDECONTAINER_H_
#define _IGDECONTAINER_H_

#include "igdeWidget.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>



/**
 * \brief IGDE UI Container Widget grouping widgets.
 */
class DE_DLL_EXPORT igdeContainer : public igdeWidget{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeContainer> Ref;
	
	
private:
	decObjectOrderedSet pChildren;
	
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget with layout elNone. */
	igdeContainer( igdeEnvironment &environment );
	
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of children. */
	int GetChildCount() const;
	
	/** \brief Get child at index. */
	igdeWidget *GetChildAt( int index ) const;
	
	/** \brief Index of child or -1 if absent. */
	int IndexOfChild( igdeWidget *widget ) const;
	
	/** \brief Add child. */
	virtual void AddChild( igdeWidget *child );
	
	/** \brief Remove child. */
	virtual void RemoveChild( igdeWidget *child );
	
	/** \brief Remove all children. */
	virtual void RemoveAllChildren();
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Drop native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DropNativeWidget();
	
	/**
	 * \brief Get native container widget pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void *GetNativeContainer() const;
	
	
	
protected:
	/**
	 * \brief Create child widget native widgets.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateChildWidgetNativeWidgets();
	
	/** \brief GuiTheme changed. */
	virtual void OnGuiThemeChanged();
	
	/**
	 * \brief Notify children gui theme changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void NotifyChildrenGuiThemeChanged();
	/*@}*/
};

#endif
