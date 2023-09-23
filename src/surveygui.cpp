///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 10 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "surveygui.h"
#include "icons.h"

///////////////////////////////////////////////////////////////////////////

SurveyCfgDlgDef::SurveyCfgDlgDef(wxWindow* parent, wxWindowID id,
    const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer1;
    bSizer1 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizer2;
    bSizer2 = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* sbSizer4;
    sbSizer4 = new wxStaticBoxSizer(
        new wxStaticBox(this, wxID_ANY, _("General settings")), wxVERTICAL);

    wxFlexGridSizer* fgSizer3;
    fgSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
    fgSizer3->SetFlexibleDirection(wxBOTH);
    fgSizer3->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    m_staticText17 = new wxStaticText(
        this, wxID_ANY, _("Opacity"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText17->Wrap(-1);
    fgSizer3->Add(m_staticText17, 0, wxALL, 5);

    m_sOpacity = new wxSlider(this, wxID_ANY, 50, 0, 255, wxDefaultPosition,
        wxDefaultSize, wxSL_HORIZONTAL);
    fgSizer3->Add(m_sOpacity, 0, wxALL | wxEXPAND, 5);

    sbSizer4->Add(fgSizer3, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer5;
    bSizer5 = new wxBoxSizer(wxHORIZONTAL);

    m_staticText221 = new wxStaticText(this, wxID_ANY, _("Display values as"),
        wxDefaultPosition, wxDefaultSize, 0);
    m_staticText221->Wrap(-1);
    bSizer5->Add(m_staticText221, 0, wxALL, 5);

    m_rbMeters = new wxRadioButton(this, wxID_ANY, _("Meters"),
        wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    m_rbMeters->SetValue(true);
    bSizer5->Add(m_rbMeters, 0, wxALL, 5);

    m_rbFeet = new wxRadioButton(
        this, wxID_ANY, _("Feet"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer5->Add(m_rbFeet, 0, wxALL, 5);

    m_rbFathoms = new wxRadioButton(
        this, wxID_ANY, _("Fathoms"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer5->Add(m_rbFathoms, 0, wxALL, 5);

    // m_cbCalcTide = new wxCheckBox(sbSizer4->GetStaticBox(), wxID_ANY,
    // _("Calculate tide"), wxDefaultPosition, wxDefaultSize, 0);
    // bSizer5->Add(m_cbCalcTide, 0, wxALL, 5);

    m_manualCorrection = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY,
        _("Correction \n"), wxDefaultPosition, wxDefaultSize, 0);
    m_manualCorrection->Wrap(-1);
    bSizer5->Add(m_manualCorrection, 0, wxALL, 5);

    m_tCorrection = new wxTextCtrl(sbSizer4->GetStaticBox(), wxID_ANY,
        _T("00.0"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer5->Add(m_tCorrection, 0, wxALL, 5);

    sbSizer4->Add(bSizer5, 1, wxEXPAND, 5);

    bSizer2->Add(sbSizer4, 0, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer* sbSizer3;
    sbSizer3 = new wxStaticBoxSizer(
        new wxStaticBox(this, wxID_ANY, _("Overlay settings")), wxVERTICAL);

    wxFlexGridSizer* fgSizer4;
    fgSizer4 = new wxFlexGridSizer(0, 3, 0, 0);
    fgSizer4->SetFlexibleDirection(wxBOTH);
    fgSizer4->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    wxBoxSizer* bSizer3;
    bSizer3 = new wxBoxSizer(wxVERTICAL);

    m_cbRenderOverlay = new wxCheckBox(sbSizer3->GetStaticBox(), wxID_ANY,
        _("Render overlay"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer3->Add(m_cbRenderOverlay, 0, wxALL, 5);

    m_rbRendedAllSurveys = new wxRadioButton(sbSizer3->GetStaticBox(), wxID_ANY,
        _("Render all data"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    m_rbRendedAllSurveys->SetValue(true);
    bSizer3->Add(m_rbRendedAllSurveys, 0, wxALL, 5);

    m_rbRenderActiveSurvey = new wxRadioButton(sbSizer3->GetStaticBox(),
        wxID_ANY, _("Render just active survey data"), wxDefaultPosition,
        wxDefaultSize, 0);
    bSizer3->Add(m_rbRenderActiveSurvey, 0, wxALL, 5);

    m_cbConnectSoundings = new wxCheckBox(sbSizer3->GetStaticBox(), wxID_ANY,
        _("Connect soundings"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer3->Add(m_cbConnectSoundings, 0, wxALL, 5);

    m_cbRenderDepthValues = new wxCheckBox(sbSizer3->GetStaticBox(), wxID_ANY,
        _("Render depth values"), wxDefaultPosition, wxDefaultSize, 0);
    m_cbRenderDepthValues->SetValue(true);
    bSizer3->Add(m_cbRenderDepthValues, 0, wxALL, 5);

    m_cbSdgsPlusCorrn = new wxCheckBox(sbSizer3->GetStaticBox(), wxID_ANY,
        _("Render sndgs plus correction"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer3->Add(m_cbSdgsPlusCorrn, 0, wxALL, 5);

    fgSizer4->Add(bSizer3, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer4;
    bSizer4 = new wxBoxSizer(wxVERTICAL);

    wxGridSizer* gSizer2;
    gSizer2 = new wxGridSizer(0, 2, 0, 0);

    m_staticText18 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY,
        _("Sounding color"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText18->Wrap(-1);
    gSizer2->Add(m_staticText18, 0, wxALL, 5);

    m_cpSoundingColor
        = new wxColourPickerCtrl(sbSizer3->GetStaticBox(), wxID_ANY, *wxBLACK,
            wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
    gSizer2->Add(m_cpSoundingColor, 0, wxALL, 5);

    m_staticText19 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY,
        _("Connector color"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText19->Wrap(-1);
    gSizer2->Add(m_staticText19, 0, wxALL, 5);

    m_cpConnectorColor
        = new wxColourPickerCtrl(sbSizer3->GetStaticBox(), wxID_ANY, *wxBLACK,
            wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
    gSizer2->Add(m_cpConnectorColor, 0, wxALL, 5);

    m_staticText23 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY,
        _("Text color"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText23->Wrap(-1);
    gSizer2->Add(m_staticText23, 0, wxALL, 5);

    m_cpFontColor = new wxColourPickerCtrl(sbSizer3->GetStaticBox(), wxID_ANY,
        *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
    gSizer2->Add(m_cpFontColor, 0, wxALL, 5);

    m_staticText21 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY,
        _("Font"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText21->Wrap(-1);
    gSizer2->Add(m_staticText21, 0, wxALL, 5);

    m_fpFont = new wxFontPickerCtrl(sbSizer3->GetStaticBox(), wxID_ANY,
        wxNullFont, wxDefaultPosition, wxDefaultSize, wxFNTP_DEFAULT_STYLE);
    m_fpFont->SetMaxPointSize(100);
    gSizer2->Add(m_fpFont, 0, wxALL, 5);

    bSizer4->Add(gSizer2, 1, wxEXPAND, 5);

    fgSizer4->Add(bSizer4, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer15;
    bSizer15 = new wxBoxSizer(wxVERTICAL);

    m_lSymbolList = new wxListBox(sbSizer3->GetStaticBox(), wxID_ANY,
        wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
    m_lSymbolList->Append(_("Cross"));
    m_lSymbolList->Append(_("Circle"));
    m_lSymbolList->Append(_("Square"));
    bSizer15->Add(m_lSymbolList, 0, wxALL, 5);

    m_cbSelectedSymbol = new wxCheckBox(sbSizer3->GetStaticBox(), wxID_ANY,
        _("Use this symbol"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer15->Add(m_cbSelectedSymbol, 0, wxALL, 5);

    m_cbUseDepthColours = new wxCheckBox(sbSizer3->GetStaticBox(), wxID_ANY,
        _("Use variable depth colours"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer15->Add(m_cbUseDepthColours, 0, wxALL, 5);

    fgSizer4->Add(bSizer15, 1, wxEXPAND, 5);

    sbSizer3->Add(fgSizer4, 1, wxEXPAND, 5);

    bSizer2->Add(sbSizer3, 0, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer* sbSizer1;
    sbSizer1 = new wxStaticBoxSizer(
        new wxStaticBox(this, wxID_ANY, _("Boat size / sensor position")),
        wxVERTICAL);

    wxFlexGridSizer* fgSizer1;
    fgSizer1 = new wxFlexGridSizer(0, 6, 0, 0);
    fgSizer1->SetFlexibleDirection(wxBOTH);
    fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    m_staticText1 = new wxStaticText(
        this, wxID_ANY, _("LOA"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText1->Wrap(-1);
    fgSizer1->Add(m_staticText1, 0, wxALL, 5);

    m_tLOA = new wxTextCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    fgSizer1->Add(m_tLOA, 0, wxALL, 5);

    m_staticText3 = new wxStaticText(
        this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText3->Wrap(-1);
    fgSizer1->Add(m_staticText3, 0, wxALL, 5);

    m_staticText2 = new wxStaticText(
        this, wxID_ANY, _("Beam"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText2->Wrap(-1);
    fgSizer1->Add(m_staticText2, 0, wxALL, 5);

    m_tBeam = new wxTextCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    fgSizer1->Add(m_tBeam, 0, wxALL, 5);

    m_staticText4 = new wxStaticText(
        this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText4->Wrap(-1);
    fgSizer1->Add(m_staticText4, 0, wxALL, 5);

    m_staticText5 = new wxStaticText(this, wxID_ANY, _("Sounder from bow"),
        wxDefaultPosition, wxDefaultSize, 0);
    m_staticText5->Wrap(-1);
    fgSizer1->Add(m_staticText5, 0, wxALL, 5);

    m_tSounderBow = new wxTextCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    fgSizer1->Add(m_tSounderBow, 0, wxALL, 5);

    m_staticText6 = new wxStaticText(
        this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText6->Wrap(-1);
    fgSizer1->Add(m_staticText6, 0, wxALL, 5);

    m_staticText7 = new wxStaticText(this, wxID_ANY, _("Waterline offset"),
        wxDefaultPosition, wxDefaultSize, 0);
    m_staticText7->Wrap(-1);
    fgSizer1->Add(m_staticText7, 0, wxALL, 5);

    m_tWaterlineOffset = new wxTextCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    fgSizer1->Add(m_tWaterlineOffset, 0, wxALL, 5);

    m_staticText8 = new wxStaticText(
        this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText8->Wrap(-1);
    fgSizer1->Add(m_staticText8, 0, wxALL, 5);

    m_staticText9 = new wxStaticText(
        this, wxID_ANY, _("GPS from bow"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText9->Wrap(-1);
    fgSizer1->Add(m_staticText9, 0, wxALL, 5);

    m_tGPSBow = new wxTextCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    fgSizer1->Add(m_tGPSBow, 0, wxALL, 5);

    m_staticText10 = new wxStaticText(
        this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText10->Wrap(-1);
    fgSizer1->Add(m_staticText10, 0, wxALL, 5);

    m_staticText11 = new wxStaticText(this, wxID_ANY, _("GPS from port"),
        wxDefaultPosition, wxDefaultSize, 0);
    m_staticText11->Wrap(-1);
    fgSizer1->Add(m_staticText11, 0, wxALL, 5);

    m_tGPSPort = new wxTextCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    fgSizer1->Add(m_tGPSPort, 0, wxALL, 5);

    m_staticText12 = new wxStaticText(
        this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText12->Wrap(-1);
    fgSizer1->Add(m_staticText12, 0, wxALL, 5);

    sbSizer1->Add(fgSizer1, 1, wxEXPAND, 5);

    bSizer2->Add(sbSizer1, 0, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer* sbSizer2;
    sbSizer2 = new wxStaticBoxSizer(
        new wxStaticBox(this, wxID_ANY, _("Filtering")), wxVERTICAL);

    wxFlexGridSizer* fgSizer2;
    fgSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
    fgSizer2->SetFlexibleDirection(wxBOTH);
    fgSizer2->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    m_staticText13 = new wxStaticText(this, wxID_ANY,
        _("Do not record values if less than"), wxDefaultPosition,
        wxDefaultSize, 0);
    m_staticText13->Wrap(-1);
    fgSizer2->Add(m_staticText13, 0, wxALL, 5);

    m_tMinDistance = new wxTextCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    fgSizer2->Add(m_tMinDistance, 0, wxALL, 5);

    m_staticText14 = new wxStaticText(
        this, wxID_ANY, _("m apart"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText14->Wrap(-1);
    fgSizer2->Add(m_staticText14, 0, wxALL, 5);

    // m_staticText15 = new wxStaticText( this, wxID_ANY, _("Automatically start
    // new survey if more than"), wxDefaultPosition, wxDefaultSize, 0 );
    // m_staticText15->Wrap( -1 );
    // fgSizer2->Add( m_staticText15, 0, wxALL, 5 );

    // m_tAutoNewDistance = new wxTextCtrl( this, wxID_ANY, wxEmptyString,
    // wxDefaultPosition, wxDefaultSize, 0 ); fgSizer2->Add( m_tAutoNewDistance,
    // 0, wxALL, 5 );

    // m_staticText16 = new wxStaticText( this, wxID_ANY, _("m from last
    // sounding"), wxDefaultPosition, wxDefaultSize, 0 ); m_staticText16->Wrap(
    // -1 ); fgSizer2->Add( m_staticText16, 0, wxALL, 5 );

    sbSizer2->Add(fgSizer2, 1, wxEXPAND, 5);

    // m_staticText22 = new wxStaticText( this, wxID_ANY, _("Leave empty if you
    // don't want to use this functionality"), wxDefaultPosition, wxDefaultSize,
    // 0 ); m_staticText22->Wrap( -1 ); sbSizer2->Add( m_staticText22, 0, wxALL,
    // 5 );

    bSizer2->Add(sbSizer2, 0, wxALL | wxEXPAND, 5);

    bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

    m_sdbSizer1 = new wxStdDialogButtonSizer();
    m_sdbSizer1OK = new wxButton(this, wxID_OK);
    m_sdbSizer1->AddButton(m_sdbSizer1OK);
    m_sdbSizer1Cancel = new wxButton(this, wxID_CANCEL);
    m_sdbSizer1->AddButton(m_sdbSizer1Cancel);
    m_sdbSizer1->Realize();

    bSizer1->Add(m_sdbSizer1, 0, wxALL | wxEXPAND, 5);

    this->SetSizer(bSizer1);
    this->Layout();

    this->Centre(wxBOTH);
}

SurveyCfgDlgDef::~SurveyCfgDlgDef() { }

SurveyDlgDef::SurveyDlgDef(wxWindow* parent, wxWindowID id,
    const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    this->SetSizeHints(wxSize(500, 400), wxDefaultSize);

    wxBoxSizer* bSizer6;
    bSizer6 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizer7;
    bSizer7 = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* bSizer8;
    bSizer8 = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* sbSizer5;
    sbSizer5 = new wxStaticBoxSizer(
        new wxStaticBox(this, wxID_ANY, _("Survey")), wxVERTICAL);

    wxArrayString m_chSurveyChoices;
    m_chSurvey = new wxChoice(
        this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_chSurveyChoices, 0);
    m_chSurvey->SetSelection(0);
    sbSizer5->Add(m_chSurvey, 1, wxALL | wxEXPAND, 5);

    wxGridBagSizer* gbSizer1;
    gbSizer1 = new wxGridBagSizer(0, 0);
    gbSizer1->SetFlexibleDirection(wxBOTH);
    gbSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    m_tNrSoundings = new wxStaticText(this, wxID_ANY, _("Soundings: XXX"),
        wxDefaultPosition, wxDefaultSize, 0);
    m_tNrSoundings->Wrap(-1);
    gbSizer1->Add(m_tNrSoundings, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

    m_tArea = new wxStaticText(this, wxID_ANY, _(""), wxDefaultPosition,
        wxDefaultSize, 0); // Area: XXXsq.m
    m_tArea->Wrap(-1);
    gbSizer1->Add(m_tArea, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);

    m_tMinDepth = new wxStaticText(this, wxID_ANY, _("Minimal depth: XXXm"),
        wxDefaultPosition, wxDefaultSize, 0);
    m_tMinDepth->Wrap(-1);
    gbSizer1->Add(m_tMinDepth, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALL, 5);

    m_tMaxDepth = new wxStaticText(this, wxID_ANY, _("Maximal depth: XXXm"),
        wxDefaultPosition, wxDefaultSize, 0);
    m_tMaxDepth->Wrap(-1);
    gbSizer1->Add(m_tMaxDepth, wxGBPosition(0, 3), wxGBSpan(1, 1), wxALL, 5);

    sbSizer5->Add(gbSizer1, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer12;
    bSizer12 = new wxBoxSizer(wxHORIZONTAL);

    m_tbRecordNMEA = new wxToggleButton(this, wxID_ANY, _("Record from NMEA"),
        wxDefaultPosition, wxDefaultSize, 0);
    bSizer12->Add(m_tbRecordNMEA, 0, wxALL, 5);

    m_btbRecord = new wxBitmapToggleButton(
        this, wxID_ANY, *_img_survey, wxDefaultPosition, wxSize(32, 32), 0);
    bSizer12->Add(m_btbRecord, 0, wxALL, 5);

    bSizer12->Add(0, 0, 1, wxEXPAND, 5);

    // m_btnAddTide = new wxButton(this, wxID_ANY, _("Add Tidal Data"),
    // wxDefaultPosition, wxDefaultSize, 0); bSizer12->Add(m_btnAddTide, 0,
    // wxALL, 5);

    bSizer12->Add(0, 0, 1, wxEXPAND, 5);

    m_pButtonAdd = new wxBitmapButton(
        this, wxID_ANY, *_img_plus, wxDefaultPosition, wxDefaultSize);
    bSizer12->Add(m_pButtonAdd, 0, wxALIGN_CENTER, 2);

    m_pButtonDelete = new wxBitmapButton(
        this, wxID_ANY, *_img_minus, wxDefaultPosition, wxDefaultSize);
    bSizer12->Add(m_pButtonDelete, 0, wxALIGN_CENTER, 2);

    m_btnLoadFromFile = new wxButton(this, wxID_ANY, _("Load NMEA File"),
        wxDefaultPosition, wxDefaultSize, 0);
    bSizer12->Add(m_btnLoadFromFile, 0, wxALL, 5);

    sbSizer5->Add(bSizer12, 0, wxEXPAND, 5);

    bSizer8->Add(sbSizer5, 0, wxEXPAND, 5);

    wxStaticBoxSizer* sbSizer6;
    sbSizer6 = new wxStaticBoxSizer(
        new wxStaticBox(this, wxID_ANY, _("Details")), wxVERTICAL);

    m_notebook1
        = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
    m_panel1 = new wxPanel(m_notebook1, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer* bSizer10;
    bSizer10 = new wxBoxSizer(wxVERTICAL);

    m_gdSoundings = new wxGrid(m_panel1, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxHSCROLL | wxVSCROLL);

    // Grid
    m_gdSoundings->CreateGrid(1, 6);
    m_gdSoundings->EnableEditing(false);
    m_gdSoundings->EnableGridLines(true);
    m_gdSoundings->EnableDragGridSize(false);
    m_gdSoundings->SetMargins(0, 0);
    m_gdSoundings->EnableScrolling(true, true);

    // Columns
    m_gdSoundings->EnableDragColMove(false);
    m_gdSoundings->EnableDragColSize(true);
    m_gdSoundings->SetColLabelSize(30);
    m_gdSoundings->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);

    // Rows
    m_gdSoundings->EnableDragRowSize(true);
    m_gdSoundings->SetRowLabelSize(80);
    m_gdSoundings->SetRowLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);

    // Label Appearance
    m_gdSoundings->SetColLabelValue(0, _("Depth"));
    m_gdSoundings->SetColLabelValue(1, _("Measured"));
    m_gdSoundings->SetColLabelValue(2, _("Lat"));
    m_gdSoundings->SetColLabelValue(3, _("Lon"));
    m_gdSoundings->SetColLabelValue(4, _("Survey_id"));
    m_gdSoundings->SetColLabelValue(5, _(""));

    // Cell Defaults
    m_gdSoundings->SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_TOP);
    bSizer10->Add(m_gdSoundings, 1, wxALL, 5);

    m_panel1->SetSizer(bSizer10);
    m_panel1->Layout();
    bSizer10->Fit(m_panel1);
    m_notebook1->AddPage(m_panel1, _("Soundings"), true);
    m_panel2 = new wxPanel(m_notebook1, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer* bSizer11;
    bSizer11 = new wxBoxSizer(wxVERTICAL);

    m_profilegraph = new wxStaticBitmap(
        m_panel2, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
    bSizer11->Add(m_profilegraph, 1, wxALL | wxEXPAND, 5);

    m_panel2->SetSizer(bSizer11);
    m_panel2->Layout();
    bSizer11->Fit(m_panel2);
    m_notebook1->AddPage(m_panel2, _("Trace"), false);

    m_panel3 = new wxPanel(m_notebook1, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer* bSizer13;
    bSizer13 = new wxBoxSizer(wxVERTICAL);

    m_profilegraph1 = new wxStaticBitmap(
        m_panel3, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
    bSizer13->Add(m_profilegraph1, 1, wxALL | wxBOTTOM | wxEXPAND, 5);

    wxArrayString num_soundingsChoices;
    num_soundings = new wxChoice(m_panel3, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, num_soundingsChoices, 0);
    num_soundings->SetSelection(0);
    bSizer13->Add(num_soundings, 0, wxALIGN_TOP, 5);

    m_panel3->SetSizer(bSizer13);
    m_panel3->Layout();
    bSizer13->Fit(m_panel3);
    m_notebook1->AddPage(m_panel3, _("Profile"), false);

    sbSizer6->Add(m_notebook1, 1, wxEXPAND | wxALL, 5);

    bSizer8->Add(sbSizer6, 1, wxEXPAND, 5);

    bSizer7->Add(bSizer8, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer9;
    bSizer9 = new wxBoxSizer(wxVERTICAL);

    m_btnNew = new wxButton(
        this, wxID_ANY, _("New"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnNew, 0, wxALL, 5);

    m_btnDelete = new wxButton(
        this, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnDelete, 0, wxALL, 5);

    m_btnProperties = new wxButton(
        this, wxID_ANY, _("Properties"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnProperties, 0, wxALL, 5);

    m_btnZoomTo = new wxButton(
        this, wxID_ANY, _("Zoom to"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnZoomTo, 0, wxALL, 5);

    m_btnEdit = new wxButton(
        this, wxID_ANY, _("Edit..."), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnEdit, 0, wxALL, 5);

    m_btnMerge = new wxButton(
        this, wxID_ANY, _("Merge..."), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnMerge, 0, wxALL, 5);

    m_btnImport = new wxButton(
        this, wxID_ANY, _("Import..."), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnImport, 0, wxALL, 5);

    m_btnExport = new wxButton(
        this, wxID_ANY, _("Export..."), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnExport, 0, wxALL, 5);

    m_btnTide = new wxButton(
        this, wxID_ANY, _("Tide Data"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnTide, 0, wxALL, 5);

    bSizer7->Add(bSizer9, 0, wxALL | wxEXPAND, 5);

    bSizer6->Add(bSizer7, 1, wxEXPAND, 5);

    m_sdbSizer2 = new wxStdDialogButtonSizer();
    m_sdbSizer2OK = new wxButton(this, wxID_OK);
    m_sdbSizer2->AddButton(m_sdbSizer2OK);
    m_sdbSizer2Cancel = new wxButton(this, wxID_CANCEL);
    m_sdbSizer2->AddButton(m_sdbSizer2Cancel);
    m_sdbSizer2->Realize();

    bSizer6->Add(m_sdbSizer2, 0, wxALL | wxEXPAND, 5);

    this->SetSizer(bSizer6);
    this->Layout();

    this->Centre(wxBOTH);

    // Connect Events

    this->Connect(
        wxEVT_CLOSE_WINDOW, wxCloseEventHandler(SurveyDlgDef::OnClose));
    m_chSurvey->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
        wxCommandEventHandler(SurveyDlgDef::OnSurveySelection), NULL, this);
    // m_btnAddTide->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
    // wxCommandEventHandler(SurveyDlgDef::OnAddTide), NULL, this);
    m_btnLoadFromFile->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::LoadFromFile), NULL, this);
    m_btbRecord->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnSurveyRecordToggleNMEA), NULL,
        this);
    m_tbRecordNMEA->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::RecordNMEA), NULL, this);
    m_btnNew->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnNewSurvey), NULL, this);
    m_btnDelete->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnDeleteSurvey), NULL, this);
    m_btnProperties->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnSurveyProperties), NULL, this);
    m_btnZoomTo->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnZoomTSurvey), NULL, this);
    m_btnEdit->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnEditSoundings), NULL, this);
    m_btnMerge->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnMergeSurvey), NULL, this);
    m_btnImport->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnImportSurvey), NULL, this);
    m_btnExport->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnExportSurvey), NULL, this);
    m_btnTide->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnSetTidalData), NULL, this);
    m_sdbSizer2Cancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnSurveyCancelClick), NULL, this);
    m_sdbSizer2OK->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnSurveyOkClick), NULL, this);
    m_notebook1->Connect(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
        wxNotebookEventHandler(SurveyDlgDef::IsPanelSelected), NULL, this);

    m_pButtonAdd->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnItemAdd), NULL, this);
    m_pButtonDelete->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnItemDelete), NULL, this);
}

SurveyDlgDef::~SurveyDlgDef()
{
    // Disconnect Events
    m_chSurvey->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED,
        wxCommandEventHandler(SurveyDlgDef::OnSurveySelection), NULL, this);
    // m_btnAddTide->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
    // wxCommandEventHandler(SurveyDlgDef::OnAddTide), NULL, this);
    m_btnLoadFromFile->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::LoadFromFile), NULL, this);
    m_btbRecord->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnSurveyRecordToggleNMEA), NULL,
        this);
    m_tbRecordNMEA->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::RecordNMEA), NULL, this);
    m_btnNew->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnNewSurvey), NULL, this);
    m_btnDelete->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnDeleteSurvey), NULL, this);
    m_btnProperties->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnSurveyProperties), NULL, this);
    m_btnZoomTo->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnZoomTSurvey), NULL, this);
    m_btnEdit->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnEditSoundings), NULL, this);
    m_btnMerge->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnMergeSurvey), NULL, this);
    m_btnImport->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnImportSurvey), NULL, this);
    m_btnExport->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnExportSurvey), NULL, this);
    m_btnTide->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnSetTidalData), NULL, this);
    m_sdbSizer2Cancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnSurveyCancelClick), NULL, this);
    m_sdbSizer2OK->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnSurveyOkClick), NULL, this);
    m_notebook1->Disconnect(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
        wxNotebookEventHandler(SurveyDlgDef::IsPanelSelected), NULL, this);

    m_pButtonAdd->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnItemAdd), NULL, this);
    m_pButtonDelete->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyDlgDef::OnItemDelete), NULL, this);
}

