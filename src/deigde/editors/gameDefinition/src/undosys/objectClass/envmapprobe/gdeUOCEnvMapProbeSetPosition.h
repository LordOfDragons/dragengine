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


#ifndef _GDEUOCENVMAPPROBESETPOSITION_H_
#define _GDEUOCENVMAPPROBESETPOSITION_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class gdeOCEnvMapProbe;
class gdeObjectClass;



/**
 * \brief Undo action object class environment map probe set position.
 */
class gdeUOCEnvMapProbeSetPosition : public igdeUndo{
private:
	gdeObjectClass *pObjectClass;
	gdeOCEnvMapProbe *pEnvMapProbe;
	
	decVector pOldValue;
	decVector pNewValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	gdeUOCEnvMapProbeSetPosition( gdeObjectClass *objectClass,
		gdeOCEnvMapProbe *component, const decVector &newValue );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~gdeUOCEnvMapProbeSetPosition();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
