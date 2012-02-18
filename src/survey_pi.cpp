/******************************************************************************
 * $Id: survey_pi.cpp,v 1.0 2011/02/26 01:54:37 nohal Exp $
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "survey_pi.h"

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new survey_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//    Survey PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

survey_pi::survey_pi(void *ppimgr)
      :opencpn_plugin(ppimgr)
{
      // Create the PlugIn icons
      initialize_images();
}

int survey_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-survey_pi") );

      // Set some default private member parameters
      m_survey_dialog_x = 0;
      m_survey_dialog_y = 0;

      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn display canvas, to use as a parent for the POI Manager dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      //    And load the configuration items
      LoadConfig();

      //    This PlugIn needs a toolbar icon, so request its insertion
      m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_survey, _img_survey, wxITEM_NORMAL,
            _("Survey"), _T(""), NULL,
             SURVEY_TOOL_POSITION, 0, this);

      m_pSurveyDialog = NULL;

      return (WANTS_CURSOR_LATLON       |
              WANTS_TOOLBAR_CALLBACK    |
              INSTALLS_TOOLBAR_TOOL     |
              WANTS_PREFERENCES         |
              WANTS_NMEA_SENTENCES      |
              WANTS_OVERLAY_CALLBACK    |
              WANTS_ONPAINT_VIEWPORT    |
              WANTS_CONFIG
           );
}

bool survey_pi::DeInit(void)
{
      //    Record the dialog position
      if (NULL != m_pSurveyDialog)
      {
            wxPoint p = m_pSurveyDialog->GetPosition();
            SetSurveyDialogX(p.x);
            SetSurveyDialogY(p.y);

            m_pSurveyDialog->Close();
            delete m_pSurveyDialog;
            m_pSurveyDialog = NULL;
      }
      SaveConfig();
      return true;
}

int survey_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int survey_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int survey_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int survey_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *survey_pi::GetPlugInBitmap()
{
      return _img_survey_pi;
}

wxString survey_pi::GetCommonName()
{
      return _("Survey");
}


wxString survey_pi::GetShortDescription()
{
      return _("Survey PlugIn for OpenCPN");
}

wxString survey_pi::GetLongDescription()
{
      return _("Records depth from the NMEA stream\n\
And let's you do various other tricks.");
}


int survey_pi::GetToolbarToolCount(void)
{
      return 1;
}

void survey_pi::SetColorScheme(PI_ColorScheme cs)
{
      if (NULL == m_pSurveyDialog)
            return;

      //DimeDialog
}

void survey_pi::OnToolbarToolCallback(int id)
{
      if(NULL == m_pSurveyDialog)
      {
            m_pSurveyDialog = new SurveyCfgDlg(m_parent_window);
            m_pSurveyDialog->Move(wxPoint(m_survey_dialog_x, m_survey_dialog_y));
      }

      m_pSurveyDialog->Show(!m_pSurveyDialog->IsShown());
}

bool survey_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings/Survey" ) );
            pConf->Read ( _T( "ViewType" ),  &m_iViewType, 1 );
            pConf->Read ( _T( "ShowAtCursor" ),  &m_bShowAtCursor, 1 );
            pConf->Read ( _T( "Opacity" ),  &m_iOpacity, 255 );

            m_survey_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
            m_survey_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );

            if((m_survey_dialog_x < 0) || (m_survey_dialog_x > m_display_width))
                  m_survey_dialog_x = 5;
            if((m_survey_dialog_y < 0) || (m_survey_dialog_y > m_display_height))
                  m_survey_dialog_y = 5;
            pConf->SetPath ( _T ( "/Directories" ) );
            wxString def;
            def = ::wxGetCwd() + _T("/plugins");
            pConf->Read ( _T ( "SurveyDataLocation" ), &m_survey_dir, def);
            return true;
      }
      else
            return false;
}

bool survey_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T ( "/Settings/Survey" ) );
            pConf->Write ( _T ( "ViewType" ), m_iViewType );
            pConf->Write ( _T ( "ShowAtCursor" ), m_bShowAtCursor );
            pConf->Write ( _T ( "Opacity" ), m_iOpacity );

            pConf->Write ( _T ( "DialogPosX" ),   m_survey_dialog_x );
            pConf->Write ( _T ( "DialogPosY" ),   m_survey_dialog_y );
            pConf->SetPath ( _T ( "/Directories" ) );
            pConf->Write ( _T ( "SurveyDataLocation" ), m_survey_dir );

            return true;
      }
      else
            return false;
}

void survey_pi::ShowPreferencesDialog( wxWindow* parent )
{
      //SurveyCfgDlg *dialog = new SurveyCfgDlg( parent, wxID_ANY, _("Survey Preferences"), wxPoint( m_survey_dialog_x, m_survey_dialog_y), wxDefaultSize, wxDEFAULT_DIALOG_STYLE );
      //dialog->Fit();
      wxColour cl;
      GetGlobalColor(_T("DILG1"), &cl);
      //dialog->SetBackgroundColour(cl);

      //if(dialog->ShowModal() == wxID_OK)
      //{

            SaveConfig();
      //}
}

