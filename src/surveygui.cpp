///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 10 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "surveygui.h"

///////////////////////////////////////////////////////////////////////////

SurveyCfgDlgDef::SurveyCfgDlgDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("General settings") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText17 = new wxStaticText( this, wxID_ANY, _("Opacity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	fgSizer3->Add( m_staticText17, 0, wxALL, 5 );
	
	m_sOpacity = new wxSlider( this, wxID_ANY, 50, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer3->Add( m_sOpacity, 0, wxALL|wxEXPAND, 5 );
	
	
	sbSizer4->Add( fgSizer3, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText221 = new wxStaticText( this, wxID_ANY, _("Display values as"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText221->Wrap( -1 );
	bSizer5->Add( m_staticText221, 0, wxALL, 5 );
	
	m_rbMeters = new wxRadioButton( this, wxID_ANY, _("Meters"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_rbMeters->SetValue( true ); 
	bSizer5->Add( m_rbMeters, 0, wxALL, 5 );
	
	m_rbFeet = new wxRadioButton( this, wxID_ANY, _("Feet"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_rbFeet, 0, wxALL, 5 );
	
	m_rbFathoms = new wxRadioButton( this, wxID_ANY, _("Fathoms"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_rbFathoms, 0, wxALL, 5 );
	
	
	sbSizer4->Add( bSizer5, 1, wxEXPAND, 5 );
	
	
	bSizer2->Add( sbSizer4, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Overlay settings") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_cbRenderOverlay = new wxCheckBox( this, wxID_ANY, _("Render overlay"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_cbRenderOverlay, 0, wxALL, 5 );
	
	m_rbRendedAllSurveys = new wxRadioButton( this, wxID_ANY, _("Render all data"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_rbRendedAllSurveys->SetValue( true ); 
	bSizer3->Add( m_rbRendedAllSurveys, 0, wxALL, 5 );
	
	m_rbRenderActiveSurvey = new wxRadioButton( this, wxID_ANY, _("Render just active survey data"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_rbRenderActiveSurvey, 0, wxALL, 5 );
	
	m_cbConnectSoundings = new wxCheckBox( this, wxID_ANY, _("Connect soundings"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_cbConnectSoundings, 0, wxALL, 5 );
	
	
	fgSizer4->Add( bSizer3, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 0, 2, 0, 0 );
	
	m_staticText18 = new wxStaticText( this, wxID_ANY, _("Sounding color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	gSizer2->Add( m_staticText18, 0, wxALL, 5 );
	
	m_cpSoundingColor = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	gSizer2->Add( m_cpSoundingColor, 0, wxALL, 5 );
	
	m_staticText19 = new wxStaticText( this, wxID_ANY, _("Connector color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	gSizer2->Add( m_staticText19, 0, wxALL, 5 );
	
	m_cpConnectorColor = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	gSizer2->Add( m_cpConnectorColor, 0, wxALL, 5 );
	
	m_staticText23 = new wxStaticText( this, wxID_ANY, _("Text color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	gSizer2->Add( m_staticText23, 0, wxALL, 5 );
	
	m_cpFontColor = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	gSizer2->Add( m_cpFontColor, 0, wxALL, 5 );
	
	m_staticText21 = new wxStaticText( this, wxID_ANY, _("Font"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	gSizer2->Add( m_staticText21, 0, wxALL, 5 );
	
	m_fpFont = new wxFontPickerCtrl( this, wxID_ANY, wxNullFont, wxDefaultPosition, wxDefaultSize, wxFNTP_DEFAULT_STYLE );
	m_fpFont->SetMaxPointSize( 100 ); 
	gSizer2->Add( m_fpFont, 0, wxALL, 5 );
	
	
	bSizer4->Add( gSizer2, 1, wxEXPAND, 5 );
	
	
	fgSizer4->Add( bSizer4, 1, wxEXPAND, 5 );
	
	
	sbSizer3->Add( fgSizer4, 1, wxEXPAND, 5 );
	
	
	bSizer2->Add( sbSizer3, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Boat size / sensor position") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, _("LOA"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL, 5 );
	
	m_tLOA = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_tLOA, 0, wxALL, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, _("Beam"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL, 5 );
	
	m_tBeam = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_tBeam, 0, wxALL, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer1->Add( m_staticText4, 0, wxALL, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, _("Sounder from bow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL, 5 );
	
	m_tSounderBow = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_tSounderBow, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer1->Add( m_staticText6, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, _("Waterline offset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer1->Add( m_staticText7, 0, wxALL, 5 );
	
	m_tWaterlineOffset = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_tWaterlineOffset, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer1->Add( m_staticText8, 0, wxALL, 5 );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, _("GPS from bow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer1->Add( m_staticText9, 0, wxALL, 5 );
	
	m_tGPSBow = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_tGPSBow, 0, wxALL, 5 );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer1->Add( m_staticText10, 0, wxALL, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, _("GPS from port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer1->Add( m_staticText11, 0, wxALL, 5 );
	
	m_tGPSPort = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_tGPSPort, 0, wxALL, 5 );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, _("m"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer1->Add( m_staticText12, 0, wxALL, 5 );
	
	
	sbSizer1->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	
	bSizer2->Add( sbSizer1, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Filtering") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, _("Do not record values if less than"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer2->Add( m_staticText13, 0, wxALL, 5 );
	
	m_tMinDistance = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_tMinDistance, 0, wxALL, 5 );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, _("m apart"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer2->Add( m_staticText14, 0, wxALL, 5 );
	
	m_staticText15 = new wxStaticText( this, wxID_ANY, _("Automatically start new survey if more than"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	fgSizer2->Add( m_staticText15, 0, wxALL, 5 );
	
	m_tAutoNewDistance = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_tAutoNewDistance, 0, wxALL, 5 );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, _("m from last sounding"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	fgSizer2->Add( m_staticText16, 0, wxALL, 5 );
	
	
	sbSizer2->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	m_staticText22 = new wxStaticText( this, wxID_ANY, _("Leave empty if you don't want to use this functionality"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	sbSizer2->Add( m_staticText22, 0, wxALL, 5 );
	
	
	bSizer2->Add( sbSizer2, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	
	bSizer1->Add( m_sdbSizer1, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

SurveyCfgDlgDef::~SurveyCfgDlgDef()
{
}

SurveyDlgDef::SurveyDlgDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 500,400 ), wxDefaultSize );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Survey") ), wxVERTICAL );
	
	wxArrayString m_chSurveyChoices;
	m_chSurvey = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_chSurveyChoices, 0 );
	m_chSurvey->SetSelection( 0 );
	sbSizer5->Add( m_chSurvey, 1, wxALL|wxEXPAND, 5 );
	
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_tNrSoundings = new wxStaticText( this, wxID_ANY, _("Soundings: XXX"), wxDefaultPosition, wxDefaultSize, 0 );
	m_tNrSoundings->Wrap( -1 );
	gbSizer1->Add( m_tNrSoundings, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_tArea = new wxStaticText( this, wxID_ANY, _("Area: XXXsq.m"), wxDefaultPosition, wxDefaultSize, 0 );
	m_tArea->Wrap( -1 );
	gbSizer1->Add( m_tArea, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_tMinDepth = new wxStaticText( this, wxID_ANY, _("Minimal depth: XXXm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_tMinDepth->Wrap( -1 );
	gbSizer1->Add( m_tMinDepth, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_tMaxDepth = new wxStaticText( this, wxID_ANY, _("Maximal depth: XXXm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_tMaxDepth->Wrap( -1 );
	gbSizer1->Add( m_tMaxDepth, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	
	sbSizer5->Add( gbSizer1, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	m_tbRecord = new wxToggleButton( this, wxID_ANY, _("Record"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_tbRecord, 0, wxALL, 5 );
	
	
	sbSizer5->Add( bSizer12, 0, wxEXPAND, 5 );
	
	
	bSizer8->Add( sbSizer5, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Details") ), wxVERTICAL );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel1 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	m_gdSoundings = new wxGrid( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_gdSoundings->CreateGrid( 1, 5 );
	m_gdSoundings->EnableEditing( false );
	m_gdSoundings->EnableGridLines( true );
	m_gdSoundings->EnableDragGridSize( false );
	m_gdSoundings->SetMargins( 0, 0 );
	
	// Columns
	m_gdSoundings->EnableDragColMove( false );
	m_gdSoundings->EnableDragColSize( true );
	m_gdSoundings->SetColLabelSize( 30 );
	m_gdSoundings->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gdSoundings->EnableDragRowSize( true );
	m_gdSoundings->SetRowLabelSize( 80 );
	m_gdSoundings->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gdSoundings->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer10->Add( m_gdSoundings, 1, wxALL, 5 );
	
	
	m_panel1->SetSizer( bSizer10 );
	m_panel1->Layout();
	bSizer10->Fit( m_panel1 );
	m_notebook1->AddPage( m_panel1, _("Soundings"), true );
	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_profilegraph = new wxStaticBitmap( m_panel2, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_profilegraph, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel2->SetSizer( bSizer11 );
	m_panel2->Layout();
	bSizer11->Fit( m_panel2 );
	m_notebook1->AddPage( m_panel2, _("Profile"), false );
	
	sbSizer6->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer8->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	
	bSizer7->Add( bSizer8, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_btnNew = new wxButton( this, wxID_ANY, _("New"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnNew, 0, wxALL, 5 );
	
	m_btnDelete = new wxButton( this, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnDelete, 0, wxALL, 5 );
	
	m_btnProperties = new wxButton( this, wxID_ANY, _("Properties"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnProperties, 0, wxALL, 5 );
	
	m_btnZoomTo = new wxButton( this, wxID_ANY, _("Zoom to"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnZoomTo, 0, wxALL, 5 );
	
	m_btnMerge = new wxButton( this, wxID_ANY, _("Merge..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnMerge, 0, wxALL, 5 );
	
	m_btnImport = new wxButton( this, wxID_ANY, _("Import..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnImport, 0, wxALL, 5 );
	
	m_btnExport = new wxButton( this, wxID_ANY, _("Export..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_btnExport, 0, wxALL, 5 );
	
	
	bSizer7->Add( bSizer9, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer6->Add( bSizer7, 1, wxEXPAND, 5 );
	
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();
	
	bSizer6->Add( m_sdbSizer2, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer6 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_chSurvey->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SurveyDlgDef::OnSurveySelection ), NULL, this );
	m_tbRecord->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnRecordToggle ), NULL, this );
	m_btnNew->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnNewSurvey ), NULL, this );
	m_btnDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnDeleteSurvey ), NULL, this );
	m_btnProperties->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnSurveyProperties ), NULL, this );
	m_btnZoomTo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnZoomToSurvey ), NULL, this );
	m_btnMerge->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnMergeSurvey ), NULL, this );
	m_btnImport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnImportSurvey ), NULL, this );
	m_btnExport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnExportSurvey ), NULL, this );
	m_sdbSizer2Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnSurveyCancelClick ), NULL, this );
	m_sdbSizer2OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnSurveyOkClick ), NULL, this );
}

SurveyDlgDef::~SurveyDlgDef()
{
	// Disconnect Events
	m_chSurvey->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SurveyDlgDef::OnSurveySelection ), NULL, this );
	m_tbRecord->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnRecordToggle ), NULL, this );
	m_btnNew->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnNewSurvey ), NULL, this );
	m_btnDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnDeleteSurvey ), NULL, this );
	m_btnProperties->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnSurveyProperties ), NULL, this );
	m_btnZoomTo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnZoomToSurvey ), NULL, this );
	m_btnMerge->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnMergeSurvey ), NULL, this );
	m_btnImport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnImportSurvey ), NULL, this );
	m_btnExport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnExportSurvey ), NULL, this );
	m_sdbSizer2Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnSurveyCancelClick ), NULL, this );
	m_sdbSizer2OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyDlgDef::OnSurveyOkClick ), NULL, this );
	
}

SurveyMergeDlgDef::SurveyMergeDlgDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText28 = new wxStaticText( this, wxID_ANY, _("Merge survey XXX with"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	bSizer14->Add( m_staticText28, 0, wxALL, 5 );
	
	wxArrayString m_cMergeWithChoices;
	m_cMergeWith = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cMergeWithChoices, 0 );
	m_cMergeWith->SetSelection( 0 );
	bSizer14->Add( m_cMergeWith, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer13->Add( bSizer14, 1, wxEXPAND, 5 );
	
	m_sdbSizer3 = new wxStdDialogButtonSizer();
	m_sdbSizer3OK = new wxButton( this, wxID_OK );
	m_sdbSizer3->AddButton( m_sdbSizer3OK );
	m_sdbSizer3Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer3->AddButton( m_sdbSizer3Cancel );
	m_sdbSizer3->Realize();
	
	bSizer13->Add( m_sdbSizer3, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer13 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_sdbSizer3Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyMergeDlgDef::OnMergeCancelClick ), NULL, this );
	m_sdbSizer3OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyMergeDlgDef::OnMergeOkClick ), NULL, this );
}

SurveyMergeDlgDef::~SurveyMergeDlgDef()
{
	// Disconnect Events
	m_sdbSizer3Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyMergeDlgDef::OnMergeCancelClick ), NULL, this );
	m_sdbSizer3OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SurveyMergeDlgDef::OnMergeOkClick ), NULL, this );
	
}
