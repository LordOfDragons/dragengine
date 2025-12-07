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

#ifndef _DEALFILEFORMAT_H_
#define _DEALFILEFORMAT_H_

#include "../../engine/modules/dealEngineModule.h"

#include "../../deObject.h"
#include "../../common/string/decString.h"



/**
 * \brief File Format.
 */
class dealFileFormat : public deObject{
private:
	dealEngineModule::eModuleTypes pType;
	decString pPattern;
	bool pSupported;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dealFileFormat> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file format. */
	dealFileFormat();
	
	/** \brief Clean up file format. */
	virtual ~dealFileFormat();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief File type. */
	inline dealEngineModule::eModuleTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType(dealEngineModule::eModuleTypes type);
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set pattern. */
	void SetPattern(const char *pattern);
	
	/** \brief File format is supported. */
	inline bool GetSupported() const{ return pSupported; }
	
	/** \brief Set if file format is supported. */
	void SetSupported(bool supported);
	/*@}*/
};

#endif
