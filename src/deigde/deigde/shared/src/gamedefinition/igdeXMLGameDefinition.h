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
class DE_DLL_EXPORT igdeXMLGameDefinition : public igdeBaseXML{
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
