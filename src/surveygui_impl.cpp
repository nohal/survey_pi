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

SurveyCfgDlg::SurveyCfgDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : SurveyCfgDlgDef( parent, id, title, pos, size, style )
{
}

SurveyDlg::SurveyDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : SurveyDlgDef( parent, id, title, pos, size, style )
{
}

void SurveyDlg::OnSurveyRecordToggleNMEA(wxCommandEvent& event)
{
	bool b = m_tbRecordNMEA->GetValue();
	bool t = m_btbRecord->GetValue();
	plugin->m_latprev = 0;

	if (b && !plugin->m_recording){
		int t = m_chSurvey->GetSelection();

		if (t == -1){
			wxMessageBox(_T("No survey selected"));
			return;
		}
		m_btbRecord->SetValue(true);
		m_btbRecord->SetBitmap(*_img_survey_recording);
		plugin->m_recording = true;
		return;
	}
	
	if (b && plugin->m_recording){
		m_btbRecord->SetValue(true);
		m_btbRecord->SetBitmap(*_img_survey_paused);
		plugin->m_recording = false;
		return;
	}

	if (b && !plugin->m_recording  && t){	
		m_btbRecord->SetBitmap(*_img_survey_recording);
		plugin->m_recording = true;
		return;
	}

	if (!b){
		m_btbRecord->SetValue(false);
		m_btbRecord->SetBitmap(*_img_survey);
		plugin->m_recording = false;
		return;
	}
}