SurveyMergeDlgDef::SurveyMergeDlgDef(wxWindow* parent, wxWindowID id,
    const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer13;
    bSizer13 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizer14;
    bSizer14 = new wxBoxSizer(wxVERTICAL);

    m_staticText28 = new wxStaticText(this, wxID_ANY,
        _("Merge survey XXX with"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText28->Wrap(-1);
    bSizer14->Add(m_staticText28, 0, wxALL, 5);

    wxArrayString m_cMergeWithChoices;
    m_cMergeWith = new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, m_cMergeWithChoices, 0);
    m_cMergeWith->SetSelection(0);
    bSizer14->Add(m_cMergeWith, 0, wxALL | wxEXPAND, 5);

    bSizer13->Add(bSizer14, 1, wxEXPAND, 5);

    m_sdbSizer3 = new wxStdDialogButtonSizer();
    m_sdbSizer3OK = new wxButton(this, wxID_OK);
    m_sdbSizer3->AddButton(m_sdbSizer3OK);
    m_sdbSizer3Cancel = new wxButton(this, wxID_CANCEL);
    m_sdbSizer3->AddButton(m_sdbSizer3Cancel);
    m_sdbSizer3->Realize();

    bSizer13->Add(m_sdbSizer3, 0, wxEXPAND, 5);

    this->SetSizer(bSizer13);
    this->Layout();

    this->Centre(wxBOTH);

    // Connect Events
    m_sdbSizer3Cancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyMergeDlgDef::OnMergeCancelClick), NULL,
        this);
    m_sdbSizer3OK->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyMergeDlgDef::OnMergeOkClick), NULL, this);
}

