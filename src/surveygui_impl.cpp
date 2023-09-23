/******************************************************************************
 * $Id: surveygui_impl.cpp,v 1.0 2011/02/26 01:54:37 nohal Exp $
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

#include "surveygui_impl.h"
#include "NavFunc.h"
#include "icons.h"
#include "survey_pi.h"
// #include <vector>

#include <fstream>
#include <iostream> // std::cout, std::ios
#include <sstream> // std::ostringstream
#include <wx/buffer.h>
#include <wx/file.h>
#include <wx/vector.h>

#include "tinyxml.h"

class GPXWayPoint;

static const wxChar* FILETYPESIMPORT = _T("HydroMagic files(*.gmp) | *.gmp |"
                                          " XYZ files(*.xyz) | *.xyz |"
                                          "CSV files(*.csv) | *.csv |"
                                          "Garmin GPX Log files(*.gpx) | *.gpx"
                                          "All files|*.*");

SurveyCfgDlg::SurveyCfgDlg(wxWindow* parent, wxWindowID id,
    const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : SurveyCfgDlgDef(parent, id, title, pos, size, style)
{
}

SurveyDlg::SurveyDlg(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style)
    : SurveyDlgDef(parent, id, title, pos, size, style)
{
    soundingdata first;
    first.latD = 50;
    first.lonD = -4;
    mysoundings.push_back(first);
}

void SurveyDlg::SetViewPort(PlugIn_ViewPort* vp)
{
    if (m_vp == vp)
        return;

    m_vp = new PlugIn_ViewPort(*vp);
}

void SurveyDlg::OnClose(wxCloseEvent& event) { plugin->OnSurveyDialogClose(); }

void SurveyDlg::OnSurveyRecordToggleNMEA(wxCommandEvent& event)
{
    bool b = m_tbRecordNMEA->GetValue();
    bool t = m_btbRecord->GetValue();
    plugin->m_latprev = 0;

    if (b && !plugin->m_recording) {
        int t = m_chSurvey->GetSelection();

        if (t == -1) {
            wxMessageBox(_("No survey selected"));
            return;
        }
        m_btbRecord->SetValue(true);
        m_btbRecord->SetBitmap(*_img_survey_recording);
        plugin->m_recording = true;
        return;
    }

    if (b && plugin->m_recording) {
        m_btbRecord->SetValue(true);
        m_btbRecord->SetBitmap(*_img_survey_paused);
        plugin->m_recording = false;
        return;
    }

    if (b && !plugin->m_recording && t) {
        m_btbRecord->SetBitmap(*_img_survey_recording);
        plugin->m_recording = true;
        return;
    }

    if (!b) {
        m_btbRecord->SetValue(false);
        m_btbRecord->SetBitmap(*_img_survey);
        plugin->m_recording = false;
        return;
    }
}

void SurveyDlg::RecordNMEA(wxCommandEvent& event)
{

    bool tb = m_tbRecordNMEA->GetValue();
    plugin->m_latprev = 0;

    if (tb) {

        int t = m_chSurvey->GetSelection();

        if (t == -1) {
            wxMessageBox(_("No survey selected, please select a survey or "
                           "create a new survey"));
            m_tbRecordNMEA->SetValue(false);
            return;
        }

        m_btbRecord->SetValue(true);
        m_btbRecord->SetBitmap(*_img_survey_recording);
        plugin->m_recording = true;
    } else {
        m_btbRecord->SetBitmap(*_img_survey);
        m_btbRecord->SetValue(false);
        plugin->m_recording = false;
        plugin->FillSurveyGrid();
        plugin->FillSurveyInfo();
        // RequestRefresh(plugin->m_pSurveyDialog->m_parent);
    }

    /*
    int id;
    wxFileDialog fdlg(GetOCPNCanvasWindow(), _("Choose a file"), wxT(""),
    m_ofilename, wxT("*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT); if
    (fdlg.ShowModal() != wxID_OK)
    {
            //SetToolbarItemState(id, false);
            return;
    }
    m_ofilename.Clear();
    m_ofilename = fdlg.GetPath();

    //m_ostream.Open( m_ofilename, wxFile::write_append );
    m_ostream.Open(m_ofilename, wxFile::write);
    m_recording = true;
    */
}

