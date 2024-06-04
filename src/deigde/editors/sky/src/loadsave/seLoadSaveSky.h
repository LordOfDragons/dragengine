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

#ifndef _SELOADSAVESKY_H_
#define _SELOADSAVESKY_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/sky/deSkyLayer.h>

class seSky;
class seController;
class seLink;
class seLayer;
class seBody;
class seControllerTarget;
class seLoadSaveSystem;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * \brief Load/Save sky.
 */
class seLoadSaveSky : public igdeBaseXML{
private:
	seLoadSaveSystem &pLSSys;
	decString pName;
	decString pPattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	seLoadSaveSky( seLoadSaveSystem &lssys, deLogger *logger, const char *source );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	
	
	/** \brief Load sky from file. */
	void LoadSky( seLoadSaveSystem &lssys, seSky &sky, decBaseFileReader &reader );
	
	/** \brief Save sky to file. */
	void SaveSky( seLoadSaveSystem &lssys, const seSky &sky, decBaseFileWriter &writer );
	/*@}*/
	
	
	
private:
	void pWriteSky( decXmlWriter &writer, const seSky &sky );
	void pWriteController( decXmlWriter &writer, const seController &controller );
	void pWriteLink( decXmlWriter &writer, const seLink &link );
	void pWriteLayer( decXmlWriter &writer, const seLayer &layer );
	void pWriteTarget( decXmlWriter &writer, const seLayer &layer, deSkyLayer::eTargets target );
	void pWriteBody( decXmlWriter &writer, const seBody &body );
	
	void pReadSky( const decXmlElementTag &root, seSky &sky );
	void pReadController( const decXmlElementTag &root, seSky &sky );
	void pReadLink( const decXmlElementTag &root, seSky &sky );
	void pReadLayer( const decXmlElementTag &root, seSky &sky );
	void pReadTarget( const decXmlElementTag &root, seSky &sky, seLayer &layer );
	void pReadBody( const decXmlElementTag &root, seSky &sky, seLayer &layer );
};

#endif
