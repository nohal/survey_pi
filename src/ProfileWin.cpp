// For compilers that support precompilation, includes "wx.h".
//#ifndef  WX_PRECOMP
  #include "wx/wx.h"
//#endif //precompiled headers

#include "ProfileWin.h"
#include "wx/utils.h"
#include <wx/datetime.h>
#include <wx/string.h>
#include "surveygui_impl.h"
#include <wx/graphics.h>
#include <wx/wxchar.h>

//
// some compilers (i.e. MSVC++) defines their own 'snprintf' function
// so if it is not defined, define it in the following lines
// please note that we cannot use the wxWidget's counterpart 'wxSnprintf'
// because the latter uses 'wxChar' but wxJSON only use 'char'
#if !defined(snprintf) && defined(_MSC_VER)
#define snprintf _snprintf
#endif

enum
{
      ID_ProfileWin_NX,
      ID_ProfileWin_PR
};

enum
{
            FORWARD_ONE_HOUR_STEP    =3600,
            FORWARD_TEN_MINUTES_STEP =600,
            FORWARD_ONE_MINUTES_STEP =60,
            BACKWARD_ONE_HOUR_STEP    =-3600,
            BACKWARD_TEN_MINUTES_STEP =-600,
            BACKWARD_ONE_MINUTES_STEP =-60
};

enum
{
      TIDE_PLOT
};

#include <wx/listimpl.cpp>
WX_DEFINE_LIST( SplineList );

BEGIN_EVENT_TABLE ( ProfileWin, wxWindow ) EVT_PAINT ( ProfileWin::OnPaint )
    EVT_SIZE ( ProfileWin::OnSize )
    EVT_BUTTON ( wxID_OK, ProfileWin::OKEvent )
    //EVT_BUTTON ( ID_ProfileWin_NX, ProfileWin::NXEvent )
    //EVT_BUTTON ( ID_ProfileWin_PR, ProfileWin::PREvent )
    EVT_CLOSE ( ProfileWin::OnCloseWindow )
    
END_EVENT_TABLE()