void SurveyDlg::LoadFromFile(wxCommandEvent& event)
{
    int t = m_chSurvey->GetSelection();

    if (t == -1) {
        wxMessageBox(_("No survey selected, please select a survey or create a "
                       "new survey"));
        return;
    }

    plugin->m_latprev = 0;
    plugin->m_lonprev = 0;
    plugin->numsoundings = 0;

    plugin->mydata.lat = _T("999");
    plugin->mydata.lon = _T("999");
    // plugin->mydata.time = _T("999");

    wxString s;
    wxFileDialog dlg(this, _("Select survey data file"), wxEmptyString,
        wxEmptyString,
        _T("NMEA Files (*.txt)|*.txt|CSV files (*.csv)|*.csv|All Files ")
        _T("(*.*)|*.*"),
        wxFD_OPEN);
    dlg.ShowModal();
    if (dlg.GetPath() != wxEmptyString) {
        s = dlg.GetPath();
    } else {
        wxMessageBox(_("No file selected"));
        return;
    }

    wxString str;
    wxTextFile m_istream;
    wxString msg, newmsg;

    int lc = 0;
    int ct = 0;
    soundingCount = 0;

    m_istream.Open(s);
    lc = m_istream.GetLineCount();

    wxProgressDialog progressdialog(_("Record from NMEA text file"),
        _("Adding soundings"), lc, m_parent, wxPD_CAN_ABORT | wxPD_APP_MODAL);

    str = m_istream.GetFirstLine();
    plugin->ParseNMEASentence(str);

    while (!m_istream.Eof()) {

        if (!progressdialog.Update(ct)) {
            // Cancelled by user.
            break;
        }

        msg = wxString::Format("%i", soundingCount);
        newmsg = msg + "  " + _("soundings have been recorded");
        progressdialog.Update(ct, newmsg);
        ct++;

        str = m_istream.GetNextLine();
        plugin->ParseNMEASentence(str);
    }

    m_istream.Close();

    RefreshSurvey();
    event.Skip();
}

void SurveyDlg::LoadSurvey_0()
{
    int s = plugin->GetSurveyId(m_chSurvey->GetStringSelection());

    m_gdSoundings->ClearGrid();
    mysoundings.clear();
    mysoundings = plugin->SetTable(s);

    int i = 0;
    for (std::vector<soundingdata>::iterator it = mysoundings.begin();
         it != mysoundings.end(); it++) {
        m_gdSoundings->SetCellValue(i, 0, it->depth);
        m_gdSoundings->SetCellValue(i, 1, it->time);
        m_gdSoundings->SetCellValue(i, 2, it->lat);
        m_gdSoundings->SetCellValue(i, 3, it->lon);
        m_gdSoundings->AppendRows(1);
        i++;
    }
}

void SurveyDlg::SetTrace()
{
    mySurveyTrace = new DashboardInstrument_BaroHistory(
        this->m_panel2, wxID_ANY, getInstrumentCaption(1));
    plugin->m_survey_trace = true;
    wxSize sz;

    sz = m_panel2->GetSize();
    mySurveyTrace->SetSize(sz);
}

void SurveyDlg::OnSurveySelection(wxCommandEvent& event)
{

    RefreshSurvey();
    event.Skip();
}

void SurveyDlg::RefreshSurvey()
{
    wxString s = m_chSurvey->GetStringSelection();

    int t = plugin->GetSurveyId(s);
    plugin->SetActiveSurvey(s);

    wxString testEdit = s.Mid(0, 4);
    if (testEdit == _T("Edt.")) {
        plugin->GetEditedSoundings(t);
        plugin->FillEditSurveyInfo();
        plugin->FillEditSurveyGrid(t);
    } else {
        plugin->FillSurveyInfo();
        plugin->FillSurveyGrid();
    }

    RequestRefresh(m_parent);
}

void SurveyDlg::OnNewSurvey(wxCommandEvent& event)
{
    wxString answer = wxGetTextFromUser(_("Enter a name for the new survey:"));
    if (answer != wxEmptyString) {
        for (int i = 0; i < m_chSurvey->GetCount(); i++) {
            if (answer == m_chSurvey->GetString(i)) {
                wxMessageBox(
                    _("Survey name already in use, please use another name"));
                return;
            }
        }
        plugin->CreateSurvey(answer);
        plugin->SetActiveSurvey(answer);
        m_gdSoundings->ClearGrid();
        plugin->FillSurveyDropdown();
        plugin->FillSurveyGrid();
        plugin->FillSurveyInfo();
    }
    RequestRefresh(m_parent);
    event.Skip();
}

void SurveyDlg::OnDeleteSurvey(wxCommandEvent& event)
{
    int t = m_chSurvey->GetSelection();

    if (t == -1) {
        wxMessageBox(_(
            "No survey selected, please select the survey you want to delete"));
        return;
    }

    int answer = wxMessageBox(
        wxString::Format(
            _("Are you sure you want to permanently delete the survey %s?"),
            m_chSurvey->GetStringSelection().c_str()),
        _("Confirm"), wxYES_NO | wxCANCEL, this);
    if (answer == wxYES) {
        int c = m_chSurvey->GetSelection();
        wxString tn = m_chSurvey->GetString(c);
        int d = plugin->GetSurveyId(tn);
        plugin->DeleteSurvey(d);

        plugin->FillSurveyDropdown();
        plugin->FillSurveyGrid();
        plugin->FillSurveyInfo();
    }
    RequestRefresh(m_parent);
    event.Skip();
}

