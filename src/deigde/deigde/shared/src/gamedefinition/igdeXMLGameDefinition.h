/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEXMLLOADGAMEDEFINITION_H_
#define _IGDEXMLLOADGAMEDEFINITION_H_

#include "../utils/igdeBaseXML.h"

class decBaseFileWriter;
class decXmlWriter;
class igdeAutoTagPatternList;
class igdeFilePatternList;
class igdeGameDefinition;
class igdeGDCamera;
class igdeGDCategory;
class igdeGDCBillboard;
class igdeGDCComponent;
class igdeGDCCTexture;
class igdeGDCEnvMapProbe;
class igdeGDCForceField;
class igdeGDClass;
class igdeGDCLight;
class igdeGDCNavigationBlocker;
class igdeGDCNavigationSpace;
class igdeGDCParticleEmitter;
class igdeGDCSnapPoint;
class igdeGDCSpeaker;
class igdeGDParticleEmitter;
class igdeGDProperty;
class igdeGDSkin;
class igdeGDSky;
class igdeEnvironment;

class deLogger;
class decStringSet;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlElementTag;
class decXmlAttValue;


/**
 * \brief Loads dragengine game definition xml files.
 */
class igdeXMLGameDefinition : public igdeBaseXML{
private:
	igdeEnvironment &pEnvironment;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition xml object. */
	igdeXMLGameDefinition( igdeEnvironment &environment, deLogger *logger );
	
	/** \brief Clean up game definition loader. */
	virtual ~igdeXMLGameDefinition();
	/*@}*/
	
	
	
	/** \name Loading */
	/*@{*/
	/** \brief Loads a game definition file. */
	void Load( decBaseFileReader &reader, igdeGameDefinition &gamedef );
	
	
	
private:
	void pParseGameDefinition( const decXmlElementTag &root, igdeGameDefinition &gamedef );
	void pParseClass( const decXmlElementTag &root, igdeGameDefinition &gamedef );
	void pParseClassComponent( const decXmlElementTag &root, igdeGDClass &gdclass );
	void pParseClassBillboard( const decXmlElementTag &root, igdeGDClass &gdclass );
	void pParseClassInherit( const decXmlElementTag &root, igdeGDClass &gdclass );
	void pParseClassComponentTexture( const decXmlElementTag &root, igdeGDClass &gdclass, igdeGDCComponent &gdccomponent );
	void pParseClassLight( const decXmlElementTag &root, igdeGDClass &gdclass );
	void pParseClassSnapPoint( const decXmlElementTag &root, igdeGDClass &gdclass );
	void pParseClassParticleEmitter( const decXmlElementTag &root, igdeGDClass &gdclass );
	void pParseClassForceField( const decXmlElementTag &root, igdeGDClass &gdclass );
	void pParseClassEnvMapProbe( const decXmlElementTag &root, igdeGDClass &gdclass );
	void pParseClassSpeaker( const decXmlElementTag &root, igdeGDClass &gdclass );
	void pParseClassNavigationSpace( const decXmlElementTag &root, igdeGDClass &gdclass );
	void pParseClassNavigationBlocker( const decXmlElementTag &root, igdeGDClass &gdclass );
	void pParseClassTexture( const decXmlElementTag &root, igdeGDClass &gdclass );
	void pParseProperty( const decXmlElementTag &root, igdeGDProperty &property );
	void pParseCustomFilePatternList( const decXmlElementTag &root, igdeFilePatternList &list );
	void pParseCamera( const decXmlElementTag &root, igdeGDCamera *camera );
	void pParseParticleEmitter( const decXmlElementTag &root, igdeGameDefinition &gamedef );
	void pParseSkin( const decXmlElementTag &root, igdeGameDefinition &gamedef );
	void pParseSky( const decXmlElementTag &root, igdeGameDefinition &gamedef );
	void pParseGDCategories( const decXmlElementTag &root, igdeGameDefinition &gamedef );
	void pParseRootCategory( const decXmlElementTag &root, igdeGDCategory *parent );
	void pParseCategory( const decXmlElementTag &root, igdeGDCategory *parent );
};

#endif
