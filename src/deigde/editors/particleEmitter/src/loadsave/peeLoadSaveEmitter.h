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