void SurveyDlg::OnSurveyProperties(wxCommandEvent& event)
{
    wxString s = m_chSurvey->GetStringSelection();
    if (s == wxEmptyString) {
        wxMessageBox(_("No survey selected, please select a survey or create a "
                       "new survey"));
        return;
    }

    int t = plugin->GetSurveyId(s);
    plugin->m_pSurveyProp = new SurveyPropDlgDef(
        this, wxID_ANY, _("Survey Properties"), { 100, 100 }, wxDefaultSize, 0);

    wxString numSoundings = plugin->GetSurveyNumSoundings(t);
    plugin->m_pSurveyProp->m_staticText28->SetLabelText(
        _("Number of soundings: ") + numSoundings);
    wxString maxDepth = plugin->GetSurveyMaxDepth(t);
    plugin->m_pSurveyProp->m_staticText29->SetLabelText(
        _("  Maximum depth: ") + maxDepth);
    wxString minDepth = plugin->GetSurveyMinDepth(t);
    plugin->m_pSurveyProp->m_staticText30->SetLabelText(
        _("  Minimum depth: ") + minDepth);

    plugin->m_pSurveyProp->Show();

    if (plugin->m_pSurveyProp->ShowModal() == wxID_OK) { }
    event.Skip();
}

void SurveyDlg::OnZoomTSurvey(wxCommandEvent& event)
{
    double mla, mlo;
    wxString la = m_gdSoundings->GetCellValue(0, 2);
    // wxMessageBox(la);

    wxString lo = m_gdSoundings->GetCellValue(0, 3);
    // wxMessageBox(lo);

    if (la.IsEmpty() || lo.IsEmpty()) {
        wxMessageBox(_("Survey does not contain any soundings"));
        return;
    }

    la.ToDouble(&mla);
    lo.ToDouble(&mlo);
    JumpToPosition(mla, mlo, plugin->viewscale);

    event.Skip();
}

void SurveyDlg::OnMergeSurvey(wxCommandEvent& event)
{
    wxString s = m_chSurvey->GetStringSelection();
    if (s == wxEmptyString) {
        wxMessageBox(_("No survey selected, please select a survey or create a "
                       "new survey"));
        return;
    }

    int t = plugin->GetSurveyId(s);
    plugin->m_pSurveyMerge = new SurveyMergeDlgDef(
        this, wxID_ANY, _("Merging Surveys"), { 100, 100 }, wxDefaultSize, 0);
    plugin->m_pSurveyMerge->m_staticText28->SetLabelText(
        _("Copy survey ") + s + _T(" soundings into survey:"));

    wxChoice* m_choice;
    m_choice = plugin->m_pSurveyMerge->m_cMergeWith;
    m_choice->Append(plugin->GetSurveyList());
    m_choice->SetSelection(0);
    plugin->m_pSurveyMerge->Show();

    if (plugin->m_pSurveyMerge->ShowModal() == wxID_OK) {
        int survey1, survey2;
        survey1 = t;
        s = m_choice->GetStringSelection();
        survey2 = plugin->GetSurveyId(s);
        plugin->dbMergeSurveys(survey1, survey2);
    }
    event.Skip();
}

void SurveyDlg::OnEditSoundings(wxCommandEvent& event)
{
    wxString s = m_chSurvey->GetStringSelection();
    if (s == wxEmptyString) {
        wxMessageBox(_("No survey selected, please select a survey"));
        return;
    }

    wxString testEdit = s.Mid(0, 4);
    if (testEdit == _T("Edt.")) {
        wxMessageBox(_("This survey has already been corrected\n\nFor amending "
                       "corrections select a survey with raw data"));
        return;
    }

    int t = plugin->GetSurveyId(s);
    plugin->m_pSoundingsEdit = new SoundingsEditDlgDef(
        this, wxID_ANY, _("Editing Soundings"), { 200, 200 }, wxDefaultSize, 0);
    plugin->m_pSoundingsEdit->m_staticText28->SetLabelText(_("\nUse survey ")
        + s
        + _("\n\nAdjust the soundings for tide and additional corrections"));

    int id = plugin->GetActiveSurveyId();
    wxString surveyName = plugin->GetSurveyName(id);

    // wxStaticText *m_choice;
    // m_choice = plugin->m_pSoundingsEdit->m_cMergeWith;
    // m_choice->SetLabel(surveyName);

    plugin->m_pSoundingsEdit->Fit();
    plugin->m_pSoundingsEdit->Show();

    if (plugin->m_pSoundingsEdit->ShowModal() == wxID_OK) {
        int survey1;
        survey1 = t;
        plugin->dbEditSoundings(survey1);
    }

    RefreshSurvey();
    event.Skip();
}

