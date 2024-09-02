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

#ifndef _DEOALENVPROBELISTENERCACHED_H_
#define _DEOALENVPROBELISTENERCACHED_H_

#include "deoalEnvProbeListener.h"



/**
 * \brief Environment probe listener parameters.
 * 
 * Stores the result of an environment probe for a specific listener position. Each probe can
 * store parameters for mutliple listener positions. This allows to cache calculated listener
 * parameters. This class stores only the listener specific parameters. Listener independent
 * parameters are stored in the owner probe itself.
 * 
 * The listener position is relative to the environment probe position.
 * 
 * Listeners have a last used number. This is used to drop long time not used listener
 * parameters to save memory and increasing look up speed.
 */
class deoalEnvProbeListenerCached : public deoalEnvProbeListener {
private:
	decVector pPosition;
	unsigned short pLastUsed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create environment probe listener. */
	deoalEnvProbeListenerCached();
	
	/** \brief Clean up environment probe listener. */
	~deoalEnvProbeListenerCached();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Listener position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set listener position. */
	void SetPosition( const decVector &position );
	
	/** \brief Last used. */
	inline unsigned short GetLastUsed() const{ return pLastUsed; }
	
	/** \brief Set last used. */
	void SetLastUsed( unsigned short lastUsed );
	/*@}*/
};

#endif

