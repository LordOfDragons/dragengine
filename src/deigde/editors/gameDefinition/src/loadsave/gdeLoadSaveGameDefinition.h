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

#ifndef _GDELOADSAVEGAMEDEFINITION_H_
#define _GDELOADSAVEGAMEDEFINITION_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>

class gdeCategory;
class gdeCategoryList;
class gdeFilePatternList;
class gdeGameDefinition;
class gdeLoadSaveSystem;
class gdeOCBillboard;
class gdeOCCamera;
class gdeOCComponent;
class gdeOCComponentTexture;
class gdeOCEnvMapProbe;
class gdeOCLight;
class gdeOCInherit;
class gdeOCNavigationBlocker;
class gdeOCNavigationSpace;
class gdeOCParticleEmitter;
class gdeOCForceField;
class gdeOCSnapPoint;
class gdeOCSpeaker;
class gdeOCWorld;
class gdeObjectClass;
class gdeParticleEmitter;
class gdeProperty;
class gdePropertyList;
class gdeSkin;
class gdeSky;

class decStringDictionary;
class decStringSet;
class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;


/**
 * \brief Load/Save game definition.
 */
class gdeLoadSaveGameDefinition : public igdeBaseXML{
private:
	gdeLoadSaveSystem &pLoadSaveSystem;
	
	decString pName;
	decString pPattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save. */
	gdeLoadSaveGameDefinition( gdeLoadSaveSystem &loadSaveSystem, deLogger *logger, const char *loggerSource );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	
	
	/** \brief Load game definition from file. */
	void LoadGameDefinition( gdeGameDefinition &gameDefinition, decBaseFileReader &reader );
	
	/** \brief Save game definition to file. */
	void SaveGameDefinition( const gdeGameDefinition &gameDefinition, decBaseFileWriter &writer );
	/*@}*/
	
	
	
private:
	void pReadGameDefinition( const decXmlElementTag &root, gdeGameDefinition &gameDefinition );
	void pReadProperty( const decXmlElementTag &root, gdePropertyList &propertyList );
	void pReadProperty( const decXmlElementTag &root, gdeProperty &property );
	void pReadObjectClass( const decXmlElementTag &root, gdeGameDefinition &gameDefinition );
	void pReadObjectClassInherit( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadObjectClassBillboard( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadObjectClassCamera( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadObjectClassComponent( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadObjectClassComponentTexture( const decXmlElementTag &root, gdeObjectClass &objectClass, gdeOCComponent &component );
	void pReadObjectClassLight( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadObjectClassSnapPoint( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadObjectClassParticleEmitter( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadObjectClassForceField( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadObjectClassEnvMapProbe( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadObjectClassSpeaker( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadObjectClassNavigationSpace( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadObjectClassNavigationBlocker( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadObjectClassWorld(const decXmlElementTag &root, gdeObjectClass &objectClass);
	void pReadObjectClassTexture( const decXmlElementTag &root, gdeObjectClass &objectClass );
	void pReadCustomFilePatternList( const decXmlElementTag &root, gdeFilePatternList &list );
	void pReadParticleEmitter( const decXmlElementTag &root, gdeGameDefinition &gameDefinition );
	void pReadSkin( const decXmlElementTag &root, gdeGameDefinition &gameDefinition );
	void pReadSky( const decXmlElementTag &root, gdeGameDefinition &gameDefinition );
	void pReadCategories( const decXmlElementTag &root, gdeGameDefinition &gameDefinition );
	void pReadCategories( const decXmlElementTag &root, gdeCategoryList *list, gdeCategory *parent );
	void pReadCategory( const decXmlElementTag &root, gdeCategoryList *list, gdeCategory *parent );
	
	void pWriteGameDefinition( decXmlWriter &writer, const gdeGameDefinition &gameDefinition );
	void pWriteObjectClass( decXmlWriter &writer, const gdeGameDefinition &gameDefinition,
		const gdeObjectClass &objectClass );
	void pWriteObjectClassInherit( decXmlWriter &writer, const gdeOCInherit &inherit );
	void pWriteObjectClassComponent( decXmlWriter &writer, const gdeOCComponent &component );
	void pWriteObjectClassComponentTexture( decXmlWriter &writer, const gdeOCComponentTexture &texture );
	void pWriteObjectClassBillboard( decXmlWriter &writer, const gdeOCBillboard &billboard );
	void pWriteObjectClassCamera( decXmlWriter &writer, const gdeOCCamera &camera );
	void pWriteObjectClassLight( decXmlWriter &writer, const gdeOCLight &light );
	void pWriteObjectClassSnapPoint( decXmlWriter &writer, const gdeOCSnapPoint &snapPoint );
	void pWriteObjectClassParticleEmitter( decXmlWriter &writer, const gdeOCParticleEmitter &particleEmitter );
	void pWriteObjectClassForceField( decXmlWriter &writer, const gdeOCForceField &forceField);
	void pWriteObjectClassEnvMapProbe( decXmlWriter &writer, const gdeOCEnvMapProbe &envMapProbe );
	void pWriteObjectClassSpeaker( decXmlWriter &writer, const gdeOCSpeaker &speaker );
	void pWriteObjectClassNavSpace( decXmlWriter &writer, const gdeOCNavigationSpace &navspace );
	void pWriteObjectClassWorld(decXmlWriter &writer, const gdeOCWorld &world);
	void pWriteObjectClassNavBlocker( decXmlWriter &writer, const gdeOCNavigationBlocker &navblocker );
	void pWriteObjectClassTexture( decXmlWriter &writer, const gdeOCComponentTexture &texture );
	void pWriteLink( decXmlWriter &writer, const decString &link, const char *tagName, const char *target );
	void pWriteTags( decXmlWriter &writer, const decStringSet &tags, const char *tagName );
	void pWriteSkin( decXmlWriter &writer, const gdeGameDefinition &gameDefinition, const gdeSkin &skin );
	void pWriteSky( decXmlWriter &writer, const gdeGameDefinition &gameDefinition, const gdeSky &sky );
	void pWriteParticleEmitter( decXmlWriter &writer, const gdeGameDefinition &gameDefinition,
		const gdeParticleEmitter &particleEmitter );
	void pWriteCategories( decXmlWriter &writer, const gdeCategoryList &categories, const char *name );
	void pWriteCategory( decXmlWriter &writer, const gdeCategory &category );
	void pWriteProperties( decXmlWriter &writer, const decStringDictionary &properties, const char *tagName );
	void pWriteProperties( decXmlWriter &writer, const gdePropertyList &properties, const char *tagName );
	void pWriteProperty( decXmlWriter &writer, const gdeProperty &property, const char *tagName );
};

#endif
