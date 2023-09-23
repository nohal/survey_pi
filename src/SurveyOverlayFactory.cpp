/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Survey Object
 * Author:   David Register, Mike Rossiter
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
 *
 */

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif // precompiled headers

#include <wx/glcanvas.h>
#include <wx/graphics.h>
#include <wx/progdlg.h>

#include "SurveyOverlayFactory.h"
#include "surveygui_impl.h"

#include "bbox.h"

#ifdef __WXMSW__
#define snprintf _snprintf
#endif // __WXMSW__

using namespace std;

class Position;
class SurveyDlg;
class PlugIn_ViewPort;
class wxBoundingBox;
class soundingdata;

#include <vector>

//----------------------------------------------------------------------------------------------------------
//    Survey Overlay Factory Implementation
//----------------------------------------------------------------------------------------------------------

SurveyOverlayFactory::SurveyOverlayFactory(SurveyDlg& dlg)
    : m_dlg(dlg)
    , m_sdgs(dlg.mysoundings)
    , m_settings(dlg.mySettings)

{
    m_dFont_map = new wxFont(
        10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_dFont_war = new wxFont(
        16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);

    m_last_vp_scale = 0.;
    m_dtUseNew = wxDateTime::Now();
}

SurveyOverlayFactory::~SurveyOverlayFactory() { }

void SurveyOverlayFactory::Reset() { }

bool SurveyOverlayFactory::RenderGLSurveyOverlay(
    wxGLContext* pcontext, PlugIn_ViewPort* vp)
{
    m_pdc = NULL; // inform lower layers that this is OpenGL render

    DrawAllSoundingsInViewPort(vp, false, false, false, m_dtUseNew);
    return true;
}

bool SurveyOverlayFactory::RenderSurveyOverlay(wxDC& dc, PlugIn_ViewPort* vp)
{

#if wxUSE_GRAPHICS_CONTEXT
    wxMemoryDC* pmdc;
    pmdc = wxDynamicCast(&dc, wxMemoryDC);
    wxGraphicsContext* pgc = wxGraphicsContext::Create(*pmdc);
    m_gdc = pgc;
    m_pdc = &dc;
#else
    m_pdc = &dc;
#endif

    m_pdc = &dc;
    DrawAllSoundingsInViewPort(vp, false, false, false, m_dtUseNew);
    return true;
}

bool SurveyOverlayFactory::DoRenderSurveyOverlay(PlugIn_ViewPort* vp)
{

    m_Message_Hiden.Empty();
    m_last_vp_scale = vp->view_scale_ppm;

    if (!m_Message_Hiden.IsEmpty())
        DrawMessageWindow(
            m_Message_Hiden, vp->pix_width, vp->pix_height, m_dFont_map);
    DrawMessageWindow(
        m_Message_Hiden, vp->pix_width, vp->pix_height, m_dFont_map);
    return true;
}

void SurveyOverlayFactory::DrawMessageWindow(
    wxString msg, int x, int y, wxFont* mfont)
{
    if (msg.empty())
        return;

    wxMemoryDC mdc;
    wxBitmap bm(1000, 1000);
    mdc.SelectObject(bm);
    mdc.Clear();

    mdc.SetFont(*mfont);
    mdc.SetPen(*wxTRANSPARENT_PEN);
    mdc.SetBrush(wxColour(243, 229, 47));
    int w, h;
    mdc.GetMultiLineTextExtent(msg, &w, &h);
    h += 2;
    int label_offset = 10;
    int wdraw = w + (label_offset * 2);
    mdc.DrawRectangle(0, 0, wdraw, h);

    mdc.DrawLabel(msg, wxRect(label_offset, 0, wdraw, h),
        wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
    mdc.SelectObject(wxNullBitmap);

    wxBitmap sbm = bm.GetSubBitmap(wxRect(0, 0, wdraw, h));

    DrawOLBitmap(sbm, 0, y - (GetChartbarHeight() + h), false);
}

wxColour SurveyOverlayFactory::GetDepthColour(double my_depth)
{

    wxColour c_blue = wxColour(0, 0, 255);
    wxColour c_sky = wxColour(0, 128, 255);
    wxColour c_cyan = wxColour(0, 255, 255);
    wxColour c_spring = wxColour(0, 255, 128);
    wxColour c_green = wxColour(0, 255, 0);
    wxColour c_lime = wxColour(128, 255, 0);
    wxColour c_yellow = wxColour(255, 255, 0);
    wxColour c_brown = wxColour(255, 191, 0);
    wxColour c_amber = wxColour(255, 126, 0);
    wxColour c_red = wxColour(255, 0, 0);

    if (my_depth < 2) {
        return c_red;
    }
    if ((my_depth >= 2) && (my_depth < 3)) {
        return c_amber;
    }
    if ((my_depth >= 3) && (my_depth < 4)) {
        return c_brown;
    }
    if ((my_depth >= 4) && (my_depth < 6)) {
        return c_yellow;
    }
    if ((my_depth >= 6) && (my_depth < 8)) {
        return c_lime;
    }
    if ((my_depth >= 8) && (my_depth < 10)) {
        return c_green;
    }
    if ((my_depth >= 10) && (my_depth < 12)) {
        return c_spring;
    }
    if ((my_depth >= 12) && (my_depth < 14)) {
        return c_cyan;
    }
    if ((my_depth >= 14) && (my_depth < 20)) {
        return c_sky;
    }
    if ((my_depth >= 20)) {
        return c_blue;
    }

    return wxColour(0, 0, 0);
}

wxImage& SurveyOverlayFactory::DrawGLText(double value, int precision)
{

    wxString labels;

    double coef = 1.0;
    int iUnits = PI_GetPLIBDepthUnitInt();
    if (iUnits == FATHOMS)
        coef = 1.8288;
    else if (iUnits == FEET)
        coef = 0.3048;
    else {
    }

    double depth = value / coef;
    int p = precision;

    labels.Printf(_T("%.*f"), p, depth);

    wxMemoryDC mdc(wxNullBitmap);

    mdc.SetFont(wxFont(m_settings.m_sFont));

    int w, h;
    mdc.GetTextExtent(labels, &w, &h);

    int label_offset = 0; // 20

    wxBitmap bm(w + label_offset * 2, h);
    mdc.SelectObject(bm);
    mdc.Clear();

    wxColour text_color;
    text_color = m_settings.m_sFontColor;

    /*
    wxColour f_Colour;
    if (m_settings.m_bUseDepthColours) {
            f_Colour = GetDepthColour(depth);
            text_color = f_Colour;
    }
*/

    // GetGlobalColor(_T("UINFD"), &text_color);
    wxPen penText(text_color);
    mdc.SetPen(penText);

    mdc.SetBrush(*wxTRANSPARENT_BRUSH);
    mdc.SetTextForeground(text_color);
    mdc.SetTextBackground(wxTRANSPARENT);

    int xd = 0;
    int yd = 0;

    mdc.DrawText(labels, label_offset + xd, yd);

    mdc.SelectObject(wxNullBitmap);

    m_labelCache[depth] = bm.ConvertToImage();

    m_labelCache[depth].InitAlpha();

    wxImage& image = m_labelCache[depth];

    unsigned char* d = image.GetData();
    unsigned char* a = image.GetAlpha();

    w = image.GetWidth(), h = image.GetHeight();
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            int r, g, b;
            int ioff = (y * w + x);
            r = d[ioff * 3 + 0];
            g = d[ioff * 3 + 1];
            b = d[ioff * 3 + 2];

            a[ioff] = 255 - (r + g + b) / 3;
        }

    return m_labelCache[depth];
}

wxImage& SurveyOverlayFactory::DrawGLTextDir(double value, int precision)
{

    wxString labels;
    double coef = 1.0;
    int iUnits = PI_GetPLIBDepthUnitInt();
    if (iUnits == FATHOMS)
        coef = 1.8288;
    else if (iUnits == FEET)
        coef = 0.3048;
    else {
    }

    double depth = value / coef;
    int p = precision;

    labels.Printf(_T("%03.*f"), p, depth);

    wxMemoryDC mdc(wxNullBitmap);

    wxFont* pTCFont;
    pTCFont = wxTheFontList->FindOrCreateFont(12, wxDEFAULT, wxNORMAL, wxBOLD,
        FALSE, wxString(_T("Eurostile Extended")));

    mdc.SetFont(*pTCFont);

    int w, h;
    mdc.GetTextExtent(labels, &w, &h);

    int label_offset = 10; // 5

    wxBitmap bm(w + label_offset * 2, h + 1);
    mdc.SelectObject(bm);
    mdc.Clear();

    wxColour text_color;
    text_color = m_settings.m_sFontColor;
    // GetGlobalColor(_T("UINFD"), &text_color);
    wxPen penText(text_color);
    mdc.SetPen(penText);

    mdc.SetBrush(*wxTRANSPARENT_BRUSH);
    mdc.SetTextForeground(text_color);
    mdc.SetTextBackground(wxTRANSPARENT);

    int xd = 0;
    int yd = 0;

    mdc.DrawText(labels, label_offset + xd, yd + 1);

    mdc.SelectObject(wxNullBitmap);

    m_labelCache[depth] = bm.ConvertToImage();

    m_labelCache[depth].InitAlpha();

    wxImage& image = m_labelCache[depth];

    unsigned char* d = image.GetData();
    unsigned char* a = image.GetAlpha();

    w = image.GetWidth(), h = image.GetHeight();
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            int r, g, b;
            int ioff = (y * w + x);
            r = d[ioff * 3 + 0];
            g = d[ioff * 3 + 1];
            b = d[ioff * 3 + 2];

            a[ioff] = 255 - (r + g + b) / 3;
        }

    return m_labelCache[depth];
}

