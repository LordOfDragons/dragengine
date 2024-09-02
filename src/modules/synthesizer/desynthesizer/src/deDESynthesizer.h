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
	 * \param[in] parameter Object to fill with information about the parameter
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
