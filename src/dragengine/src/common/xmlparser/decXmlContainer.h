/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DECXMLCONTAINER_H_
#define _DECXMLCONTAINER_H_

#include "decXmlElement.h"
#include "../collection/decObjectOrderedSet.h"


/**
 * \brief XML Container.
 *
 * This is the base class for all XML elements which can contain other XML elements.
 */
class DE_DLL_EXPORT decXmlContainer : public decXmlElement{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decXmlContainer> Ref;
	
	
private:
	decObjectOrderedSet pElements;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml container. */
	decXmlContainer();
	
protected:
	/** \brief Clean up xml container. */
	virtual ~decXmlContainer();
	/*@}*/
	
	
	
public:
	/** \name Element Management */
	/*@{*/
	/** \brief Number of elements. */
	virtual int GetElementCount() const;
	
	/** \brief Any elements in container. */
	virtual bool IsEmpty() const;
	
	/** \brief Element at index. */
	virtual decXmlElement *GetElementAt( int index ) const;
	
	/** \brief Add element. */
	virtual void AddElement( decXmlElement *element );
	
	/** \brief Add element. */
	virtual void InsertElement(decXmlElement *element, int beforeIndex);
	
	/** \brief Remove element. */
	virtual void RemoveElement( decXmlElement *element );
	
	/** \brief Remove all elements. */
	virtual void RemoveAllElements();
	
	/** \brief Index of element or -1 if absent. */
	virtual int IndexOfElement( decXmlElement *element );
	
	/** \brief Element is present. */
	virtual bool HasElement( decXmlElement *element );
	
	/** \brief Visit all elements. */
	virtual void VisitElements( decXmlVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	virtual void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToContainer() const;
	virtual decXmlContainer *CastToContainer();
	/*@}*/
};

#endif