void SurveyDlg::OnImportSurvey(wxCommandEvent& event)
{
    int t = m_chSurvey->GetSelection();

    if (t == -1) {
        wxMessageBox(_(
            "No survey selected, please select or create/select a new survey"));
        return;
    }

    wxFileDialog dlg(this, _("Select survey data file"), wxEmptyString,
        wxEmptyString,
        _T("HydroMagic files (*.gmp)|*.gmp|XYZ files (*.xyz)|*.xyz;*XYZ|CSV ")
        _T("files (*.csv)|*.csv;*CSV|Garmin GPX Log files(*.gpx)|*.gpx"),
        wxFD_OPEN);
    if (dlg.ShowModal() == wxID_OK) {

        if (dlg.GetPath() != wxEmptyString) {

            wxString show = dlg.GetPath();
            // wxMessageBox(show);

            if (dlg.GetPath().Right(3) == _T("gmp")
                || dlg.GetPath().Right(3) == _T("GMP")) {
                plugin->ImportHydromagic(dlg.GetPath());
            } else {
                if (dlg.GetPath().Right(3) == _T("xyz")
                    || dlg.GetPath().Right(3) == _T("XYZ")) {
                    plugin->ImportXYZ(dlg.GetPath());
                } else {
                    if (dlg.GetPath().Right(3) == _T("csv")
                        || dlg.GetPath().Right(3) == _T("CSV")) {
                        plugin->ImportCSV(dlg.GetPath());
                    } else {
                        if (dlg.GetPath().Right(3) == _T("gpx")
                            || dlg.GetPath().Right(3) == _T("GPX")) {
                            plugin->ImportGPX(dlg.GetPath());
                        }
                    }
                }
            }

        } else
            wxMessageBox(_("No file entered"));

        RefreshSurvey();
        event.Skip();
    }
}

