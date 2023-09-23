/******************************************************************************
 *
 * Project:  OpenCPN
 *
 ***************************************************************************
 *   Copyright (C) 2013 by David S. Register                               *
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

#ifndef __ProfileWin_H__
#define __ProfileWin_H__

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif // precompiled headers

#if !wxUSE_GRAPHICS_CONTEXT
#define wxGCDC wxDC
#endif

// #include <vector>
// #include "TideFindergui_impl.h"
#include <wx/datetime.h>
#include <wx/dialog.h>
#include <wx/list.h>
#include <wx/timer.h>
// #include "tcmgr.h"
#include "surveygui.h"
#include "wx/dcgraph.h"
#include "wx/event.h"
#include <assert.h>
#include <cstdio>
#include <stdio.h>

#define snprintf _snprintf
#define _sprintf _sprintf_s
#define _snprintf _snprintf_s

class wxCommandEvent;
class wxCloseEvent;
class wxTextCtrl;
class wxButton;
class wxListBox;

// class Harmonics;
// class TidalFactors;
// class SCV;

class Dlg;

using namespace std;

WX_DECLARE_LIST(wxPoint, SplineList); // for spline curve points

class ProfileWin : public wxControl {
public:
    ProfileWin(wxWindow* parent, wxWindowID id, int x, int y, int c_soundings,
        double tcv[3000], double tcd[3000], double tcmax, double tcmin);
    ~ProfileWin();

    // void OnSize(wxSizeEvent& event);
    void OnPaint(wxPaintEvent& event);
    void MouseEvent(wxMouseEvent& event);
    void OnCloseWindow(wxCloseEvent& event);

    void Resize(void);

    void RePosition(void);
    void Test();
    wxString myTCPortNo;
    double m_tcv[3000];
    double m_tcd[3000];
    int m_t_graphday_00_at_station;
    wxDateTime m_graphday;
    wxString m_tclist[16];
    int m_soundings;

private:
    double m_tcmax, m_tcmin;

    // TCMgr *ptcmgr;
    // IDX_entry *pIDX;
    wxColour c_black1;
    wxColour c_grey;
    wxColour c_grey2;
    wxColour c_blue;
    int m_passPort;
    wxString m_passName;
    int m_passStation;
    wxDateTime m_passGraphDay;
    wxString m_units;
    wxStaticText* m_ptextctrl;

    int curs_x;
    int curs_y;
    int m_plot_type;

    wxButton* OK_button;
    wxButton* NX_button;
    wxButton* PR_button;

    int im;
    int ib;
    int it;
    int val_off;
    wxRect m_graph_rect;

    wxListBox* m_tList;
    bool btc_valid;
    wxWindow* pParent;
    int m_corr_mins;
    wxString m_stz;
    int m_plot_y_offset;

    SplineList m_sList;

    wxFont* pSFont;
    wxFont* pSMFont;
    wxFont* pMFont;
    wxFont* pLFont;
    wxFont* pVLFont;

    wxPen* pblack_1;
    wxPen* pblack_2;
    wxPen* pblack_3;
    wxPen* pred_2;
    wxBrush* pltgray;
    wxBrush* pltgray2;

    DECLARE_EVENT_TABLE()
};

#endif
