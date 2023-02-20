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

#ifndef _IGDEPROGRESSBAR_H_
#define _IGDEPROGRESSBAR_H_

#include <stdlib.h>

#include "igdeWidget.h"

#include <dragengine/common/string/decString.h>


/**
 * \brief IGDE UI ProgressBar.
 */
class DE_DLL_EXPORT igdeProgressBar : public igdeWidget{
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
	int pValue;
	decString pDescription;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create textfield. */
	igdeProgressBar( igdeEnvironment &environment, int lower, int upper,
		eOrientation orientation, const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeProgressBar();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Orientation. */
	inline eOrientation GetOrientation() const{ return pOrientation; }
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	
	
	/** \brief Value. */
	inline int GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue( int value );
	
	/** \brief Range lower value. */
	inline int GetLower() const{ return pLower; }
	
	/** \brief Range upper value. */
	inline int GetUpper() const{ return pUpper; }
	
	/** \brief Set range. */
	void SetRange( int lower, int upper );
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
	/** \brief Range or tick spacing changed. */
	virtual void OnRangeChanged();
	
	/** \brief Value changed. */
	virtual void OnValueChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	/*@}*/
};

#endif