wxImage& SurveyOverlayFactory::DrawGLTextString(wxString myText)
{

    wxString labels;
    labels = myText;

    wxMemoryDC mdc(wxNullBitmap);

    wxFont* pTCFont;
    pTCFont = wxTheFontList->FindOrCreateFont(12, wxDEFAULT, wxNORMAL, wxBOLD,
        FALSE, wxString(_T("Eurostile Extended")));
    mdc.SetFont(*pTCFont);

    int w, h;
    mdc.GetTextExtent(labels, &w, &h);

    int label_offset = 10; // 5

    wxBitmap bm(w + label_offset * 2, h + 1);
    mdc.SelectObject(bm);
    mdc.Clear();

    wxColour text_color;
    text_color = m_settings.m_sFontColor;
    // GetGlobalColor(_T("UINFD"), &text_color);
    wxPen penText(text_color);
    mdc.SetPen(penText);

    mdc.SetBrush(*wxTRANSPARENT_BRUSH);
    mdc.SetTextForeground(text_color);
    mdc.SetTextBackground(wxTRANSPARENT);

    int xd = 0;
    int yd = 0;

    mdc.DrawText(labels, label_offset + xd, yd + 1);
    mdc.SelectObject(wxNullBitmap);

    m_labelCacheText[myText] = bm.ConvertToImage();

    m_labelCacheText[myText].InitAlpha();

    wxImage& image = m_labelCacheText[myText];

    unsigned char* d = image.GetData();
    unsigned char* a = image.GetAlpha();

    w = image.GetWidth(), h = image.GetHeight();
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            int r, g, b;
            int ioff = (y * w + x);
            r = d[ioff * 3 + 0];
            g = d[ioff * 3 + 1];
            b = d[ioff * 3 + 2];

            a[ioff] = 255 - (r + g + b) / 3;
        }

    return m_labelCacheText[myText];
}

