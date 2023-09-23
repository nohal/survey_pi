///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 10 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __SURVEYGUI_H__
#define __SURVEYGUI_H__

#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/colour.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/dialog.h>
#include <wx/font.h>
#include <wx/fontpicker.h>
#include <wx/gbsizer.h>
#include <wx/gdicmn.h>
#include <wx/grid.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/listbox.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/statbmp.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/tglbtn.h>
#include <wx/timectrl.h>
#include <wx/xrc/xmlres.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class SurveyCfgDlgDef
///////////////////////////////////////////////////////////////////////////////
class SurveyCfgDlgDef : public wxDialog {
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
    // wxStaticText* m_staticText15;
    // wxStaticText* m_staticText16;
    // wxStaticText* m_staticText22;
    wxStdDialogButtonSizer* m_sdbSizer1;
    wxButton* m_sdbSizer1OK;
    wxButton* m_sdbSizer1Cancel;
    wxStaticText* m_manualCorrection;

public:
    // wxListCtrl* m_listCtrl1;

    wxSlider* m_sOpacity;
    wxRadioButton* m_rbMeters;
    wxRadioButton* m_rbFeet;
    wxRadioButton* m_rbFathoms;
    // wxCheckBox* m_cbCalcTide;
    wxTextCtrl* m_tCorrection;
    wxCheckBox* m_cbRenderOverlay;
    wxRadioButton* m_rbRendedAllSurveys;
    wxRadioButton* m_rbRenderActiveSurvey;
    wxCheckBox* m_cbConnectSoundings;
    wxCheckBox* m_cbRenderDepthValues;
    wxCheckBox* m_cbSdgsPlusCorrn;
    wxColourPickerCtrl* m_cpSoundingColor;
    wxColourPickerCtrl* m_cpConnectorColor;
    wxColourPickerCtrl* m_cpFontColor;
    wxFontPickerCtrl* m_fpFont;
    wxListBox* m_lSymbolList;
    wxCheckBox* m_cbSelectedSymbol;
    wxCheckBox* m_cbUseDepthColours;
    wxTextCtrl* m_tLOA;
    wxTextCtrl* m_tBeam;
    wxTextCtrl* m_tSounderBow;
    wxTextCtrl* m_tWaterlineOffset;
    wxTextCtrl* m_tGPSBow;
    wxTextCtrl* m_tGPSPort;
    wxTextCtrl* m_tMinDistance;
    // wxTextCtrl* m_tAutoNewDistance;

    SurveyCfgDlgDef(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& title = _("Survey preferences"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(658, 680),
        long style = wxDEFAULT_DIALOG_STYLE);
    ~SurveyCfgDlgDef();
};

///////////////////////////////////////////////////////////////////////////////
/// Class SurveyDlgDef
///////////////////////////////////////////////////////////////////////////////
class SurveyDlgDef : public wxDialog {
private:
protected:
    wxStaticText* m_tArea;

    // wxButton*m_btnAddTide;
    wxButton* m_btnLoadFromFile;
    wxBitmapToggleButton* m_btbRecord;
    wxToggleButton* m_tbRecordNMEA;
    // wxToggleButton* m_tbStopRecordN;

    wxStaticBitmap* m_profilegraph;
    wxStaticBitmap* m_profilegraph1;
    wxButton* m_btnNew;
    wxButton* m_btnDelete;
    wxButton* m_btnProperties;
    wxButton* m_btnZoomTo;
    wxButton* m_btnEdit;
    wxButton* m_btnMerge;
    wxButton* m_btnImport;
    wxButton* m_btnExport;
    wxButton* m_btnTide;
    wxStdDialogButtonSizer* m_sdbSizer2;
    wxButton* m_sdbSizer2OK;
    wxButton* m_sdbSizer2Cancel;
    wxButton* m_pButtonDelete;
    wxButton* m_pButtonAdd;
    wxListCtrl* m_listCtrl1;