void SurveyDlg::OnExportSurvey(wxCommandEvent& event)
{
    wxFileDialog dlg(this, _("Export survey as"), wxEmptyString, wxEmptyString,
        _T("HydroMagic files (*.gmp)|*.gmp|XYZ files (*.xyz)|*.xyz|CSV files ")
        _T("(*.csv)|*.csv"),
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    dlg.ShowModal();
    if (dlg.GetPath() != wxEmptyString) {
        if (dlg.GetPath().Right(3) == _T("gmp")) {
            plugin->ExportHydromagic(
                plugin->GetActiveSurveyId(), dlg.GetPath());
        } else {
            if (dlg.GetPath().Right(3) == _T("xyz")) {
                plugin->ExportXYZ(plugin->GetActiveSurveyId(), dlg.GetPath());
            } else {
                if (dlg.GetPath().Right(3) == _T("csv")) {
                    plugin->ExportCSV(
                        plugin->GetActiveSurveyId(), dlg.GetPath());
                }
            }
        }
    } else
        wxMessageBox(_("No file entered"));
    event.Skip();
}

void SurveyDlg::OnSurveyCancelClick(wxCommandEvent& event)
{

    plugin->OnSurveyDialogClose();
    event.Skip();
}

void SurveyDlg::OnSurveyOkClick(wxCommandEvent& event)
{

    plugin->OnSurveyDialogClose();
    event.Skip();
}

void SurveyDlg::IsPanelSelected(wxNotebookEvent& event)
{
    int i = m_notebook1->GetSelection();
    // wxString st = wxString::Format(wxT("%i"), i);
    // wxMessageBox(st);
    if (i = 1) {
        if (!plugin->m_survey_trace) {
            SetTrace();
            mySurveyTrace->Show();
        }
    }
    if (i = 2) {
        SetProfile();
    }
    if (i = 0) {
        mySurveyTrace->Destroy();
        myProfile->Destroy();
        plugin->FillSurveyGrid();
        plugin->FillSurveyInfo();
    }
    event.Skip();
}

void SurveyDlg::SetProfile()
{

    mysoundings.clear();
    int t = m_chSurvey->GetSelection();

    if (t == -1) {
        wxMessageBox(_("No survey selected, please select a survey"));
        return;
    }

    plugin->SetActiveSurvey(m_chSurvey->GetStringSelection());
    int s = plugin->GetSurveyId(m_chSurvey->GetStringSelection());

    mysoundings = plugin->SetTable(s);

    double tcv[3000];
    double tcd[3000];
    /*
            if (mysoundings.size() < 3) {
                    wxMessageBox(_("No profile available.\n\nAt least 3 sounding
       records in the table are needed.")); myProfile = new ProfileWin(m_panel3,
       wxID_ANY, 0, 0, 2, tcv, tcd, 10, 0); // blank profile return;
            }
    */
    wxDateTime m_graphday = wxDateTime::Now();
    double value;

    int i = 0;
    // wxArrayInt myRows[1];
    // myRows[0] = m_gdSoundings->GetSelectedRows();
    // int r1 = myRows[0][0];
    // int r2 = myRows[0][1];

    double m_lat, m_latp = 50.462939;
    double m_lon, m_lonp = -4.211521;
    double brg, dist;

    m_lat = 50.462948;
    m_lon = -4.211521;

    double tdist = 0;

    for (std::vector<soundingdata>::iterator it = mysoundings.begin();
         it != mysoundings.end(); it++) {

        if (i > 2999) {

            wxMessageBox(
                _("Unable to create a profile if more than 3000 soundings"));

            tcv[0] = 0;
            tcd[0] = 0;
            // Make a blank profile
            myProfile
                = new ProfileWin(m_panel3, wxID_ANY, 0, 0, 2, tcv, tcd, 10, 0);
            return;
        }

        it->depth.ToDouble(&value);
        tcv[i] = value;
        // Work out distance from first sounding
        m_lat = it->latD;
        m_lon = it->lonD;

        if (i == 0) {
            m_latp = m_lat;
            m_lonp = m_lon;
        }

        DistanceBearingMercator_Plugin(
            m_latp, m_lonp, m_lat, m_lon, &brg, &dist);

        dist = dist * 1852;
        wxString myDist = wxString::Format(_T("%8.1f"), (double)dist);
        tdist = tdist + dist;
        tcd[i] = tdist;
        i++;
    }

    double dMax = 0;
    double dMin = 1000;

    int c = i - 1;

    if (c == -1 | c < 3) { // We have no soundings or less than 3
        tcv[0] = 0;
        tcd[0] = 0;
        // Make a blank profile
        myProfile
            = new ProfileWin(m_panel3, wxID_ANY, 0, 0, 2, tcv, tcd, 10, 0);
        return;
    }
    for (i = 0; i < c; i++) {
        if (tcv[i] > dMax) {
            dMax = tcv[i];
        }
        if (tcv[i] < dMin) {
            dMin = tcv[i];
        }
    }

    // wxString tclist[12];
    // tclist[0] = _T("MaxDepth");
    // tclist[11] = _T("MinDepth");

    myProfile
        = new ProfileWin(m_panel3, wxID_ANY, 0, 0, c, tcv, tcd, dMax, dMin);
}

void SurveyDlg::OnItemAdd(wxCommandEvent& event)
{

    wxMessageBox(_("Not yet implemented"));
}

void SurveyDlg::OnItemDelete(wxCommandEvent& event)
{

    wxArrayInt myRows;
    myRows = m_gdSoundings->GetSelectedRows();

    int c = myRows.Count();
    if (c == 0) {
        wxMessageBox(_("No sounding/s selected"));
        return;
    }

    int i = 0;
    int d = 0;
    int sid = 0;
    wxString sel;
    // wxString sel = wxString::Format(wxT("%i"), myRows[i]);
    // wxMessageBox(sel);
    for (i = 0; i < c; i++) {
        sel = m_gdSoundings->GetCellValue(myRows[i], 4);
        d = wxAtoi(sel);
        plugin->DeleteSounding(d);
    }

    m_gdSoundings->DeleteRows(myRows[0], c, true);
    RequestRefresh(m_parent);
    this->Fit();
    this->Refresh();
}

wxString SurveyDlg::getInstrumentCaption(unsigned int id)
{
    switch (id) {
    case 1:
        return _(
            "Profile........................................................");
    }
    return _T("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP")
           _T("PPPPPPPPPPP");
}

void SurveyDlg::OnSetTidalData(wxCommandEvent& event)
{

    // wxMessageBox(_("Sorry ... Not yet implemented"));
    // return;
    // if (NULL == m_pSurveyTidalDialog) {
    // FunDlg *m_pFunctiondialog = new FunDlg(this);
    m_pSurveyTidalDialog = new SurveyTidalDlg(this);

    m_pSurveyTidalDialog->m_chTidalSurvey->Append(plugin->GetSurveyList());
    m_pSurveyTidalDialog->m_gdTidalHeights->SetColLabelValue(0, "Date/Time");
    m_pSurveyTidalDialog->m_gdTidalHeights->SetColLabelValue(1, "Tide");
    m_pSurveyTidalDialog->m_gdTidalHeights->SetColLabelValue(2, "Corrn");
    m_pSurveyTidalDialog->Plugin_Dialog = this;

    //}

    m_pSurveyTidalDialog->Show();
    m_pSurveyTidalDialog->Fit();
    m_pSurveyTidalDialog->Refresh();
}

// void SurveyDlg::OnAddTide(wxCommandEvent& event) {
// OnTestViewport();
// OnOpenGarminGPX();
//}

void SurveyDlg::OnTestViewport()
{

    // AdjustVP(PlugIn_ViewPort &vp_last, PlugIn_ViewPort &vp_proposed);
    PlugIn_ViewPort myViewport;
    // Dlg->GetParent()->SetCurrentViewPort(myViewport);
}

void SurveyDlg::OnOpenGarminGPX()
{

    trkpts.clear();
    rtepts.clear();

    int count;
    TiXmlElement* e;
    TiXmlElement* f;
    TiXmlElement* g;
    TiXmlElement* h;
    TiXmlElement* j;
    TiXmlElement* k;

    TiXmlAttribute* t;

    wxString new_file
        = ::wxFileSelector(_("Select Garmin GPX file"), m_currentGPXFile);
    if (!new_file.empty()) {
        m_currentGPXFile = new_file;
        // wxMessageBox(m_currentGPXFile);
    }

    std::ostringstream ostrm;
    std::ifstream inFile(m_currentGPXFile.mb_str());
    ostrm << inFile.rdbuf();

    TiXmlDocument doc;
    doc.Parse(ostrm.str().c_str());

    inFile.close();

    TiXmlElement* pRoot = doc.RootElement();

    for (TiXmlElement* e = pRoot->FirstChildElement(); e;
         e = e->NextSiblingElement())
        count++;
    int i = 0;
    for (e = pRoot->FirstChildElement(); e; e = e->NextSiblingElement(), i++) {
        wxString s = wxString(e->Value(), wxConvUTF8);
        // wxMessageBox(s);

        for (f = e->FirstChildElement(); f; f = f->NextSiblingElement()) {
            wxString s1 = wxString(f->Value(), wxConvUTF8);
            // wxMessageBox(s1);

            for (g = f->FirstChildElement(); g; g = g->NextSiblingElement()) {
                wxString s2 = wxString(g->Value(), wxConvUTF8);
                // wxMessageBox(s2);

                // s2 = wxString(t->Value(), wxConvUTF8);
                if (s2 == _T("trkpt")) {
                    const char* c = g->Attribute("lat");
                    if (c != NULL) {
                        s2 = wxString(c, wxConvUTF8);
                        trkpt.lat = s2;
                    }
                    // wxMessageBox(s2);
                    const char* c1 = g->Attribute("lon");
                    if (c1 != NULL) {
                        s2 = wxString(c1, wxConvUTF8);
                        trkpt.lon = s2;
                    }
                    // wxMessageBox(s2);
                }
                for (h = g->FirstChildElement(); h;
                     h = h->NextSiblingElement()) {
                    wxString s3 = wxString(h->Value(), wxConvUTF8);

                    // wxMessageBox(s3);

                    if (s3 == _T("time")) {
                        wxString tm = wxString(h->GetText(), wxConvUTF8);
                        trkpt.time = tm;
                    }

                    if (s3 == _T("extensions")) {

                        for (j = h->FirstChildElement(); j;
                             j = j->NextSiblingElement()) {
                            wxString s4 = wxString(j->Value(), wxConvUTF8);
                            // wxMessageBox(s4);

                            for (k = j->FirstChildElement(); k;
                                 k = k->NextSiblingElement()) {
                                wxString s5 = wxString(k->Value(), wxConvUTF8);
                                // wxMessageBox(s5, _T("temp"));
                                if (s5 == _T("gpxtpx:depth")
                                    || s5 == _T("gpxx:Depth")) {
                                    s5 = wxString(k->GetText(), wxConvUTF8);
                                    trkpt.depth = s5;
                                    trkpts.push_back(trkpt);
                                }
                                //
                            }
                        }
                    }
                }
            }
        }
    }

    int sz = trkpts.size();
    wxString siz = wxString::Format("%i", sz);
    // wxMessageBox(siz);

    WriteGPX(trkpts);
}

void SurveyDlg::WriteGPX(std::vector<GPXWayPoint> inRoute)
{

    wxString s;

    wxString new_file
        = ::wxFileSelector(_("Select Output GPX file"), m_currentGPXFile);
    if (!new_file.empty()) {
        m_currentGPXFile = new_file;
        // wxMessageBox(m_currentGPXFile);

        Refresh();
    }
    bool write_file = true;
    s = m_currentGPXFile;

    // Start GPX
    TiXmlDocument doc;

    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "");
    doc.LinkEndChild(decl);

    TiXmlElement* root = new TiXmlElement("gpx");
    TiXmlElement* Route = new TiXmlElement("rte");
    TiXmlElement* RouteName = new TiXmlElement("name");
    // TiXmlText * text4 = new TiXmlText( this->m_Route->GetValue().ToUTF8() );

    TiXmlText* text4 = new TiXmlText("myRoute");

    if (write_file) {
        doc.LinkEndChild(root);

        Route->LinkEndChild(RouteName);
        RouteName->LinkEndChild(text4);

        TiXmlElement* Extensions = new TiXmlElement("extensions");

        TiXmlElement* StartN = new TiXmlElement("opencpn:start");
        TiXmlText* text5 = new TiXmlText("Start");
        Extensions->LinkEndChild(StartN);
        StartN->LinkEndChild(text5);

        TiXmlElement* EndN = new TiXmlElement("opencpn:end");
        TiXmlText* text6 = new TiXmlText("End");
        Extensions->LinkEndChild(EndN);
        EndN->LinkEndChild(text6);

        Route->LinkEndChild(Extensions);

        for (std::vector<GPXWayPoint>::iterator it = inRoute.begin();
             it != inRoute.end(); it++) {

            Addpoint(Route, (*it).lat, (*it).lon,
                (*it).depth + " " + (*it).time, _T("xmgreen"), _T("WPT"));
            // wxMessageBox((*it).lat);
        }

        // my_points.clear()
    }

    if (write_file) {
        root->LinkEndChild(Route);
        // check if string ends with .gpx or .GPX
        // if (!wxFileExists(s)){
        // s = s + _T(".gpx");
        //}
        wxCharBuffer buffer = s.ToUTF8();
        doc.SaveFile(buffer.data());
    }
    // end of if no error occured
}

