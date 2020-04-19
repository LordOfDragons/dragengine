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

#ifndef _SELSSYNTHESIZER_H_
#define _SELSSYNTHESIZER_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class seSynthesizer;
class seController;
class seLink;
class seLinkMapping;
class seEffect;
class seEffectStretch;
class seSource;
class seSourceSound;
class seSourceWave;
class seSourceChain;
class seSourceGroup;
class seSourceSynthesizer;
class seControllerTarget;
class seLoadSaveSystem;

class decXmlWriter;
class decXmlElementTag;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * \brief Load/Save Synthesizer.
 */
class seLoadSaveSynthesizer : public igdeBaseXML{
private:
	seLoadSaveSystem *pLSSys;
	decString pName;
	decString pPattern;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	seLoadSaveSynthesizer( seLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource );
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Load synthesizer from file. */
	void LoadSynthesizer( seSynthesizer &synthesizer, decBaseFileReader &reader );
	
	/** \brief Save synthesizer to file. */
	void SaveSynthesizer( const seSynthesizer &synthesizer, decBaseFileWriter &writer );
	/*@}*/
	
	
	
private:
	void pWriteSynthesizer( decXmlWriter &writer, const seSynthesizer &synthesizer );
	
	void pWriteController( decXmlWriter &writer, const seController &controller );
	
	void pWriteLink( decXmlWriter &writer, const seSynthesizer &synthesizer, const seLink &link );
	
	void pWriteSourceCommon( decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seSource &source );
	
	void pWriteControllerTarget( decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seControllerTarget &target, const char *name );
	
	void pWriteSource( decXmlWriter &writer, const seSynthesizer &synthesizer, const seSource &source );
	
	void pWriteSourceSound( decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seSourceSound &source );
	
	void pWriteSourceWave( decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seSourceWave &source );
	
	void pWriteSourceChain( decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seSourceChain &source );
	
	void pWriteSourceGroup( decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seSourceGroup &source );
	
	void pWriteSourceSynthesizer( decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seSourceSynthesizer &source );
	
	void pWriteEffectCommon( decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seEffect &effect );
	
	void pWriteEffect( decXmlWriter &writer, const seSynthesizer &synthesizer, const seEffect &effect );
	
	void pWriteEffectStretch( decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seEffectStretch &effect );
	
	
	
	void pReadSynthesizer( const decXmlElementTag &root, seSynthesizer &synthesizer );
	
	void pReadController( const decXmlElementTag &root, seSynthesizer &synthesizer );
	
	void pReadControllerLimit( const decXmlElementTag &root, seController &controller );
	
	void pReadLink( const decXmlElementTag &root, seSynthesizer &synthesizer );
	
	seSource *pReadSource( const decXmlElementTag &root, seSynthesizer &synthesizer );
	
	seSource *pReadSourceSound( const decXmlElementTag &root, seSynthesizer &synthesizer );
	
	seSource *pReadSourceWave( const decXmlElementTag &root, seSynthesizer &synthesizer );
	
	seSource *pReadSourceChain( const decXmlElementTag &root, seSynthesizer &synthesizer );
	
	seSource *pReadSourceGroup( const decXmlElementTag &root, seSynthesizer &synthesizer );
	
	seSource *pReadSourceSynthesizer( const decXmlElementTag &root, seSynthesizer &synthesizer );
	
	bool pReadSourceCommon( const decXmlElementTag &root, seSynthesizer &synthesizer, seSource &source );
	
	void pReadControllerTarget( const decXmlElementTag &root,
		seSynthesizer &synthesizer, seControllerTarget &target );
	
	seEffect *pReadEffect( const decXmlElementTag &root, seSynthesizer &synthesizer );
	
	seEffect *pReadEffectStretch( const decXmlElementTag &root, seSynthesizer &synthesizer );
	
	bool pReadEffectCommon( const decXmlElementTag &root, seSynthesizer &synthesizer, seEffect &effect );
};

#endif