void SurveyDlg::RecordNMEA(wxCommandEvent& event){


	bool tb = m_tbRecordNMEA->GetValue();
	plugin->m_latprev = 0;

	if (tb){
		
		int t = m_chSurvey->GetSelection();

		if (t == -1){
			wxMessageBox(_T("No survey selected, please select a survey or create a new survey"));
			m_tbRecordNMEA->SetValue(false);
			return;
		}
        
		m_btbRecord->SetValue(true);
		m_btbRecord->SetBitmap(*_img_survey_recording);
		plugin->m_recording = true;
	}
	else {
		m_btbRecord->SetBitmap(*_img_survey);
		m_btbRecord->SetValue(false);
		plugin->m_recording = false;
		plugin->FillSurveyGrid();
		plugin->FillSurveyInfo();
		RequestRefresh(plugin->m_pSurveyDialog->m_parent);
	}
	
	/*
	int id;
	wxFileDialog fdlg(GetOCPNCanvasWindow(), _("Choose a file"), wxT(""), m_ofilename, wxT("*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (fdlg.ShowModal() != wxID_OK)
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

void SurveyDlg::LoadFromFile( wxCommandEvent& event )
{
	int t = m_chSurvey->GetSelection();
	
	if (t == -1){
		wxMessageBox(_T("No survey selected, please select a survey or create a new survey"));
		return;
	 }

	plugin->m_latprev = 0;
	plugin->m_lonprev = 0;
	plugin->numsoundings = 0;
	plugin->mydata.lat = _T("999");
	plugin->mydata.lon = _T("999");

	wxString s; 
	wxFileDialog dlg(this, _("Select survey data file"), wxEmptyString, wxEmptyString, _T("NMEA Files (*.txt)|*.txt|CSV files (*.csv)|*.csv|All Files (*.*)|*.*"), wxFD_OPEN);
    dlg.ShowModal();
    if (dlg.GetPath() != wxEmptyString){
		s = dlg.GetPath();
	}
	else{
		wxMessageBox(_T("No file selected"));
		return;
    }
	int n;
	n = wxGetNumberFromUser(_T(""), _T("Enter number of soundings"), _T("Number of soundings"), 1 , 1, 1000, this, wxDefaultPosition);
	if (n == -1){
		return; // User has cancelled
	}


	wxString		str;
	wxTextFile      m_istream;

	m_istream.Open(s);	
	str = m_istream.GetFirstLine();
    plugin->ParseNMEASentence(str);

	while(!m_istream.Eof()){
		if (plugin->numsoundings == n){
          m_istream.Close();
		  plugin->FillSurveyGrid();
		  plugin->FillSurveyInfo();
		  RequestRefresh(plugin->m_pSurveyDialog->m_parent);
	   }
	   else {
		   str = m_istream.GetNextLine();		  
		   plugin->ParseNMEASentence(str);
	   }	  
	}
	m_istream.Close();
	event.Skip(); 
}



void SurveyDlg::LoadSurvey_0()
{
		int s = plugin->GetSurveyId(m_chSurvey->GetStringSelection());

		m_gdSoundings->ClearGrid();
		mysoundings.clear();
		mysoundings = plugin->SetTable(s);

		int i = 0;
		for (std::vector<soundingdata>::iterator it = mysoundings.begin(); it != mysoundings.end(); it++)
		{
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
	mySurveyTrace = new DashboardInstrument_BaroHistory(this->m_panel2, wxID_ANY, getInstrumentCaption(1));
	plugin->m_survey_trace = true;
	wxSize sz;
	
	sz = m_panel2->GetSize();
	mySurveyTrace->SetSize(sz); 
}

void SurveyDlg::OnSurveySelection( wxCommandEvent& event )
{

	  wxString s = m_chSurvey->GetStringSelection();
	  
	  int t = plugin->GetSurveyId(s);
	  plugin->SetActiveSurvey(s);
	  plugin->FillSurveyInfo();
	  plugin->FillSurveyGrid();

	  
	  /*
	  wxString maxD, minD, numD, areaD;
	  maxD = _("Maximum depth: ") + plugin->GetSurveyMaxDepth(t) + _("m");
	  minD = _("Minimum depth: ") + plugin->GetSurveyMinDepth(t) + _("m");
	  numD = _("Soundings: ") + plugin->GetSurveyNumSoundings(t);
	  areaD = plugin->GetSurveyAreaSoundings(t);
 
	  //wxMessageBox(areaD);

	  plugin->m_pSurveyDialog->m_tMaxDepth->SetLabel(maxD);
	  plugin->m_pSurveyDialog->m_tMinDepth->SetLabel(minD);
	  plugin->m_pSurveyDialog->m_tNrSoundings->SetLabel(numD);
	  */
	  /*
	  mysoundings.clear();
	  m_gdSoundings->ClearGrid();
	  mysoundings = plugin->SetTable(t);

	  int i = 0;
	  for(std::vector<soundingdata>::iterator it = mysoundings.begin();  it != mysoundings.end(); it++)
	{           
		m_gdSoundings->SetCellValue(i,0, it->depth);
		m_gdSoundings->SetCellValue(i,1, it->time);
		m_gdSoundings->SetCellValue(i, 2, it->lat);
		m_gdSoundings->SetCellValue(i, 3, it->lon);

		//for (int col = 0;col<5; col++){
		//	wxString s = it->depth;
		//	m_gdSoundings->SetCellValue(i,0,s);
		//}
		m_gdSoundings->AppendRows(1);	
		i++;
	} 
	*/

	  RequestRefresh(plugin->m_pSurveyDialog->m_parent);
      event.Skip(); 
}

