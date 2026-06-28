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

#ifndef _IGDEMETAPROPERTYWIDGET_H_
#define _IGDEMETAPROPERTYWIDGET_H_

#include <functional>

#include "../igdeMetaProperty.h"
#include "../../igdeMetaContext.h"
#include "../../../gui/igdeButton.h"
#include "../../../gui/igdeLabel.h"
#include "../../../gui/event/igdeActionContextMenu.h"
#include "../../../gui/menu/igdeMenuCascade.h"

class igdeContainer;
class igdeUIHelper;
class igdeFilter;


/**
 * \brief Meta property widget class able to add itself to a widget holder.
 */
class DE_DLL_EXPORT igdeMetaPropertyWidget : public deObject{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyWidget>;
	
	/** \brief List of property widgets. */
	using List = decTObjectOrderedSet<igdeMetaPropertyWidget>;
	
	
	/** \brief Builder interface. */
	class DE_DLL_EXPORT Builder{
	private:
		igdeUIHelper &pHelper;
		igdeMetaPropertyWidget::List *pCollectWidgets;
		igdeMetaContext::Ref pContext;
		
	protected:
		Builder(igdeUIHelper &helper, igdeMetaPropertyWidget::List *collectWidgets,
			const igdeMetaContext::Ref &context);
		virtual ~Builder();
		
	public:
		/** \brief UI Helper. */
		inline igdeUIHelper &GetHelper() const{ return pHelper; }
		
		/** \brief List to add created widgets to or nullptr. */
		inline igdeMetaPropertyWidget::List *GetCollectWidgets() const{ return pCollectWidgets; }
		
		/** \brief Set list to add created widgets to or nullptr. */
		void SetCollectWidgets(igdeMetaPropertyWidget::List *collectWidgets);
		
		/** \brief Add to collect widget list if list is not nullptr. */
		void CollectWidget(const igdeMetaPropertyWidget::Ref &widget);
		
		/** \brief Context or nullptr. */
		inline const igdeMetaContext::Ref &GetContext() const{ return pContext; }
		
		/** \brief Set context or nullptr. */
		void SetContext(const igdeMetaContext::Ref &context);
		
		/** \brief Add property line with label and edit widget. */
		virtual void AddLine(igdeWidget *label, igdeWidget *edit) = 0;
		
		/** \brief Add property line with only edit widget. */
		virtual void AddLine(igdeWidget *edit) = 0;
		
		/** \brief Open group. */
		virtual void OpenGroup(igdeWidget *group, igdeContainer *container) = 0;
		
		/** \brief Close group. */
		virtual void CloseGroup() = 0;
		
		/** \brief Create property widgets. */
		virtual void CreatePropertyWidgets(const igdeMetaProperty::List &properties) = 0;
	};
	
	
private:
	const igdeMetaProperty::Ref pProperty;
	igdeMetaContext::Ref pContext;
	igdeLabel::Ref pLabel;
	igdeContainer::Ref pEditContainer;
	igdeButton::Ref pButtonContextMenu;
	igdeFilter::Matchable pMatchable;
	bool pFilteredOut = false;
	bool pPreventUpdate = false;
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create meta property widget for property.
	 */
	explicit igdeMetaPropertyWidget(igdeMetaProperty &property);
	
	/** \brief Clean up widget. */
	~igdeMetaPropertyWidget();
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Meta property associated with this widget. */
	inline const igdeMetaProperty::Ref &GetProperty() const{return pProperty;}
	
	/** \brief Meta context or nullptr. */
	inline const igdeMetaContext::Ref &GetContext() const{return pContext;}
	
	/**
	 * \brief Set meta context or nullptr.
	 * 
	 * Updates value and UI relevant data obtained through property getter calls.
	 */
	void SetContext(const igdeMetaContext::Ref &context);
	
	/** \brief Environment throwing exception if not available. */
	virtual igdeEnvironment &GetEnvironment() const;
	
	/** \brief Widget is filtered out. */
	inline bool GetFilteredOut() const{ return pFilteredOut; }
	
	/** \brief Set widget is filtered out. */
	void SetFilteredOut(bool filteredOut);
	
	/** \brief Filter widget. */
	virtual void Filter(const igdeFilter &filter);
	
	/** \brief Matchable. */
	inline const igdeFilter::Matchable &GetMatchable() const{ return pMatchable; }
	
	/** \brief Update matchable. */
	void UpdateMatchable();
	
	
	/** \brief Create UI widgets adding them to container. */
	virtual void Create(Builder &builder, bool noLabel) = 0;
	
	/** \brief Drop UI widgets. */
	virtual void Drop();
	
	/** \brief Update UI widgets with current property values. */
	virtual void Update();
	
	
	/** \brief Label or nullptr. */
	inline const igdeLabel::Ref &GetLabel() const{ return pLabel; }
	
	/** \brief Context menu button or nullptr. */
	inline const igdeButton::Ref &GetButtonContextMenu() const{ return pButtonContextMenu; }
	
	
	/** \brief Prevent create undo or update value when widget changes. */
	inline bool GetPreventUpdate() const{ return pPreventUpdate; }
	
	/** \brief Run function while preventing create undo or update value when widget changes. */
	void RunWithPreventUpdate(const std::function<void()> &function);
	
	
	/**
	 * \brief Add context menu entries.
	 * 
	 * Calls igdeMetaProperty::AddContextMenuEntries(). Subclass has to super call before
	 * adding menu entries to ensure special menu entries are located last in the menu.
	 */
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu);
	
	/** \brief Property is valid. */
	virtual bool IsPropertyValid() const;
	/*@}*/
	
	
protected:
	/** \brief Create label and context menu button wrapping edit widget. */
	void WrapEditWidget(Builder &builder, bool noLabel, igdeWidget *widget,
		igdeWidget *sideWidget = nullptr);
	
	/** \brief Update filtered out. */
	virtual void UpdateFilteredOut();
	
	/** \brief Context changed. */
	virtual void OnContextChanged();
	
	/** \brief Edit container for use as widget target only. */
	inline const igdeContainer::Ref &GetEditContainer() const{ return pEditContainer; }
};

#endif
