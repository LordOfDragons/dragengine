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

#ifndef _IGDESCROLLBAR_H_
#define _IGDESCROLLBAR_H_

#include "igdeWidget.h"
#include "event/igdeAction::Ref.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>


class igdeScrollBarListener;


/**
 * \brief IGDE UI ScrollBar.
 */
class DE_DLL_EXPORT igdeScrollBar : public igdeWidget{
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
	
	/** \brief Set range. */
	void SetRange( int lower, int upper );
	
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
