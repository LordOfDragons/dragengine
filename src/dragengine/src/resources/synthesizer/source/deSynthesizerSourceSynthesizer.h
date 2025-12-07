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

#ifndef _DESYNTHESIZERSOURCESYNTHESIZER_H_
#define _DESYNTHESIZERSOURCESYNTHESIZER_H_

#include "deSynthesizerSource.h"

class deSound;


/**
 * \brief Synthesizer source using another synthesizer.
 */
class DE_DLL_EXPORT deSynthesizerSourceSynthesizer : public deSynthesizerSource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerSourceSynthesizer> Ref;
	
	
private:
	deSynthesizer *pSynthesizer;
	
	int *pConnections;
	int pConnectionCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	deSynthesizerSourceSynthesizer();
	
protected:
	/**
	 * \brief Clean up synthesizer source.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deSynthesizerSourceSynthesizer() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Synthesizer or NULL if silent. */
	inline deSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or NULL if silent. */
	void SetSynthesizer(deSynthesizer *synthesizer);
	
	
	
	/**
	 * \brief Update connection count.
	 * 
	 * You have to call this method after you changed the child synthesizer without
	 * first removing it from this synthesizer.
	 */
	void UpdateConnectionCount();
	
	/**
	 * \brief Number of connections.
	 * 
	 * This is the number of controllers in the child synthesizer if present or 0 if absent.
	 */
	inline int GetConnectionCount() const{ return pConnectionCount; }
	
	/** \brief Index of local controller for child controller. */
	int GetConnectionAt(int childController) const;
	
	/** \brief Set index of local controller for child controller. */
	void SetConnectionAt(int childController, int localController);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit source. */
	void Visit(deSynthesizerSourceVisitor &visitor) override;
	/*@}*/
};

#endif
