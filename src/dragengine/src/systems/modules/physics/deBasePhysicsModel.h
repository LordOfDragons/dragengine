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

#ifndef _DEBASEPHYSICSMODEL_H_
#define _DEBASEPHYSICSMODEL_H_

#include "../../../dragengine_export.h"
#include "../../../resources/rig/deRig.h"


/**
 * \brief Physics Module Model Peer.
 */
class DE_DLL_EXPORT deBasePhysicsModel{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBasePhysicsModel();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsModel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Generate collision shapes as a rig if supported by the physics module.
	 * \param[in] convexHullThreshold Threshold for generating convex hulls.
	 * \param[in] weightThreshold Threshold for model weights to consider.
	 * 
	 * Creates a rig containing collision shapes generated from the model. Generated shapes can be
	 * analytic shapes (sphere, box, capsule, cylinder) or convex hulls.
	 * 
	 * The convex hull threshold is multiplied by the threshold used by the physics module.
	 * It is recommended to use 1 as default value. If you want to generate only convex hulls
	 * set value to 0. If you want to not generate convex hulls set value to a large number like
	 * 1000. Other values can be used to fine tune the ratio of analytic shapes to convex hulls.
	 * 
	 * Weight threshold indicates the minimum weight required for a vertex to be included in
	 * collision shape calculation. The weight threshold is relative to the maximum weight of
	 * all vertices affecting the same bone. Recommended default value is 0.45. This makes
	 * collision shapes overlap to some degree. For models with broadly smeared weights using
	 * a larger value helps. 
	 * 
	 * Default implementation returns null reference.
	 */
	virtual deRig::Ref GenerateCollisionShapes(float convexHullThreshold, float weightThreshold);
	/*@}*/
};

#endif
