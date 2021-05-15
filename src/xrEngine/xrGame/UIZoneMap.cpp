#include "stdafx.h"

#include "UIZoneMap.h"

#include "Actor.h"
#include "ai_space.h"
#include "game_cl_base.h"
#include "game_graph.h"
#include "Grenade.h"
#include "HUDManager.h"
#include "InfoPortion.h"
#include "Level.h"
#include "PDA.h"
#include "UI/UIMap.h"
#include "UI/UIXmlInit.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CUIZoneMap::CUIZoneMap()
{
	m_fZoomFactor = 1.f;
}

CUIZoneMap::~CUIZoneMap()
{
	
}

void CUIZoneMap::Init()
{
	CUIXml								uiXml;
	bool					xml_result	= uiXml.Init(CONFIG_PATH, UI_PATH, "zone_map.xml");
	R_ASSERT3							(xml_result, "xml file not found", "zone_map.xml");

	CUIXmlInit							xml_init;
	xml_init.InitStatic					(uiXml, "minimap", 0, &m_Minimap);

	xml_init.InitStatic					(uiXml, "minimap:level_frame", 0, &m_ClipFrame);
	m_Minimap.AttachChild				(&m_ClipFrame);

	xml_init.InitStatic					(uiXml, "minimap:compass", 0, &m_Compass);
	m_Minimap.AttachChild				(&m_Compass);

	xml_init.InitStatic					(uiXml, "minimap:center", 0, &m_Center);
	m_ClipFrame.AttachChild				(&m_Center);
	m_Center.SetWndPos					(m_ClipFrame.GetWidth() / 2, m_ClipFrame.GetHeight() / 2);

	xml_init.InitStatic					(uiXml, "minimap:background", 0, &m_Background);
	m_Minimap.AttachChild				(&m_Background);

	if (IsGameTypeSingle())
	{
		xml_init.InitStatic				(uiXml, "minimap:zoom_text", 0, &m_ZoomText);
		m_Minimap.AttachChild			(&m_ZoomText);

		xml_init.InitStatic				(uiXml, "minimap:dist_text", 0, &m_DistanceText);
		m_Minimap.AttachChild			(&m_DistanceText);
	}

	m_ActiveMap							= xr_new<CUIMiniMap>();
	m_ClipFrame.AttachChild				(m_ActiveMap);
	m_ActiveMap->SetAutoDelete			(true);
	m_ActiveMap->EnableHeading			(true);
}

void CUIZoneMap::Render()
{
	m_ClipFrame.Draw	();
	m_Compass.Draw		();
	m_Minimap.Draw		();
}

void CUIZoneMap::SetHeading(float angle)
{
	m_ActiveMap->SetHeading	(angle);
	m_Compass.SetHeading	(angle);
};

void CUIZoneMap::UpdateRadar(Fvector pos)
{
	m_ClipFrame.Update			();
	m_Minimap.Update			();
	m_ActiveMap->SetActivePoint	(pos);

	if (IsGameTypeSingle())
	{
		string64					dstr, zstr;

		if (m_ActiveMap->GetPointerDistance() > 0.5f)
		{
			sprintf_s				(dstr,"%.f m.", m_ActiveMap->GetPointerDistance());
			m_DistanceText.SetText	(dstr);
		}
		else
		{
			m_DistanceText.SetText	("");
		}

		sprintf_s					(zstr, "x%.1f", GetZoomFactor());
		m_ZoomText.SetText			(zstr);
	}
}

bool CUIZoneMap::ZoomIn()
{
	return true;
}

bool CUIZoneMap::ZoomOut()
{
	return true;
}

void CUIZoneMap::SetupCurrentMap()
{
	CInifile*	pLtx			= pGameIni;

	if (!pLtx->section_exist(Level().name()))
		pLtx					= Level().pLevel;

	m_ActiveMap->Init			(Level().name(), *pLtx, "hud\\default");

	Frect						r;
	m_ClipFrame.GetAbsoluteRect	(r);
	m_ActiveMap->SetClipRect	(r);
	
	Fvector2					wnd_size;
	float		zoom_factor		= float(m_ClipFrame.GetWndRect().width()) / 100.0f;
	wnd_size.x					= m_ActiveMap->BoundRect().width() * zoom_factor;
	wnd_size.y					= m_ActiveMap->BoundRect().height() * zoom_factor;
	m_ActiveMap->SetWndSize		(wnd_size);
	SetZoomFactor				(1.f);
}

void CUIZoneMap::SetZoomFactor(float value)
{
	if (value > pMaxZoomFactor)
		value = pMaxZoomFactor;

	if (value < pMinZoomFactor)
		value = pMinZoomFactor;

	if (m_fZoomFactor == value)
		return;

	m_fZoomFactor				= value;
	Fvector2	wnd_size;
	const float	zoom_factor		= (static_cast<float>(m_ClipFrame.GetWndRect().width()) / 100.0f) * value;
	wnd_size.x					= m_ActiveMap->BoundRect().width() * zoom_factor;
	wnd_size.y					= m_ActiveMap->BoundRect().height() * zoom_factor;
	m_ActiveMap->SetWndSize		(wnd_size);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////