void SurveyDlg::OnNewSurvey( wxCommandEvent& event )
{
      wxString answer = wxGetTextFromUser(_("Enter a name for the new survey:"));
      if (answer != wxEmptyString)
      {
		  for (int i = 0; i < m_chSurvey->GetCount(); i++){
			  if (answer == m_chSurvey->GetString(i)){
				wxMessageBox(_("Survey name already in use, please use another name"));
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
	  RequestRefresh(plugin->m_pSurveyDialog->m_parent);
      event.Skip(); 
}

void SurveyDlg::OnDeleteSurvey( wxCommandEvent& event )
{
      int t = m_chSurvey->GetSelection();
    
	  if (t == -1){
		wxMessageBox(_T("No survey selected, please select the survey you want to delete"));
		return;
	  }

	  int answer = wxMessageBox(wxString::Format(_("Are you sure you want to permanently delete the survey %s?"), m_chSurvey->GetStringSelection().c_str()), _("Confirm"),
                            wxYES_NO | wxCANCEL, this);
      if (answer == wxYES)
      {     
		  int c = m_chSurvey->GetSelection();
		  wxString tn = m_chSurvey->GetString(c);
		  int d = plugin->GetSurveyId(tn);
		  plugin->DeleteSurvey(d); 

		  plugin->FillSurveyDropdown();
		  plugin->FillSurveyGrid();
		  plugin->FillSurveyInfo();
		  RequestRefresh(plugin->m_pSurveyDialog->m_parent);
      }
	 
      event.Skip(); 
}

void SurveyDlg::OnSurveyProperties( wxCommandEvent& event )
{
	wxString s = m_chSurvey->GetStringSelection();
	if (s == wxEmptyString){
		wxMessageBox(_T("No survey selected, please select a survey or create a new survey"));
		return;
	}

	int t = plugin->GetSurveyId(s);
	plugin->m_pSurveyProp = new SurveyPropDlgDef(this, wxID_ANY, _T("Survey Properties"), { 100, 100 }, wxDefaultSize, 0);


	plugin->m_pSurveyProp->Show();

	if (plugin->m_pSurveyProp->ShowModal() == wxID_OK)
	{
		
	}
      event.Skip(); 
}

void SurveyDlg::OnZoomToSurvey( wxCommandEvent& event )
{
	double mla, mlo;
	wxString la = m_gdSoundings->GetCellValue(0, 2);
	
	wxString lo = m_gdSoundings->GetCellValue(0, 3);
	if (la.IsEmpty()  || lo.IsEmpty()){
		wxMessageBox(_T("Survey does not contain any soundings"));
		return;
	}

	la.ToDouble(&mla);
	lo.ToDouble(&mlo);
	JumpToPosition(mla, mlo, plugin->viewscale);

    event.Skip(); 
}

void SurveyDlg::OnMergeSurvey( wxCommandEvent& event )
{
	wxString s = m_chSurvey->GetStringSelection();
	if (s == wxEmptyString){
		wxMessageBox(_T("No survey selected, please select a survey or create a new survey"));
		return;
	}

	int t = plugin->GetSurveyId(s);
	plugin->m_pSurveyMerge = new SurveyMergeDlgDef(this, wxID_ANY, _T("Merging Surveys"), { 100, 100 }, wxDefaultSize, 0);
	plugin->m_pSurveyMerge->m_staticText28->SetLabelText(_T("Copy survey ") + s + _T(" soundings into survey:"));
	
	wxChoice *m_choice;
	m_choice = plugin->m_pSurveyMerge->m_cMergeWith;
	m_choice->Append(plugin->GetSurveyList());
	m_choice->SetSelection(0);
	plugin->m_pSurveyMerge->Show();

	if (plugin->m_pSurveyMerge->ShowModal() == wxID_OK)
	{   
		int survey1, survey2;
		survey1 = t;
		s = m_choice->GetStringSelection();
		survey2 = plugin->GetSurveyId(s);
		plugin->dbMergeSurveys(survey1, survey2);
	}
    event.Skip(); 
}

void SurveyDlg::OnImportSurvey( wxCommandEvent& event ) 
{
	int t = m_chSurvey->GetSelection();
    
	if (t == -1){
		wxMessageBox(_T("No survey selected, please select or create/select a new survey"));
		return;
	}

	wxFileDialog dlg(this, _("Select survey data file"), wxEmptyString, wxEmptyString, _T("HydroMagic files (*.gmp)|*.gmp|CSV files (*.csv)|*.csv"), wxFD_OPEN);
      dlg.ShowModal();
	  if (dlg.GetPath() != wxEmptyString){
		  plugin->ImportHydromagic(dlg.GetPath());
		  plugin->FillSurveyGrid();
		  plugin->FillSurveyInfo();
		  RequestRefresh(plugin->m_pSurveyDialog->m_parent);
	  }
	  else wxMessageBox(_T("No file entered"));
      event.Skip();
}

void SurveyDlg::OnExportSurvey( wxCommandEvent& event )
{
      wxFileDialog dlg(this, _("Export survey as"), wxEmptyString, wxEmptyString, _T("HydroMagic files (*.gmp)|*.gmp|CSV files (*.csv)|*.csv"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
      dlg.ShowModal();
      if (dlg.GetPath() != wxEmptyString)
            plugin->ExportHydromagic(plugin->GetActiveSurveyId(), dlg.GetPath());
	  else wxMessageBox(_T("No file entered"));
      event.Skip(); 
}

void SurveyDlg::OnSurveyCancelClick( wxCommandEvent& event ) { event.Skip(); }

void SurveyDlg::OnSurveyOkClick( wxCommandEvent& event ) { event.Skip(); }

void SurveyDlg::IsPanelSelected(wxNotebookEvent& event){
	int i = m_notebook1->GetSelection();
	//wxString st = wxString::Format(wxT("%i"), i);
	//wxMessageBox(st);
	if (i = 1){	
		if (!plugin->m_survey_trace){
			SetTrace();
			mySurveyTrace->Show();
		}
	}
	if (i = 2){		
		SetProfile();
	}
	if (i = 0){
		mySurveyTrace->Destroy();
		myProfile->Destroy();
		plugin->FillSurveyGrid();
		plugin->FillSurveyInfo();
	}
	event.Skip();
}

void SurveyDlg::SetProfile(){

	mysoundings.clear();
	int t = m_chSurvey->GetSelection();
    
	if (t == -1){
		wxMessageBox(_T("No survey selected, please select a survey"));
		return;
	}

	plugin->SetActiveSurvey(m_chSurvey->GetStringSelection());
	int s = plugin->GetSurveyId(m_chSurvey->GetStringSelection());
	
	mysoundings = plugin->SetTable(s);

	wxDateTime m_graphday = wxDateTime::Now();
    double value;
	double tcv[3000];
	double tcd[3000];

	int i = 0;
	wxArrayInt myRows[1];
	myRows[0] = m_gdSoundings->GetSelectedRows();
	int r1 = myRows[0][0];
	int r2 = myRows[0][1];

	double m_lat, m_latp = 50.462939;
	double m_lon, m_lonp = -4.211521;
	double brg, dist;
	
	m_lat = 50.462948;
	m_lon = -4.211521;
	
	double tdist = 0;
	
	for(std::vector<soundingdata>::iterator it = mysoundings.begin();  it != mysoundings.end(); it++){ 
		it->depth.ToDouble(&value);
		tcv[i] = value;
		// Work out distance from first sounding
		m_lat = it->latD;
		m_lon = it->lonD;
		
		if (i == 0){
			m_latp = m_lat;
			m_lonp = m_lon;
		}

		DistanceBearingMercator_Plugin(m_latp, m_lonp, m_lat, m_lon, &brg, &dist);
		
		dist = dist * 1852;
		wxString myDist = wxString::Format(_T("%8.1f"), (double)dist);
		tdist = tdist + dist;
		tcd[i] = tdist;
		i++;
	}
		
	double dMax = 0;
	double dMin = 1000;
	
	int c = i-1;
	if (c == -1){ // We have no soundings
		tcv[0] = 0;
		tcd[0] = 0;
		// Make a blank profile
		myProfile = new ProfileWin(m_panel3, wxID_ANY, 0, 0, 2, tcv, tcd, 10, 0);
		return;
	}
	for (i = 0; i<c; i++){
		if (tcv[i] > dMax){
			dMax = tcv[i];
		}
		if (tcv[i] < dMin){
			dMin = tcv[i];
		}
	}

	//wxString tclist[12];
	//tclist[0] = _T("MaxDepth");
	//tclist[11] = _T("MinDepth");

	myProfile = new ProfileWin(m_panel3, wxID_ANY, 0, 0, c, tcv, tcd, dMax, dMin);

}


wxString SurveyDlg::getInstrumentCaption(unsigned int id)
{
	switch (id){
	case 1:
		return _("Profile........................................................");
	}
	return _T("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP");
}
