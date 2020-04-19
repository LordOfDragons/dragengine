/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#ifndef _PEELOADSAVEEMITTER_H_
#define _PEELOADSAVEEMITTER_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>

class peeEmitter;
class peeController;
class peeType;
class peeLoadSaveSystem;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load and save particle emitter.
 */
class peeLoadSaveEmitter : public igdeBaseXML{
private:
	peeLoadSaveSystem *pLSSys;
	decString pName;
	decString pPattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	peeLoadSaveEmitter( peeLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	
	
	/** \brief Load emitter from file. */
	void LoadEmitter( peeLoadSaveSystem &lssys, peeEmitter &emitter, decBaseFileReader &reader );
	
	/** \brief Save emitter to file. */
	void SaveEmitter( peeLoadSaveSystem &lssys, const peeEmitter &emitter, decBaseFileWriter &writer );
	/*@}*/
	
	
	
private:
	void pWriteEmitter( decXmlWriter &writer, const peeEmitter &emitter );
	void pWriteController( decXmlWriter &writer, const peeController &controller );
	void pWriteType( decXmlWriter &writer, const peeEmitter &emitter, const peeType &type );
	void pWriteParameter( decXmlWriter &writer, const peeEmitter &emitter, const peeType &type,
		deParticleEmitterType::eParameters parameterType, const char *name );
	
	void pReadEmitter( const decXmlElementTag &root, peeEmitter &emitter );
	void pReadController( const decXmlElementTag &root, peeEmitter &emitter );
	void pReadType( const decXmlElementTag &root, peeEmitter &emitter );
	void pReadParameter( const decXmlElementTag &root, peeEmitter &emitter, peeType &type );
};

#endif
