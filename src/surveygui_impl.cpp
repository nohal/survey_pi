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
      wxMessageBox(_("Sorry, this function is not yet implemented..."));
      event.Skip(); 
}

void SurveyDlg::OnSurveySelection( wxCommandEvent& event )
{
      plugin->SetActiveSurvey(m_chSurvey->GetStringSelection());
      //m_gdSoundings->SetTable(
      event.Skip(); 
}

void SurveyDlg::OnNewSurvey( wxCommandEvent& event )
{
      wxString answer = wxGetTextFromUser(_("Enter a name for the new survey:"));
      if (answer != wxEmptyString)
      {
            plugin->CreateSurvey(answer);
            plugin->SetActiveSurvey(answer);
            plugin->FillSurveyDropdown();
      }
      event.Skip(); 
}

void SurveyDlg::OnDeleteSurvey( wxCommandEvent& event )
{
      int answer = wxMessageBox(wxString::Format(_("Are you sure you want to permanently delete the survey %s?"), m_chSurvey->GetStringSelection().c_str()), _("Confirm"),
                            wxYES_NO | wxCANCEL, this);
      if (answer == wxYES)
      {
            plugin->DeleteSurvey(plugin->GetActiveSurveyId());
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
      wxFileDialog dlg(this, _("Select survey data file"), wxEmptyString, wxEmptyString, _T("HydroMagic files (*.gmp)|*.gmp|CSV files (*.csv)|*.csv"), wxFD_OPEN);
      dlg.ShowModal();
      if (dlg.GetPath() != wxEmptyString)
            plugin->ImportHydromagic(dlg.GetPath());
      event.Skip();
}

void SurveyDlg::OnExportSurvey( wxCommandEvent& event )
{
      wxFileDialog dlg(this, _("Export survey as"), wxEmptyString, wxEmptyString, _T("HydroMagic files (*.gmp)|*.gmp|CSV files (*.csv)|*.csv"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
      dlg.ShowModal();
      if (dlg.GetPath() != wxEmptyString)
            plugin->ExportHydromagic(plugin->GetActiveSurveyId(), dlg.GetPath());
      event.Skip(); 
}

void SurveyDlg::OnSurveyCancelClick( wxCommandEvent& event ) { event.Skip(); }

void SurveyDlg::OnSurveyOkClick( wxCommandEvent& event ) { event.Skip(); }