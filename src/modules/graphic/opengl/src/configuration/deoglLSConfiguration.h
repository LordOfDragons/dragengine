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

#ifndef _DEOGLLSCONFIGURATION_H_
#define _DEOGLLSCONFIGURATION_H_

class deoglConfiguration;
class decBaseFileReader;
class decXmlElementTag;
class decXmlAttValue;
class deGraphicOpenGl;



/**
 * Load/Save Configurating from/to XML file.
 */
class deoglLSConfiguration{
private:
	deGraphicOpenGl &pOgl;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create load/save configuration. */
	deoglLSConfiguration(deGraphicOpenGl &ogl);
	
	/** Clean up load/save configuration. */
	~deoglLSConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Load configuration from XML files. */
	void LoadConfig(deoglConfiguration &configuration);
	
	/** Save configuration to XML file. */
	void SaveConfig(const deoglConfiguration &configuration);
	/*@}*/
	
private:
	void pLoadConfigOpenGL(deoglConfiguration &configuration, decBaseFileReader &file);
	decXmlElementTag *pGetTagAt(decXmlElementTag *tag, int index);
	decXmlAttValue *pFindAttribute(decXmlElementTag *tag, const char *name);
	const char *pGetAttributeString(decXmlElementTag *tag, const char *name);
	int pGetAttributeInt(decXmlElementTag *tag, const char *name);
	float pGetAttributeFloat(decXmlElementTag *tag, const char *name);
};

#endif
