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

#ifndef _IGDEWPMETACONTEXT_H_
#define _IGDEWPMETACONTEXT_H_

#include "../layout/igdeContainerFlow.h"
#include "../layout/igdeContainerScroll.h"
#include "../../meta/igdeMetaContext.h"
#include "../../meta/property/igdeMetaPropertyGroup.h"
#include "../../meta/property/widget/igdeMetaPropertyWidget.h"
#include "../../utils/igdeFilter.h"


/**
 * \brief Window properties meta context panel.
 *
 * Displays properties defined in a meta context in a vertical list.
 */
class DE_DLL_EXPORT igdeWPMetaContext : public igdeContainerScroll{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<igdeWPMetaContext>;
	
	/** \brief List of property widgets. */
	using PropertyWidgetList = decTObjectOrderedSet<igdeMetaPropertyWidget>;
	
	/** \brief Map of property widgets. */
	using PropertyWidgetMap = decTDictionary<igdeMetaProperty::Ref, igdeMetaPropertyWidget::Ref>;
	
	
private:
	class ContextListener : public igdeMetaContext::Listener{
		igdeWPMetaContext &pPanel;
		
	public:
		using Ref = deTObjectReference<ContextListener>;
		ContextListener(igdeWPMetaContext &panel);
		
	protected:
		virtual ~ContextListener() override;
		
	public:
		void OnPropertiesChanged(igdeMetaContext *context) override;
	};
	
	
	igdeMetaContext::Ref pContext;
	ContextListener::Ref pContextListener;
	igdeFilter pFilter;
	
	igdeMetaContext::PropertyList::Ref pProperties;
	igdeContainerFlow::Ref pPropertyContainer;
	PropertyWidgetList pPropertyWidgets;
	PropertyWidgetMap pPropertyWidgetCache;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeWPMetaContext(igdeEnvironment &environment);
	
	
protected:
	/** \brief Clean up panel. */
	virtual ~igdeWPMetaContext() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Meta context or nullptr. */
	inline const igdeMetaContext::Ref &GetContext() const{ return pContext; }
	
	/** \brief Set meta context or nullptr. */
	void SetContext(const igdeMetaContext::Ref &context);
	
	/** \brief Properties. */
	inline const igdeMetaContext::PropertyList::Ref &GetProperties() const{ return pProperties; }
	
	/** \brief Set properties. */
	void SetProperties(const igdeMetaContext::PropertyList::Ref &properties);
	
	/** \brief Filter. */
	inline const igdeFilter &GetFilter() const{ return pFilter; }
	
	/** \brief Set filter. */
	void SetFilter(const igdeFilter &filter);
	void SetFilter(igdeFilter &&filter);
	
	/** \brief Update property widgets. */
	void UpdatePropertyWidgets();
	/*@}*/
	
	
private:
	void pCreateContent();
	
	void pCreatePropertyWidgets();
	
	igdeMetaPropertyWidget::Ref pCreatePropertyGroupWidget(igdeContainer &container,
		const igdeMetaPropertyGroup::Ref &groupProperty);
	
	void pClearPropertyWidgets();
	
	void pFilterPropertyWidgets();
};

#endif
