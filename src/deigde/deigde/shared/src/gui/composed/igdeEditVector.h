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

#ifndef _IGDEEDITVECTOR_H_
#define _IGDEEDITVECTOR_H_

#include "../igdeTextFieldReference.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerBoxAlternate.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class igdeEditVectorListener;
class igdeUIHelper;


/**
 * \brief IGDE UI Edit Vector.
 * 
 * Composed widget to edit decVector.
 */
class igdeEditVector : public igdeContainerBoxAlternate{
protected:
	/** \brief Text field listener. */
	class cListener : public igdeTextFieldListener{
	protected:
		igdeEditVector &pEditVector;
		igdeTextFieldReference pTextX;
		igdeTextFieldReference pTextY;
		igdeTextFieldReference pTextZ;
		
	public:
		cListener( igdeEditVector &editVector, igdeTextField *textX,
			igdeTextField *textY, igdeTextField *textz );
		virtual ~cListener();
		virtual void OnTextChanged( igdeTextField *textField );
		virtual void OnTextChanging( igdeTextField *textField );
	};
	
	
	
private:
	bool pEnabled;
	decVector pVector;
	bool pEditable;
	decString pDescription;
	int pColumns;
	int pPrecision;
	
	igdeTextFieldReference pTextX;
	igdeTextFieldReference pTextY;
	igdeTextFieldReference pTextZ;
	bool pPreventUpdate;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create edit vector. */
	igdeEditVector( igdeUIHelper &helper, int columns, int precision,
		const char *description = "" );
	
	igdeEditVector( igdeUIHelper &helper, int columns, int precision,
		bool editable, const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       vectorer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeEditVector();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Widget is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if widget is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Visible columns in edit fields. */
	inline int GetColumns() const{ return pColumns; }
	
	/** \brief Widget is editable. */
	inline bool GetEditable() const{ return pEditable; }
	
	/** \brief Set if widget is editable. */
	void SetEditable( bool editable );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Precision for floating point values as digits after period. */
	inline int GetPrecision() const{ return pPrecision; }
	
	/** \brief Set precision for floating point values as digits after period. */
	void SetPrecision( int precision );
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Vector. */
	inline const decVector &GetVector() const{ return pVector; }
	
	/** \brief Set vector. */
	void SetVector( const decVector &vector );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeEditVectorListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeEditVectorListener *listener );
	
	/** \brief Notify listeners vector changed. */
	virtual void NotifyVectorChanged();
	/*@}*/
	
	
	
protected:
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/** \brief Vector changed. */
	virtual void OnVectorChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Editable changed. */
	virtual void OnEditableChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Precision changed. */
	virtual void OnPrecisionChanged();
	/*@}*/
	
	
	
private:
	void pCreateContent( igdeUIHelper &helper );
};

#endif
