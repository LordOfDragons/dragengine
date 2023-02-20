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

#ifndef _IGDEEDITPOINT3_H_
#define _IGDEEDITPOINT3_H_

#include "../igdeTextFieldReference.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerBoxAlternate.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class igdeEditPoint3Listener;
class igdeUIHelper;


/**
 * \brief IGDE UI Edit Point3.
 * 
 * Composed widget to edit decPoint3.
 */
class DE_DLL_EXPORT igdeEditPoint3 : public igdeContainerBoxAlternate{
protected:
	/** \brief Text field listener. */
	class cListener : public igdeTextFieldListener{
	protected:
		igdeEditPoint3 &pEditPoint3;
		igdeTextFieldReference pTextX;
		igdeTextFieldReference pTextY;
		igdeTextFieldReference pTextZ;
		
	public:
		cListener( igdeEditPoint3 &editPoint3, igdeTextField *textX,
			igdeTextField *textY, igdeTextField *textZ );
		virtual ~cListener();
		virtual void OnTextChanged( igdeTextField *textField );
		virtual void OnTextChanging( igdeTextField *textField );
	};
	
	
	
private:
	bool pEnabled;
	decPoint3 pPoint3;
	int pColumns;
	bool pEditable;
	decString pDescription;
	
	igdeTextFieldReference pTextX;
	igdeTextFieldReference pTextY;
	igdeTextFieldReference pTextZ;
	bool pPreventUpdate;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create edit point. */
	igdeEditPoint3( igdeUIHelper &helper, int columns, const char *description = "" );
	
	igdeEditPoint3( igdeUIHelper &helper, int columns, bool editable,
		const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeEditPoint3();
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
	
	
	
	/** \brief Point3. */
	inline const decPoint3 &GetPoint3() const{ return pPoint3; }
	
	/** \brief Set point. */
	void SetPoint3( const decPoint3 &point );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeEditPoint3Listener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeEditPoint3Listener *listener );
	
	/** \brief Notify listeners point changed. */
	virtual void NotifyPoint3Changed();
	/*@}*/
	
	
	
protected:
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/** \brief Point3 changed. */
	virtual void OnPoint3Changed();
	
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