void SurveyOverlayFactory::DrawGLLine(
    double x1, double y1, double x2, double y2, double width, wxColour myColour)
{
    {
        wxColour isoLineColor = myColour;
        glColor4ub(isoLineColor.Red(), isoLineColor.Green(),
            isoLineColor.Blue(), 255 /*isoLineColor.Alpha()*/);

        glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LINE_BIT | GL_ENABLE_BIT
            | GL_POLYGON_BIT | GL_HINT_BIT); // Save state
        {

            //      Enable anti-aliased lines, at best quality
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glLineWidth(width);

            glBegin(GL_LINES);
            glVertex2d(x1, y1);
            glVertex2d(x2, y2);
            glEnd();
        }

        glPopAttrib();
    }
}
void SurveyOverlayFactory::DrawOLBitmap(
    const wxBitmap& bitmap, wxCoord x, wxCoord y, bool usemask)
{
    wxBitmap bmp;
    if (x < 0 || y < 0) {
        int dx = (x < 0 ? -x : 0);
        int dy = (y < 0 ? -y : 0);
        int w = bitmap.GetWidth() - dx;
        int h = bitmap.GetHeight() - dy;
        /* picture is out of viewport */
        if (w <= 0 || h <= 0)
            return;
        wxBitmap newBitmap = bitmap.GetSubBitmap(wxRect(dx, dy, w, h));
        x += dx;
        y += dy;
        bmp = newBitmap;
    } else {
        bmp = bitmap;
    }
    if (m_pdc)
        m_pdc->DrawBitmap(bmp, x, y, usemask);
    else {
        wxImage image = bmp.ConvertToImage();
        int w = image.GetWidth(), h = image.GetHeight();

        if (usemask) {
            unsigned char* d = image.GetData();
            unsigned char* a = image.GetAlpha();

            unsigned char mr, mg, mb;
            if (!image.GetOrFindMaskColour(&mr, &mg, &mb) && !a)
                printf("trying to use mask to draw a bitmap without alpha or "
                       "mask\n");

            unsigned char* e = new unsigned char[4 * w * h];
            {
                for (int y = 0; y < h; y++)
                    for (int x = 0; x < w; x++) {
                        unsigned char r, g, b;
                        int off = (y * image.GetWidth() + x);
                        r = d[off * 3 + 0];
                        g = d[off * 3 + 1];
                        b = d[off * 3 + 2];

                        e[off * 4 + 0] = r;
                        e[off * 4 + 1] = g;
                        e[off * 4 + 2] = b;

                        e[off * 4 + 3] = a
                            ? a[off]
                            : ((r == mr) && (g == mg) && (b == mb) ? 0 : 255);
                    }
            }

            glColor4f(1, 1, 1, 1);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2i(x, y);
            glPixelZoom(1, -1);
            glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, e);
            glPixelZoom(1, 1);
            glDisable(GL_BLEND);

            delete[](e);
        } else {
            glRasterPos2i(x, y);
            glPixelZoom(1, -1); /* draw data from top to bottom */
            glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, image.GetData());
            glPixelZoom(1, 1);
        }
    }
}

