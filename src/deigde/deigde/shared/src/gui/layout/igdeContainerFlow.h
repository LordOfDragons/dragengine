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

#ifndef _IGDECONTAINERFLOW_H_
#define _IGDECONTAINERFLOW_H_

#include "../igdeContainer.h"


/**
 * \brief IGDE UI Container Widget grouping widgets in Flow layout.
 * 
 * Widgets are placed next to each other along the X or Y axis. Widget sizes along the axis
 * are sized to the widget desired size. The widget sizes along the other axis is set to
 * fill the available space. Optionally the first or last widget size along the axis can
 * be stretched to fill the remaining space.
 */
class DE_DLL_EXPORT igdeContainerFlow : public igdeContainer{
public:
	/** \brief Layout axis for subclasses. */
	enum eAxis{
		/** \brief Layout along the X axis. */
		eaX,
		
		/** \brief Layout along the Y axis. */
		eaY,
		
		/** \brief Layout along the X axis in reversed order. */
		eaXReverse,
		
		/** \brief Layout along the Y axis in reversed order. */
		eaYReverse,
		
		/** \brief Layout along the X axis centered. */
		eaXCenter,
		
		/** \brief Layout along the Y axis centered. */
		eaYCenter
	};
	
	/** \brief Stretching. */
	enum eStretching{
		/** \brief No stretching. */
		esNone,
		
		/** \brief Stretch first widget to fill remaining space. */
		esFirst,
		
		/** \brief Stretch last widget to fill remaining space. */
		esLast
	};
	
	
	
private:
	eAxis pAxis;
	eStretching pStretching;
	int pSpacing;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeContainerFlow( igdeEnvironment &environment, eAxis axis,
		eStretching stretching = esNone, int spacing = 0 );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainerFlow();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Layout axis. */
	inline eAxis GetAxis() const{ return pAxis; }
	
	/** \brief Stretching. */
	inline eStretching GetStretching() const{ return pStretching; }
	
	/** \brief Spacing. */
	inline int GetSpacing() const{ return pSpacing; }
	
	
	
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
	
	/**
	 * \brief Get native container widget pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void *GetNativeContainer() const;
};

#endif
