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

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/fileconf.h>
#include <wx/hashmap.h>

#include "tinyxml.h"

#define     PLUGIN_VERSION_MAJOR    0
#define     PLUGIN_VERSION_MINOR    2

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    8

#define METERS  1
#define FEET    2
#define FATHOMS 3

//World Mercator
#define PROJECTION 3395

#include "../../../include/ocpn_plugin.h"
#include "nmea0183/nmea0183.h"

#include "surveygui_impl.h"
#include "libspatialite-amalgamation-3.0.1/headers/spatialite/sqlite3.h"
#include "libspatialite-amalgamation-3.0.1/headers/spatialite.h"

class SurveyDlg;

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define SURVEY_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class survey_pi : public opencpn_plugin_18
{
public:
      survey_pi(void *ppimgr);

//    The required PlugIn Methods
      int Init(void);
      bool DeInit(void);
      
      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();

//    The required override PlugIn Methods
      int GetToolbarToolCount(void);
      void ShowPreferencesDialog( wxWindow* parent );

      void OnToolbarToolCallback(int id);

//    Optional plugin overrides
      void SetColorScheme(PI_ColorScheme cs);
      void SetNMEASentence(wxString &sentence);

//    The override PlugIn Methods
      bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
      //bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);

//    Other public methods
      void SetSurveyDialogX    (int x){ m_survey_dialog_x = x;};
      void SetSurveyDialogY    (int x){ m_survey_dialog_y = x;}

      void OnSurveyDialogClose();

      bool              ImportHydromagic(wxString filename);
      void              ExportHydromagic(int survey_id, wxString filename);
      int               CreateSurvey(wxString name);
      void              DeleteSurvey(int id);
      void              FillSurveyDropdown();
      int               InsertSounding(double depth, double lat, double lon, double tide = 0.0, time_t timestamp = 0, int projection = PROJECTION);
      wxArrayString     GetSurveyList();
      int               GetSurveyId(wxString survey_name);
      wxString          GetSurveyName(int survey_id);

      int               GetActiveSurveyId(){ return m_activesurvey; }
      void              SetActiveSurveyId(int id){ m_activesurvey = id; }
      void              SetActiveSurvey(wxString name){ m_activesurveyname = name; m_activesurvey = GetSurveyId(name); }

private:
      NMEA0183          m_NMEA0183;                 // Used to parse NMEA Sentences

      sqlite3          *m_database;
      sqlite3_stmt     *m_stmt;
      int               ret;
      char             *err_msg;
      bool              b_dbUsable;

      wxFileConfig     *m_pconfig;
      wxWindow         *m_parent_window;
      bool              LoadConfig(void);
      bool              SaveConfig(void);

      void              StoreSounding(double depth);
      double            m_lat, m_lon;
      wxDateTime        m_lastPosReport;

      bool PointInLLBox(PlugIn_ViewPort *vp, double x, double y);
      void DrawSounding(wxDC &dc, int x, int y, double depth, long sounding_id, long survey_id);

      SurveyDlg        *m_pSurveyDialog;

      int               m_survey_dialog_x, m_survey_dialog_y;
      int               m_display_width, m_display_height;
      bool              m_bRenderOverlay;
      int               m_iOpacity;
      int               m_iUnits;
      bool              m_bRenderAllSurveys;
      bool              m_bConnectSoundings;
      wxString          m_sSoundingColor;
      wxString          m_sConnectorColor;
      wxString          m_sFont;
      wxString          m_sFontColor;
      double            m_fLOA;
      double            m_fBeam;
      double            m_fSounderBow;
      double            m_fWaterlineOffset;
      double            m_fGPSBow;
      double            m_fGPSPort;
      double            m_fMinDistance;
      double            m_fAutoNewDistance;

      int               m_leftclick_tool_id;

      int               m_activesurvey;
      wxString          m_activesurveyname;
      int               m_projection;

      short             mPriPosition, mPriDepth;
      int               mLastX, mLastY;
      long              mLastSdgId, mLastSurveyId;
      bool              dbQuery(wxString sql);
      void              dbGetTable(wxString sql, char ***results, int &n_rows, int &n_columns);
      void              dbFreeResults(char **results);
      int               dbGetIntNotNullValue(wxString sql);
      wxString          dbGetStringValue(wxString sql);
      void              ImportHydromagicTrack(TiXmlElement *track);
};

#endif