SurveyMergeDlgDef::~SurveyMergeDlgDef()
{
    // Disconnect Events
    m_sdbSizer3Cancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyMergeDlgDef::OnMergeCancelClick), NULL,
        this);
    m_sdbSizer3OK->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyMergeDlgDef::OnMergeOkClick), NULL, this);
}

SoundingsEditDlgDef::SoundingsEditDlgDef(wxWindow* parent, wxWindowID id,
    const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer13;
    bSizer13 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizer14;
    bSizer14 = new wxBoxSizer(wxVERTICAL);

    m_staticText28 = new wxStaticText(this, wxID_ANY,
        _("Merge survey XXX with"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText28->Wrap(-1);
    bSizer14->Add(m_staticText28, 0, wxALL, 5);

    m_cMergeWith = new wxStaticText(
        this, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, 0);
    bSizer14->Add(m_cMergeWith, 0, wxALL | wxEXPAND, 5);

    bSizer13->Add(bSizer14, 1, wxEXPAND, 5);

    m_sdbSizer3 = new wxStdDialogButtonSizer();
    m_sdbSizer3OK = new wxButton(this, wxID_OK);
    m_sdbSizer3->AddButton(m_sdbSizer3OK);
    m_sdbSizer3Cancel = new wxButton(this, wxID_CANCEL);
    m_sdbSizer3->AddButton(m_sdbSizer3Cancel);
    m_sdbSizer3->Realize();

    bSizer13->Add(m_sdbSizer3, 0, wxEXPAND, 5);

    this->SetSizer(bSizer13);
    this->Layout();

    this->Centre(wxBOTH);

    // Connect Events
    m_sdbSizer3Cancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SoundingsEditDlgDef::OnEditCancelClick), NULL,
        this);
    m_sdbSizer3OK->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SoundingsEditDlgDef::OnEditOkClick), NULL, this);
}

