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

#ifndef _IGDEMETAPROPERTYLIST_H_
#define _IGDEMETAPROPERTYLIST_H_

#include "igdeMetaProperty.h"
#include "../igdeTMetaData.h"
#include "../../gui/model/igdeListItem.h"

class igdeMetaContextItemInfo;
class igdeMetaPropertyListUndo;

/**
 * \brief List meta property.
 * 
 * Includes active object and object selection if supported.
 */
class DE_DLL_EXPORT igdeMetaPropertyList : public igdeMetaProperty{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyList>;
	
	/** \brief List of objects. */
	using List = decTObjectOrderedSet<deObject>;
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public TListener<igdeMetaPropertyList>{
	public:
		/** \brief Active object changed. */
		virtual void OnActiveChanged(igdeMetaPropertyList *property, const igdeMetaContext::Ref &context);
		
		/** \brief Object selection changed. */
		virtual void OnSelectionChanged(igdeMetaPropertyList *property, const igdeMetaContext::Ref &context);
	};
	
	
private:
	int pRows;
	igdeTListenerList<Listener> pListeners;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	
	/** \brief Create object meta property with label and description. */
	igdeMetaPropertyList(const char *id, const char *name, const char *description);
	
protected:
	/** \brief Clean up object meta property. */
	~igdeMetaPropertyList() override;
	
public:
	/*@}*/
	
	
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
	void NotifyValueChanged(const igdeMetaContext::Ref &context);
	
	/** \brief Notify listeners about active object changed. */
	void NotifyActiveChanged(const igdeMetaContext::Ref &context);
	
	/** \brief Notify listeners about object selection changed. */
	void NotifySelectionChanged(const igdeMetaContext::Ref &context);
	
	
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
	virtual const List &GetPropertyValue(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set property value matching context.
	 *
	 * Implemented by subclass.
	 */
	virtual void SetPropertyValue(const igdeMetaContext::Ref &context, const List &value) = 0;
	
	/**
	 * \brief Change property value matching context with undo support.
	 *
	 * If the context has an undo system the change is recorded as an undo action.
	 * Otherwise SetPropertyValue() is called directly.
	 */
	deTObjectReference<igdeMetaPropertyListUndo> ChangePropertyValue(
		const igdeMetaContext::Ref &context, const List &newValue);
	
	/**
	 * \brief Get active object or nullptr if no active object.
	 */
	virtual const deObject::Ref &GetActiveObject(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set active object.
	 */
	virtual void SetActiveObject(const igdeMetaContext::Ref &context, const deObject::Ref &activeObject) = 0;
	
	/**
	 * \brief Get object selection.
	 */
	virtual List GetSelection(const igdeMetaContext::Ref &context) const = 0;
	
	/**
	 * \brief Set object selection.
	 */
	virtual void SetSelection(const igdeMetaContext::Ref &context, const List &selection) = 0;
	
	/**
	 * \brief Get object item information.
	 */
	virtual void GetObjectItemInfo(const deObject::Ref &object, igdeMetaContextItemInfo &info) const = 0;
	
	
	/**
	 * \brief Create UI widget.
	 *
	 * This object is able to add itself to a widget holder in the appropriate way.
	 */
	deTObjectReference<igdeMetaPropertyWidget> CreateWidget(const igdeMetaContext::Ref &context) override;
	/*@}*/
};

#endif
