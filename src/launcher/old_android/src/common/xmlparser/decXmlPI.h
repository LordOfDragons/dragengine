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

#ifndef _DECXMLPI_H_
#define _DECXMLPI_H_

#include "decXmlElement.h"
#include "../string/decString.h"



/**
 * \brief XML Process Instruction.
 */
class decXmlPI : public decXmlElement{
private:
	decString pTarget;
	decString pCommand;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create xml process instruction. */
	decXmlPI(const char *target);
	
protected:
	/** \brief Clean up xml process instruction. */
	virtual ~decXmlPI();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Target. */
	inline const decString &GetTarget() const{return pTarget;}
	
	/** \brief Set target. */
	void SetTarget(const char *target);
	
	/** \brief Command. */
	inline const decString &GetCommand() const{return pCommand;}
	
	/** \brief Set command. */
	void SetCommand(const char *command);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void Visit(decXmlVisitor &visitor);
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	virtual bool CanCastToPI() const;
	virtual decXmlPI *CastToPI();
	/*@}*/
};

#endif
