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

#ifndef _GDEOCLIGHT_H_
#define _GDEOCLIGHT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/light/deLight.h>



/**
 * \brief Object class light.
 */
class gdeOCLight : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeOCLight> Ref;
	
	
	/** \brief Properties. */
	enum eProperties{
		/** \brief Light type. */
		epType,
		
		/** \brief Light color. */
		epColor,
		
		/** \brief Light intensity. */
		epIntensity,
		
		/** \brief Light ambient intensity ratio. */
		epAmbientRatio,
		
		/** \brief Range. */
		epRange,
		
		/** \brief Half intensity distance. */
		epHalfIntDist,
		
		/** \brief Spot angle. */
		epSpotAngle,
		
		/** \brief Spot ratio. */
		epSpotRatio,
		
		/** \brief Spot smoothness. */
		epSpotSmoothness,
		
		/** \brief Spot exponent. */
		epSpotExponent,
		
		/** \brief Light skin. */
		epLightSkin,
		
		/** \brief Activated. */
		epActivated,
		
		/** \brief Cast shadows. */
		epCastShadows,
		
		/** \brief Light important hint. */
		epHintLightImportance,
		
		/** \brief Shadow important hint. */
		epHintShadowImportance,
		
		/** \brief Attach position. */
		epAttachPosition,
		
		/** \brief Attach rotation. */
		epAttachRotation
	};
	
	/** \brief Triggers. */
	enum eTriggers{
		/** \brief Activated. */
		etActivated
	};
	
	
	
private:
	deLight::eLightTypes pType;
	decColor pColor;
	float pIntensity;
	decVector pPosition;
	decVector pRotation;
	decString pBoneName;
	float pRange;
	float pHalfIntensityDistance;
	float pAmbientRatio;
	float pSpotAngle;
	float pSpotRatio;
	float pSpotSmoothness;
	float pSpotExponent;
	bool pActivated;
	bool pCastShadows;
	int pHintLightImportance;
	int pHintShadowImportance;
	deLight::eMovementHints pHintMovement;
	deLight::eParameterHints pHintParameter;
	decString pLightSkinPath;
	
	decString pPropertyNames[ epAttachRotation + 1 ];
	decString pTriggerNames[ etActivated + 1 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create light. */
	gdeOCLight();
	
	/** \brief Create copy of light. */
	gdeOCLight( const gdeOCLight &light );
	
	/** \brief Clean up light. */
	virtual ~gdeOCLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Type. */
	inline deLight::eLightTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( deLight::eLightTypes type );
	
	
	
	/** \brief Color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set color. */
	void SetColor( const decColor &color );
	
	/** \brief Intensity. */
	inline float GetIntensity() const{ return pIntensity; }
	
	/** \brief Set intensity. */
	void SetIntensity( float intensity );
	
	/** \brief Light range in meters. */
	inline float GetRange() const{ return pRange; }
	
	/** \brief Set light range in meters. */
	void SetRange( float range );
	
	/** \brief Half intensity distance in meters. */
	inline float GetHalfIntensityDistance() const{ return pHalfIntensityDistance; }
	
	/** \brief Set half intensity distance in meters. */
	void SetHalfIntensityDistance( float distance );
	
	/** \brief Ratio of ambient light in relation to the total intensity. */
	inline float GetAmbientRatio() const{ return pAmbientRatio; }
	
	/** \brief Set ratio of ambient light in relation to the total intensity. */
	void SetAmbientRatio( float ratio );
	
	
	
	/** \brief Position relative to parent object. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Sets position relative to parent object. */
	void SetPosition( const decVector &position );
	
	/** \brief Rotation relative to parent object. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set orientation relative to parent object. */
	void SetRotation( const decVector &orientation );
	
	
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of the bone to attach to or empty string. */
	void SetBoneName( const char *boneName );
	
	
	
	/** \brief Outer spot Angle. */
	inline float GetSpotAngle() const{ return pSpotAngle; }
	
	/** \brief Set outer spot angle. */
	void SetSpotAngle( float angle );
	
	/** \brief Spot ratio as height divided by width. */
	inline float GetSpotRatio() const{ return pSpotRatio; }
	
	/** \brief Set spot ratio as height divided by width. */
	void SetSpotRatio( float ratio );
	
	/** \brief Spot smoothness. */
	inline float GetSpotSmoothness() const{ return pSpotSmoothness; }
	
	/** \brief Set spot smoothness. */
	void SetSpotSmoothness( float smoothness );
	
	/** \brief Spot exponent. */
	inline float GetSpotExponent() const{ return pSpotExponent; }
	
	/** \brief Set spot exponent. */
	void SetSpotExponent( float exponent );
	
	
	
	/** \brief Light importance hint. */
	inline int GetHintLightImportance() const{ return pHintLightImportance; }
	
	/** \brief Set light importance hint. */
	void SetHintLightImportance( int importance );
	
	/** \brief Shadow importance hint. */
	inline int GetHintShadowImportance() const{ return pHintShadowImportance; }
	
	/** \brief Set shadow importance hint. */
	void SetHintShadowImportance( int importance );
	
	/** \brief Movement hint. */
	inline deLight::eMovementHints GetHintMovement() const{ return pHintMovement; }
	
	/** \brief Set movement hint. */
	void SetHintMovement( deLight::eMovementHints hint );
	
	/** \brief Parameter hint. */
	inline deLight::eParameterHints GetHintParameter() const{ return pHintParameter; }
	
	/** \brief Set parameter hint. */
	void SetHintParameter( deLight::eParameterHints hint );
	
	
	
	/** \brief Light is activated emitting light. */
	inline bool GetActivated() const{ return pActivated; }
	
	/** \brief Set if light is activated emitting light. */
	void SetActivated( bool activated );
	
	/** \brief Light casts shadows. */
	inline bool GetCastShadows() const{ return pCastShadows; }
	
	/** \brief Set if light casts shadows. */
	void SetCastShadows( bool castShadows );
	
	
	
	/** \brief Light skin path. */
	inline const decString &GetLightSkinPath() const{ return pLightSkinPath; }
	
	/** \brief Set light skin path. */
	void SetLightSkinPath( const char *path );
	
	
	
	/** \brief Property name is set. */
	bool IsPropertySet( eProperties property ) const;
	
	/** \brief Name of property. */
	const decString &GetPropertyName( eProperties property ) const;
	
	/** \brief Set name of property. */
	void SetPropertyName( eProperties property, const char *name );
	
	/** \brief One or more properties use a name. */
	bool HasPropertyWithName( const char *name ) const;
	
	
	
	/** \brief Trigger name is set. */
	bool IsTriggerSet( eTriggers trigger ) const;
	
	/** \brief Name of trigger. */
	const decString &GetTriggerName( eTriggers trigger ) const;
	
	/** \brief Set name of trigger. */
	void SetTriggerName( eTriggers trigger, const char *name );
	/*@}*/
};

#endif