void SurveyDlg::Addpoint(TiXmlElement* Route, wxString ptlat, wxString ptlon,
    wxString ptname, wxString ptsym, wxString pttype)
{
    // add point

    // wxMessageBox("in here");
    TiXmlElement* RoutePoint = new TiXmlElement("rtept");
    RoutePoint->SetAttribute("lat", ptlat.mb_str());
    RoutePoint->SetAttribute("lon", ptlon.mb_str());

    TiXmlElement* Name = new TiXmlElement("name");
    TiXmlText* text = new TiXmlText(ptname.mb_str());
    RoutePoint->LinkEndChild(Name);
    Name->LinkEndChild(text);

    TiXmlElement* Symbol = new TiXmlElement("sym");
    TiXmlText* text1 = new TiXmlText(ptsym.mb_str());
    RoutePoint->LinkEndChild(Symbol);
    Symbol->LinkEndChild(text1);

    TiXmlElement* Type = new TiXmlElement("type");
    TiXmlText* text2 = new TiXmlText(pttype.mb_str());
    RoutePoint->LinkEndChild(Type);
    Type->LinkEndChild(text2);
    Route->LinkEndChild(RoutePoint);
    // done adding point
}

myOffset SurveyDlg::GetOffset(
    double lat, double lon, double offsetstbd, double offsetfwd, double hdg)
{

    myOffset newPos;
    double distmtr;
    distmtr = offsetstbd;

    double dist = distmtr / 1852;

    // destLoxodrome(lat, lon, hdg, dist, &newPos.finalLat, &newPos.finalLon);

    return newPos;
}

