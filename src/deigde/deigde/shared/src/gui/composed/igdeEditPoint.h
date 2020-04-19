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

#ifndef _IGDEEDITPOINT_H_
#define _IGDEEDITPOINT_H_

#include "../igdeTextFieldReference.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerBoxAlternate.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class igdeEditPointListener;
class igdeUIHelper;


/**
 * \brief IGDE UI Edit Point.
 * 
 * Composed widget to edit decPoint.
 */
class igdeEditPoint : public igdeContainerBoxAlternate{
protected:
	/** \brief Text field listener. */
	class cListener : public igdeTextFieldListener{
	protected:
		igdeEditPoint &pEditPoint;
		igdeTextFieldReference pTextX;
		igdeTextFieldReference pTextY;
		
	public:
		cListener( igdeEditPoint &editPoint, igdeTextField *textX, igdeTextField *textY );
		virtual ~cListener();
		virtual void OnTextChanged( igdeTextField *textField );
		virtual void OnTextChanging( igdeTextField *textField );
	};
	
	
	
private:
	bool pEnabled;
	decPoint pPoint;
	int pColumns;
	bool pEditable;
	decString pDescription;
	
	igdeTextFieldReference pTextX;
	igdeTextFieldReference pTextY;
	bool pPreventUpdate;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create edit point. */
	igdeEditPoint( igdeUIHelper &helper, int columns, const char *description = "" );
	
	igdeEditPoint( igdeUIHelper &helper, int columns, bool editable,
		const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeEditPoint();
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
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Point. */
	inline const decPoint &GetPoint() const{ return pPoint; }
	
	/** \brief Set point. */
	void SetPoint( const decPoint &point );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeEditPointListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeEditPointListener *listener );
	
	/** \brief Notify listeners point changed. */
	virtual void NotifyPointChanged();
	/*@}*/
	
	
	
protected:
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/** \brief Point changed. */
	virtual void OnPointChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Editable changed. */
	virtual void OnEditableChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	/*@}*/
	
	
	
private:
	void pCreateContent( igdeUIHelper &helper );
};

#endif
