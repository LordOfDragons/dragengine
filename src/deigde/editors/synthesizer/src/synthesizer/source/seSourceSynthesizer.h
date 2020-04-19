/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SESOURCESYNTHESIZER_H_
#define _SESOURCESYNTHESIZER_H_

#include "seSource.h"

#include <dragengine/common/string/decString.h>

class seController;

class deSynthesizer;
class deSynthesizerSourceSynthesizer;



/**
 * \brief Synthesizer source synthesizer.
 */
class seSourceSynthesizer : public seSource{
private:
	deEngine *pEngine;
	
	decString pPathSynthesizer;
	deSynthesizer *pChildSynthesizer;
	decString *pControllerNames;
	int pControllerNameCount;
	
	seController **pConnections;
	int pConnectionCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create source. */
	seSourceSynthesizer( deEngine *engine );
	
	/** \brief Create copy of source. */
	seSourceSynthesizer( const seSourceSynthesizer &copy );
	
	/** \brief Clean up source. */
	virtual ~seSourceSynthesizer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Path to child synthesizer. */
	inline const decString &GetPathSynthesizer() const{ return pPathSynthesizer; }
	
	/** \brief Set path to child synthesizer. */
	void SetPathSynthesizer( const char *path );
	
	/** \brief Child synthesizer or \em NULL if absent. */
	inline deSynthesizer *GetChildSynthesizer() const{ return pChildSynthesizer; }
	
	/** \brief Update child synthesizer using stored path. */
	void UpdateChildSynthesizer();
	
	
	
	/** \brief Number of controller names. */
	inline int GetControllerNameCount() const{ return pControllerNameCount; }
	
	/** \brief Controller name at position. */
	const decString &GetControllerNameAt( int position ) const;
	
	
	
	/** \brief Number of connections. */
	inline int GetConnectionCount() const{ return pConnectionCount; }
	
	/** \brief Set number of connections. */
	void SetConnectionCount( int count );
	
	/** \brief Controller for child controller or \em NULL if not set. */
	seController *GetControllerAt( int position ) const;
	
	/** \brief Set controller for child controller or \em NULL if not set. */
	void SetControllerAt( int position, seController *controller );
	
	
	
	/** \brief Create an engine synthesizer source. */
	virtual deSynthesizerSource *CreateEngineSource();
	
	/** \brief Create a copy of this source. */
	virtual seSource *CreateCopy() const;
	
	/** \brief List all links of all source targets. */
	virtual void ListLinks( seLinkList& list );
	
	/** \brief Parent synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Synthesizer directory changed. */
	virtual void SynthesizerDirectoryChanged();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another sub synthesizer source to this sub synthesizer source. */
	virtual seSourceSynthesizer &operator=( const seSourceSynthesizer &copy );
	/*@}*/
	
	
	
private:
	void pUpdateConnections( deSynthesizerSourceSynthesizer &source ) const;
};

#endif
