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

#ifndef _IGDELOADSYNTHESIZER_H_
#define _IGDELOADSYNTHESIZER_H_

#include "../utils/igdeBaseXML.h"

#include <dragengine/common/string/decString.h>

class igdeEnvironment;
class igdeGameProject;

class decBaseFileReader;
class deSynthesizer;
class deSynthesizerSource;
class deSynthesizerController;
class deSynthesizerControllerTarget;
class deSynthesizerEffect;


/**
 * \brief Load synthesizer file in the Drag[en]gine Synthesizer format.
 */
class DE_DLL_EXPORT igdeLoadSynthesizer : public igdeBaseXML{
private:
	igdeEnvironment &pEnvironment;
	
	decString pName;
	decString pPattern;
	decString pDefaultExtension;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load object. */
	igdeLoadSynthesizer( igdeEnvironment &environment, deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up load object. */
	~igdeLoadSynthesizer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name to display in a file dialog. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name to display in a file dialog. */
	void SetName( const char *name );
	
	/** \brief File pattern to display in a file dialog. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set file pattern to display in a file dialog. */
	void SetPattern( const char *pattern );
	
	/** \brief Default file extension. */
	inline const decString &GetDefaultExtension() const{ return pDefaultExtension; }
	
	/** \brief Set default file extension. */
	void SetDefaultExtension( const char *extension );
	
	
	
	/** \brief Load synthesizer using a file reader. */
	void Load( const decString &pathSynthesizer, deSynthesizer &synthesizer, decBaseFileReader &reader );
	/*@}*/
	
	
	
private:
	void pReadSynthesizer( const decXmlElementTag &root, const char *basePath,
		deSynthesizer &synthesizer );
	
	void pReadController( const decXmlElementTag &root, deSynthesizer &synthesizer );
	
	void pReadLink( const decXmlElementTag &root, deSynthesizer &synthesizer );
	
	deSynthesizerSource *pReadSource( const decXmlElementTag &root,
		const char *basePath, deSynthesizer &synthesizer );
	
	deSynthesizerSource *pReadSourceSound( const decXmlElementTag &root,
		const char *basePath, deSynthesizer &synthesizer );
	
	deSynthesizerSource *pReadSourceWave( const decXmlElementTag &root,
		deSynthesizer &synthesizer );
	
	deSynthesizerSource *pReadSourceChain( const decXmlElementTag &root,
		const char *basePath, deSynthesizer &synthesizer );
	
	deSynthesizerSource *pReadSourceGroup( const decXmlElementTag &root,
		const char *basePath, deSynthesizer &synthesizer );
	
	deSynthesizerSource *pReadSourceSynthesizer( const decXmlElementTag &root,
		const char *basePath, deSynthesizer &synthesizer );
	
	bool pReadSourceCommon( const decXmlElementTag &root,
		deSynthesizer &synthesizer, deSynthesizerSource &source );
	
	void pReadControllerTarget( const decXmlElementTag &root,
		deSynthesizer &synthesizer, deSynthesizerControllerTarget &target );
	
	deSynthesizerEffect *pReadEffect( const decXmlElementTag &root,
		deSynthesizer &synthesizer );
	
	deSynthesizerEffect *pReadEffectStretch( const decXmlElementTag &root,
		deSynthesizer &synthesizer );
	
	bool pReadEffectCommon( const decXmlElementTag &root,
		deSynthesizer &synthesizer, deSynthesizerEffect &effect );
};

#endif
