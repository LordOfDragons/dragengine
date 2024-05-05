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

#ifndef _DEOALLSCONFIGURATION_H_
#define _DEOALLSCONFIGURATION_H_

class decXmlAttValue;
class decXmlElementTag;
class decBaseFileReader;
class deAudioOpenAL;
class deoalConfiguration;


/**
 * \brief OpenAL Configuration.
 */
class deoalLSConfiguration{
private:
	deAudioOpenAL &pOal;
	deoalConfiguration &pConfig;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates configuration. */
	deoalLSConfiguration( deAudioOpenAL &oal, deoalConfiguration &config );
	
	/** \brief Clean up openal configuration. */
	~deoalLSConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load configuration. */
	void LoadConfig();
	
	/** \brief Save configuration. */
	void SaveConfig();
	
	
	
private:
	void pLoadConfig( decBaseFileReader *file );
	decXmlElementTag *pGetTagAt( const decXmlElementTag &tag, int index );
	decXmlAttValue *pFindAttribute( const decXmlElementTag &tag, const char *name );
	const char *pGetAttributeString( const decXmlElementTag &tag, const char *name );
	int pGetAttributeInt( const decXmlElementTag &tag, const char *name );
	float pGetAttributeFloat( const decXmlElementTag &tag, const char *name );
	const char *pGetCData( const decXmlElementTag &tag, const char *defaultValue );
	bool pGetCDataBool( const decXmlElementTag &tag, bool defaultValue );
	int pGetCDataInt( const decXmlElementTag &tag, int defaultValue );
	float pGetCDataFloat( const decXmlElementTag &tag, float defaultValue );
};

#endif
