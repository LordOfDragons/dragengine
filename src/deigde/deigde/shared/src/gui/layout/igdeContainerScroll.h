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

#ifndef _IGDECONTAINERSCROLL_H_
#define _IGDECONTAINERSCROLL_H_

#include "../igdeContainer.h"


/**
 * \brief IGDE UI Container Widget scrolling one widget.
 */
class DE_DLL_EXPORT igdeContainerScroll : public igdeContainer{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeContainerScroll> Ref;
	
	
private:
	bool pCanScrollX;
	bool pCanScrollY;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create scrollbox. */
	igdeContainerScroll(igdeEnvironment &environment, bool canScrollX = true, bool canScrollY = true);
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainerScroll();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Can scroll X. */
	inline bool GetCanScrollX() const{ return pCanScrollX; }
	
	/** \brief Can scroll Y. */
	inline bool GetCanScrollY() const{ return pCanScrollY; }
	
	
	
	/** \brief Add child. */
	virtual void AddChild(igdeWidget *child);
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
	/*@}*/
};

#endif
