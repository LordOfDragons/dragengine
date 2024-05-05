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

#ifndef _REWPBONELISTENER_H_
#define _REWPBOELISTENER_H_

#include "../../rig/reRigNotifier.h"

class reWPBone;



/**
 * \brief Bone panel listenerer.
 */
class reWPBoneListener : public reRigNotifier{
private:
	reWPBone &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	reWPBoneListener( reWPBone &panel );
	
	/** \brief Clean up listener. */
	virtual ~reWPBoneListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief A bone changed. */
	void BoneChanged( reRig *rig, reRigBone *bone );
	
	/** \brief The active bone changed. */
	void ActiveBoneChanged( reRig *rig );
	/*@}*/
};

#endif
