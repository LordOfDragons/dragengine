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

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeTextField> Ref;
	
	
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
	igdeTextField(igdeEnvironment &environment, int columns, const char *description = "");
	
	igdeTextField(igdeEnvironment &environment, int columns, bool editable,
		const char *description = "");
	
	
	
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
	inline bool GetEnabled() const{return pEnabled;}
	
	/** \brief Set if button is enabled. */
	void SetEnabled(bool enabled);
	
	/** \brief Visible columns in edit field. */
	inline int GetColumns() const{return pColumns;}
	
	/** \brief Text is editable. */
	inline bool GetEditable() const{return pEditable;}
	
	/** \brief Set if text is editable. */
	void SetEditable(bool editable);
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{return pDescription;}
	
	/** \brief Set description shown in tool tips. */
	void SetDescription(const char *description);
	
	/** \brief Precision for floating point values as digits after period. */
	inline int GetPrecision() const{return pPrecision;}
	
	/** \brief Set precision for floating point values as digits after period. */
	void SetPrecision(int precision);
	
	/** \brief Mark widget as having an invalue value. */
	inline bool GetInvalidValue() const{return pInvalidValue;}
	
	/** \brief Set to mark widget as having an invalid value. */
	void SetInvalidValue(bool invalidValue);
	
	
	
	/** \brief Text. */
	inline const decString &GetText() const{return pText;}
	
	/** \brief Set text. */
	void SetText(const char *text, bool changing = false, bool forceNotify = false);
	
	/** \brief Clear text. */
	void ClearText();
	
	/** \brief Get integer text. */
	int GetInteger() const;
	
	/** \brief Set integer text. */
	void SetInteger(int value, bool changing = false);
	
	/** \brief Get floating point text. */
	float GetFloat() const;
	
	/** \brief Set floating point text. */
	void SetFloat(float value, bool changing = false);
	
	/** \brief Get double precision floating point text. */
	double GetDouble() const;
	
	/** \brief Set double precision floating point text. */
	void SetDouble(double value, bool changing = false);
	
	
	
	/** \brief Cursor position as offset from start of text. */
	int GetCursorPosition() const;
	
	/** \brief Set cursor position as offset from start of text. */
	void SetCursorPosition(int position);
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Add listener. */
	void AddListener(igdeTextFieldListener *listener);
	
	/** \brief Remove listener. */
	void RemoveListener(igdeTextFieldListener *listener);
	
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
