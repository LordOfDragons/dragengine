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

#ifndef _IGDEMETAPROPERTYDVECTOR_H_
#define _IGDEMETAPROPERTYDVECTOR_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageComplexIsEqualTo.h"
#include "../../clipboard/igdeClipboardData.h"

#include <dragengine/common/math/decMath.h>

class igdeMetaPropertyDVectorUndo;


/**
 * \brief DVector meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyDVector : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyDVector>;
	
	
	/** \brief Clipboard data. */
	class DE_DLL_EXPORT ClipboardData : public igdeTClipboardData<decDVector>{
	public:
		using Ref = deTObjectReference<ClipboardData>;
		
		/** \brief Type name. */
		static constexpr const char *TypeName = "MetaProperty.DVector";
		
		explicit ClipboardData(const igdeMetaProperty &property, const decDVector &value) :
			igdeTClipboardData<decDVector>(property.GetClipboardDataTypeName(), value){}
		
		explicit ClipboardData(const igdeMetaProperty &property, decDVector &&value) :
			igdeTClipboardData<decDVector>(property.GetClipboardDataTypeName(), std::move(value)){}
		
		explicit ClipboardData(const char *typeName, const decDVector &value) :
			igdeTClipboardData<decDVector>(typeName, value){}
		
		explicit ClipboardData(const char *typeName, decDVector &&value) :
			igdeTClipboardData<decDVector>(typeName, std::move(value)){}
		
	protected:
		/** \brief Clean up object. */
		~ClipboardData() override = default;
	};
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyDVector>{
	};
	
	
private:
	int pPrecision;
	decDVector pDefaultValue;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create dvector meta property with label and description. */
	igdeMetaPropertyDVector(const char *id, const char *name, const char *description);
	
	/** \brief Create dvector meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyDVector(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up dvector meta property. */
	~igdeMetaPropertyDVector() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Precision for text field. */
	inline int GetPrecision() const{ return pPrecision; }
	
	/** \brief Set precision for text field. */
	void SetPrecision(int precision);
	
	/** \brief Default value. */
	inline const decDVector &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const decDVector &value);
	
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value change. */
	void NotifyValueChanged(const ContextRef &context);
	
	
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
	virtual const decDVector &GetPropertyValue(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const ContextRef &context, const decDVector &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 * 
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	virtual deTObjectReference<igdeMetaPropertyDVectorUndo> ChangePropertyValue(
		const ContextRef &context, const decDVector &newValue,
		const char *undoInfo = nullptr, const char *undoInfoLong = nullptr);
	
	/**
	 * \brief Create UI widget.
	 *
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget() override;
	/*@}*/
};


/**
 * \brief DVector meta property using storage.
 */
class DE_DLL_EXPORT igdeMetaPropertyDVectorStorage : public igdeMetaPropertyDVector{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageComplexIsEqualTo<decDVector, igdeMetaPropertyDVectorStorage>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create dvector meta property with label and description. */
	igdeMetaPropertyDVectorStorage(const char *id, const char *name, const char *description);
	
	/** \brief Create meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyDVectorStorage(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up dvector meta property. */
	~igdeMetaPropertyDVectorStorage() override;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const ContextRef &context) const = 0;
	
	
	const decDVector &GetPropertyValue(const ContextRef &context) const override;
	void SetPropertyValue(const ContextRef &context, const decDVector &value) override;
};

#endif
