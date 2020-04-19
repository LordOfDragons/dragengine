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

#ifndef _IGDECONTAINERSPLITTED_H_
#define _IGDECONTAINERSPLITTED_H_

#include "../igdeContainer.h"



/**
 * \brief IGDE UI Container Widget splitting space between two widgets.
 * 
 * The layout contains at most 2 widgets. Both widgets are set to fill the available space.
 * The first widget has desired size along the placement direction. The second widget is
 * stretched along the placement direction. Between the two widgets a grabber is displayed
 * allowing the user to change the size of the side widget.
 */
class igdeContainerSplitted : public igdeContainer{
public:
	/** \brief Side widget placement. */
	enum eSidePlacement{
		/** \brief Side left, center right. */
		espLeft,
		
		/** \brief Side right, center left. */
		espRight,
		
		/** \brief Side top, center below. */
		espTop,
		
		/** \brief Side bottom, center above. */
		espBottom
	};
	
	/** \brief Area. */
	enum eArea{
		/** \brief Side. */
		eaSide,
		
		/** \brief Center. */
		eaCenter
	};
	
	
	
private:
	eSidePlacement pSidePlacement;
	int pSplitPosition;
	float pSplitPositionRelative;
	igdeWidget *pWidgetSide;
	igdeWidget *pWidgetCenter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeContainerSplitted( igdeEnvironment &environment, eSidePlacement sidePlacement );
	
	/** \brief Create widget. */
	igdeContainerSplitted( igdeEnvironment &environment,
		eSidePlacement sidePlacement, int splitPosition );
	
	/** \brief Create widget. */
	igdeContainerSplitted( igdeEnvironment &environment,
		eSidePlacement sidePlacement, float splitPositionRelative );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainerSplitted();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Side placement. */
	inline eSidePlacement GetSidePlacement() const{ return pSidePlacement; }
	
	/** \brief Split position. */
	inline int GetSplitPosition() const{ return pSplitPosition; }
	
	/** \brief Set split position. */
	void SetSplitPosition( int splitPosition );
	
	/** \brief Relative split position. */
	inline float GetSplitPositionRelative() const{ return pSplitPositionRelative; }
	
	/** \brief Set relative split position. */
	void SetSplitPositionRelative( float splitPosition );
	
	
	
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
	
	/**
	 * \brief Create child widget native widgets.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateChildWidgetNativeWidgets();
	/*@}*/
};

#endif
