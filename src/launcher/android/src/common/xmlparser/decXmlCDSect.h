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

#ifndef _DECXMLCDSECT_H_
#define _DECXMLCDSECT_H_

#include "decXmlCharacterData.h"


/**
 * \brief XML CD Section.
 */
class decXmlCDSect : public decXmlCharacterData{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml cd section with the given data. */
	decXmlCDSect( const char *data );
	
protected:
	/** \brief Clean up xml cd section. */
	virtual ~decXmlCDSect();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToCDSect() const;
	virtual decXmlCDSect *CastToCDSect();
	/*@}*/
};

#endif
