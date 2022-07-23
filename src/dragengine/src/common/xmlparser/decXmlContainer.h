/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
