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

#ifndef _DEOGLSKYLAYERTRACKER_H_
#define _DEOGLSKYLAYERTRACKER_H_

#include <dragengine/common/math/decMath.h>

class deSkyLayer;



/**
 * Sky Layer Tracker.
 * Helper class tracking the state changes of a sky layer using thresholds.
 * The setter methods of this class return true if a new value is more than
 * the matching treshold value apart. The new value is only set in this case.
 */
class deoglSkyLayerTracker{
public:
	/** Threshold for one degree in squared distance metric. */
	static const float THRESHOLD_ONE_DEGREE_ORIENTATION;
	
private:
	/** Threshold for squared distance changes in orientation values. */
	float pThresholdOrientation;
	/** Threshold for absolute value changes in intensity values. */
	float pThresholdIntensity;
	/** Threshold for absolute value changes in transparency values. */
	float pThresholdTransparency;
	/** Threshold for squared distance changes in color values. */
	float pThresholdColor;
	
	/** Last layer orientation. */
	decQuaternion pOrientation;
	/** Last layer color. */
	decColor pColor;
	/** Last layer intensity. */
	float pIntensity;
	/** Last layer transparency. */
	float pTransparency;
	
	/** Last light orientation. */
	decQuaternion pLightOrientation;
	/** Last light color. */
	decColor pLightColor;
	/** Last light intensity. */
	float pLightIntensity;
	/** Last ambient intensity. */
	float pAmbientIntensity;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new sky layer tracker with values inited from a sky layer. */
	deoglSkyLayerTracker();
	/** Cleans up the sky tracker. */
	~deoglSkyLayerTracker();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the threshold for squared distance changes in orientation values. */
	inline float GetThresholdOrientation() const{ return pThresholdOrientation; }
	/** Sets the threshold for squared distance changes in orientation values. */
	void SetThresholdOrientation(float threshold);
	/** Retrieves the threshold for absolute value changes in intensity values. */
	inline float GetThresholdIntensity() const{ return pThresholdIntensity; }
	/** Sets the threshold for absolute value changes in intensity values. */
	void SetThresholdIntensity(float threshold);
	/** Retrieves the threshold for absolute value changes in transparency values. */
	inline float GetThresholdTransparency() const{ return pThresholdTransparency; }
	/** Sets the threshold for absolute value changes in transparency values. */
	void SetThresholdTransparency(float threshold);
	/** Retrieves the threshold for squared distance changes in color values. */
	inline float GetThresholdColor() const{ return pThresholdColor; }
	/** Sets the threshold for squared distance changes in color values. */
	void SetThresholdColor(float threshold);
	
	/** Retrieves the last layer orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	/**
	 * Updates the last layer orientation if the new orientation is more than the orientation
	 * threshold apart from the last layer orientation. Returns true if the last layer
	 * orientation changed or false otherwise.
	 */
	bool UpdateOrientation(const decQuaternion &orientation);
	/** Retrieves the last layer color. */
	inline const decColor &GetColor() const{ return pColor; }
	/**
	 * Updates the last layer color if the new color is more than the color threshold apart
	 * from the last layer color. Returns true if the last layer color changed or false otherwise.
	 */
	bool UpdateColor(const decColor &color);
	/** Retrieves the last layer intensity. */
	inline float GetIntensity() const{ return pIntensity; }
	/**
	 * Updates the last layer intensity if the new intensity is more than the intensity threshold
	 * apart from the last layer intensity. Returns true if the last layer intensity changed or
	 * false otherwise.
	 */
	bool UpdateIntensity(float intensity);
	/** Retrieves the Last layer transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	/**
	 * Updates the last layer transparency. if the new transparency is more than the transparency
	 * threshold apart from the last layer transparency. Returns true if the last layer
	 * transparency changed or false otherwise.
	 */
	bool UpdateTransparency(float transparency);
	
	/** Retrieves the last light orientation. */
	inline const decQuaternion &GetLightOrientation() const{ return pLightOrientation; }
	/**
	 * Updates the last layer light orientation if the new orientation is more than the orientation
	 * threshold apart from the last layer light orientation. Returns true if the last layer light
	 * orientation changed or false otherwise.
	 */
	bool UpdateLightOrientation(const decQuaternion &orientation);
	/** Retrieves the last light color. */
	inline const decColor &GetLightColor() const{ return pLightColor; }
	/**
	 * Updates the last layer light color if the new color is more than the color threshold apart
	 * from the last layer light color. Returns true if the last layer light color changed or
	 * false otherwise.
	 */
	bool UpdateLightColor(const decColor &color);
	/** Retrieves the last light intensity. */
	inline float GetLightIntensity() const{ return pLightIntensity; }
	/**
	 * Updates the last layer light intensity if the new intensity is more than the intensity
	 * threshold apart from the last layer light intensity. Returns true if the last layer
	 * light intensity changed or false otherwise.
	 */
	bool UpdateLightIntensity(float intensity);
	/** Retrieves the last ambient intensity. */
	inline float GetAmbientIntensity() const{ return pAmbientIntensity; }
	/**
	 * Updates the last layer ambient intensity if the new intensity is more than the intensity
	 * threshold apart from the last layer ambient intensity. Returns true if the last layer
	 * ambient intensity changed or false otherwise.
	 */
	bool UpdateAmbientIntensity(float intensity);
	/*@}*/
};

#endif
