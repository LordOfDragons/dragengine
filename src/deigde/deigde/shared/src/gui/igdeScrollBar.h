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

#ifndef _IGDESCROLLBAR_H_
#define _IGDESCROLLBAR_H_

#include "igdeWidget.h"
#include "event/igdeActionReference.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>


class igdeScrollBarListener;


/**
 * \brief IGDE UI ScrollBar.
 */
class igdeScrollBar : public igdeWidget{
public:
	/** \brief Orientation. */
	enum eOrientation{
		/** \brief Horizontal. */
		eoHorizontal,
		
		/** \brief Vertical. */
		eoVertical
	};
	
	
	
private:
	eOrientation pOrientation;
	int pLower;
	int pUpper;
	int pPageSize;
	int pValue;
	bool pEnabled;
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create button. */
	igdeScrollBar( igdeEnvironment &environment, eOrientation orientation );
	
	/** \brief Create button. */
	igdeScrollBar( igdeEnvironment &environment, eOrientation orientation,
		int lower, int upper, int pageSize, int value );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeScrollBar();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Orientation. */
	inline eOrientation GetOrientation() const{ return pOrientation; }
	
	/** \brief Range lower value. */
	inline int GetLower() const{ return pLower; }
	
	/** \brief Set range lower value. */
	void SetLower( int lower );
	
	/** \brief Range upper value. */
	inline int GetUpper() const{ return pUpper; }
	
	/** \brief Set range upper value. */
	void SetUpper( int upper );
	
	/** \brief Page size. */
	inline int GetPageSize() const{ return pPageSize; }
	
	/** \brief Set page size. */
	void SetPageSize( int pageSize );
	
	/** \brief Value. */
	inline int GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue( int value );
	
	/** \brief ScrollBar is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if button is enabled. */
	void SetEnabled( bool enabled );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeScrollBarListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeScrollBarListener *listener );
	
	/** \brief Notify listeners value changed. */
	void NotifyValueChanged();
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
	
	
	
protected:
	/** \brief Range or page size changed. */
	virtual void OnRangeChanged();
	
	/** \brief Value changed. */
	virtual void OnValueChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	/*@}*/
};

#endif
