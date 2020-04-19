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

#ifndef _IGDECONTAINERBORDER_H_
#define _IGDECONTAINERBORDER_H_

#include "../igdeContainer.h"



/**
 * \brief IGDE UI Container Widget grouping widgets in border layout.
 * 
 * The layout contains 5 areas: top, left, right, bottom and center. Each area can contain
 * at most one widget. Thus in total at most 5 Widgets can be added to this layout. The
 * widgets in the side areas are stretched sidewards to close gaps with the center oriented
 * size being set to the desired widget size. The center widget is stretched in both axis
 * to fill the available space. The top and bottom sides stretch along the entire widget
 * size. The left and right areas touch up to the top and bottom widget. Hence the top and
 * bottom sides have priority over the left and side side.
 */
class igdeContainerBorder : public igdeContainer{
public:
	/** \brief Area. */
	enum eArea{
		/** \brief Top side. */
		eaTop,
		
		/** \brief Bottom side. */
		eaBottom,
		
		/** \brief Left side. */
		eaLeft,
		
		/** \brief Right side. */
		eaRight,
		
		/** \brief Center. */
		eaCenter
	};
	
	
	
private:
	int pSpacing;
	igdeWidget *pWidgetTop;
	igdeWidget *pWidgetBottom;
	igdeWidget *pWidgetLeft;
	igdeWidget *pWidgetRight;
	igdeWidget *pWidgetCenter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeContainerBorder( igdeEnvironment &environment, int spacing = 0 );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainerBorder();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Spacing. */
	inline int GetSpacing() const{ return pSpacing; }
	
	/** \brief Add child to area. */
	virtual void AddChild( igdeWidget *child, eArea area );
	
	/** \brief Add child not possible without specifying an area. */
	virtual void AddChild( igdeWidget *child );
	
	/** \brief Remove child. */
	virtual void RemoveChild( igdeWidget *child );
	
	/** \brief Remove all children. */
	virtual void RemoveAllChildren();
	
	/** \brief Widget in area or NULL. */
	igdeWidget *GetWidgetIn( eArea area ) const;
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
