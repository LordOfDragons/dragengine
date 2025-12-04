/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _IGDEEDITDVECTOR_H_
#define _IGDEEDITDVECTOR_H_

#include "../igdeTextField.h"
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

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeEditDVector> Ref;
	
	
	
protected:
	/** \brief Text field listener. */
	class DE_DLL_EXPORT cListener : public igdeTextFieldListener{
	protected:
		igdeEditDVector &pEditDVector;
		igdeTextField::Ref pTextX;
		igdeTextField::Ref pTextY;
		igdeTextField::Ref pTextZ;
		
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
	
	igdeTextField::Ref pTextX;
	igdeTextField::Ref pTextY;
	igdeTextField::Ref pTextZ;
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
