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

#ifndef _IGDECONTAINERBOXALTERNATE_H_
#define _IGDECONTAINERBOXALTERNATE_H_

#include "igdeContainerBox.h"



/**
 * \brief IGDE UI Container Widget grouping widgets in alternating Box layout.
 * 
 * Works similar to igdeContainerBox with the exception that every second widget is kept at
 * the minimum size instead of being stretched. This allows to create equally stretched
 * widgets separated by non-stretched widgets.
 */
class DE_DLL_EXPORT igdeContainerBoxAlternate : public igdeContainerBox{
private:
	bool pStretchOdd;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create widget.
	 * 
	 * If \em stretchOdd is \em true widgets with odd indices are stretched.
	 * Otherwise widgets with even indices are stretched.
	 */
	igdeContainerBoxAlternate( igdeEnvironment &environment, eAxis axis,
		bool stretchOdd, int spacing = 0 );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainerBoxAlternate();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Stretch widgets with odd indices. */
	inline bool GetStretchOdd() const{ return pStretchOdd; }
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
