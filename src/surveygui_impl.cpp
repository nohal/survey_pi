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

SurveyCfgDlg::SurveyCfgDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : SurveyCfgDlgDef( parent, id, title, pos, size, style )
{
}

SurveyDlg::SurveyDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : SurveyDlgDef( parent, id, title, pos, size, style )
{
}

void SurveyDlg::OnRecordToggle( wxCommandEvent& event )
{
	int t = m_chSurvey->GetSelection();
    
	if (t == -1){
		wxMessageBox(_T("No survey selected, please select a survey or create a new survey"));
		return;
	 }

	wxString s; 
	wxFileDialog dlg(this, _("Select survey data file"), wxEmptyString, wxEmptyString, _T("NMEA Files (*.txt)|*.txt|CSV files (*.csv)|*.csv"), wxFD_OPEN);
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
	plugin->numsoundings = 0;
	wxString str;
	wxTextFile        m_istream;
	m_istream.Open(s);
	
	//wxFileInputStream input();
	//wxTextInputStream text(input, wxT("\x09"), wxConvUTF8 );
	//plugin->timeToZDA(_T("2015-07-09T10:37:35.000Z"));
	str = m_istream.GetFirstLine();
	//wxMessageBox(str);
	//str = str + _T("\r\n");
	//wxMessageBox(str);
	// do something with the string
    plugin->ParseNMEASentence(str);

	while(!m_istream.Eof()){
		if (plugin->numsoundings == n){
          m_istream.Close();
		  return;
	   }
	   else {
		   str = m_istream.GetNextLine();
		   //str = str + _T("\r\n");
		   // wxMessageBox(str);
		   // do something with the string
		   plugin->ParseNMEASentence(str);
	   }	  
	}

	m_istream.Close();
	//wxMessageBox(plugin->posdepth[0], _T("in function record"));

	event.Skip(); 
}

void SurveyDlg::OnSurveySelection( wxCommandEvent& event )
{
      plugin->SetActiveSurvey(m_chSurvey->GetStringSelection());
	  int s = plugin->GetSurveyId(m_chSurvey->GetStringSelection());
	  wxArrayString myTable;

	  mysoundings = plugin->SetTable(s);

	  int i = 0;
	  for(std::vector<soundingdata>::iterator it = mysoundings.begin();  it != mysoundings.end(); it++)
	{           
		m_gdSoundings->SetCellValue(i,0, it->depth);
		m_gdSoundings->SetCellValue(i,1, it->time);
		//for (int col = 0;col<5; col++){
		//	wxString s = it->depth;
		//	m_gdSoundings->SetCellValue(i,0,s);
		//}
		m_gdSoundings->AppendRows(1);//_T("test");		
		i++;
	} 

	  //int c = mysoundings.Count();
	  //wxString mystring = wxString::Format(wxT("%i"),c);
	  // wxMessageBox(mystring);
	  //for (int i = 0; i<c;i++){
		       
	  //}
      event.Skip(); 
}

void SurveyDlg::OnNewSurvey( wxCommandEvent& event )
{
      wxString answer = wxGetTextFromUser(_("Enter a name for the new survey:"));
      if (answer != wxEmptyString)
      {
            plugin->CreateSurvey(answer);
            plugin->SetActiveSurvey(answer);
			m_gdSoundings->ClearGrid();  
            plugin->FillSurveyDropdown();
      }
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
      {     int c = plugin->GetSurveyId( m_chSurvey->GetStringSelection());
            plugin->DeleteSurvey(c); 
			m_gdSoundings->ClearGrid();  
            plugin->FillSurveyDropdown();
      }

      event.Skip(); 
}

void SurveyDlg::OnSurveyProperties( wxCommandEvent& event )
{
      wxMessageBox(_("Sorry, this function is not yet implemented..."));
      event.Skip(); 
}

void SurveyDlg::OnZoomToSurvey( wxCommandEvent& event )
{
      wxMessageBox(_("Sorry, this function is not yet implemented..."));
      event.Skip(); 
}

void SurveyDlg::OnMergeSurvey( wxCommandEvent& event )
{
      wxMessageBox(_("Sorry, this function is not yet implemented..."));
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
      if (dlg.GetPath() != wxEmptyString)
            plugin->ImportHydromagic(dlg.GetPath());
	  else wxMessageBox(_T("No file entered"));
      event.Skip();
	  m_gdSoundings->Refresh();
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

void SurveyDlg::IsProfileSelected( wxNotebookEvent& event ){

	//wxMessageBox(_T("Profile drawing"));
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
	
	int i = 0;
	
	for(std::vector<soundingdata>::iterator it = mysoundings.begin();  it != mysoundings.end(); it++){ 
		it->depth.ToDouble(&value);
		tcv[i] = value;
		i++;
	}
	
	double dMax = 0;
	double dMin = 1000;
	
	int c = i-1;
	
	for (i = 0; i<c; i++){
		if (tcv[i] > dMax){
			dMax = tcv[i];
		}
		if (tcv[i] < dMin){
			dMin = tcv[i];
		}
	}



	wxString tclist[12];
	tclist[0] = _T("MaxDepth");
	tclist[11] = _T("MinDepth");

	ProfileWin *myProfileWin = new ProfileWin(m_panel2,200,200, 1, _T("Survey"), 20, m_graphday, _T("Metres"), 
					tcv, c, dMax, dMin, tclist);
		
	myProfileWin->Show();
	event.Skip(); 
}

void SurveyDlg::ProfileShown( wxPaintEvent& event ){

	wxMessageBox(_T("Profile drawing"));
	
}