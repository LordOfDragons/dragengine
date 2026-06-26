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
#include "storage/igdeMetaPropertyStorageFloat.h"
#include "../../clipboard/igdeClipboardData.h"

class igdeMetaPropertyFloatUndo;


/**
 * \brief Float meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyFloat : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyFloat>;
	
	
	/** \brief Clipboard data. */
	class DE_DLL_EXPORT ClipboardData : public igdeTClipboardData<float>{
	public:
		using Ref = deTObjectReference<ClipboardData>;
		
		/** \brief Type name. */
		static constexpr const char *TypeName = "MetaProperty.Float";
		
		explicit ClipboardData(const igdeMetaProperty &property, float value) :
			igdeTClipboardData<float>(property.GetClipboardDataTypeName(), value){}
		
		explicit ClipboardData(const char *typeName, float value) :
			igdeTClipboardData<float>(typeName, value){}
		
	protected:
		/** \brief Clean up object. */
		~ClipboardData() override = default;
	};
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyFloat>{
	public:
		/** \brief Limits changed. */
		virtual void OnLimitsChanged(igdeMetaPropertyFloat *property, const ContextRef &context);
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
	
	/** \brief Create float meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyFloat(const char *id, const char *translationTag);
	
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
	void NotifyValueChanged(const ContextRef &context);
	
	/** \brief Notify listeners about limits change. */
	void NotifyLimitsChanged(const ContextRef &context);
	
	/**
	 * \brief Capture context.
	 * 
	 * This returns an immutable context always returning the necessary state for getting or setting the property.
	 */
	virtual ContextRef Capture(const ContextRef &context) const = 0;
	
	/**
	 * \brief Property is valid.
	 * 
	 * This means calling GetPropertyValue() nor SetPropertyValue() throws an exception.
	 */
	virtual bool IsValid(const ContextRef &context) const = 0;
	
	/**
	 * \brief Get property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual float GetPropertyValue(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 * 
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const ContextRef &context, float value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 * 
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	virtual deTObjectReference<igdeMetaPropertyFloatUndo> ChangePropertyValue(
		const ContextRef &context, float newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
	/** \brief Get property lower limit matching context. */
	virtual float GetPropertyLowerLimit(const ContextRef &context) const;
	
	/** \brief Get property upper limit matching context. */
	virtual float GetPropertyUpperLimit(const ContextRef &context) const;
	
	/** \brief Get property tick spacing matching context. */
	virtual float GetPropertyTickSpacing(const ContextRef &context) const;
	
	/**
	 * \brief Create UI widget.
	 * 
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget() override;
	/*@}*/
};


/**
 * \brief Float meta property using storage.
 */
class DE_DLL_EXPORT igdeMetaPropertyFloatStorage : public igdeMetaPropertyFloat{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageFloat<igdeMetaPropertyFloatStorage>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create float meta property with label and description. */
	igdeMetaPropertyFloatStorage(const char *id, const char *name, const char *description);
	
	/** \brief Create meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyFloatStorage(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up float meta property. */
	~igdeMetaPropertyFloatStorage() override;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const ContextRef &context) const = 0;
	
	
	float GetPropertyValue(const ContextRef &context) const override;
	void SetPropertyValue(const ContextRef &context, float value) override;
	float GetPropertyLowerLimit(const ContextRef &context) const override;
	float GetPropertyUpperLimit(const ContextRef &context) const override;
	float GetPropertyTickSpacing(const ContextRef &context) const override;
};


#include "undo/igdeMetaPropertyFloatUndo.h"

#endif
