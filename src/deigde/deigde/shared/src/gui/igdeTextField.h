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

#ifndef _IGDETEXTFIELD_H_
#define _IGDETEXTFIELD_H_

#include <stdlib.h>

#include "igdeWidget.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>


class igdeTextFieldListener;


/**
 * \brief IGDE UI TextField.
 */
class DE_DLL_EXPORT igdeTextField : public igdeWidget{
private:
	bool pEnabled;
	decString pText;
	int pColumns;
	bool pEditable;
	decString pDescription;
	int pPrecision;
	bool pInvalidValue;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create textfield. */
	igdeTextField( igdeEnvironment &environment, int columns, const char *description = "" );
	
	igdeTextField( igdeEnvironment &environment, int columns, bool editable,
		const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTextField();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief TextField is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if button is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Visible columns in edit field. */
	inline int GetColumns() const{ return pColumns; }
	
	/** \brief Text is editable. */
	inline bool GetEditable() const{ return pEditable; }
	
	/** \brief Set if text is editable. */
	void SetEditable( bool editable );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Precision for floating point values as digits after period. */
	inline int GetPrecision() const{ return pPrecision; }
	
	/** \brief Set precision for floating point values as digits after period. */
	void SetPrecision( int precision );
	
	/** \brief Mark widget as having an invalue value. */
	inline bool GetInvalidValue() const{ return pInvalidValue; }
	
	/** \brief Set to mark widget as having an invalid value. */
	void SetInvalidValue( bool invalidValue );
	
	
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const char *text, bool changing = false, bool forceNotify = false );
	
	/** \brief Clear text. */
	void ClearText();
	
	/** \brief Get integer text. */
	int GetInteger() const;
	
	/** \brief Set integer text. */
	void SetInteger( int value, bool changing = false );
	
	/** \brief Get floating point text. */
	float GetFloat() const;
	
	/** \brief Set floating point text. */
	void SetFloat( float value, bool changing = false );
	
	/** \brief Get double precision floating point text. */
	double GetDouble() const;
	
	/** \brief Set double precision floating point text. */
	void SetDouble( double value, bool changing = false );
	
	
	
	/** \brief Cursor position as offset from start of text. */
	int GetCursorPosition() const;
	
	/** \brief Set cursor position as offset from start of text. */
	void SetCursorPosition( int position );
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeTextFieldListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeTextFieldListener *listener );
	
	/** \brief Notify listeners text changed. */
	virtual void NotifyTextChanged();
	
	/** \brief Notify listeners text is changing. */
	virtual void NotifyTextChanging();
	
	/** \brief Notify listeners enter key has been pressed. */
	virtual void NotifyEnterKey();
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
	/** \brief Text changed. */
	virtual void OnTextChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Editable changed. */
	virtual void OnEditableChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Invalid value changed. */
	virtual void OnInvalidValueChanged();
	/*@}*/
};

#endif
