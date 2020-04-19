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

#ifndef _DEALGPMPARAMETERLIST_H_
#define _DEALGPMPARAMETERLIST_H_

#include "../../common/collection/decObjectList.h"

class dealGPMParameter;



/**
 * \brief Game Profile Module Parameter List.
 */
class dealGPMParameterList{
private:
	decObjectList pParameters;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile parameter list. */
	dealGPMParameterList();
	
	/** \brief Clean up game profile parameter list. */
	~dealGPMParameterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of parameters. */
	int GetParameterCount() const;
	
	/** \brief Parameter at index. */
	dealGPMParameter *GetParameterAt( int index ) const;
	
	/** \brief Parameter with name or \em NULL if not found. */
	dealGPMParameter *GetParameterNamed( const char *name ) const;
	
	/** \brief Parameter exists. */
	bool HasParameter( dealGPMParameter *parameter ) const;
	
	/** \brief Parameter with name exists. */
	bool HasParameterNamed( const char *name ) const;
	
	/** \brief Index of a parameter or -1 if not found. */
	int IndexOfParameter( dealGPMParameter *parameter ) const;
	
	/** \brief Index of parameter with name or -1 if not found. */
	int IndexOfParameterNamed( const char *name ) const;
	
	/** \brief Add parameter. */
	void AddParameter( dealGPMParameter *parameter );
	
	/** \brief Remove parameter. */
	void RemoveParameter( dealGPMParameter *parameter );
	
	/** \brief Remove all parameters. */
	void RemoveAllParameters();
	/*@}*/
};

#endif
