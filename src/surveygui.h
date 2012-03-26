///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Mar 17 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __SURVEYGUI_H__
#define __SURVEYGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/radiobut.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>
#include <wx/fontpicker.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class SurveyCfgDlg
///////////////////////////////////////////////////////////////////////////////
class SurveyCfgDlg : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText17;
		wxStaticText* m_staticText221;
		wxStaticText* m_staticText18;
		wxStaticText* m_staticText19;
		wxStaticText* m_staticText23;
		wxStaticText* m_staticText21;
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText2;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText5;
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;
		wxStaticText* m_staticText9;
		wxStaticText* m_staticText10;
		wxStaticText* m_staticText11;
		wxStaticText* m_staticText12;
		wxStaticText* m_staticText13;
		wxStaticText* m_staticText14;
		wxStaticText* m_staticText15;
		wxStaticText* m_staticText16;
		wxStaticText* m_staticText22;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
	
	public:
		wxSlider* m_sOpacity;
		wxRadioButton* m_rbMeters;
		wxRadioButton* m_rbFeet;
		wxRadioButton* m_rbFathoms;
		wxCheckBox* m_cbRenderOverlay;
		wxRadioButton* m_rbRendedAllSurveys;
		wxRadioButton* m_rbRenderActiveSurvey;
		wxCheckBox* m_cbConnectSoundings;
		wxColourPickerCtrl* m_cpSoundingColor;
		wxColourPickerCtrl* m_cpConnectorColor;
		wxColourPickerCtrl* m_cpFontColor;
		wxFontPickerCtrl* m_fpFont;
		wxTextCtrl* m_tLOA;
		wxTextCtrl* m_tBeam;
		wxTextCtrl* m_tSounderBow;
		wxTextCtrl* m_tWaterlineOffset;
		wxTextCtrl* m_tGPSBow;
		wxTextCtrl* m_tGPSPort;
		wxTextCtrl* m_tMinDistance;
		wxTextCtrl* m_tAutoNewDistance;
		
		SurveyCfgDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Survey preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 496,587 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~SurveyCfgDlg();
	
};

#endif //__SURVEYGUI_H__
