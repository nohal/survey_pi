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

#include "surveygui.h"
#include "ocpn_plugin.h"
#include "ProfileWin.h"

#include <wx/filedlg.h>
#include <wx/textfile.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/numdlg.h>

#include "baro_history.h"
#include "survey_profile.h"
#include "instrument.h"

using namespace std;

class survey_pi;

class SurveyOverlayFactory;
class PlugIn_ViewPort;

#include <vector>

struct SurveyOverlaySettings
{
	int               m_iOpacity;
	int               m_iUnits;
	bool              m_bCalcTide;
	wxString          m_sCorrection;
	bool              m_bRenderOverlay;
	bool              m_bRenderAllSurveys;
	bool              m_bConnectSoundings;
	bool              m_bRenderSoundingText;
	bool              m_bRenderWithTide;
	int               m_iSoundingShape;
	bool              m_bUseSymbol;
	bool              m_bUseDepthColours;
	wxString          m_sSoundingColor;
	wxString          m_sConnectorColor;
	wxString          m_sFont;
	wxString          m_sFontColor;

	bool              m_activesurvey;

	double            m_fLOA;
	double            m_fBeam;
	double            m_fSounderBow;
	double            m_fWaterlineOffset;
	double            m_fGPSBow;
	double            m_fGPSPort;
	double            m_fMinDistance;
	double            m_fAutoNewDistance;

	int               mLastX, mLastY;
	long              mLastSdgId, mLastSurveyId;
}
;
class soundingdata{
public:

	wxString lat, lon, ele, time, magvar, geoidheight, name, cmt, desc,
		src, link, sym, type, fix, sat, hdop, vdop, pdop,
		ageofdgpsdata, dgpsid, xmltag, speed, depth, temp, ZDA, tide, cog;
	int sdgid, surid;
	double latD, lonD;
private:
};

class SurveyCfgDlg : public SurveyCfgDlgDef
{
public:
      SurveyCfgDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Survey preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 496,587 ), long style = wxDEFAULT_DIALOG_STYLE );
};

class SurveyDlg : public SurveyDlgDef
{
public:
      SurveyDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Survey"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,550 ), long style = wxDEFAULT_DIALOG_STYLE );

	  void SetViewPort(PlugIn_ViewPort *vp);
	  void OnClose(wxCloseEvent& event);
	  PlugIn_ViewPort *vp;
	  void LoadSurvey_0();

	  void SetFactoryOptions(bool set_val = false);

	  void SetTrace();
	  void OnSurveySelection( wxCommandEvent& event );
      void LoadFromFile( wxCommandEvent& event );
	  void OnSurveyRecordToggleNMEA(wxCommandEvent& event);
	  void RecordNMEA(wxCommandEvent& event) ;

	void OnNewSurvey( wxCommandEvent& event );
	void OnDeleteSurvey( wxCommandEvent& event );
	void OnSurveyProperties( wxCommandEvent& event );
	void OnZoomTSurvey( wxCommandEvent& event );
	void OnMergeSurvey( wxCommandEvent& event );
	void OnImportSurvey( wxCommandEvent& event );
	void OnExportSurvey( wxCommandEvent& event );
	void OnSurveyCancelClick( wxCommandEvent& event );
	void OnSurveyOkClick( wxCommandEvent& event );
	void IsPanelSelected(wxNotebookEvent& event);
	void SetProfile();
	void OnItemAdd(wxCommandEvent& event);
	void OnItemDelete(wxCommandEvent& event);

	wxString getInstrumentCaption(unsigned int id);

	wxString myString;

    survey_pi *plugin;
	vector<soundingdata> mysoundings;

	DashboardInstrument *mySurveyTrace;
	ProfileWin* myProfile;
	SurveyOverlaySettings mySettings;

private:
	wxString          m_ifilename;
	wxString          m_ofilename;
	int               m_interval;
	wxTextFile        m_istream;
	wxFile            m_ostream;

	PlugIn_ViewPort  *m_vp;

};

#endif