void SurveyOverlayFactory::DrawGLLabels(SurveyOverlayFactory* pof, wxDC* dc,
    PlugIn_ViewPort* vp, wxImage& imageLabel, double myLat, double myLon,
    int offset)
{

    //---------------------------------------------------------
    // Ecrit les labels
    //---------------------------------------------------------

    wxPoint ab;
    GetCanvasPixLL(vp, &ab, myLat, myLon);

    // wxPoint cd;
    // GetCanvasPixLL(vp, &cd,myLat, myLon);

    int w = imageLabel.GetWidth();
    int h = imageLabel.GetHeight();

    int label_offset = 0;
    int xd = (ab.x - w / 2);
    if (!m_settings.m_bUseSymbol) {
        offset = 0;
    }
    int yd = (ab.y - h / 2) + offset;

    if (dc) {
        /* don't use alpha for isobars, for some reason draw bitmap ignores
           the 4th argument (true or false has same result) */
        wxImage img(w, h, imageLabel.GetData(), true);
        dc->DrawBitmap(img, xd, yd, false);
    } else { /* opengl */

        int w = imageLabel.GetWidth(), h = imageLabel.GetHeight();

        unsigned char* d = imageLabel.GetData();
        unsigned char* a = imageLabel.GetAlpha();

        unsigned char mr, mg, mb;
        if (!imageLabel.GetOrFindMaskColour(&mr, &mg, &mb) && !a)
            wxMessageBox(
                _T(
			"trying to use mask to draw a bitmap without alpha or mask\n"));

        unsigned char* e = new unsigned char[4 * w * h];
        {
            for (int y = 0; y < h; y++)
                for (int x = 0; x < w; x++) {
                    unsigned char r, g, b;
                    int off = (y * imageLabel.GetWidth() + x);
                    r = d[off * 3 + 0];
                    g = d[off * 3 + 1];
                    b = d[off * 3 + 2];

                    e[off * 4 + 0] = r;
                    e[off * 4 + 1] = g;
                    e[off * 4 + 2] = b;

                    e[off * 4 + 3] = a
                        ? a[off]
                        : ((r == mr) && (g == mg) && (b == mb) ? 0 : 255);
                }
        }

        glColor4f(1, 1, 1, 1);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        int of;
        if (!m_settings.m_bUseSymbol) {
            of = 0;
        } else {
            of = 10;
        }
        glRasterPos2i(xd + of, yd + of); //(xd + 10, yd + 10);
        glPixelZoom(1, -1);
        glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, e);
        glPixelZoom(1, 1);
        glDisable(GL_BLEND);

        delete[](e);
    }
}

