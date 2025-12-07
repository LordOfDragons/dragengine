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

#ifndef _IGDECONTAINERFORM_H_
#define _IGDECONTAINERFORM_H_

#include "../igdeContainer.h"



/**
 * \brief IGDE UI Container Widget grouping widgets in Form layout.
 * 
 * Widgets are placed in rows from top to bothm in groups of two widgets each row.
 * The first widgets of all rows have the size of the widest first widget along the X axis.
 * The second widgets of all rows are sized to fill the remaining space.
 * 
 * This layout is best used for dialog or side panels showing labelled properties.
 */
class DE_DLL_EXPORT igdeContainerForm : public igdeContainer{
public:
	/** \brief Strong reference. */
	typedef deTObjectReference<igdeContainerForm> Ref;
	
	
	/** \brief Stretching. */
	enum eStretching{
		/** \brief No stretching. */
		esNone,
		
		/** \brief Stretch first row to fill remaining space. */
		esFirst,
		
		/** \brief Stretch last row to fill remaining space. */
		esLast
	};
	
	
	
private:
	eStretching pStretching;
	int pColumnSpacing;
	int pRowSpacing;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeContainerForm(igdeEnvironment &environment, eStretching stretching = esNone,
		int columnSpacing = 15, int rowSpacing = 1);
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainerForm();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Stretching. */
	inline eStretching GetStretching() const{return pStretching;}
	
	/** \brief Spacing between columns in pixels. */
	inline int GetColumnSpacing() const{return pColumnSpacing;}
	
	/** \brief Spacing between rows in pixels. */
	inline int GetRowSpacing() const{return pRowSpacing;}
	
	
	
	/** \brief Remove child. */
	virtual void RemoveChild(igdeWidget *child);
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
