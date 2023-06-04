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

#ifndef _IGDEEDITDVECTOR_H_
#define _IGDEEDITDVECTOR_H_

#include "../igdeTextFieldReference.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerBoxAlternate.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class igdeEditDVectorListener;
class igdeUIHelper;


/**
 * \brief IGDE UI Edit DVector.
 * 
 * Composed widget to edit decDVector.
 */
class DE_DLL_EXPORT igdeEditDVector : public igdeContainerBoxAlternate{
protected:
	/** \brief Text field listener. */
	class DE_DLL_EXPORT cListener : public igdeTextFieldListener{
	protected:
		igdeEditDVector &pEditDVector;
		igdeTextFieldReference pTextX;
		igdeTextFieldReference pTextY;
		igdeTextFieldReference pTextZ;
		
	public:
		cListener( igdeEditDVector &editDVector, igdeTextField *textX,
			igdeTextField *textY, igdeTextField *textz );
		virtual ~cListener();
		virtual void OnTextChanged( igdeTextField *textField );
		virtual void OnTextChanging( igdeTextField *textField );
	};
	
	
	
private:
	bool pEnabled;
	decDVector pDVector;
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
	igdeEditDVector( igdeUIHelper &helper, int columns, int precision,
		const char *description = "" );
	
	igdeEditDVector( igdeUIHelper &helper, int columns, int precision,
		bool editable, const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       vectorer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeEditDVector();
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
	
	
	
	/** \brief DVector. */
	inline const decDVector &GetDVector() const{ return pDVector; }
	
	/** \brief Set vector. */
	void SetDVector( const decDVector &vector );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeEditDVectorListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeEditDVectorListener *listener );
	
	/** \brief Notify listeners vector changed. */
	virtual void NotifyDVectorChanged();
	/*@}*/
	
	
	
protected:
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/** \brief DVector changed. */
	virtual void OnDVectorChanged();
	
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
