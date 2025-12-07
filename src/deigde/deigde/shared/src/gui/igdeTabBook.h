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

#ifndef _IGDETABBOOK_H_
#define _IGDETABBOOK_H_

#include "igdeContainer.h"
#include "resources/igdeIcon.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief IGDE UI Tab book.
 */
class DE_DLL_EXPORT igdeTabBook : public igdeContainer{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeTabBook> Ref;
	
	
private:
	decObjectList pHeaders;
	int pActivePanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeTabBook(igdeEnvironment &environment);
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTabBook();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active panel index. */
	inline int GetActivePanel() const{ return pActivePanel; }
	
	/** \brief Set active panel by index. */
	void SetActivePanel(int index);
	
	
	
	/** \brief Add child . */
	virtual void AddChild(igdeWidget *child, const char *text);
	
	/** \brief Add child . */
	virtual void AddChild(igdeWidget *child, const char *text, igdeIcon *icon);
	
	/** \brief Add child . */
	virtual void AddChild(igdeWidget *child, const char *text, igdeIcon *icon,
		const char *description);
	
	/** \brief Add child not possible without header information. */
	virtual void AddChild(igdeWidget *child);
	
	/** \brief Remove child. */
	virtual void RemoveChild(igdeWidget *child);
	
	/** \brief Remove all children. */
	virtual void RemoveAllChildren();
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Header.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	class DE_DLL_EXPORT cHeader : public deObject{
	public:
		/** \brief Type holding strong reference. */
		typedef deTObjectReference<cHeader> Ref;
		
		
		decString text;
		decString description;
		igdeIcon::Ref icon;
		
		cHeader(const char *text, igdeIcon *icon, const char *description);
		virtual ~cHeader();
	};
	
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
	/*@}*/
};

#endif