wxImage& SurveyOverlayFactory::DrawGLPolygon(double myDepth)
{

    wxString labels;
    labels = _T(""); // dummy label for drawing with

    wxColour s_Colour;

    if (m_settings.m_bUseDepthColours) {
        s_Colour = GetDepthColour(myDepth);
    } else {
        s_Colour = m_settings.m_sSoundingColor;
    }

    wxPen penText(s_Colour);
    wxBrush backBrush(s_Colour);

    wxMemoryDC mdc(wxNullBitmap);

    // wxFont mfont( 9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
    // wxFONTWEIGHT_NORMAL );
    //  mdc.SetFont( mfont );

    int w, h;
    // mdc.GetTextExtent(labels, &w, &h);

    w = 10;
    h = 10;

    wxBitmap bm(w, h);
    mdc.SelectObject(bm);
    mdc.Clear();

    mdc.SetPen(penText);
    mdc.SetBrush(backBrush);
    mdc.SetTextForeground(s_Colour);
    mdc.SetTextBackground(s_Colour);

    int xd = 0;
    int yd = 0;

    int label_offset = 0;

    if (m_settings.m_bUseSymbol) {
        switch (m_settings.m_iSoundingShape) {
        case 1:
            mdc.DrawCircle(xd + 5, yd + 5, 5);
            break;
        case 2:
            mdc.DrawRectangle(xd, yd, w, h);
            break;
        }
    }

    mdc.SelectObject(wxNullBitmap);

    m_labelCacheText[labels] = bm.ConvertToImage();

    m_labelCacheText[labels].InitAlpha();

    wxImage& image = m_labelCacheText[labels];

    unsigned char* d = image.GetData();
    unsigned char* a = image.GetAlpha();

    w = image.GetWidth(), h = image.GetHeight();
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            int r, g, b;
            int ioff = (y * w + x);
            r = d[ioff * 3 + 0];
            g = d[ioff * 3 + 1];
            b = d[ioff * 3 + 2];

            a[ioff] = 255 - (r + g + b) / 3;
        }

    return m_labelCacheText[labels];
}
void SurveyOverlayFactory::drawGLPolygons(SurveyOverlayFactory* pof, wxDC* dc,
    PlugIn_ViewPort* vp, wxImage& imageLabel, double myLat, double myLon,
    int sdgid, int surid, int pixxc, int pixyc, int offset)
{

    //---------------------------------------------------------
    // Ecrit les labels
    //---------------------------------------------------------

    wxPoint ab;
    GetCanvasPixLL(vp, &ab, myLat, myLon);

    wxPoint cd;
    GetCanvasPixLL(vp, &cd, myLat, myLon);

    int w = imageLabel.GetWidth();
    int h = imageLabel.GetHeight();

    int label_offset = 0;
    int xd = (ab.x + cd.x - w) / 2;
    int yd = (ab.y + cd.y - h) / 2 + offset;

    if (dc) {
        /* don't use alpha for isobars, for some reason draw bitmap ignores
           the 4th argument (true or false has same result) */
        wxImage img(w, h, imageLabel.GetData(), true);
        dc->DrawBitmap(img, xd, yd, false);
    } else { /* opengl */

        int w = imageLabel.GetWidth(), h = imageLabel.GetHeight();

        unsigned char* d = imageLabel.GetData();
        unsigned char* a = imageLabel.GetAlpha();

        unsigned char mr, mg, mb;
        if (!imageLabel.GetOrFindMaskColour(&mr, &mg, &mb) && !a)
            wxMessageBox(
                _T(
			"trying to use mask to draw a bitmap without alpha or mask\n"));

        unsigned char* e = new unsigned char[4 * w * h];
        {
            for (int y = 0; y < h; y++)
                for (int x = 0; x < w; x++) {
                    unsigned char r, g, b;
                    int off = (y * imageLabel.GetWidth() + x);
                    r = d[off * 3 + 0];
                    g = d[off * 3 + 1];
                    b = d[off * 3 + 2];

                    e[off * 4 + 0] = r;
                    e[off * 4 + 1] = g;
                    e[off * 4 + 2] = b;

                    e[off * 4 + 3] = a
                        ? a[off]
                        : ((r == mr) && (g == mg) && (b == mb) ? 0 : 255);
                }
        }

        glColor4f(1, 1, 1, 1);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRasterPos2i(xd, yd);
        glPixelZoom(1, -1);
        glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, e);
        glPixelZoom(1, 1);
        glDisable(GL_BLEND);

        delete[](e);
    }

    wxColour colour;
    if (m_settings.m_bConnectSoundings) {
        int x = pixxc;
        int y = pixyc;

        if (m_settings.mLastX != -1 && m_settings.mLastY != -1
            && m_settings.mLastSdgId == sdgid - 1
            && m_settings.mLastSurveyId == surid) {
            colour = wxColour(m_settings.m_sConnectorColor);
            DrawGLLine(
                x, y, m_settings.mLastX, m_settings.mLastY, 0.25, colour);
        }
        m_settings.mLastX = x;
        m_settings.mLastY = y;
        m_settings.mLastSdgId = sdgid;
        m_settings.mLastSurveyId = surid;
    }
}

