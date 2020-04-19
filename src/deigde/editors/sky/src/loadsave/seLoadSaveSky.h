/* 
 * Drag[en]gine IGDE Sky Editor
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
