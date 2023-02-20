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

#ifndef _IGDECONTAINERBOX_H_
#define _IGDECONTAINERBOX_H_

#include "../igdeContainer.h"



/**
 * \brief IGDE UI Container Widget grouping widgets in Box layout.
 * 
 * Widgets are placed next to each other along the X or Y axis. Widget sizes along the axis
 * are equally sized relative to their desired size to fill all the available space.
 * The widget sizes along the other axis is set to fill the available space.
 */
class DE_DLL_EXPORT igdeContainerBox : public igdeContainer{
public:
	/** \brief Layout axis for subclasses. */
	enum eAxis{
		/** \brief Layout along the X axis. */
		eaX,
		
		/** \brief Layout along the Y axis. */
		eaY,
	};
	
	
	
private:
	eAxis pAxis;
	int pSpacing;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeContainerBox( igdeEnvironment &environment, eAxis axis, int spacing = 0 );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainerBox();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Layout axis. */
	inline eAxis GetAxis() const{ return pAxis; }
	
	/** \brief Spacing. */
	inline int GetSpacing() const{ return pSpacing; }
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