void SurveyOverlayFactory::DrawAllSoundingsInViewPort(PlugIn_ViewPort* BBox,
    bool bRebuildSelList, bool bforce_redraw_currents, bool bdraw_mono_for_mask,
    wxDateTime myTime)
{
    vector<soundingdata> myfactorysoundings = m_dlg.mysoundings;

    if (BBox->chart_scale > 500000) {
        return;
    }

    double lon = -4;
    double lat = 50;
    double value;
    double depth;
    int sdgid;
    int surid;
    double tid;
    double corrn;

    for (std::vector<soundingdata>::iterator it = myfactorysoundings.begin();
         it != myfactorysoundings.end(); it++) {

        lat = it->latD;
        lon = it->lonD;
        // wxMessageBox(it->depth);
        it->depth.ToDouble(&depth);
        sdgid = it->sdgid;
        surid = it->surid;
        // it->tide.ToDouble(&tid);

        myLLBox = new LLBBox;
        wxBoundingBox LLBBox(
            BBox->lon_min, BBox->lat_min, BBox->lon_max, BBox->lat_max);
        double testLon;

        if (LLBBox.PointInBox(lon, lat, 0)) {

            double myLat = LLBBox.GetMaxY();
            double myLon = LLBBox.GetMaxX();

            int pixxc, pixyc;
            wxPoint cpoint;
            GetCanvasPixLL(BBox, &cpoint, lat, lon);
            pixxc = cpoint.x;
            pixyc = cpoint.y;

            wxColour f_Colour;
            f_Colour = m_settings.m_sFontColor;

            wxColour s_Colour;
            s_Colour = m_settings.m_sSoundingColor;

            wxColour c_Colour;
            if (m_settings.m_bUseDepthColours) {
                c_Colour = GetDepthColour(depth);
            } else {
                c_Colour
                    = wxColour(m_settings.m_sFontColor); // wxColour(0, 0, 0);
            }

            if (m_settings.m_bRenderWithCorrn) {
                if (m_settings.m_sCorrection.IsEmpty()) {
                    corrn = 0;
                } else {
                    m_settings.m_sCorrection.ToDouble(&corrn);
                }

                depth = depth + corrn;
            }

            if (!m_pdc) {

                if (m_settings.m_bUseSymbol
                    && (m_settings.m_iSoundingShape == 1
                        || m_settings.m_iSoundingShape == 2)) {
                    drawGLPolygons(this, m_pdc, BBox, DrawGLPolygon(depth), lat,
                        lon, sdgid, surid, pixxc, pixyc, 0);
                } else {
                    s_Colour = GetDepthColour(depth);
                    DrawGLSoundingMark(
                        pixxc, pixyc, 0, depth, sdgid, surid, s_Colour);
                }
                //
                if (m_settings.m_bRenderSoundingText) {
                    DrawGLLabels(
                        this, m_pdc, BBox, DrawGLText(depth, 1), lat, lon, 5);
                }
            }

            if (m_pdc) {
                DrawSounding(*m_pdc, pixxc, pixyc, depth, sdgid, surid, 5);
            }
        }
    }
}

