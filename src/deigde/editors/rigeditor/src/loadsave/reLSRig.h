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

#ifndef _RELSRIG_H_
#define _RELSRIG_H_

#include "reLSRig.h"

#include <dragengine/common/string/decString.h>


class reRig;
class deBaseRigModule;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * \brief Loads and saves rigs in the Drag[en]gine Actor Rig XML format.
 */
class reLSRig{
private:
	deBaseRigModule *pModule;
	
	decString pName;
	decString pPattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load-save. */
	reLSRig(deBaseRigModule *module);
	
	/** \brief Clean up load-save. */
	~reLSRig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{return pPattern;}
	
	/** \brief Set pattern. */
	void SetPattern(const char *pattern);
	/*@}*/
	
	
	
	/** \name Loading and Saving */
	/*@{*/
	/** \brief Load from file. */
	void LoadRig(reRig *rig, decBaseFileReader *file);
	
	/** \brief Write to file. */
	void SaveRig(reRig *rig, decBaseFileWriter *file);
	/*@}*/
};

#endif