// Define a constructor
ProfileWin::ProfileWin( wxWindow * parent, int x, int y, int PortNo, wxString PortName, int graphday_00_s, wxDateTime graphDayD, wxString myUnits, 
					double tcv[3000], double c_soundings, double tcmax, double tcmin, wxString tclist[12])
{
	m_soundings = c_soundings;
	m_passPort = PortNo;
	m_passName = PortName;
    m_passStation = graphday_00_s;
	m_graphday = graphDayD;
	m_units = myUnits;
	
	int i;



	for (i=0; i<m_soundings; i++){
		m_tcv[i] = tcv[i];
	}


	for (i = 0 ; i<12 ; i++){
		m_tclist[i] = wxEmptyString;
	}

	for (i=0; i<12; i++){
		if (!tclist[i].empty()){
			m_tclist[i] = tclist[i];
		}
	}

	m_tcmax = tcmax;
	m_tcmin = tcmin;

	//wxString MyLatD = wxString::Format(wxT("%5.2f"),(double)m_tcv[0]);
	//wxMessageBox(MyLatD, _T("in ProfileWin"));

    //    As a display optimization....
    //    if current color scheme is other than DAY,
    //    Then create the dialog ..WITHOUT.. borders and title bar.
    //    This way, any window decorations set by external themes, etc
    //    will not detract from night-vision
	long wstyle = wxCLIP_CHILDREN | wxDEFAULT_DIALOG_STYLE|wxSIMPLE_BORDER ;

   
    wxDialog::Create( parent, wxID_ANY, wxString( _T ( "test" ) ), wxPoint( x, y ),
                      wxSize( 550, 480 ), wstyle );

	c_blue = wxColour(220, 220, 220);
	SetBackgroundColour(c_blue);
    pParent = parent;


    SetTitle( wxString( _( "Survey Profile" ) ) );
	 m_plot_type = TIDE_PLOT;


    int sx, sy;
    GetClientSize( &sx, &sy );
    int swx, swy;
    GetSize( &swx, &swy );
    int parent_sx, parent_sy;
    pParent->GetClientSize( &parent_sx, &parent_sy );

    int xc = x + 8;
    int yc = y;

//  Arrange for ProfileWindow to be always totally visible
    if( ( x + 8 + swx ) > parent_sx ) xc = xc - swx - 16;
    if( ( y + swy ) > parent_sy ) yc = yc - swy;

//  Don't let the window origin move out of client area
    if( yc < 0 ) yc = 0;
    if( xc < 0 ) xc = 0;

    wxPoint r( xc, yc );
    Move( r );
	

	/*
  	myDlg = new Dlg(pParent, wxID_ANY,  _T("Tide Finder"), wxDefaultPosition, wxSize( -1,-1 ), wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
	
	myDlg->intPortNo = m_passPort;
	myDlg->m_graphday = m_graphday;
	myDlg->m_t_graphday_00_at_station = m_passStation;
	myDlg->m_graphday = m_passGraphDay;
	m_t_graphday_00_at_station = m_passStation;
	m_graphday = m_passGraphDay;
	*/
//    Figure out this computer timezone minute offset
    wxDateTime this_now = wxDateTime::Now();
    wxDateTime this_gmt = this_now.ToGMT();

#if wxCHECK_VERSION(2, 6, 2)
    wxTimeSpan diff = this_now.Subtract( this_gmt );
#else
    wxTimeSpan diff = this_gmt.Subtract ( this_now );
#endif

    int diff_mins = diff.GetMinutes();

    int station_offset = 0;

    m_corr_mins = station_offset - diff_mins;
    if( this_now.IsDST() ) m_corr_mins += 60;

//    Establish the inital drawing day as today
	
    btc_valid = false;

    wxString* TClist = NULL;
    m_tList = new wxListBox( this, -1, wxPoint( sx * 65 / 100, 11 ),
                             wxSize( ( sx * 32 / 100 ), ( sy * 20 / 70 ) ), 0, TClist,
                             wxLB_SINGLE | wxLB_NEEDED_SB );

    OK_button = new wxButton( this, wxID_OK, _( "OK" ), wxPoint( sx - 100, sy - 32 ),
                              wxDefaultSize );

    //PR_button = new wxButton( this, ID_ProfileWin_PR, _( "Prev" ), wxPoint( 10, sy - 32 ),
      //                        wxSize( 60, -1 ) );

    m_ptextctrl = new wxStaticText( this, -1, _T(""), wxPoint( sx * 5 / 100, 30 ),
                                  wxSize( ( sx * 60 / 100 ), ( sy *29 / 100 ) ) ,
                                  wxALIGN_CENTRE);
    int bsx, bsy, bpx, bpy;
    //PR_button->GetSize( &bsx, &bsy );
    //PR_button->GetPosition( &bpx, &bpy );

    //NX_button = new wxButton( this, ID_ProfileWin_NX, _( "Next" ), wxPoint( bpx + bsx + 5, bpy ),
    //                          wxSize( 60, -1 ) );

    //  establish some graphic element sizes/locations
    int x_graph = sx * 1 / 10;
    int y_graph = sy * 32 / 100;
    int x_graph_w = sx * 8 / 10;
    int y_graph_h = sy * 50 / 100;
    m_graph_rect = wxRect(x_graph, y_graph, x_graph_w, y_graph_h);


    // Build graphics tools

    pSFont = wxTheFontList->FindOrCreateFont( 8, wxFONTFAMILY_SWISS, wxNORMAL,
                                                    wxFONTWEIGHT_NORMAL, FALSE, wxString( _T ( "Arial" ) ) );
    pSMFont = wxTheFontList->FindOrCreateFont( 10, wxFONTFAMILY_SWISS, wxNORMAL,
                                                       wxFONTWEIGHT_NORMAL, FALSE, wxString( _T ( "Arial" ) ) );
    pMFont = wxTheFontList->FindOrCreateFont( 11, wxFONTFAMILY_SWISS, wxNORMAL, wxBOLD,
                                                      FALSE, wxString( _T ( "Arial" ) ) );
    pLFont = wxTheFontList->FindOrCreateFont( 12, wxFONTFAMILY_SWISS, wxNORMAL, wxBOLD,
                                                      FALSE, wxString( _T ( "Arial" ) ) );
	pVLFont = wxTheFontList->FindOrCreateFont( 12, wxFONTFAMILY_SWISS, wxNORMAL, wxBOLD,
                                                      FALSE, wxString( _T ( "Arial" ) ) );

    wxColour c_blue = wxColour(128, 248, 248);
	wxColour c_green = wxColour(0, 166, 80);
	wxColour c_yellow_orange = wxColour(253, 184, 19);
	wxColour c_orange = wxColour(248, 128, 64);
	
	c_black1 = wxColour(0, 0, 0);
	wxColour c_red = wxColour(248, 0, 0);
    c_grey = wxColour(236,233,216);
	c_grey2 = wxColour(212,208,200);

    pblack_1 = wxThePenList->FindOrCreatePen( c_black1, 1, wxSOLID );
                                                                         
    pblack_2 = wxThePenList->FindOrCreatePen( c_black1, 2,wxSOLID );
                                                                          
    pblack_3 = wxThePenList->FindOrCreatePen( c_black1, 1, wxSOLID );
                                                                         
    pred_2 = wxThePenList->FindOrCreatePen( c_red, 4, wxSOLID );
                                                                       
    pltgray = wxTheBrushList->FindOrCreateBrush( c_grey,wxSOLID );
                                                                               
    pltgray2 = wxTheBrushList->FindOrCreateBrush( c_grey2, wxSOLID );
                                                                               

    //  Fill in some static text control information
    //  Tidal station information
    // write the first line
   m_ptextctrl->SetFont( *pVLFont );
   m_ptextctrl->SetForegroundColour(c_black1);
   m_ptextctrl->SetBackgroundColour(c_blue);
   //m_ptextctrl->SetLabel(wxT("DOVER"));

}