void SurveyOverlayFactory::DrawSounding(wxDC& dc, int x, int y, double depth,
    int sounding_id, int survey_id, int text_offset)
{
    double coef = 1.0;
    int iUnits = PI_GetPLIBDepthUnitInt();
    if (iUnits == FATHOMS)
        coef = 1.8288;
    else if (iUnits == FEET)
        coef = 0.3048;
    else {
    }

    wxColour s_Colour;
    s_Colour = m_settings.m_sSoundingColor;

    if (!m_settings.m_bUseSymbol) {
        // do nothing
    } else {

        if (m_settings.m_bUseDepthColours) {
            s_Colour = GetDepthColour(depth);
        }

        wxPen p(s_Colour, 2);
        dc.SetPen(p);

        switch (m_settings.m_iSoundingShape) {
        case 0:
            dc.DrawLine(x - 5, y - 5, x + 5, y + 5);
            dc.DrawLine(x - 5, y + 5, x + 5, y - 5);
            break;
        case 1:
            dc.DrawCircle(x, y, 5);
            dc.DrawCircle(x, y, 3);
            dc.DrawCircle(x, y, 1);
            break;
        case 2:
            dc.DrawRectangle(x - 4, y - 3, 8, 8);
            dc.DrawRectangle(x - 4, y - 3, 6, 6);
            dc.DrawRectangle(x - 4, y - 3, 4, 4);
            dc.DrawRectangle(x - 4, y - 3, 2, 2);
            break;
        }
    }

    dc.SetFont(wxFont(m_settings.m_sFont));

    wxColour text_color;
    text_color = m_settings.m_sFontColor;

    /*
    wxColour f_Colour;
    if (m_settings.m_bUseDepthColours) {
            f_Colour = GetDepthColour(depth);
            text_color = f_Colour;
    }
    */

    dc.SetTextForeground(text_color);

    // Want to show depth values
    if (m_settings.m_bRenderSoundingText) {
        dc.DrawText(
            wxString::Format(wxT("%.1f"), depth / coef), x, y + text_offset);
    }

    // Want connecting lines
    if (m_settings.m_bConnectSoundings) {
        if (m_settings.mLastX != -1 && m_settings.mLastY != -1
            && m_settings.mLastSdgId == sounding_id - 1
            && m_settings.mLastSurveyId == survey_id) {
            wxColour c_Colour;
            c_Colour = m_settings.m_sConnectorColor;
            wxPen pc(c_Colour, 1);
            dc.SetPen(pc);
            dc.DrawLine(x, y, m_settings.mLastX, m_settings.mLastY);
        }
        m_settings.mLastX = x;
        m_settings.mLastY = y;
        m_settings.mLastSdgId = sounding_id;
        m_settings.mLastSurveyId = survey_id;
    }
}

void SurveyOverlayFactory::DrawGLSoundingMark(int x, int y, double scale,
    double depth, int sounding_id, int survey_id, wxColour c)
{

    if (m_settings.m_bUseSymbol) {
        DrawGLLine(x - 5, y - 5, x + 5, y + 5, 1, c);
        DrawGLLine(x - 5, y + 5, x + 5, y - 5, 1, c);
    }
    wxColour colour;
    if (m_settings.m_bConnectSoundings) {

        if (m_settings.mLastX != -1 && m_settings.mLastY != -1
            && m_settings.mLastSdgId == sounding_id - 1
            && m_settings.mLastSurveyId == survey_id) {
            colour = wxColour(m_settings.m_sConnectorColor);
            DrawGLLine(
                x, y, m_settings.mLastX, m_settings.mLastY, 0.25, colour);
        }
        m_settings.mLastX = x;
        m_settings.mLastY = y;
        m_settings.mLastSdgId = sounding_id;
        m_settings.mLastSurveyId = survey_id;
    }
}
