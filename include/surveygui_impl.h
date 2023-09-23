/******************************************************************************
 * $Id: surveygui_impl.h,v 1.0 2011/02/26 01:54:37 nohal Exp $
 *
 * Project:  OpenCPN
 * Purpose:  Survey Plugin
 * Author:   Pavel Kalian
 *
 ***************************************************************************
 *   Copyright (C) 2012 by Pavel Kalian   *
 *   $EMAIL$   *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

#ifndef _SURVEYGUI_IMPL_H_
#define _SURVEYGUI_IMPL_H_

#include "ProfileWin.h"
#include "ocpn_plugin.h"
#include "surveygui.h"

#include <wx/filedlg.h>
#include <wx/numdlg.h>
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include "baro_history.h"
#include "instrument.h"
#include "survey_profile.h"

#include "tinyxml.h"

using namespace std;

class survey_pi;
class SurveyOverlayFactory;
class PlugIn_ViewPort;
class SurveyDlg;
class SurveyTidalDlg;

#include <vector>

class GPXWayPoint {
public:
    wxString lat, lon, ele, time, magvar, geoidheight, name, cmt, desc, src,
        link, sym, type, fix, sat, hdop, vdop, pdop, ageofdgpsdata, dgpsid,
        xmltag, speed, depth, temp, ZDA;

private:
};

struct myOffset {
    double finalLat;
    double finalLon;
};

class SurveyTidalDlg : public SurveyTidalDlgDef {
public:
    SurveyTidalDlg(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& title = _("Add Tidal Data"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(-1, -1),
        long style = wxCAPTION | wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX
            | wxRESIZE_BORDER);
    void OnEditSurveySelection(wxCommandEvent& event);
    void OnSaveTides(wxCommandEvent& event);
    void OnCancelTides(wxCommandEvent& event);
    void AutoFill(wxCommandEvent& event);
    void OnSelectAll(wxCommandEvent& event);
    void SetGridDateTime(wxDateTime myDT, wxDateTime myDT2);
    void SetTidalHeight(wxCommandEvent& event);
    void SetCorrection(wxCommandEvent& event);

    SurveyDlg* Plugin_Dialog;

private:
};

struct SurveyOverlaySettings {
    int m_iOpacity;
    int m_iUnits;
    bool m_bCalcTide;
    wxString m_sCorrection;
    bool m_bRenderOverlay;
    bool m_bRenderAllSurveys;
    bool m_bConnectSoundings;
    bool m_bRenderSoundingText;
    bool m_bRenderWithCorrn;
    int m_iSoundingShape;
    bool m_bUseSymbol;
    bool m_bUseDepthColours;
    wxString m_sSoundingColor;
    wxString m_sConnectorColor;
    wxString m_sFont;
    wxString m_sFontColor;

    bool m_activesurvey;

    double m_fLOA;
    double m_fBeam;
    double m_fSounderBow;
    double m_fWaterlineOffset;
    double m_fGPSBow;
    double m_fGPSPort;
    double m_fMinDistance;
    double m_fAutoNewDistance;

    int mLastX, mLastY;
    long mLastSdgId, mLastSurveyId;
};
class soundingdata {
public:
    wxString lat, lon, ele, time, magvar, geoidheight, name, cmt, desc, src,
        link, sym, type, fix, sat, hdop, vdop, pdop, ageofdgpsdata, dgpsid,
        xmltag, speed, depth, temp, ZDA, tide, cog;
    int sdgid, surid;
    double latD, lonD;

private:
};

class SurveyCfgDlg : public SurveyCfgDlgDef {
public:
    SurveyCfgDlg(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& title = _("Survey preferences"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(496, 587),
        long style = wxDEFAULT_DIALOG_STYLE);
};

class SurveyDlg : public SurveyDlgDef {
public:
    SurveyDlg(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& title = _("Survey"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(700, 550),
        long style = wxDEFAULT_DIALOG_STYLE | wxMINIMIZE_BOX | wxRESIZE_BORDER);

    void SetViewPort(PlugIn_ViewPort* vp);
    void OnClose(wxCloseEvent& event);
    PlugIn_ViewPort* vp;
    void LoadSurvey_0();

    void SetFactoryOptions(bool set_val = false);

    void SetTrace();
    void OnSurveySelection(wxCommandEvent& event);
    void RefreshSurvey();
    void LoadFromFile(wxCommandEvent& event);
    void OnSurveyRecordToggleNMEA(wxCommandEvent& event);
    void RecordNMEA(wxCommandEvent& event);

    void OnNewSurvey(wxCommandEvent& event);
    void OnDeleteSurvey(wxCommandEvent& event);
    void OnSurveyProperties(wxCommandEvent& event);
    void OnZoomTSurvey(wxCommandEvent& event);
    void OnMergeSurvey(wxCommandEvent& event);
    void OnEditSoundings(wxCommandEvent& event);
    void OnImportSurvey(wxCommandEvent& event);
    void OnExportSurvey(wxCommandEvent& event);
    void OnSurveyCancelClick(wxCommandEvent& event);
    void OnSurveyOkClick(wxCommandEvent& event);
    void IsPanelSelected(wxNotebookEvent& event);
    void SetProfile();
    void OnItemAdd(wxCommandEvent& event);
    void OnItemDelete(wxCommandEvent& event);
    void OnSetTidalData(wxCommandEvent& event);
    // void OnAddTide(wxCommandEvent& event);
    void OnTestViewport();
    void OnOpenGarminGPX();
    void WriteGPX(std::vector<GPXWayPoint> inRoute);
    void Addpoint(TiXmlElement* Route, wxString ptlat, wxString ptlon,
        wxString ptname, wxString ptsym, wxString pttype);

    myOffset GetOffset(double lat, double lon, double offsetstbd,
        double offsetfwd, double hdg);

    wxString getInstrumentCaption(unsigned int id);

    wxString myString;

    survey_pi* plugin;
    vector<soundingdata> mysoundings;

    DashboardInstrument* mySurveyTrace;
    ProfileWin* myProfile;
    SurveyOverlaySettings mySettings;

    SurveyTidalDlg* m_pSurveyTidalDialog;

    // GPXWayPoint trkpt;
    std::vector<GPXWayPoint> trkpts;
    std::vector<GPXWayPoint> rtepts;
    GPXWayPoint trkpt, rtept;
    wxString m_currentGPXFile;

    int soundingCount;

private:
    wxString m_ifilename;
    wxString m_ofilename;
    int m_interval;
    wxTextFile m_istream;
    wxFile m_ostream;

    PlugIn_ViewPort* m_vp;
};

#endif
