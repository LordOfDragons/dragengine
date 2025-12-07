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

#ifndef _REWPVIEWLISTENER_H_
#define _REWPVIEWLISTENER_H_

#include "../../rig/reRigNotifier.h"

class reWPView;



/**
 * \brief View panel rig listener.
 */
class reWPViewListener : public reRigNotifier{
private:
	reWPView &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	reWPViewListener(reWPView &panel);
	
	/** \brief Clean up listener. */
	~reWPViewListener() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief The root bone changed. */
	virtual void RootBoneChanged(reRig *rig);
	
	/** \brief The resource changed. */
	void ResourceChanged(reRig *rig) override;
	
	/** \brief The sky changed. */
	void SkyChanged(reRig *rig) override;
	
	/** \brief The environment object changed. */
	void EnvObjectChanged(reRig *rig) override;
	
	/** \brief The view changed. */
	void ViewChanged(reRig *rig) override;
	
	/** \brief The camera changed. */
	void CameraChanged(reRig *rig) override;
	
	/** \brief The camera view changed. */
	void CameraViewChanged(reRig *rig) override;
	
	/** \brief Bone count changed. */
	void BoneCountChanged(reRig *rig) override;
	
	/** \brief A bone changed. */
	void BoneChanged(reRig *rig, reRigBone *bone) override;
	/*@}*/
};

#endif
