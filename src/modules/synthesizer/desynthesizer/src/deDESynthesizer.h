/* 
 * Drag[en]gine Synthesizer Module
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

#ifndef _DEDESYNTHESIZER_H_
#define _DEDESYNTHESIZER_H_

#include <dragengine/systems/modules/synthesizer/deBaseSynthesizerModule.h>

class desynCaches;
class desynCommandExecuter;
class desynConfiguration;
class desynDecodeBuffer;
class desynParameterList;
class desynSharedBufferList;



/**
 * \brief DESynthesizer module.
 */
class deDESynthesizer : public deBaseSynthesizerModule{
private:
	desynConfiguration *pConfiguration;
	desynCommandExecuter *pCommandExecuter;
	desynParameterList *pParameterList;
	
	desynDecodeBuffer *pDecodeBuffer;
	desynSharedBufferList *pSharedBufferList;
	desynCaches *pCaches;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create desynthesizer module. */
	deDESynthesizer( deLoadableModule &loadableModule );
	
	/** \brief Clean up desynthesizer module. */
	virtual ~deDESynthesizer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Init module. */
	virtual bool Init();
	
	/** \brief Clean up module. */
	virtual void CleanUp();
	
	/** \brief Configuration. */
	inline desynConfiguration &GetConfiguration() const{ return *pConfiguration; }
	
	/** \brief Shared decode buffer. */
	inline desynDecodeBuffer &GetDecodeBuffer() const{ return *pDecodeBuffer; }
	
	/** \brief Shared buffer list. */
	inline desynSharedBufferList &GetSharedBufferList() const{ return *pSharedBufferList; }
	
	/** \brief Caches. */
	inline desynCaches &GetCaches() const{ return *pCaches; }
	/*@}*/
	
	
	
	/** \name Synthesizer management */
	/*@{*/
	/** \brief Create peer for sound. */
	virtual deBaseSynthesizerSound *CreateSound( deSound *sound );
	
	/** \brief Create peer for synthesizer or \em NULL if no explicit peer is required. */
	virtual deBaseSynthesizerSynthesizer *CreateSynthesizer( deSynthesizer *synthesizer );
	
	/** \brief Create peer for synthesizer instance or \em NULL if no explicit peer is required. */
	virtual deBaseSynthesizerSynthesizerInstance *CreateSynthesizerInstance( deSynthesizerInstance *instance );
	/*@}*/
	
	
	
	/** \name Parameters */
	/*@{*/
	/** \brief Number of parameters. */
	virtual int GetParameterCount() const;
	
	/**
	 * \brief Get information about parameter.
	 * \param[in] index Index of the parameter
	 * \param[in] parameter Object to fill with informations about the parameter
	 */
	virtual void GetParameterInfo( int index, deModuleParameter &parameter ) const;
	
	/** \brief Index of named parameter or -1 if not found. */
	virtual int IndexOfParameterNamed( const char *name ) const;
	
	/** \brief Value of named parameter. */
	virtual decString GetParameterValue( const char *name ) const;
	
	/** \brief Set value of named parameter. */
	virtual void SetParameterValue( const char *name, const char *value );
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Send command to module. */
	virtual void SendCommand( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/*@}*/
};

#endif
