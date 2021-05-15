#pragma once

#include "UI/UIStatic.h"

class CActor;
class CUICustomMap;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const float pUpdateZoomFactorByClick	= 0.2;
const float pMaxZoomFactor				= 2.0;
const float pMinZoomFactor				= 1.0;

class CUIZoneMap
{
	CUICustomMap*	m_ActiveMap;

	CUIStatic		m_Minimap;
	CUIStatic		m_ClipFrame;
	CUIStatic		m_Compass;
	CUIStatic		m_Center;
	CUIStatic		m_Background;
	CUIStatic		m_ZoomText;
	CUIStatic		m_DistanceText;

	float			m_fScale;
	float			m_fZoomFactor;

public:
					CUIZoneMap		();
	virtual			~CUIZoneMap		();

	void			SetHeading		(float angle);
	void			Init			();

	void			Render			();
	void			UpdateRadar		(Fvector pos);

	void			SetScale		(float s)				{ m_fScale = s; }
	float			GetScale		()						{ return m_fScale; }

	bool			ZoomIn			();
	bool			ZoomOut			();

	CUIStatic&		Minimap			()						{ return m_Minimap; };
	void			SetupCurrentMap	();

	void			SetZoomFactor	(float value);
	float			GetZoomFactor	()				const	{ return m_fZoomFactor; }

	void			IncZoom			()						{ SetZoomFactor(GetZoomFactor() + pUpdateZoomFactorByClick); }
	void			DecZoom			()						{ SetZoomFactor(GetZoomFactor() - pUpdateZoomFactorByClick); }
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////