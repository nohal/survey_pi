/******************************************************************************
 * $Id: survey_pi.h,v 1.0 2011/02/26 01:54:37 nohal Exp $
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

#ifndef _SURVEYPI_H_
#define _SURVEYPI_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include <wx/glcanvas.h>
#endif // precompiled headers

#include "circle.xpm"
#include "cross.xpm"
#include "ocpn_plugin.h"
#include "square.xpm"
#include "tinyxml.h"
#include "wx/progdlg.h"
#include <map>
#include <sstream>
#include <wx/dc.h>
#include <wx/dcmemory.h>
#include <wx/fileconf.h>
#include <wx/glcanvas.h>
#include <wx/hashmap.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/withimages.h>

#include "config.h"

// #include "SurveyOverlayFactory.h"

#define FEET 0
#define METERS 1
#define FATHOMS 2

// World Mercator
#define PROJECTION 3395

#include "SurveyOverlayFactory.h"
#include "ocpn_plugin.h"
#include "surveygui_impl.h"

#include "nmea0183.h"
#include <wx/textfile.h>
#include <wx/tokenzr.h>

#include <algorithm>

#include <sqlite3.h>
#include <wx/filefn.h>

#include <locale>
#include <vector>

using namespace std;

class GPXTimeAndZDA;
class soundingdata;
class SurveyDlg;
class SurveyTidalDlg;

class GPXTimeAndZDA {
public:
    wxString m_date, m_time, m_ZDA;
};

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define SURVEY_TOOL_POSITION -1 // Request default positioning of toolbar tool

class survey_pi : public opencpn_plugin_118 {
public:
    survey_pi(void* ppimgr);

    //    The required PlugIn Methods
    int Init(void);
    bool DeInit(void);

    int GetAPIVersionMajor();
    int GetAPIVersionMinor();
    int GetPlugInVersionMajor();
    int GetPlugInVersionMinor();
    wxBitmap* GetPlugInBitmap();
    wxString GetCommonName();
    wxString GetShortDescription();
    wxString GetLongDescription();

    //    The required override PlugIn Methods
    int GetToolbarToolCount(void);
    void ShowPreferencesDialog(wxWindow* parent);

    void OnToolbarToolCallback(int id);
    void SetSettings();

    //    Optional plugin overrides
    void SetColorScheme(PI_ColorScheme cs);
    void SetNMEASentence(wxString& sentence);

    //    The override PlugIn Methods
    bool RenderOverlay(wxDC& pmdc, PlugIn_ViewPort* vp);
    bool RenderGLOverlay(wxGLContext* pcontext, PlugIn_ViewPort* vp);
    // bool RenderGLSurveyOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
    bool GetSurveySoundings(int as);
    bool GetEditedSoundings(int as);

    //    Other public methods
    void SetSurveyDialogX(int x) { m_survey_dialog_x = x; };
    void SetSurveyDialogY(int x) { m_survey_dialog_y = x; }

    void OnSurveyDialogClose();
    wxString StandardPath();

    bool ImportHydromagic(wxString filename);
    bool ImportXYZ(wxString filename);
    bool ImportCSV(wxString filename);
    bool ImportGPX(wxString filename);
    void ExportHydromagic(int survey_id, wxString filename);
    void ExportXYZ(int survey_id, wxString filename);
    void ExportCSV(int survey_id, wxString filename);
    int CreateSurvey(wxString name);
    void DeleteSurvey(int id);
    void DeleteSounding(int id);
    void FillSurveyDropdown();
    void FillSurveyGrid();
    void FillEditSurveyGrid(int surv_id);
    void FillSurveyInfo();
    void FillEditSurveyInfo();

    int InsertSounding(double depth, double lat, double lon, double tide = 0.0,
        time_t timestamp = 0, int projection = PROJECTION);
    wxArrayString GetSurveyList();
    int GetSurveyId(wxString survey_name);

    wxString GetSurveyName(int survey_id);
    wxString GetSurveyMaxDepth(int survey_id);
    wxString GetSurveyMinDepth(int survey_id);
    wxString GetSurveyNumSoundings(int survey_id);
    wxString GetSurveyAreaSoundings(int survey_id);

    wxString GetEditSurveyMaxDepth(int survey_id);
    wxString GetEditSurveyMinDepth(int survey_id);
    wxString GetEditSurveyNumSoundings(int survey_id);
    wxString GetEditSurveyAreaSoundings(int survey_id);

    int GetActiveSurveyId() { return m_activesurvey; }
    void SetActiveSurveyId(int id) { m_activesurvey = id; }
    void SetActiveSurvey(wxString name)
    {
        m_activesurveyname = name;
        m_activesurvey = GetSurveyId(name);
    }
    vector<soundingdata> SetTable(int i);
    vector<soundingdata> SetEditTable(int i);

    wxArrayString SetSoundings(int i);
    void ParseNMEASentence(wxString sentence);
    void timeToZDA(wxString myGPXTime);
    GPXTimeAndZDA myTZDA;
    wxString DDMLatToDecimal(wxString myString);
    wxString DDMLonToDecimal(wxString myString);

    bool b_gotdepth;
    bool b_gotposn;
    soundingdata mydata;
    vector<soundingdata> all_soundingdata;
    int numsoundings;
    bool m_recording;
    bool m_survey_trace;

    SoundingsEditDlgDef* m_pSoundingsEdit;
    SurveyMergeDlgDef* m_pSurveyMerge;
    SurveyPropDlgDef* m_pSurveyProp;

    double viewscale;
    bool dbMergeSurveys(int survey1, int survey2);
    bool dbEditSoundings(int survey1);
    double dbGetHeightOfTide(wxDateTime depthTime, int surv);
    bool dbSaveTideHeights(int survey1);
    wxString dbGetBeginDate(int surv);
    wxString dbGetEndDate(int surv);
    double m_latprev;
    double m_lonprev;

    vector<soundingdata> mySurveySoundings;

    SurveyOverlayFactory* GetSurveyOverlayFactory()
    {
        return m_pSurveyOverlayFactory;
    }

    double GetCursorLon(void) { return m_cursor_lon; }
    double GetCursorLat(void) { return m_cursor_lat; }

    int GetSoundingID(wxString lat, wxString lon);

    void OnContextMenuItemCallback(int id);
    void SetCursorLatLon(double lat, double lon);

    bool m_bShowSurvey;
    wxBitmap m_panelBitmap;
    bool m_bSurveyShowIcon;
    SurveyDlg* m_pSurveyDialog;

    wxString m_activesurveyname;

    void dbGetTable(wxString sql, char*** results, int& n_rows, int& n_columns);
    void dbFreeResults(char** results);

private:
    int m_delete_menu_id;
    double m_cursor_lon;
    double m_cursor_lat;

    SurveyOverlayFactory* m_pSurveyOverlayFactory;

    // wxDC *m_pdc;
    // wxGraphicsContext *m_gdc;

    NMEA0183 m_NMEA0183; // Used to parse NMEA Sentences
    // survey_pi         *dlg;
    sqlite3* m_database;
    // sqlite3_stmt     *m_stmt;
    int ret;
    char* err_msg;
    bool b_dbUsable;

    wxFileConfig* m_pconfig;
    wxWindow* m_parent_window;
    bool LoadConfig(void);
    bool SaveConfig(void);

    void StoreSounding(double depth);
    double m_lat, m_lon, m_cog;

    wxDateTime m_lastPosReport;

    int m_trimcount;
    // bool PointInLLBox(PlugIn_ViewPort *vp, double x, double y);
    void DrawSounding(
        wxDC& dc, int x, int y, double depth, long sounding_id, long survey_id);

    int m_survey_dialog_x, m_survey_dialog_y;
    int m_display_width, m_display_height;
    bool m_bRenderOverlay;
    int m_iOpacity;
    int m_iUnits;

    bool m_bCalcTide;
    wxString m_sCorrection;
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
    double m_fLOA;
    double m_fBeam;
    double m_fSounderBow;
    double m_fWaterlineOffset;
    double m_fGPSBow;
    double m_fGPSPort;
    double m_fMinDistance;
    // double            m_fAutoNewDistance;

    int mLastX, mLastY;
    long mLastSdgId, mLastSurveyId;

    int m_leftclick_tool_id;

    int m_activesurvey;

    int m_projection;

    short mPriPosition, mPriDepth;
    // int               mLastX, mLastY;
    // long              mLastSdgId, mLastSurveyId;

    bool dbQuery(wxString sql);

    int dbGetIntNotNullValue(wxString sql);

    wxString dbGetStringValue(wxString sql);

    void ImportHydromagicTrack(TiXmlElement* track);

    std::map<double, wxImage> m_labelCache;
    std::map<wxString, wxImage> m_labelCacheText;
};

#endif
