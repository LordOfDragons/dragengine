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

#ifndef _IGDEWPMETACONTEXTLIST_H_
#define _IGDEWPMETACONTEXTLIST_H_

#include "../igdeTabBook.h"
#include "../igdeTextField.h"
#include "../layout/igdeContainerBorder.h"
#include "../../meta/igdeMetaContext.h"
#include "../../utils/igdeFilter.h"

#include <dragengine/common/collection/decTOrderedSet.h>


class igdeWPMetaContext;


/**
 * \brief Window properties meta context list panel.
 *
 * Displays multiple meta context panels in tabbed view.
 */
class DE_DLL_EXPORT igdeWPMetaContextList : public igdeContainerBorder{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<igdeWPMetaContextList>;
	
	/** \brief List of context panels. */
	using ContextPanelList = decTObjectOrderedSet<igdeWPMetaContext>;
	
	
private:
	igdeMetaContext::Data::Ref pData;
	igdeTabBook::Ref pTabBook;
	igdeTextField::Ref pEditFilter;
	igdeFilter pFilter;
	ContextPanelList pContextPanels;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeWPMetaContextList(igdeEnvironment &environment);
	
	
protected:
	/** \brief Clean up panel. */
	virtual ~igdeWPMetaContextList() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Meta context list or nullptr. */
	inline const igdeMetaContext::Data::Ref &GetData() const{ return pData; }
	
	/** \brief Set meta context list or nullptr. */
	void SetData(const igdeMetaContext::Data::Ref &data);
	
	/** \brief Filter. */
	inline const igdeFilter &GetFilter() const{ return pFilter; }
	
	/** \brief Set filter. */
	void SetFilter(const igdeFilter &filter);
	/*@}*/
	
	
private:
	void pCreateContent();
	void pCreateContextPanels();
	void pClearContextPanels();
	void pFilterContextPanels();
};

#endif
