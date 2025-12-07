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

#ifndef _IGDESPINTEXTFIELD_H_
#define _IGDESPINTEXTFIELD_H_

#include <stdlib.h>

#include "igdeWidget.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>


class igdeSpinTextFieldListener;


/**
 * \brief IGDE UI SpinTextField.
 */
class DE_DLL_EXPORT igdeSpinTextField : public igdeWidget{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeSpinTextField> Ref;
	
	
private:
	bool pEnabled;
	int pColumns;
	int pLower;
	int pUpper;
	int pValue;
	decString pDescription;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create textfield. */
	igdeSpinTextField(igdeEnvironment &environment, int lower, int upper,
		int columns, const char *description = "");
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeSpinTextField();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief SpinTextField is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if button is enabled. */
	void SetEnabled(bool enabled);
	
	/** \brief Visible columns in edit field. */
	inline int GetColumns() const{ return pColumns; }
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription(const char *description);
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Value. */
	inline int GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(int value);
	
	/** \brief Range lower value. */
	inline int GetLower() const{ return pLower; }
	
	/** \brief Range upper value. */
	inline int GetUpper() const{ return pUpper; }
	
	/** \brief Set range. */
	void SetRange(int lower, int upper);
	
	
	
	/** \brief Add listener. */
	void AddListener(igdeSpinTextFieldListener *listener);
	
	/** \brief Remove listener. */
	void RemoveListener(igdeSpinTextFieldListener *listener);
	
	/** \brief Notify listeners value changed. */
	virtual void NotifyValueChanged();
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
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	/*@}*/
};

#endif
