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

#ifndef _IGDEMETAPROPERTYCURVEBEZIER_H_
#define _IGDEMETAPROPERTYCURVEBEZIER_H_

#include "igdeMetaProperty.h"
#include "storage/igdeMetaPropertyStorageComplex.h"
#include "../../clipboard/igdeClipboard.h"

#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/math/decMath.h>

class igdeMetaPropertyCurveBezierUndo;


/**
 * \brief Curve bezier meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyCurveBezier : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyCurveBezier>;
	
	
	/** \brief Clipboard data. */
	class DE_DLL_EXPORT ClipboardData : public igdeTClipboardData<decCurveBezier>{
	public:
		using Ref = deTObjectReference<ClipboardData>;
		
		/** \brief Type name. */
		static constexpr const char *TypeName = "MetaProperty.CurveBezier";
		
		explicit inline ClipboardData(const decCurveBezier &value) : igdeTClipboardData<decCurveBezier>(TypeName, value){}
		explicit inline ClipboardData(decCurveBezier &&value) : igdeTClipboardData<decCurveBezier>(TypeName, value){}
		
	protected:
		/** \brief Clean up object. */
		~ClipboardData() override = default;
	};
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyCurveBezier>{
	};
	
	
private:
	decVector2 pClampMin;
	decVector2 pClampMax;
	bool pClamp;
	decCurveBezier pDefaultValue;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create curve bezier meta property with label and description. */
	igdeMetaPropertyCurveBezier(const char *id, const char *name, const char *description);
	
	/** \brief Create curve bezier meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyCurveBezier(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up curve bezier meta property. */
	~igdeMetaPropertyCurveBezier() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Clamp minimum. */
	inline const decVector2 &GetClampMin() const{ return pClampMin; }
	
	/** \brief Set clamp minimum. */
	void SetClampMin(const decVector2 &clamp);
	
	/** \brief Clamp maximum. */
	inline const decVector2 &GetClampMax() const{ return pClampMax; }
	
	/** \brief Set clamp maximum. */
	void SetClampMax(const decVector2 &clamp);
	
	/** \brief Clamp is enabled. */
	inline bool GetClamp() const{ return pClamp; }
	
	/** \brief Set clamp enabled. */
	void SetClamp(bool clamp);
	
	/** \brief Default value. */
	inline const decCurveBezier &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const decCurveBezier &value);
	
	
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
	virtual const decCurveBezier &GetPropertyValue(const ContextRef &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const ContextRef &context, const decCurveBezier &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	virtual deTObjectReference<igdeMetaPropertyCurveBezierUndo> ChangePropertyValue(
		const ContextRef &context, const decCurveBezier &newValue,
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
 * \brief Curve bezier meta property using storage.
 */
class DE_DLL_EXPORT igdeMetaPropertyCurveBezierStorage : public igdeMetaPropertyCurveBezier{
public:
	/** \brief Storage type. */
	using Storage = igdeMetaPropertyStorageComplex<decCurveBezier, igdeMetaPropertyCurveBezierStorage>;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create curve bezier meta property with label and description. */
	igdeMetaPropertyCurveBezierStorage(const char *id, const char *name, const char *description);
	
	/** \brief Create meta property with label, description, filter and undo info set from translation tag. */
	igdeMetaPropertyCurveBezierStorage(const char *id, const char *translationTag);
	
protected:
	/** \brief Clean up curve bezier meta property. */
	~igdeMetaPropertyCurveBezierStorage() override;
	
public:
	/*@}*/
	/** \brief Storage. */
	virtual Storage &GetStorage(const ContextRef &context) const = 0;
	
	
	const decCurveBezier &GetPropertyValue(const ContextRef &context) const override;
	void SetPropertyValue(const ContextRef &context, const decCurveBezier &value) override;
};

#endif
