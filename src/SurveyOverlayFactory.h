/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Survey Plugin Freinds
 * Author:   David Register
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */

#include "bbox.h"
#include "ocpn_plugin.h"
#include "tcmgr.h"
#include <map>
#include <wx/string.h>

struct SurveyOverlaySettings;
class soundingdata;

using namespace std;

#define PI 3.1412
#define METERS 1
#define FEET 2
#define FATHOMS 3

// World Mercator
#define PROJECTION 3395

//----------------------------------------------------------------------------------------------------------
//    Survey Overlay Specification
//----------------------------------------------------------------------------------------------------------
/*
class SurveyOverlay {
public:
    SurveyOverlay( void )
    {
        m_iTexture = 0;
        m_pDCBitmap = NULL, m_pRGBA = NULL;
    }

    ~SurveyOverlay( void )
    {
        if(m_iTexture)
          glDeleteTextures( 1, &m_iTexture );
        delete m_pDCBitmap, delete[] m_pRGBA;
    }

    unsigned int m_iTexture;

    wxBitmap *m_pDCBitmap;
    unsigned char *m_pRGBA;

    int m_width;
    int m_height;
};*/

//----------------------------------------------------------------------------------------------------------
//    Survey Overlay Factory Specification
//----------------------------------------------------------------------------------------------------------
class SurveyDlg;

class SurveyOverlayFactory {
public:
    SurveyOverlayFactory(SurveyDlg& dlg);
    ~SurveyOverlayFactory();

    void SetSettings(bool hiDefGraphics, bool GradualColors)
    {
        m_hiDefGraphics = hiDefGraphics;
        m_bGradualColors = GradualColors;
    }

    void SetMessage(wxString message) { m_Message = message; }
    void SetTimeZone(int TimeZone) { m_TimeZone = TimeZone; }
    void SetParentSize(int w, int h)
    {
        m_ParentSize.SetWidth(w);
        m_ParentSize.SetHeight(h);
    }
    bool RenderGLSurveyOverlay(wxGLContext* pcontext, PlugIn_ViewPort* vp);
    bool RenderSurveyOverlay(wxDC& dc, PlugIn_ViewPort* vp);

    void DrawAllSoundingsInViewPort(PlugIn_ViewPort* BBox, bool bRebuildSelList,
        bool bforce_redraw_currents, bool bdraw_mono_for_mask,
        wxDateTime myTime);
    void Reset();
    wxImage& DrawGLText(double value, int precision);
    wxImage& DrawGLTextDir(double value, int precision);
    wxImage& DrawGLTextString(wxString myText);
    wxImage& DrawGLPolygon(double myDepth);

    void DrawGLLabels(SurveyOverlayFactory* pof, wxDC* dc, PlugIn_ViewPort* vp,
        wxImage& imageLabel, double myLat, double myLon, int offset);

    void drawGLPolygons(SurveyOverlayFactory* pof, wxDC* dc,
        PlugIn_ViewPort* vp, wxImage& imageLabel, double myLat, double myLon,
        int sdgid, int surid, int pixxc, int pixyc, int offset);

    void DrawGLLine(double x1, double y1, double x2, double y2, double width,
        wxColour myColour);
    void DrawOLBitmap(
        const wxBitmap& bitmap, wxCoord x, wxCoord y, bool usemask);
    PlugIn_ViewPort* vp;
    wxDateTime m_dtUseNew;

private:
    bool inGL;

    bool DoRenderSurveyOverlay(PlugIn_ViewPort* vp);
    void RenderMyArrows(PlugIn_ViewPort* vp);

    void DrawGLSoundingMark(int x, int y, double scale, double depth,
        int sounding_id, int survey_id, wxColour c);
    void DrawSounding(wxDC& dc, int x, int y, double depth, int sounding_id,
        int survey_id, int text_offset);
    void DrawMessageWindow(wxString msg, int x, int y, wxFont* mfont);

    void drawWindArrowWithBarbs(int config, int x, int y, double vx, double vy,
        bool polar, bool south, wxColour arrowColor);
    void drawWaveArrow(int i, int j, double dir, wxColour arrowColor);

    wxColour GetDepthColour(double my_depth);

    double m_last_vp_scale;

    // wxPoint p_basic[9];
    //
    wxString m_Message;
    wxString m_Message_Hiden;
    int m_TimeZone;
    wxSize m_ParentSize;

    wxDC* m_pdc;
    wxGraphicsContext* m_gdc;

    wxFont* m_dFont_map;
    wxFont* m_dFont_war;

    bool m_hiDefGraphics;
    bool m_bGradualColors;

    std::map<double, wxImage> m_labelCache;
    std::map<wxString, wxImage> m_labelCacheText;

    SurveyDlg& m_dlg;
    SurveyOverlaySettings& m_settings;
    vector<soundingdata>& m_sdgs;

    wxString myString;
    TCMgr* ctcmgr;
    wxBoundingBox* myBox;
    LLBBox* myLLBox;

    wxString* pTC_Dir;
};