    // Virtual event handlers, overide them in your derived class
    virtual void OnClose(wxCloseEvent& event) { event.Skip(); }
    virtual void OnSurveySelection(wxCommandEvent& event) { event.Skip(); }
    // virtual void OnAddTide(wxCommandEvent& event) { event.Skip(); }
    virtual void LoadFromFile(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSurveyRecordToggleNMEA(wxCommandEvent& event)
    {
        event.Skip();
    }
    virtual void RecordNMEA(wxCommandEvent& event) { event.Skip(); }
    virtual void OnNewSurvey(wxCommandEvent& event) { event.Skip(); }
    virtual void OnDeleteSurvey(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSurveyProperties(wxCommandEvent& event) { event.Skip(); }
    virtual void OnZoomTSurvey(wxCommandEvent& event) { event.Skip(); }
    virtual void OnEditSoundings(wxCommandEvent& event) { event.Skip(); }
    virtual void OnMergeSurvey(wxCommandEvent& event) { event.Skip(); }
    virtual void OnImportSurvey(wxCommandEvent& event) { event.Skip(); }
    virtual void OnExportSurvey(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSetTidalData(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSurveyCancelClick(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSurveyOkClick(wxCommandEvent& event) { event.Skip(); }
    virtual void IsPanelSelected(wxNotebookEvent& event) { event.Skip(); }
    virtual void OnItemAdd(wxCommandEvent& event) { event.Skip(); }
    virtual void OnItemDelete(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* m_tNrSoundings;
    wxStaticText* m_tMinDepth;
    wxStaticText* m_tMaxDepth;

    wxChoice* m_chSurvey;
    wxNotebook* m_notebook1;
    wxGrid* m_gdSoundings;

    wxPanel* m_panel1;
    wxPanel* m_panel2;
    wxPanel* m_panel3;
    wxChoice* num_soundings;
    SurveyDlgDef(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& title = _("Survey"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(-1, -1),
        long style = wxDEFAULT_DIALOG_STYLE | wxMINIMIZE_BOX | wxRESIZE_BORDER);
    ~SurveyDlgDef();
};

///////////////////////////////////////////////////////////////////////////////
/// Class SurveyMergeDlgDef
///////////////////////////////////////////////////////////////////////////////
class SurveyMergeDlgDef : public wxDialog {
private:
protected:
    wxStdDialogButtonSizer* m_sdbSizer3;
    wxButton* m_sdbSizer3OK;
    wxButton* m_sdbSizer3Cancel;

    // Virtual event handlers, overide them in your derived class
    virtual void OnMergeCancelClick(wxCommandEvent& event) { event.Skip(); }
    virtual void OnMergeOkClick(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* m_staticText28;
    wxChoice* m_cMergeWith;
    SurveyMergeDlgDef(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& title = _("Merge surveys"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(-1, -1),
        long style = wxDEFAULT_DIALOG_STYLE);
    ~SurveyMergeDlgDef();
};

///////////////////////////////////////////////////////////////////////////////
/// Class SoundingsEditDlgDef
///////////////////////////////////////////////////////////////////////////////
class SoundingsEditDlgDef : public wxDialog {
private:
protected:
    wxStdDialogButtonSizer* m_sdbSizer3;
    wxButton* m_sdbSizer3OK;
    wxButton* m_sdbSizer3Cancel;

    // Virtual event handlers, overide them in your derived class
    virtual void OnEditCancelClick(wxCommandEvent& event) { event.Skip(); }
    virtual void OnEditOkClick(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* m_staticText28;
    wxStaticText* m_cMergeWith;
    SoundingsEditDlgDef(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& title = _("Edit soundings"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(-1, -1),
        long style = wxDEFAULT_DIALOG_STYLE);
    ~SoundingsEditDlgDef();
};
///////////////////////////////////////////////////////////////////////////////
/// Class SurveyPropDlgDef
///////////////////////////////////////////////////////////////////////////////
class SurveyPropDlgDef : public wxDialog {
private:
protected:
    wxStdDialogButtonSizer* m_sdbSizer3;
    wxButton* m_sdbSizer3OK;

    // Virtual event handlers, overide them in your derived class
    virtual void OnPropOkClick(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* m_staticText28;
    wxStaticText* m_staticText29;
    wxStaticText* m_staticText30;
    wxStaticText* m_staticText31;
    SurveyPropDlgDef(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& title = _("Survey Properties"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(525, 286),
        long style = wxDEFAULT_DIALOG_STYLE);
    ~SurveyPropDlgDef();
};

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/colour.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/dialog.h>
#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/grid.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/timectrl.h>
#include <wx/xrc/xmlres.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class SurveyTidalDlgDef
///////////////////////////////////////////////////////////////////////////////
class SurveyTidalDlgDef : public wxDialog {
private:
protected:
    wxButton* m_bAutoFill;
    wxStaticText* m_staticText42;
    wxChoice* m_choice5;
    wxStaticText* m_staticText43;
    wxStaticText* m_staticText39;
    wxTimePickerCtrl* m_timePicker2;
    wxStaticText* m_staticText40;
    wxDatePickerCtrl* m_datePicker1;
    wxStaticText* m_staticText38;
    wxStaticText* m_staticText391;
    wxTimePickerCtrl* m_timePicker21;
    wxStaticText* m_staticText401;
    wxDatePickerCtrl* m_datePicker11;
    wxStaticText* m_staticText411;
    wxButton* m_btnAddTidalHeight1;
    wxNotebook* m_notebook1;
    wxPanel* m_panel1;
    wxPanel* m_panel2;
    wxStaticBitmap* m_profileTide;
    wxButton* m_btnSave;
    wxButton* m_btnCancelTidalHeights;
    wxButton* m_btnImportTide;
    wxButton* m_btnExportTide;

    // Virtual event handlers, overide them in your derived class
    virtual void OnEditSurveySelection(wxCommandEvent& event) { event.Skip(); }
    virtual void AutoFill(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSelectAll(wxCommandEvent& event) { event.Skip(); }
    virtual void SetCorrection(wxCommandEvent& event) { event.Skip(); }
    virtual void SetTidalHeight(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSaveTides(wxCommandEvent& event) { event.Skip(); }
    virtual void OnCancelTides(wxCommandEvent& event) { event.Skip(); }
    virtual void OnImportTides(wxCommandEvent& event) { event.Skip(); }
    virtual void OnExportTides(wxCommandEvent& event) { event.Skip(); }

public:
    wxButton* m_btnSelectAll;
    wxChoice* m_chTidalSurvey;
    wxTextCtrl* m_tcCorrection;
    wxButton* m_btnAddCorrection;
    wxTextCtrl* m_tcHeightOfTide1;
    wxGrid* m_gdTidalHeights;

    SurveyTidalDlgDef(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& title = _("Survey"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(700, 550),
        long style = wxDEFAULT_DIALOG_STYLE | wxMINIMIZE_BOX | wxRESIZE_BORDER);
    ~SurveyTidalDlgDef();
};

#endif //__SURVEYGUI_H__
