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

#ifndef _DECXMLPI_H_
#define _DECXMLPI_H_

#include "decXmlElement.h"
#include "../string/decString.h"


/**
 * \brief XML Process Instruction.
 */
class decXmlPI : public decXmlElement{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<decXmlPI> Ref;
	
	
	
private:
	decString pTarget;
	decString pCommand;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml process instruction. */
	decXmlPI( const char *target );
	
protected:
	/** \brief Clean up xml process instruction. */
	virtual ~decXmlPI();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Target. */
	inline const decString &GetTarget() const{ return pTarget; }
	
	/** \brief Set target. */
	void SetTarget( const char *target );
	
	/** \brief Command. */
	inline const decString &GetCommand() const{ return pCommand; }
	
	/** \brief Set command. */
	void SetCommand( const char *command );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void Visit( decXmlVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToPI() const;
	virtual decXmlPI *CastToPI();
	/*@}*/
};

#endif
