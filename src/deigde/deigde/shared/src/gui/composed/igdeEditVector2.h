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

#ifndef _IGDEEDITVECTOR2_H_
#define _IGDEEDITVECTOR2_H_

#include "../igdeTextFieldReference.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerBoxAlternate.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class igdeEditVector2Listener;
class igdeUIHelper;


/**
 * \brief IGDE UI Edit Vector2.
 * 
 * Composed widget to edit decVector2.
 */
class DE_DLL_EXPORT igdeEditVector2 : public igdeContainerBoxAlternate{
protected:
	/** \brief Text field listener. */
	class DE_DLL_EXPORT cListener : public igdeTextFieldListener{
	protected:
		igdeEditVector2 &pEditVector2;
		igdeTextFieldReference pTextX;
		igdeTextFieldReference pTextY;
		
	public:
		cListener( igdeEditVector2 &editVector2, igdeTextField *textX, igdeTextField *textY );
		virtual ~cListener();
		virtual void OnTextChanged( igdeTextField *textField );
		virtual void OnTextChanging( igdeTextField *textField );
	};
	
	
	
private:
	bool pEnabled;
	decVector2 pVector2;
	int pColumns;
	bool pEditable;
	decString pDescription;
	int pPrecision;
	
	igdeTextFieldReference pTextX;
	igdeTextFieldReference pTextY;
	bool pPreventUpdate;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create edit vector. */
	igdeEditVector2( igdeUIHelper &helper, int columns, int precision, const char *description = "" );
	
	igdeEditVector2( igdeUIHelper &helper, int columns, int precision, bool editable,
		const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       vectorer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeEditVector2();
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
	
	
	
	/** \brief Vector2. */
	inline const decVector2 &GetVector2() const{ return pVector2; }
	
	/** \brief Set vector. */
	void SetVector2( const decVector2 &vector );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeEditVector2Listener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeEditVector2Listener *listener );
	
	/** \brief Notify listeners vector changed. */
	virtual void NotifyVector2Changed();
	/*@}*/
	
	
	
protected:
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/** \brief Vector2 changed. */
	virtual void OnVector2Changed();
	
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