//
// Now comes the tidal correction work
//

SurveyTidalDlg::SurveyTidalDlg(wxWindow* parent, wxWindowID id,
    const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : SurveyTidalDlgDef(parent, id, title, pos, size, style)
{
}

void SurveyTidalDlg::OnEditSurveySelection(wxCommandEvent& event)
{

    wxString s = m_chTidalSurvey->GetStringSelection();

    int t = Plugin_Dialog->plugin->GetSurveyId(s);
    // Plugin_Dialog->plugin->SetActiveSurvey(s);

    wxString test1 = Plugin_Dialog->plugin->dbGetBeginDate(t);

    wxMessageBox(test1);

    wxString test2 = Plugin_Dialog->plugin->dbGetEndDate(t);

    wxMessageBox(test2);

    m_timePicker2->SetTime(1, 1, 1);

    wxString testEdit = s.Mid(0, 4);
    /*
    if (testEdit != _T("Edt.")) {

    }
    else {

    }
*/
    RequestRefresh(m_parent);
    event.Skip();
}

void SurveyTidalDlg::OnSaveTides(wxCommandEvent& event)
{
    int c = m_chTidalSurvey->GetSelection();
    if (c == -1) {
        wxMessageBox(_("No survey has been selected"));
        return;
    }
    wxString tn = m_chTidalSurvey->GetString(c);
    int d = Plugin_Dialog->plugin->GetSurveyId(tn);
    Plugin_Dialog->plugin->dbSaveTideHeights(d);
}

void SurveyTidalDlg::OnCancelTides(wxCommandEvent& event) { this->Close(); }

void SurveyTidalDlg::AutoFill(wxCommandEvent& event)
{
    wxDateTime dm = m_datePicker1->GetValue();

    int h, m, s;

    m_timePicker2->GetTime(&h, &m, &s);

    dm.Add(wxTimeSpan(h, m, s));

    wxDateTime m_dtNow = dm;

    // now for the end date/time

    wxDateTime dm2 = m_datePicker11->GetValue();
    int h2, m2, s2;
    m_timePicker21->GetTime(&h2, &m2, &s2);
    dm2.Add(wxTimeSpan(h2, m2, s2));

    SetGridDateTime(m_dtNow, dm2);

    m_gdTidalHeights->AutoSize();
}

void SurveyTidalDlg::OnSelectAll(wxCommandEvent& event)
{

    int rowCount = m_gdTidalHeights->GetNumberRows();

    m_gdTidalHeights->SetSelectionMode(wxGrid::wxGridSelectRows);
    // wxString myCount = wxString::Format("%i", rowCount);
    // wxMessageBox(myCount);

    for (int i = 0; i < rowCount; i++) {
        m_gdTidalHeights->SelectRow(i, true);
    }

    m_gdTidalHeights->AutoSize();
}

void SurveyTidalDlg::SetGridDateTime(wxDateTime myDT, wxDateTime myDT2)
{

    int s = m_choice5->GetSelection();
    wxString c = m_choice5->GetString(s);
    double value;
    c.ToDouble(&value);
    double m_dInterval = value;

    wxTimeSpan m_ts = wxTimeSpan::Minutes(m_dInterval);

    int i = 0;

    do {
        wxString str = myDT.Format(wxT("%Y-%m-%d %H:%M:%S"));

        m_gdTidalHeights->SetCellValue(i, 0, str);
        m_gdTidalHeights->AppendRows(1);
        myDT.Add(m_ts);
        i++;

    } while (myDT <= myDT2);
    m_gdTidalHeights->DeleteRows(i, 1, false);
}

void SurveyTidalDlg::SetTidalHeight(wxCommandEvent& event)
{

    wxArrayInt myRows;
    myRows = m_gdTidalHeights->GetSelectedRows();

    int c = myRows.Count();
    if (c == 0) {
        wxMessageBox(_("No row selected"));
        return;
    }

    int i = 0;
    int d = 0;
    int sid = 0;
    wxString sel, test_mt;
    // wxString sel = wxString::Format(wxT("%i"), myRows[i]);
    // wxMessageBox(sel);
    for (i = 0; i < c; i++) {
        test_mt = m_gdTidalHeights->GetCellValue(myRows[i], 0);
        if (test_mt.IsEmpty()) {
            wxMessageBox(_("No time entered\nUse AutoFill"));
            return;
        }

        sel = m_gdTidalHeights->GetCellValue(myRows[i], 1);

        d = wxAtoi(sel);
        wxString value;
        value = m_tcHeightOfTide1->GetValue();
        m_gdTidalHeights->SetCellValue(i, 1, value);
    }

    m_gdTidalHeights->AutoSize();
}

void SurveyTidalDlg::SetCorrection(wxCommandEvent& event)
{

    wxArrayInt myRows;
    myRows = m_gdTidalHeights->GetSelectedRows();

    int c = myRows.Count();
    if (c == 0) {
        wxMessageBox(_("No row selected"));
        return;
    }

    int i = 0;
    int d = 0;
    int sid = 0;
    wxString sel, test_mt;
    // wxString sel = wxString::Format(wxT("%i"), myRows[i]);
    // wxMessageBox(sel);
    for (i = 0; i < c; i++) {
        test_mt = m_gdTidalHeights->GetCellValue(myRows[i], 0);
        if (test_mt.IsEmpty()) {
            wxMessageBox(_("No time entered\nUse AutoFill"));
            return;
        }

        // sel = m_gdTidalHeights->GetCellValue(myRows[i], 1);
        // d = wxAtoi(sel);

        wxString value;
        value = m_tcCorrection->GetValue();
        m_gdTidalHeights->SetCellValue(i, 2, value);
    }

    m_gdTidalHeights->AutoSize();
}
