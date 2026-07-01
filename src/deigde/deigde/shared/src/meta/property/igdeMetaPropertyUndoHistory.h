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

#ifndef _IGDEMETAPROPERTYUNDOHISTORY_H_
#define _IGDEMETAPROPERTYUNDOHISTORY_H_

#include "igdeMetaProperty.h"
#include "../igdeTMetaData.h"

class igdeUndoSystem;


/**
 * \brief Undo history meta property.
 * 
 * Exposes igdeUndoHistory as meta property.
 */
class DE_DLL_EXPORT igdeMetaPropertyUndoHistory : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyUndoHistory>;
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyUndoHistory>{
	};
	
	
private:
	int pRows = 4;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo history meta property. */
	igdeMetaPropertyUndoHistory(const char *id, const char *translationTag = "Igde.MetaPropertyUndoHistory.Property");
	igdeMetaPropertyUndoHistory(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up undo history meta property. */
	~igdeMetaPropertyUndoHistory() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rows. */
	inline int GetRows() const{ return pRows; }
	
	/** \brief Set rows. */
	void SetRows(int rows);
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about value changed. */
	void NotifyValueChanged(const ContextRef &context);
	
	
	/**
	 * \brief Property is valid.
	 *
	 * This means calling GetPropertyValue() does not throw an exception.
	 */
	virtual bool IsValid(const ContextRef &context) const;
	
	/**
	 * \brief Get property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual igdeUndoSystem *GetPropertyValue(const ContextRef &context) const;
	
	
	/**
	 * \brief Create UI widget.
	 *
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget() override;
	/*@}*/
};

#endif