ProfileWin::~ProfileWin()
{
    //Do Nothing;
}

void ProfileWin::OKEvent( wxCommandEvent& event )
{
    wxString m_PortNo = wxString::Format( wxT("T%i"), m_passPort );

    DeleteSingleWaypoint( m_PortNo );
    Hide();
    delete m_tList;
    Destroy(); // that hurts
	
}

void ProfileWin::OnCloseWindow( wxCloseEvent& event )
{
    wxString m_PortNo = wxString::Format( wxT("T%i"), m_passPort );

    DeleteSingleWaypoint( m_PortNo );
    Hide();
    delete m_tList;
    Destroy(); // that hurts
}

void ProfileWin::Resize( void )
{
}

void ProfileWin::RePosition( void )
{
//    Position the window
    double lon = -5;
    double lat = 50;

    wxPoint r;
    Move( r );
}

void ProfileWin::OnPaint( wxPaintEvent& event )
{
	//MyTides mTides;
	//wxString tcv_string = wxString::Format(wxT("%05.1f"),(float) tcv[0]);
	//wxMessageBox(tcv_string, _T("Tidal Height"));

	int x, y;
    int i;
    char sbuf[100];
    int w;
    
    GetClientSize( &x, &y );

    wxPaintDC dc( this );

    {
        int x_textbox = x * 5 / 100;
        int y_textbox = 6;

        int x_textbox_w = x * 51 / 100;
        int y_textbox_h = y * 25 / 80;

        // box the location text & tide-current table
        dc.SetPen( *pblack_3 );
        dc.SetBrush( *pltgray2 );

		m_ptextctrl->SetFont(*pVLFont);
		m_ptextctrl->SetForegroundColour(c_black1);
		m_ptextctrl->SetBackgroundColour(c_blue);
		if (m_passName == wxT("")){m_passName = wxT("DOVER");}
		m_ptextctrl->SetLabel( m_passName );

        wxRect tab_rect = m_tList->GetRect();
        dc.DrawRoundedRectangle( tab_rect.x - 4, y_textbox, tab_rect.width + 8, y_textbox_h, 4 ); //tide-current table box

        //    Box the graph
        dc.SetPen( *pblack_1 );
        dc.SetBrush( *pltgray );
        dc.DrawRectangle( m_graph_rect.x, m_graph_rect.y, m_graph_rect.width, m_graph_rect.height );

        //    Horizontal axis
        dc.SetFont( *pSFont );
        for( i = 0; i < m_soundings; i++ ) {
            int xd = m_graph_rect.x + ( ( i ) * m_graph_rect.width / m_soundings );
            dc.DrawLine( xd, m_graph_rect.y, xd, m_graph_rect.y + m_graph_rect.height + 5 );

            char sbuf[5];
            sprintf( sbuf, "%02d", i );
#ifdef __WXMSW__
            wxString sst;
            sst.Printf( _T("%02d"), i );
            dc.DrawRotatedText( sst, xd + ( m_graph_rect.width / m_soundings ) / 2, m_graph_rect.y + m_graph_rect.height + 8, 270. );
#else
            int x_shim = -12;
            dc.DrawText ( wxString ( sbuf, wxConvUTF8 ), xd + x_shim + ( m_graph_rect.width/m_soundings ) /2, m_graph_rect.y + m_graph_rect.height + 8 );
#endif
        }

        //    Make a line for "right now"

		time_t t_now = m_graphday.GetTicks(); // now, in ticks

        float t_ratio = m_graph_rect.width * ( t_now - m_t_graphday_00_at_station ) / ( m_soundings * 3600 );

        //must eliminate line outside the graph (in that case put it outside the window)
        int xnow = ( t_ratio < 0 || t_ratio > m_graph_rect.width ) ? -1 : m_graph_rect.x + (int) t_ratio;
        dc.SetPen( *pred_2 );
        dc.DrawLine( xnow, m_graph_rect.y, xnow, m_graph_rect.y + m_graph_rect.height );
        dc.SetPen( *pblack_1 );
	
        //    Build the array of values, capturing max and min and HW/LW list
	    btc_valid = false;
        if( !btc_valid ) {

            float dir;            
            float val;
            m_tList->Clear();
            int list_index = 0;
            bool wt;
			int i;
			
			for( i = 0; i<12 ; i++){
				if (!m_tclist[i].empty()){
					m_tList->Insert( m_tclist[i], list_index );                       // update table list
					list_index++;
				}
			}
			
//    Set up the vertical parameters based on Tide or Current plot
  
                ib = (int) m_tcmin;
                if( m_tcmin < 0 ) ib -= 1;
                it = (int) m_tcmax + 1;

                im = it - ib; //abs ( ib ) + abs ( it );
                m_plot_y_offset = ( m_graph_rect.height * ( it - ib ) ) / im;
                val_off = ib;

//    Build spline list of points

            m_sList.DeleteContents( true );
            m_sList.Clear();

            for( i = 0; i < m_soundings; i++ ) {
                wxPoint *pp = new wxPoint;
                pp->x = m_graph_rect.x + ( ( i ) * m_graph_rect.width /m_soundings );
                pp->y = m_graph_rect.y + ( m_plot_y_offset )
                - (int) ( ( m_tcv[i] - val_off ) * m_graph_rect.height / im );

                m_sList.Append( pp );
            }

            btc_valid = true;
        }
		wxColour c_black1 = wxColour(0, 0, 0);
        dc.SetTextForeground(c_black1);

        //    Vertical Axis

        //      Maybe skip some lines and legends if the range is too high
        int height_stext;
         dc.GetTextExtent( _T("1"), NULL, &height_stext );

        int i_skip = 1;
        if( height_stext > m_graph_rect.height / im ) i_skip = 2;

        i = ib;
        while( i < it + 1 ) {
            int yd = m_graph_rect.y + ( m_plot_y_offset ) - ( ( i - val_off ) * m_graph_rect.height / im );

            if( ( m_plot_y_offset + m_graph_rect.y ) == yd ) dc.SetPen( *pblack_2 );
            else
                dc.SetPen( *pblack_1 );

            dc.DrawLine( m_graph_rect.x, yd, m_graph_rect.x + m_graph_rect.width, yd );
            snprintf( sbuf, 99, "%d", i );
            dc.DrawText( wxString( sbuf, wxConvUTF8 ), m_graph_rect.x - 20, yd - 5 );
            i += i_skip;

        }

        //    Draw the Value curve
#if wxCHECK_VERSION(2, 9, 0)
        wxPointList *list = (wxPointList *)&m_sList;
#else
        wxList *list = (wxList *) &m_sList;
#endif

        dc.SetPen(  *pblack_2);
#if wxUSE_SPLINES
        dc.DrawSpline( list );
#else
        dc.DrawLines ( list );
#endif
        //  More Info

///
        int station_offset = 0;
        int h = station_offset / 60;
        int m = station_offset - ( h * 60 );
        if( m_graphday.IsDST() ) h += 1;
        m_stz.Printf( _T("Z %+03d:%02d"), h, m );

        dc.SetFont( *pSFont );
        dc.GetTextExtent( m_stz, &w, &h );
        dc.DrawText( m_stz, x / 2 - w / 2, y * 88 / 100 );

        // There seems to be some confusion about format specifiers
        //  Hack this.....
        //  Find and use the longest "sprintf" result......
        wxString sdate;
        wxString s1 = m_graphday.Format( _T ( "%#x" ) );
        wxString s2 = m_graphday.Format( _T ( "%x" ) );

        if( s2.Len() > s1.Len() ) sdate = s2;
        else
            sdate = s1;
        dc.SetFont( *pMFont );
        dc.GetTextExtent( sdate, &w, &h );
        dc.DrawText( sdate, x / 2 - w / 2, y * 92 / 100 );

        ///
       
            dc.GetTextExtent(  wxT("meters"), &w, &h );
            dc.DrawRotatedText(  wxT("meters"), 5,
                                m_graph_rect.y + m_graph_rect.height / 2 + w / 2, 90. );
  

//    Today or tomorrow
			/*
        wxString sday;
        wxDateTime this_now = wxDateTime::Now();

        int day = m_graphday.GetDayOfYear();
        if( m_graphday.GetYear() == this_now.GetYear() ) {
            if( day == this_now.GetDayOfYear() ) sday.Append( _( "Today" ) );
            else if( day == this_now.GetDayOfYear() + 1 ) sday.Append( _( "Tomorrow" ) );
            else
                sday.Append( m_graphday.GetWeekDayName( m_graphday.GetWeekDay() ) );
        } else if( m_graphday.GetYear() == this_now.GetYear() + 1
                   && day == this_now.Add( wxTimeSpan::Day() ).GetDayOfYear() ) sday.Append(
                           _( "Tomorrow" ) );

        dc.SetFont( *pSFont );
        dc.GetTextExtent( sday, &w, &h );
        dc.DrawText( sday, 55 - w / 2, y * 88 / 100 );
*/
    }
	
}


void ProfileWin::OnSize( wxSizeEvent& event )
{
    int width, height;
    GetClientSize( &width, &height );
    int x, y;
    GetPosition( &x, &y );
}

