/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEMETAPROPERTYFLOAT_H_
#define _IGDEMETAPROPERTYFLOAT_H_

#include "igdeMetaProperty.h"

class igdeMetaPropertyFloatUndo;


/**
 * \brief Float meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyFloat : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyFloat>;
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyFloat>{
	};
	
	
private:
	int pPrecision;
	float pDefaultValue;
	float pLowerLimit, pUpperLimit, pTickSpacing;
	bool pEnableLowerLimit, pEnableUpperLimit;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create float meta property with label and description. */
	igdeMetaPropertyFloat(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up float meta property. */
	~igdeMetaPropertyFloat() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Precision for text field. */
	inline int GetPrecision() const{ return pPrecision; }
	
	/** \brief Set precision for text field. */
	void SetPrecision(int precision);
	
	/** \brief Default value. */
	inline float GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(float value);
	
	
	/** \brief Lower limit. */
	inline float GetLowerLimit() const{ return pLowerLimit; }
	
	/** \brief Set lower limit. */
	void SetLowerLimit(float value);
	
	/** \brief Upper limit. */
	inline float GetUpperLimit() const{ return pUpperLimit; }
	
	/** \brief Set upper limit. */
	void SetUpperLimit(float value);
	
	/** \brief Tick spacing. */
	inline float GetTickSpacing() const{ return pTickSpacing; }
	
	/** \brief Set tick spacing. */
	void SetTickSpacing(float value);
	
	/** \brief Enable lower limit. */
	inline bool GetEnableLowerLimit() const{ return pEnableLowerLimit; }
	
	/** \brief Set enable lower limit. */
	void SetEnableLowerLimit(bool enable);
	
	/** \brief Enable upper limit. */
	inline bool GetEnableUpperLimit() const{ return pEnableUpperLimit; }
	
	/** \brief Set enable upper limit. */
	void SetEnableUpperLimit(bool enable);
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value change. */
	void NotifyValueChanged(const igdeMetaContext::Ref &context);
	
	
	/**
	 * \brief Capture context.
	 * 
	 * This returns an immutable context always returning the necessary state for getting or setting the property.
	 */
	virtual igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Property is valid.
	 * 
	 * This means calling GetPropertyValue() nor SetPropertyValue() throws an exception.
	 */
	virtual bool IsValid(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Get property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual float GetPropertyValue(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const igdeMetaContext::Ref &context, float value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 * 
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	deTObjectReference<igdeMetaPropertyFloatUndo> ChangePropertyValue(
		const igdeMetaContext::Ref &context, float newValue);
	
	
	/**
	 * \brief Create UI widget.
	 * 
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget(const igdeMetaContext::Ref &context) override;
	/*@}*/
};

#endif