SoundingsEditDlgDef::~SoundingsEditDlgDef()
{
    // Disconnect Events
    m_sdbSizer3Cancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SoundingsEditDlgDef::OnEditCancelClick), NULL,
        this);
    m_sdbSizer3OK->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SoundingsEditDlgDef::OnEditOkClick), NULL, this);
}

SurveyPropDlgDef::SurveyPropDlgDef(wxWindow* parent, wxWindowID id,
    const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer13;
    bSizer13 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizer14;
    bSizer14 = new wxBoxSizer(wxVERTICAL);

    m_staticText28 = new wxStaticText(this, wxID_ANY,
        _("Number of soundings: "), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText28->Wrap(-1);
    bSizer14->Add(m_staticText28, 0, wxALL, 5);

    m_staticText29 = new wxStaticText(this, wxID_ANY, _("Maximum Depth: "),
        wxDefaultPosition, wxDefaultSize, 0);
    m_staticText29->Wrap(-1);
    bSizer14->Add(m_staticText29, 0, wxALL, 5);

    m_staticText30 = new wxStaticText(this, wxID_ANY, _("Minimum Depth: "),
        wxDefaultPosition, wxDefaultSize, 0);
    m_staticText30->Wrap(-1);
    bSizer14->Add(m_staticText30, 0, wxALL, 5);

    m_staticText31 = new wxStaticText(
        this, wxID_ANY, _("Extra: "), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText31->Wrap(-1);
    bSizer14->Add(m_staticText31, 0, wxALL, 5);

    bSizer13->Add(bSizer14, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer15;
    bSizer15 = new wxBoxSizer(wxVERTICAL);

    m_sdbSizer3 = new wxStdDialogButtonSizer();
    m_sdbSizer3OK = new wxButton(this, wxID_OK);
    m_sdbSizer3->AddButton(m_sdbSizer3OK);
    m_sdbSizer3->Realize();

    bSizer15->Add(m_sdbSizer3, 0, wxEXPAND, 5);

    bSizer13->Add(bSizer15, 1, wxEXPAND, 5);

    this->SetSizer(bSizer13);
    this->Layout();

    this->Centre(wxBOTH);

    // Connect Events
    m_sdbSizer3OK->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyPropDlgDef::OnPropOkClick), NULL, this);
}

SurveyPropDlgDef::~SurveyPropDlgDef()
{
    // Disconnect Events
    m_sdbSizer3OK->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyPropDlgDef::OnPropOkClick), NULL, this);
}
///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "surveygui.h"

