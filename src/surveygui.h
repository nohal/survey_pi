///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 10 2012)
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
#include <wx/choice.h>
#include <wx/gbsizer.h>
#include <wx/tglbtn.h>
#include <wx/grid.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/notebook.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class SurveyCfgDlgDef
///////////////////////////////////////////////////////////////////////////////
class SurveyCfgDlgDef : public wxDialog 
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
		
		SurveyCfgDlgDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Survey preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 496,587 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~SurveyCfgDlgDef();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SurveyDlgDef
///////////////////////////////////////////////////////////////////////////////
class SurveyDlgDef : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_tNrSoundings;
		wxStaticText* m_tArea;
		wxStaticText* m_tMinDepth;
		wxStaticText* m_tMaxDepth;
		wxToggleButton* m_tbRecord;
		wxNotebook* m_notebook1;
		wxPanel* m_panel1;
		wxGrid* m_gdSoundings;
		wxPanel* m_panel2;
		wxStaticBitmap* m_profilegraph;
		wxButton* m_btnNew;
		wxButton* m_btnDelete;
		wxButton* m_btnProperties;
		wxButton* m_btnZoomTo;
		wxButton* m_btnMerge;
		wxButton* m_btnImport;
		wxButton* m_btnExport;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnSurveySelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRecordToggle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNewSurvey( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteSurvey( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSurveyProperties( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnZoomToSurvey( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMergeSurvey( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImportSurvey( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportSurvey( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSurveyCancelClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSurveyOkClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxChoice* m_chSurvey;
		
		SurveyDlgDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Survey"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,550 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~SurveyDlgDef();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SurveyMergeDlgDef
///////////////////////////////////////////////////////////////////////////////
class SurveyMergeDlgDef : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText28;
		wxChoice* m_cMergeWith;
		wxStdDialogButtonSizer* m_sdbSizer3;
		wxButton* m_sdbSizer3OK;
		wxButton* m_sdbSizer3Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMergeCancelClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMergeOkClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SurveyMergeDlgDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Merge surveys"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 525,286 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~SurveyMergeDlgDef();
	
};

#endif //__SURVEYGUI_H__
