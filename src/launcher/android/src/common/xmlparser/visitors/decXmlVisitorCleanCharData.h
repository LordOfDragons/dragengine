/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DECXMLVISITORCLEANCHARDATA_H_
#define _DECXMLVISITORCLEANCHARDATA_H_

#include "../decXmlVisitor.h"



/**
 * \brief Visitor cleaning character data in element tags.
 */
class decXmlVisitorCleanCharData : public decXmlVisitor{
private:
	bool pIsFirstElement;
	bool pIsLastElement;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	decXmlVisitorCleanCharData();
	
	/** \brief Clean up visitor. */
	virtual ~decXmlVisitorCleanCharData();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit container. */
	virtual void VisitContainer( decXmlContainer &container );
	
	/** \brief Visit element character data. */
	virtual void VisitCharacterData( decXmlCharacterData &data );
	/*@}*/
	
	
	
private:
	static bool IsSpace( int character );
};

#endif