///////////////////////////////////////////////////////////////////////////

SurveyTidalDlgDef::SurveyTidalDlgDef(wxWindow* parent, wxWindowID id,
    const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    this->SetSizeHints(wxSize(500, 400), wxDefaultSize);

    wxBoxSizer* bSizer6;
    bSizer6 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizer7;
    bSizer7 = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* bSizer8;
    bSizer8 = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* sbSizer5;
    sbSizer5 = new wxStaticBoxSizer(
        new wxStaticBox(this, wxID_ANY, _("Survey")), wxVERTICAL);

    wxArrayString m_chTidalSurveyChoices;
    m_chTidalSurvey = new wxChoice(sbSizer5->GetStaticBox(), wxID_ANY,
        wxDefaultPosition, wxDefaultSize, m_chTidalSurveyChoices, 0);
    m_chTidalSurvey->SetSelection(0);
    sbSizer5->Add(m_chTidalSurvey, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer* bSizer12;
    bSizer12 = new wxBoxSizer(wxHORIZONTAL);

    m_staticText42 = new wxStaticText(sbSizer5->GetStaticBox(), wxID_ANY,
        _("Height every"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText42->Wrap(-1);
    bSizer12->Add(m_staticText42, 0, wxALL, 5);

    wxString m_choice5Choices[]
        = { _("5"), _("10"), _("15"), _("30"), _("60") };
    int m_choice5NChoices = sizeof(m_choice5Choices) / sizeof(wxString);
    m_choice5
        = new wxChoice(sbSizer5->GetStaticBox(), wxID_ANY, wxDefaultPosition,
            wxDefaultSize, m_choice5NChoices, m_choice5Choices, 0);
    m_choice5->SetSelection(0);
    bSizer12->Add(m_choice5, 0, wxALL, 5);

    m_staticText43 = new wxStaticText(sbSizer5->GetStaticBox(), wxID_ANY,
        _("minutes"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText43->Wrap(-1);
    bSizer12->Add(m_staticText43, 0, wxALL, 5);

    bSizer12->Add(0, 0, 1, wxEXPAND, 5);

    m_bAutoFill = new wxButton(sbSizer5->GetStaticBox(), wxID_ANY,
        _("Auto-fill times"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer12->Add(m_bAutoFill, 0, wxALL, 5);

    bSizer12->Add(0, 0, 1, wxEXPAND, 5);

    m_btnSelectAll = new wxButton(sbSizer5->GetStaticBox(), wxID_ANY,
        _("Select all records"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer12->Add(m_btnSelectAll, 0, wxALL, 5);

    bSizer12->Add(0, 0, 1, wxEXPAND, 5);

    sbSizer5->Add(bSizer12, 0, wxEXPAND, 5);

    wxBoxSizer* bSizer121;
    bSizer121 = new wxBoxSizer(wxHORIZONTAL);

    m_staticText39 = new wxStaticText(sbSizer5->GetStaticBox(), wxID_ANY,
        _("BeginTime"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText39->Wrap(-1);
    m_staticText39->SetFont(
        wxFont(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT,
            wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString));

    bSizer121->Add(m_staticText39, 0, wxALL, 5);

    m_timePicker2 = new wxTimePickerCtrl(sbSizer5->GetStaticBox(), wxID_ANY,
        wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxTP_DEFAULT);
    bSizer121->Add(m_timePicker2, 0, wxALL, 5);

    m_staticText40 = new wxStaticText(sbSizer5->GetStaticBox(), wxID_ANY,
        _("Date"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText40->Wrap(-1);
    bSizer121->Add(m_staticText40, 0, wxALL, 5);

    m_datePicker1 = new wxDatePickerCtrl(sbSizer5->GetStaticBox(), wxID_ANY,
        wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT);
    bSizer121->Add(m_datePicker1, 0, wxALL, 5);

    m_staticText38 = new wxStaticText(sbSizer5->GetStaticBox(), wxID_ANY,
        _("Correction      "), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText38->Wrap(-1);
    bSizer121->Add(m_staticText38, 0, wxALL, 5);

    m_tcCorrection = new wxTextCtrl(sbSizer5->GetStaticBox(), wxID_ANY,
        wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    bSizer121->Add(m_tcCorrection, 0, wxALL, 5);

    m_btnAddCorrection = new wxButton(sbSizer5->GetStaticBox(), wxID_ANY,
        _("Add"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer121->Add(m_btnAddCorrection, 0, wxALL, 5);

    sbSizer5->Add(bSizer121, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer1211;
    bSizer1211 = new wxBoxSizer(wxHORIZONTAL);

    m_staticText391 = new wxStaticText(sbSizer5->GetStaticBox(), wxID_ANY,
        _("End   Time"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText391->Wrap(-1);
    m_staticText391->SetFont(
        wxFont(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT,
            wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString));

    bSizer1211->Add(m_staticText391, 0, wxALL, 5);

    m_timePicker21 = new wxTimePickerCtrl(sbSizer5->GetStaticBox(), wxID_ANY,
        wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxTP_DEFAULT);
    bSizer1211->Add(m_timePicker21, 0, wxALL, 5);

    m_staticText401 = new wxStaticText(sbSizer5->GetStaticBox(), wxID_ANY,
        _("Date"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText401->Wrap(-1);
    bSizer1211->Add(m_staticText401, 0, wxALL, 5);

    m_datePicker11 = new wxDatePickerCtrl(sbSizer5->GetStaticBox(), wxID_ANY,
        wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT);
    bSizer1211->Add(m_datePicker11, 0, wxALL, 5);

    m_staticText411 = new wxStaticText(sbSizer5->GetStaticBox(), wxID_ANY,
        _("Height of Tide"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText411->Wrap(-1);
    bSizer1211->Add(m_staticText411, 0, wxALL, 5);

    m_tcHeightOfTide1 = new wxTextCtrl(sbSizer5->GetStaticBox(), wxID_ANY,
        wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    bSizer1211->Add(m_tcHeightOfTide1, 0, wxALL, 5);

    m_btnAddTidalHeight1 = new wxButton(sbSizer5->GetStaticBox(), wxID_ANY,
        _("Add"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer1211->Add(m_btnAddTidalHeight1, 0, wxALL, 5);

    sbSizer5->Add(bSizer1211, 1, wxEXPAND, 5);

    bSizer8->Add(sbSizer5, 0, wxEXPAND, 5);

    wxStaticBoxSizer* sbSizer6;
    sbSizer6 = new wxStaticBoxSizer(
        new wxStaticBox(this, wxID_ANY, _("Details")), wxVERTICAL);

    m_notebook1 = new wxNotebook(sbSizer6->GetStaticBox(), wxID_ANY,
        wxDefaultPosition, wxDefaultSize, 0);
    m_panel1 = new wxPanel(m_notebook1, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer* bSizer10;
    bSizer10 = new wxBoxSizer(wxVERTICAL);

    m_gdTidalHeights = new wxGrid(m_panel1, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxHSCROLL | wxVSCROLL);

    // Grid
    m_gdTidalHeights->CreateGrid(1, 3);
    m_gdTidalHeights->EnableEditing(true);
    m_gdTidalHeights->EnableGridLines(true);
    m_gdTidalHeights->EnableDragGridSize(false);
    m_gdTidalHeights->SetMargins(0, 0);

    // Columns
    m_gdTidalHeights->EnableDragColMove(false);
    m_gdTidalHeights->EnableDragColSize(true);
    m_gdTidalHeights->SetColLabelSize(30);
    m_gdTidalHeights->SetColLabelAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

    // Rows
    m_gdTidalHeights->EnableDragRowSize(true);
    m_gdTidalHeights->SetRowLabelSize(80);
    m_gdTidalHeights->SetRowLabelAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

    // Label Appearance

    // Cell Defaults
    m_gdTidalHeights->SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_TOP);
    bSizer10->Add(m_gdTidalHeights, 1, wxALL | wxEXPAND, 5);

    m_panel1->SetSizer(bSizer10);
    m_panel1->Layout();
    bSizer10->Fit(m_panel1);
    m_notebook1->AddPage(m_panel1, _("Height of tide"), true);
    m_panel2 = new wxPanel(m_notebook1, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer* bSizer11;
    bSizer11 = new wxBoxSizer(wxVERTICAL);

    m_profileTide = new wxStaticBitmap(
        m_panel2, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
    bSizer11->Add(m_profileTide, 1, wxALL | wxEXPAND, 5);

    m_panel2->SetSizer(bSizer11);
    m_panel2->Layout();
    bSizer11->Fit(m_panel2);
    m_notebook1->AddPage(m_panel2, _("Tidal Graph"), false);

    sbSizer6->Add(m_notebook1, 1, wxEXPAND | wxALL, 5);

    bSizer8->Add(sbSizer6, 1, wxEXPAND, 5);

    bSizer7->Add(bSizer8, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer9;
    bSizer9 = new wxBoxSizer(wxVERTICAL);

    m_btnSave = new wxButton(
        this, wxID_ANY, _("Save"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnSave, 0, wxALL, 5);

    m_btnCancelTidalHeights = new wxButton(
        this, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnCancelTidalHeights, 0, wxALL, 5);

    m_btnImportTide = new wxButton(
        this, wxID_ANY, _("Import..."), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnImportTide, 0, wxALL, 5);

    m_btnExportTide = new wxButton(
        this, wxID_ANY, _("Export..."), wxDefaultPosition, wxDefaultSize, 0);
    bSizer9->Add(m_btnExportTide, 0, wxALL, 5);

    bSizer7->Add(bSizer9, 0, wxALL | wxEXPAND, 5);

    bSizer6->Add(bSizer7, 1, wxEXPAND, 5);

    this->SetSizer(bSizer6);
    this->Layout();

    this->Centre(wxBOTH);

    // Connect Events
    m_chTidalSurvey->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
        wxCommandEventHandler(SurveyTidalDlgDef::OnEditSurveySelection), NULL,
        this);
    m_bAutoFill->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::AutoFill), NULL, this);
    m_btnSelectAll->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::OnSelectAll), NULL, this);
    m_btnAddCorrection->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::SetCorrection), NULL, this);
    m_btnAddTidalHeight1->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::SetTidalHeight), NULL, this);
    m_btnSave->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::OnSaveTides), NULL, this);
    m_btnCancelTidalHeights->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::OnCancelTides), NULL, this);
    m_btnImportTide->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::OnImportTides), NULL, this);
    m_btnExportTide->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::OnExportTides), NULL, this);
}

SurveyTidalDlgDef::~SurveyTidalDlgDef()
{
    // Disconnect Events
    m_chTidalSurvey->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED,
        wxCommandEventHandler(SurveyTidalDlgDef::OnEditSurveySelection), NULL,
        this);
    m_bAutoFill->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::AutoFill), NULL, this);
    m_btnSelectAll->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::OnSelectAll), NULL, this);
    m_btnAddCorrection->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::SetCorrection), NULL, this);
    m_btnAddTidalHeight1->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::SetTidalHeight), NULL, this);
    m_btnSave->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::OnSaveTides), NULL, this);
    m_btnCancelTidalHeights->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::OnCancelTides), NULL, this);
    m_btnImportTide->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::OnImportTides), NULL, this);
    m_btnExportTide->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(SurveyTidalDlgDef::OnExportTides), NULL, this);
}
