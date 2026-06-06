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

#ifndef _IGDEMETAPROPERTYDIRECTORYWIDGET_H_
#define _IGDEMETAPROPERTYDIRECTORYWIDGET_H_

#include "igdeMetaPropertyWidget.h"
#include "../igdeMetaPropertyDirectory.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/composed/igdeEditDirectory.h"
#include "../../../gui/composed/igdeEditDirectoryListener.h"


/**
 * \brief Directory meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyDirectoryWidget : public igdeMetaPropertyWidget{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyDirectoryWidget>;
	
	
private:
	class PropertyListener : public igdeMetaPropertyDirectory::Listener{
		igdeMetaPropertyDirectoryWidget &pWidget;
		
	public:
		using Ref = deTObjectReference<PropertyListener>;
		PropertyListener(igdeMetaPropertyDirectoryWidget &widget);
		
	protected:
		virtual ~PropertyListener() override;
		
	public:
		void OnValueChanged(igdeMetaPropertyDirectory *property, const igdeMetaContext::Ref &context) override;
	};
	
	
	igdeMetaPropertyDirectory &pPropertyDirectory;
	PropertyListener::Ref pPropertyListener;
	igdeEditDirectory::Ref pEditDirectory;
	igdeEditDirectoryListener::Ref pListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property and context.
	 */
	igdeMetaPropertyDirectoryWidget(igdeMetaPropertyDirectory &property);
	
protected:
	/** \brief Clean up widget. */
	~igdeMetaPropertyDirectoryWidget() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Assigned property directory. */
	inline igdeMetaPropertyDirectory &GetPropertyDirectory() const{ return pPropertyDirectory; }
	
	
	/** \brief Create UI widgets adding them to container. */
	void Create(igdeContainer &container, igdeUIHelper &helper) override;
	
	/** \brief Drop UI widgets. */
	void Drop() override;
	
	/** \brief Update UI widgets with current property values. */
	void Update() override;
	
	
	/** \brief Edit directory widget or nullptr. */
	inline const igdeEditDirectory::Ref &GetEditDirectory() const{ return pEditDirectory; }
	/*@}*/
	
	
protected:
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override;
	void UpdateFilteredOut() override;
};

#endif
