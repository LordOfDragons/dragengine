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

#ifndef _IGDEXMLGAMEPROJECT_H_
#define _IGDEXMLGAMEPROJECT_H_

#include "../utils/igdeBaseXML.h"

class igdeGameProject;

class deLogger;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlElementTag;
class decXmlWriter;



/**
 * \brief Load and save XML game project files.
 */
class DE_DLL_EXPORT igdeXMLGameProject : public igdeBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game project xml object. */
	igdeXMLGameProject(deLogger *logger);
	
	/** \brief Clean up game project xml object. */
	virtual ~igdeXMLGameProject();
	/*@}*/
	
	
	
	/** \name Loading and Saving */
	/*@{*/
	/** \brief Load a game project from. */
	void Load(decBaseFileReader &reader, igdeGameProject &project);
	
	/** \brief Saves the given game definition. */
	void Save(decBaseFileWriter &writer, const igdeGameProject &project);
	/*@}*/
	
	
	
private:
	void pReadProject(const decXmlElementTag &root, igdeGameProject &project);
	
	void pWriteProject(decXmlWriter &writer, const igdeGameProject &project);
};

#endif
