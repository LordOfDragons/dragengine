/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
class igdeContainerForm : public igdeContainer{
public:
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
	igdeContainerForm( igdeEnvironment &environment, eStretching stretching = esNone,
		int columnSpacing = 15, int rowSpacing = 1 );
	
	
	
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
	inline eStretching GetStretching() const{ return pStretching; }
	
	/** \brief Spacing between columns in pixels. */
	inline int GetColumnSpacing() const{ return pColumnSpacing; }
	
	/** \brief Spacing between rows in pixels. */
	inline int GetRowSpacing() const{ return pRowSpacing; }
	
	
	
	/** \brief Remove child. */
	virtual void RemoveChild( igdeWidget *child );
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
