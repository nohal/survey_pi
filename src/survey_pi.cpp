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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */
#define DATABASE_NAME "survey.sqlite"

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif // precompiled headers

#include "survey_pi.h"
#include "surveygui.h"
#include "surveygui_impl.h"
#include <wx/stdpaths.h>

#include <fstream>
#include <iostream> // std::cout, std::ios
#include <sstream> // std::ostringstream
#include <wx/buffer.h>
#include <wx/file.h>
#include <wx/vector.h>

#include "tinyxml.h"

class soundingdata;

#include <vector>

//  Missing from MSW include files
#ifdef _MSC_VER
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif

wxFont* g_pFontTitle;
wxFont* g_pFontData;
wxFont* g_pFontLabel;
wxFont* g_pFontSmall;

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void* ppimgr)
{
    return new survey_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p) { delete p; }

void appendOSDirSlash(wxString* pString)
{
    wxChar sep = wxFileName::GetPathSeparator();
    if (pString->Last() != sep)
        pString->Append(sep);
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

/**
 * Load a icon, possibly using SVG
 * Parameters
 *  - api_name: Argument to GetPluginDataDir()
 *  - icon_name: Base name of icon living in data/ directory. When using
 *    SVG icon_name.svg is used, otherwise icon_name.png
 */

static wxBitmap load_plugin(const char* icon_name, const char* api_name)
{
    wxBitmap bitmap;
    wxFileName fn;
    auto path = GetPluginDataDir(api_name);
    fn.SetPath(path);
    fn.AppendDir("data");
    fn.SetName(icon_name);
#ifdef SHIPDRIVER_USE_SVG
    wxLogDebug("Loading SVG icon");
    fn.SetExt("svg");
    const static int ICON_SIZE = 48; // FIXME: Needs size from GUI
    bitmap = GetBitmapFromSVGFile(fn.GetFullPath(), ICON_SIZE, ICON_SIZE);
#else
    wxLogDebug("Loading png icon");
    fn.SetExt("png");
    path = fn.GetFullPath();
    if (!wxImage::CanRead(path)) {
        wxLogDebug("Initiating image handlers.");
        wxInitAllImageHandlers();
    }
    wxImage panelIcon(path);
    bitmap = wxBitmap(panelIcon);
#endif
    wxLogDebug("Icon loaded, result: %s", bitmap.IsOk() ? "ok" : "fail");
    return bitmap;
}

survey_pi::survey_pi(void* ppimgr)
    : opencpn_plugin_118(ppimgr)
{
    // Create the PlugIn icons
    initialize_images();
    m_panelBitmap = load_plugin("survey_panel_icon", "survey_pi");

    m_bShowSurvey = false;
}

void survey_pi::ImportHydromagicTrack(TiXmlElement* track)
{

    TiXmlElement* parameter = track->FirstChildElement();
    while (parameter) {
        wxString paramname = wxString::FromUTF8(parameter->Value());
        if (paramname == _T("name")) {
            wxString trackname = wxString::FromUTF8(parameter->GetText());
            m_activesurvey = 1; // CreateSurvey(trackname);
        } else if (paramname == _T("desc")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (paramname == _T("visible")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (paramname == _T("lines")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (paramname == _T("values")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (paramname == _T("active")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (paramname == _T("fixes")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (paramname == _T("color")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (paramname == _T("width")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (paramname == _T("textunits")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (paramname == _T("units")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (paramname == _T("textsize")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (paramname == _T("trackpoints")) {
            int ct = 10000;
            int ic = 0;
            wxString msg, newmsg;
            wxProgressDialog progressdialog(_("Import Hydromagic file"),
                _("Adding soundings"), ct, m_pSurveyDialog,
                wxPD_CAN_ABORT | wxPD_APP_MODAL);

            TiXmlElement* trackpoint = parameter->FirstChildElement();
            while (trackpoint) {
                if (!progressdialog.Update(ic)) {
                    // Cancelled by user.
                    break;
                }
                // wxMessageBox(_T("trackpoint"));
                float lat = atof(trackpoint->Attribute("lat"));
                float lon = atof(trackpoint->Attribute("lon"));
                float dpt = atof(trackpoint->Attribute("dpt"));
                float tide = atof(trackpoint->Attribute("tide"));
                float ele = atof(trackpoint->Attribute("ele"));
                long time = atol(trackpoint->Attribute("time"));
                int mark = atoi(trackpoint->Attribute("mark"));

                InsertSounding(dpt, lat, lon, tide, time, 3395);

                msg = wxString::Format("%i", ic);
                newmsg = msg + "  " + _("soundings have been recorded");
                progressdialog.Update(ic, newmsg);
                ic++;

                trackpoint = trackpoint->NextSiblingElement();
            }
        }
        parameter = parameter->NextSiblingElement();
    }
}

void survey_pi::ExportHydromagic(int survey_id, wxString filename)
{
    TiXmlDocument* doc = new TiXmlDocument();
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "");
    TiXmlElement* root = new TiXmlElement("project");

    doc->LinkEndChild(decl);

    TiXmlElement* projection = new TiXmlElement("projection");
    TiXmlElement* grid = new TiXmlElement("grid");
    grid->LinkEndChild(
        new TiXmlText(wxString::Format(_T("%i"), m_projection).ToUTF8()));
    projection->LinkEndChild(grid);
    root->LinkEndChild(projection);

    TiXmlElement* currentview = new TiXmlElement("currentview");
    root->LinkEndChild(currentview);

    TiXmlElement* colors = new TiXmlElement("colors");
    root->LinkEndChild(colors);

    TiXmlElement* tides = new TiXmlElement("tides");
    root->LinkEndChild(tides);

    TiXmlElement* layers = new TiXmlElement("layers");
    root->LinkEndChild(layers);

    TiXmlElement* comments = new TiXmlElement("comments");
    root->LinkEndChild(comments);

    TiXmlElement* tracks = new TiXmlElement("tracks");
    tracks->SetAttribute("visible", 1);
    TiXmlElement* track = new TiXmlElement("track");

    TiXmlElement* name = new TiXmlElement("name");
    name->LinkEndChild(new TiXmlText(GetSurveyName(survey_id).mb_str()));
    track->LinkEndChild(name);

    TiXmlElement* desc = new TiXmlElement("desc");
    track->LinkEndChild(desc);

    TiXmlElement* visible = new TiXmlElement("visible");
    visible->LinkEndChild(new TiXmlText("1"));
    track->LinkEndChild(visible);

    TiXmlElement* lines = new TiXmlElement("lines");
    lines->LinkEndChild(new TiXmlText("1"));
    track->LinkEndChild(lines);

    TiXmlElement* values = new TiXmlElement("values");
    values->LinkEndChild(new TiXmlText("1"));
    track->LinkEndChild(values);

    TiXmlElement* active = new TiXmlElement("active");
    active->LinkEndChild(new TiXmlText("1"));
    track->LinkEndChild(active);

    TiXmlElement* fixes = new TiXmlElement("fixes");
    fixes->LinkEndChild(new TiXmlText("1"));
    track->LinkEndChild(fixes);

    TiXmlElement* color = new TiXmlElement("color");
    color->LinkEndChild(new TiXmlText("0"));
    track->LinkEndChild(color);

    TiXmlElement* width = new TiXmlElement("width");
    width->LinkEndChild(new TiXmlText("4"));
    track->LinkEndChild(width);

    TiXmlElement* textunits = new TiXmlElement("textunits");
    textunits->LinkEndChild(new TiXmlText("0"));
    track->LinkEndChild(textunits);

    TiXmlElement* units = new TiXmlElement("units");
    units->LinkEndChild(new TiXmlText("0"));
    track->LinkEndChild(units);

    TiXmlElement* textsize = new TiXmlElement("textsize");
    textsize->LinkEndChild(new TiXmlText("12"));
    track->LinkEndChild(textsize);

    TiXmlElement* trackpoints = new TiXmlElement("trackpoints");

    wxString sql
        = wxString::Format(_T("SELECT depth, AsText(geom), tide, measured ")
                           _T("FROM sounding WHERE survey_id=%i"),
            survey_id);
    char** results;
    int n_rows;
    int n_columns;
    char* dpt;
    char* pos;
    char* tid;
    char* tim;

    ret = sqlite3_get_table(
        m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
    if (ret != SQLITE_OK) {
        /* some error occurred */
        wxLogMessage(_T("Spatialite SQL error: %s\n"), err_msg);
        sqlite3_free(err_msg);
        return;
    }

    for (int i = 1; i <= n_rows; i++) {
        dpt = results[(i * n_columns) + 0];
        pos = results[(i * n_columns) + 1];
        tid = results[(i * n_columns) + 2];
        tim = results[(i * n_columns) + 3];
        double tide = atof(tid);
        wxString s(pos, wxConvUTF8);

        int f = s.First(_T(" "));
        int l = s.Len();

        wxString latl;
        wxString lonl;

        lonl = s.Mid(6, f - 6);
        latl = s.Mid(f + 1, l - f - 2);

        wxString t(tim, wxConvUTF8);

        wxDateTime ttt;
        ttt.ParseDateTime(t);

        time_t t32 = ttt.GetTicks();
        int64_t i64 = *(
            (int64_t*)&t32); // To avoid using strftime(), keep everything UTC

        TiXmlElement* trackpoint = new TiXmlElement("trackpoint");
        trackpoint->SetAttribute("lat", latl.ToUTF8());
        trackpoint->SetAttribute("lon", lonl.ToUTF8());
        trackpoint->SetAttribute("dpt", dpt);
        trackpoint->SetAttribute("tide", tid);
        trackpoint->SetAttribute("ele", 0);
        trackpoint->SetAttribute("time", i64);
        trackpoint->SetAttribute("mark", 0);
        trackpoints->LinkEndChild(trackpoint);
    }
    track->LinkEndChild(trackpoints);
    tracks->LinkEndChild(track);
    root->LinkEndChild(tracks);

    TiXmlElement* waypoints = new TiXmlElement("waypoints");
    root->LinkEndChild(waypoints);

    TiXmlElement* routes = new TiXmlElement("routes");
    root->LinkEndChild(routes);

    TiXmlElement* areas = new TiXmlElement("areas");
    root->LinkEndChild(areas);

    TiXmlElement* sections = new TiXmlElement("sections");
    root->LinkEndChild(sections);

    TiXmlElement* soundvelocity = new TiXmlElement("soundvelocity");
    root->LinkEndChild(soundvelocity);

    TiXmlElement* draftcorrections = new TiXmlElement("draftcorrections");
    root->LinkEndChild(draftcorrections);

    doc->LinkEndChild(root);

    doc->SaveFile(filename.mb_str());

    doc->Clear();

    delete doc;
}

void survey_pi::ExportXYZ(int survey_id, wxString filename)
{
    wxString sql
        = wxString::Format(_T("SELECT depth, AsText(geom), tide, measured ")
                           _T("FROM sounding WHERE survey_id=%i"),
            survey_id);
    char** results;
    int n_rows;
    int n_columns;
    char* dpt;
    char* pos;
    char* tid;
    char* tim;

    ret = sqlite3_get_table(
        m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
    if (ret != SQLITE_OK) {
        /* some error occurred */
        wxLogMessage(_T("Spatialite SQL error: %s\n"), err_msg);
        sqlite3_free(err_msg);
        return;
    }

    wxTextFile file(filename);
    file.Open();
    file.Clear();

    wxString sFinal;
    wxString sSpace = _T(" ");

    for (int i = 1; i <= n_rows; i++) {
        dpt = results[(i * n_columns) + 0];
        pos = results[(i * n_columns) + 1];
        tid = results[(i * n_columns) + 2];
        tim = results[(i * n_columns) + 3];
        double tide = atof(tid);
        wxString s(pos, wxConvUTF8);

        int f = s.First(_T(" "));
        int l = s.Len();

        wxString latl;
        wxString lonl;
        wxString depth(dpt, wxConvUTF8);

        wxDateTime dt;
        wxString time(tim, wxConvUTF8);
        dt.ParseDateTime(time);

        wxString str = dt.Format(wxT("%Y-%m-%dT%H:%M:%SZ"));

        lonl = s.Mid(6, f - 6);
        latl = s.Mid(f + 1, l - f - 2);

        sFinal = lonl + sSpace + latl + sSpace + depth + sSpace + str;

        file.AddLine(sFinal);

        /*
        TiXmlElement * trackpoint = new TiXmlElement("trackpoint");
        trackpoint->SetAttribute("lat", latl.ToUTF8());
        trackpoint->SetAttribute("lon", lonl.ToUTF8());
        trackpoint->SetAttribute("dpt", dpt);
        trackpoint->SetAttribute("tide", tid);
        trackpoint->SetAttribute("ele", 0);
        trackpoint->SetAttribute("time", tim);
        trackpoint->SetAttribute("mark", 0);
        trackpoints->LinkEndChild(trackpoint);
        */
    }

    file.Write();
    file.Close();
}

void survey_pi::ExportCSV(int survey_id, wxString filename)
{
    wxString sql
        = wxString::Format(_T("SELECT depth, AsText(geom), tide, measured ")
                           _T("FROM sounding WHERE survey_id=%i"),
            survey_id);
    char** results;
    int n_rows;
    int n_columns;
    char* dpt;
    char* pos;
    char* tid;
    char* tim;

    ret = sqlite3_get_table(
        m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
    if (ret != SQLITE_OK) {
        /* some error occurred */
        wxLogMessage(_T("Spatialite SQL error: %s\n"), err_msg);
        sqlite3_free(err_msg);
        return;
    }

    wxTextFile file(filename);
    file.Open();
    file.Clear();

    wxString sFinal;
    wxString sComma = _T(",");

    for (int i = 1; i <= n_rows; i++) {
        dpt = results[(i * n_columns) + 0];
        pos = results[(i * n_columns) + 1];
        tid = results[(i * n_columns) + 2];
        tim = results[(i * n_columns) + 3];
        double tide = atof(tid);
        wxString s(pos, wxConvUTF8);

        int f = s.First(_T(" "));
        int l = s.Len();

        wxString latl;
        wxString lonl;
        wxString depth(dpt, wxConvUTF8);

        wxDateTime dt;
        wxString time(tim, wxConvUTF8);
        dt.ParseDateTime(time);

        wxString str = dt.Format(wxT("%Y-%m-%dT%H:%M:%SZ"));

        latl = s.Mid(6, f - 6);
        lonl = s.Mid(f + 1, l - f - 2);

        sFinal = latl + sComma + lonl + sComma + depth + sComma + str;

        file.AddLine(sFinal);

        /*
        TiXmlElement * trackpoint = new TiXmlElement("trackpoint");
        trackpoint->SetAttribute("lat", latl.ToUTF8());
        trackpoint->SetAttribute("lon", lonl.ToUTF8());
        trackpoint->SetAttribute("dpt", dpt);
        trackpoint->SetAttribute("tide", tid);
        trackpoint->SetAttribute("ele", 0);
        trackpoint->SetAttribute("time", tim);
        trackpoint->SetAttribute("mark", 0);
        trackpoints->LinkEndChild(trackpoint);
        */
    }
    file.Write();
    file.Close();
}
bool survey_pi::ImportHydromagic(wxString filename)
{
    TiXmlDocument* doc = new TiXmlDocument();
    doc->LoadFile(filename.mb_str());
    TiXmlElement* project = doc->FirstChildElement();
    if (!project) {
        doc->Clear();
        delete doc;
        return false;
    }
    TiXmlElement* section = project->FirstChildElement();

    while (section) {
        wxString secname = wxString::FromUTF8(section->Value());
        if (secname == _T("projection")) {
            const char* val = section->FirstChildElement()->GetText();
            m_projection = atoi(val);
        } else if (secname == _T("currentview")) {
            // TODO: We do not need this, so let's ignore it for now
            /*
             <currentview>
              <MinX>-13631297.850343</MinX>
              <MinY>4526885.079842</MinY>
              <MaxX>-13630637.648546</MaxX>
              <MaxY>4527386.011166</MaxY>
              <DelX>660.201797</DelX>
              <DelY>500.931324</DelY>
              <Skew>0.00</Skew>
             </currentview>
            */
        } else if (secname == _T("colors")) {
            // TODO: We do not need this, so let's ignore it for now
            /*
             <colors>
              <color lower="0.00" upper="1.00" value="FF3F3F" />
              <color lower="1.00" upper="2.00" value="FF653F" />
              <color lower="2.00" upper="3.00" value="FF8C3F" />
              <color lower="3.00" upper="4.00" value="FFB23F" />
              <color lower="4.00" upper="5.00" value="FFD83F" />
              <color lower="5.00" upper="6.00" value="FFFF3F" />
              <color lower="6.00" upper="7.00" value="D8FF3F" />
              <color lower="7.00" upper="8.00" value="B2FF3F" />
              <color lower="8.00" upper="9.00" value="8CFF3F" />
              <color lower="9.00" upper="10.00" value="65FF3F" />
              <color lower="10.00" upper="11.00" value="3FFF3F" />
              <color lower="11.00" upper="12.00" value="3FFF66" />
              <color lower="12.00" upper="13.00" value="3FFF8C" />
              <color lower="13.00" upper="14.00" value="3FFFB2" />
              <color lower="14.00" upper="15.00" value="3FFFD8" />
              <color lower="15.00" upper="16.00" value="3FFFFF" />
              <color lower="16.00" upper="17.00" value="3FD8FF" />
              <color lower="17.00" upper="18.00" value="3FB2FF" />
              <color lower="18.00" upper="19.00" value="3F8CFF" />
              <color lower="19.00" upper="20.00" value="3F66FF" />
             </colors>
            */
        } else if (secname == _T("tides")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (secname == _T("layers")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (secname == _T("comments")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (secname == _T("tracks")) {
            TiXmlElement* track = section->FirstChildElement();
            while (track) {
                ImportHydromagicTrack(track);
                track = track->NextSiblingElement();
            }
        } else if (secname == _T("waypoints")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (secname == _T("routes")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (secname == _T("areas")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (secname == _T("sections")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (secname == _T("soundvelocity")) {
            // TODO: We do not need this, so let's ignore it for now
        } else if (secname == _T("draftcorrections")) {
            // TODO: We do not need this, so let's ignore it for now
        }
        section = section->NextSiblingElement();
    }

    doc->Clear();

    delete doc;
    return true;
}

bool survey_pi::ImportXYZ(wxString filename)
{

    wxString str;
    wxTextFile m_istream;
    wxString token[40];
    wxString s0, s1, m_sDT;
    wxDateTime dt;
    double lat;
    double lon;
    double depth;

    int l = 0;

    m_istream.Open(filename);

    int ct = 10000;
    int ic = 0;
    wxString msg, newmsg;
    wxProgressDialog progressdialog(_("Import XYZ file"), _("Adding soundings"),
        ct, m_pSurveyDialog, wxPD_CAN_ABORT | wxPD_APP_MODAL);

    for (str = m_istream.GetFirstLine(); !m_istream.Eof();
         str = m_istream.GetNextLine()) {
        if (!progressdialog.Update(ic)) {
            // Cancelled by user.
            break;
        }

        wxStringTokenizer tokenizer(str, wxT(" "));
        int i = 0;
        while (tokenizer.HasMoreTokens()) {
            token[i] = tokenizer.GetNextToken();
            i++;
        }

        token[0].ToDouble(&lon);
        token[1].ToDouble(&lat);
        token[2].ToDouble(&depth);

        s0 = token[3].Mid(0, 10);
        s1 = token[3].Mid(11, 8);
        m_sDT = s0 + _T(" ") + s1;

        dt.ParseDateTime(m_sDT); // When not testing

        time_t t = dt.GetTicks();
        int64_t tim = *((int64_t*)&t);

        InsertSounding(depth, lat, lon, 0.0, t, 3395);

        msg = wxString::Format("%i", ic);
        newmsg = msg + "  " + _("soundings have been recorded");
        progressdialog.Update(ic, newmsg);
        ic++;
    }

    m_istream.Close();
    return true;
}

bool survey_pi::ImportCSV(wxString filename)
{

    wxString str;
    wxTextFile m_istream;
    wxString token[4];
    wxString s0, s1, m_sDT;
    wxDateTime dt;
    double lat;
    double lon;
    double depth;

    int l = 0;

    m_istream.Open(filename);

    int lc = 0;
    int ct = 0;
    m_pSurveyDialog->soundingCount = 0;
    wxString msg, newmsg;
    lc = m_istream.GetLineCount();

    wxProgressDialog progressdialog(_("Import CSV file"), _("Adding soundings"),
        lc, m_parent_window, wxPD_CAN_ABORT | wxPD_APP_MODAL);

    for (str = m_istream.GetFirstLine(); !m_istream.Eof();
         str = m_istream.GetNextLine()) {

        if (!progressdialog.Update(ct)) {
            // Cancelled by user.
            break;
        }

        wxStringTokenizer tokenizer(str, wxT(","));
        int i = 0;
        while (tokenizer.HasMoreTokens()) {
            token[i] = tokenizer.GetNextToken();
            i++;
        }

        token[0].ToDouble(&lon);
        token[1].ToDouble(&lat);
        token[2].ToDouble(&depth);

        s0 = token[3].Mid(0, 10);
        s1 = token[3].Mid(11, 8);
        m_sDT = s0 + _T(" ") + s1;

        dt.ParseDateTime(m_sDT); // When not testing

        time_t t = dt.GetTicks();
        int64_t tim = *((int64_t*)&t);

        InsertSounding(depth, lat, lon, 0.0, t, 3395);

        msg = wxString::Format("%i", m_pSurveyDialog->soundingCount);
        newmsg = msg + "  " + _("soundings have been recorded");
        progressdialog.Update(ct, newmsg);
        ct++;
    }
    m_istream.Close();
    return true;
}

bool survey_pi::ImportGPX(wxString filename)
{
    m_pSurveyDialog->trkpts.clear();
    m_pSurveyDialog->rtepts.clear();

    int count;
    TiXmlElement* e;
    TiXmlElement* f;
    TiXmlElement* g;
    TiXmlElement* h;
    TiXmlElement* j;
    TiXmlElement* k;

    TiXmlAttribute* t;

    std::ostringstream ostrm;
    std::ifstream inFile(filename.mb_str());
    ostrm << inFile.rdbuf();

    TiXmlDocument doc;
    doc.Parse(ostrm.str().c_str());

    inFile.close();

    TiXmlElement* pRoot = doc.RootElement();

    for (TiXmlElement* e = pRoot->FirstChildElement(); e;
         e = e->NextSiblingElement())
        count++;
    int i = 0;
    for (e = pRoot->FirstChildElement(); e; e = e->NextSiblingElement(), i++) {
        wxString s = wxString(e->Value(), wxConvUTF8);
        // wxMessageBox(s);

        for (f = e->FirstChildElement(); f; f = f->NextSiblingElement()) {
            wxString s1 = wxString(f->Value(), wxConvUTF8);
            // wxMessageBox(s1);

            for (g = f->FirstChildElement(); g; g = g->NextSiblingElement()) {
                wxString s2 = wxString(g->Value(), wxConvUTF8);
                // wxMessageBox(s2);

                // s2 = wxString(t->Value(), wxConvUTF8);
                if (s2 == _T("trkpt")) {
                    const char* c = g->Attribute("lat");
                    if (c != NULL) {
                        s2 = wxString(c, wxConvUTF8);
                        m_pSurveyDialog->trkpt.lat = s2;
                    }
                    // wxMessageBox(s2);
                    const char* c1 = g->Attribute("lon");
                    if (c1 != NULL) {
                        s2 = wxString(c1, wxConvUTF8);
                        m_pSurveyDialog->trkpt.lon = s2;
                    }
                    // wxMessageBox(s2);
                }
                for (h = g->FirstChildElement(); h;
                     h = h->NextSiblingElement()) {
                    wxString s3 = wxString(h->Value(), wxConvUTF8);

                    // wxMessageBox(s3);

                    if (s3 == _T("time")) {
                        wxString tm = wxString(h->GetText(), wxConvUTF8);
                        m_pSurveyDialog->trkpt.time = tm;
                    }

                    if (s3 == _T("extensions")) {

                        for (j = h->FirstChildElement(); j;
                             j = j->NextSiblingElement()) {
                            wxString s4 = wxString(j->Value(), wxConvUTF8);
                            // wxMessageBox(s4);

                            for (k = j->FirstChildElement(); k;
                                 k = k->NextSiblingElement()) {
                                wxString s5 = wxString(k->Value(), wxConvUTF8);
                                // wxMessageBox(s5, _T("temp"));
                                if (s5 == _T("gpxtpx:depth")
                                    || s5 == _T("gpxx:Depth")
                                    || s5 == _T("gpxx:depth")) {
                                    s5 = wxString(k->GetText(), wxConvUTF8);
                                    m_pSurveyDialog->trkpt.depth = s5;
                                    m_pSurveyDialog->trkpts.push_back(
                                        m_pSurveyDialog->trkpt);
                                }
                                //
                            }
                        }
                    }
                }
            }
        }
    }

    int ct = m_pSurveyDialog->trkpts.size();

    wxProgressDialog progressdialog(_("Import Garmin GPX"),
        _("Adding soundings"), ct, m_pSurveyDialog,
        wxPD_CAN_ABORT | wxPD_APP_MODAL);

    double depth, lat, lon;
    wxDateTime dt;
    wxString m_sDT, sTime, s0, s1, msg, newmsg;
    int ic = 0;

    for (std::vector<GPXWayPoint>::iterator it
         = m_pSurveyDialog->trkpts.begin();
         it != m_pSurveyDialog->trkpts.end(); it++) {

        if (!progressdialog.Update(ic)) {
            // Cancelled by user.
            break;
        }

        msg = wxString::Format("%i", ic);
        newmsg = msg + "  " + _("soundings have been recorded");
        progressdialog.Update(ic, newmsg);
        ic++;

        wxString dep((*it).depth);
        dep.ToDouble(&depth);
        depth += m_fWaterlineOffset;

        dep = (*it).lat;
        dep.ToDouble(&lat);

        dep = (*it).lon;
        dep.ToDouble(&lon);

        sTime = (*it).time;

        s0 = sTime.Mid(0, 10);
        s1 = sTime.Mid(11, 8);
        m_sDT = s0 + _T(" ") + s1;

        dt.ParseDateTime(m_sDT);

        time_t t = dt.GetTicks();
        int64_t tim = *((int64_t*)&t);

        InsertSounding(depth, lat, lon, 0.0, t, 3395);
    }
    return true;
}

bool survey_pi::dbMergeSurveys(int survey1, int survey2)
{

    wxString sql = wxString::Format(
        _T("SELECT depth, AsText(geom), tide, strftime('%%s', measured) FROM ")
        _T("sounding WHERE survey_id=%i"),
        survey1);
    char** results;
    int n_rows;
    int n_columns;
    char* dpt;
    // char *pos;
    char* tid;
    char* tim;

    ret = sqlite3_get_table(
        m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
    if (ret != SQLITE_OK) {
        /* some error occurred */
        wxLogMessage(_T("Spatialite SQL error: %s\n"), err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    int i = 0;
    double lat, lon;
    wxString s0, s1;
    double depth;
    double tide;

    int projection = 3395;
    for (i = 1; i <= n_rows; i++) {
        dpt = results[(i * n_columns) + 0];
        wxString dep(dpt, wxConvUTF8);
        dep.ToDouble(&depth);

        tim = results[(i * n_columns) + 3];
        wxString time(tim, wxConvUTF8);

        char* geom = results[(i * n_columns) + 1];
        wxString point(geom, wxConvUTF8);

        int f = point.Find(_T(" "));
        int l = point.Len();

        s0 = point.Mid(6, f - 6);
        s0.ToDouble(&lon);

        s1 = point.Mid(f + 1, l - f - 2);
        s1.ToDouble(&lat);

        tid = results[(i * n_columns) + 2];
        wxString ti(tid, wxConvUTF8);
        ti.ToDouble(&tide);

        wxString sql = wxString::Format(
            _T("INSERT INTO \"sounding\" (\"depth\", \"measured\", ")
            _T("\"survey_id\", \"geom\", \"tide\") VALUES (%f , %s, %i, ")
            _T("GeomFromText('POINT(%f %f)', %i), %f)"),
            depth, time, survey2, lon, lat, projection, tide);
        dbQuery(sql);
    }
    return true;
}

bool survey_pi::dbEditSoundings(int survey1)
{

    int surv_id;
    wxString rawSurveyName, editedSurveyName;
    rawSurveyName = GetSurveyName(survey1);
    editedSurveyName = _T("Edt.") + rawSurveyName;
    CreateSurvey(editedSurveyName);
    SetActiveSurvey(editedSurveyName);

    surv_id = GetSurveyId(editedSurveyName);

    wxString sql = wxString::Format(
        _T("SELECT depth, AsText(geom), tide, datetime(measured) FROM ")
        _T("sounding WHERE survey_id=%i"),
        survey1);
    char** results;
    int n_rows;
    int n_columns;
    char* dpt;
    // char *pos;
    char* tid;
    char* tim;
    string measuredTime;

    ret = sqlite3_get_table(
        m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
    if (ret != SQLITE_OK) {
        /* some error occurred */
        wxLogMessage(_T("Spatialite SQL error: %s\n"), err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    int i = 0;
    double lat, lon;
    wxString s0, s1;
    double depth;
    double tide;
    double offset = 0;

    int projection = 3395;
    for (i = 1; i <= n_rows; i++) {
        dpt = results[(i * n_columns) + 0];
        wxString dep(dpt, wxConvUTF8);
        dep.ToDouble(&depth);

        tim = results[(i * n_columns) + 3];
        wxString time(tim, wxConvUTF8);

        wxDateTime dt;
        dt.ParseDateTime(time);

        double depthCorrection = dbGetHeightOfTide(dt, survey1);
        depth += depthCorrection;

        // wxString depthOut = wxString::Format("%5.2f", depthCorrection);
        // wxMessageBox(depthOut);

        time_t timestamp = dt.GetTicks();

        if (timestamp > 0) {
            time = _T("'")
                + wxDateTime(timestamp).FormatISODate().Append(_T(" ")).Append(
                    wxDateTime(timestamp).FormatISOTime())
                + _T("'");
        }

        // wxMessageBox(time);

        char* geom = results[(i * n_columns) + 1];
        wxString point(geom, wxConvUTF8);

        int f = point.Find(_T(" "));
        int l = point.Len();

        s0 = point.Mid(6, f - 6);
        s0.ToDouble(&lon);

        s1 = point.Mid(f + 1, l - f - 2);
        s1.ToDouble(&lat);

        // tid = results[(i * n_columns) + 2];
        // wxString ti(tid, wxConvUTF8);
        // ti.ToDouble(&tide);

        if (time != wxEmptyString) {
            wxString sql = wxString::Format(
                _T("INSERT INTO \"edited_sounding\" (\"depth\", \"measured\", ")
                _T("\"survey_id\", \"geom\", \"tide\") VALUES (%f , %s, %i, ")
                _T("GeomFromText('POINT(%f %f)', %i), %f)"),
                depth, time, surv_id, lon, lat, projection, depthCorrection);
            // wxMessageBox(sql);
            dbQuery(sql);
        }
    }
    SetActiveSurveyId(surv_id);
    GetEditedSoundings(surv_id);

    FillSurveyDropdown();
    FillSurveyGrid();
    FillEditSurveyInfo();

    return 1;
}

double survey_pi::dbGetHeightOfTide(wxDateTime depthTime, int surv)
{

    wxString sql = wxString::Format(
        _T("SELECT measured, height FROM tide_height WHERE survey_id=%i"),
        surv);
    char** results;
    int n_rows;
    int n_columns;
    char* dpt;
    // char *pos;
    char* tid;
    char* tim;
    string measuredTime;

    wxDateTime dt, prevTime, nextTime, foundTime;
    double prevDepth, nextDepth;

    // wxString zeroDate = "1970-01-01 00:00:00";
    // prevTime.ParseDateTime(zeroDate);

    ret = sqlite3_get_table(
        m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
    if (ret != SQLITE_OK) {
        /* some error occurred */
        wxLogMessage(_T("Spatialite SQL error: %s\n"), err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    int i = 0;
    int t = 0;
    double depth;
    double tide;
    double offset = 0;

    if (n_rows == 0) {
        wxMessageBox(_("No tides available for this survey"));
        return 999.9;
    }

    for (i = 1; i <= n_rows; i++) {

        if (n_rows == 1) {
            dpt = results[(i * n_columns) + 1];
            wxString dep(dpt, wxConvUTF8);
            dep.ToDouble(&depth);
            return depth;
        } else {

            tim = results[(i * n_columns) + 0];
            wxString time(tim, wxConvUTF8);

            dt.ParseDateTime(time);

            if (dt.IsEarlierThan(depthTime)) {
                prevTime = dt;
                dpt = results[(i * n_columns) + 1];
                wxString dep(dpt, wxConvUTF8);
                dep.ToDouble(&prevDepth);
            }

            if (dt.IsLaterThan(depthTime)) {
                nextTime = dt;
                dpt = results[(i * n_columns) + 1];
                wxString dep(dpt, wxConvUTF8);
                dep.ToDouble(&nextDepth);

                // wxString depthOut = wxString::Format("%5.2f", nextDepth);
                // wxMessageBox(depthOut);

                break;
            }
        }
    }

    wxTimeSpan tsp = depthTime - prevTime;
    wxTimeSpan tsn = nextTime - depthTime;

    if (tsp == 0 || tsn == 0) {
        foundTime = depthTime;
    } else {

        if (tsp < tsn) {
            // wxMessageBox("tsp < tsn");
            foundTime = prevTime;
            return prevDepth;

        } else if (tsn < tsp) {
            // wxMessageBox("tsn < tsp");
            foundTime = nextTime;
            return nextDepth;
        }
    }

    return 999.9;
}

bool survey_pi::dbSaveTideHeights(int survey1)
{

    int surv_id = survey1;
    int n_rows;
    int i = 0;
    double tideheight, totalcorrection;
    wxString tidetime, time;
    wxDateTime dt;

    n_rows = m_pSurveyDialog->m_pSurveyTidalDialog->m_gdTidalHeights
                 ->GetNumberRows();

    if (n_rows == 0) {
        wxMessageBox(_("No tides entered"));
        return 0;
    }

    int projection = 3395;
    for (i = 0; i < n_rows; i++) {

        tidetime = m_pSurveyDialog->m_pSurveyTidalDialog->m_gdTidalHeights
                       ->GetCellValue(i, 0);
        dt.ParseDateTime(tidetime);

        time_t timestamp = dt.GetTicks();

        if (timestamp > 0) {
            time = _T("'")
                + wxDateTime(timestamp).FormatISODate().Append(_T(" ")).Append(
                    wxDateTime(timestamp).FormatISOTime())
                + _T("'");
        }

        wxString ti = m_pSurveyDialog->m_pSurveyTidalDialog->m_gdTidalHeights
                          ->GetCellValue(i, 1);
        wxString tc = m_pSurveyDialog->m_pSurveyTidalDialog->m_gdTidalHeights
                          ->GetCellValue(i, 2);

        // the tide heights to apply
        if (!ti.IsEmpty()) {
            ti.ToDouble(&tideheight);
        } else {
            tideheight = 0;
        }
        // the corrections to apply
        if (!tc.IsEmpty()) {
            tc.ToDouble(&totalcorrection);
        } else {
            totalcorrection = 0;
        }

        tideheight += totalcorrection;

        if (tidetime != wxEmptyString) {
            wxString sql = wxString::Format(
                _T("INSERT INTO \"tide_height\" (\"measured\", \"height\", ")
                _T("\"survey_id\") VALUES (%s , %f, %i)"),
                time, tideheight, surv_id);
            // wxMessageBox(sql);
            dbQuery(sql);
        }
    }

    return 1;
}

bool survey_pi::dbQuery(wxString sql)
{
    if (!b_dbUsable)
        return false;
    ret = sqlite3_exec(m_database, sql.mb_str(), NULL, NULL, &err_msg);
    if (ret != SQLITE_OK) {
        // some error occurred
        wxLogMessage(
            _T("Database error: %s in query: %s\n"), err_msg, sql.c_str());
        sqlite3_free(err_msg);
        b_dbUsable = false;
    }
    return b_dbUsable;
}

void survey_pi::dbGetTable(
    wxString sql, char*** results, int& n_rows, int& n_columns)
{
    ret = sqlite3_get_table(
        m_database, sql.mb_str(), results, &n_rows, &n_columns, &err_msg);
    if (ret != SQLITE_OK) {
        wxLogMessage(
            _T("Database error: %s in query: %s\n"), err_msg, sql.c_str());
        sqlite3_free(err_msg);
        b_dbUsable = false;
    }
}

wxArrayString survey_pi::GetSurveyList()
{
    char** result;
    int n_rows;
    int n_columns;

    dbGetTable(_T("SELECT * FROM survey"), &result, n_rows, n_columns);
    wxArrayString surveys;
    for (int i = 1; i <= n_rows; i++) {
        char* id = result[(i * n_columns) + 0];
        char* name = result[(i * n_columns) + 1];
        int survey_id = atoi(id);
        wxString survey_name(name, wxConvUTF8);
        surveys.Add(survey_name);
    }
    dbFreeResults(result);
    return surveys;
}

vector<soundingdata> survey_pi::SetTable(int i)
{
    char** result;
    int n_rows = 0;
    int n_columns;
    soundingdata mysounding;

    all_soundingdata.clear();

    // wxString myInt = wxString::Format(wxT("%i"), i);

    wxString myString = wxString::Format(
        _T("SELECT sounding_id, depth, measured, AsText(geom), tide FROM ")
        _T("sounding WHERE survey_id = %i"),
        i);
    dbGetTable(myString, &result, n_rows, n_columns);
    wxArrayString soundings;
    wxString s0, s1;
    double lat, lon;
    if (n_rows == 0) {
        return all_soundingdata;
    }
    for (int i = 1; i <= n_rows; i++) {

        char* id = result[(i * n_columns) + 0];

        int sounding_id = atoi(id);
        mysounding.sdgid = sounding_id;

        char* depth = result[(i * n_columns) + 1];

        wxString sounding(depth, wxConvUTF8);
        soundings.Add(sounding);
        mysounding.depth = sounding;

        char* measured = result[(i * n_columns) + 2];

        wxString time_measured(measured, wxConvUTF8);
        mysounding.time = time_measured;

        char* geom = result[(i * n_columns) + 3];

        char* tide = result[(i * n_columns) + 4];
        wxString mt(tide, wxConvUTF8);

        mysounding.tide = mt;

        wxString point(geom, wxConvUTF8);
        // wxMessageBox(point);

        int f = point.Find(_T(" "));
        int l = point.Len();

        s0 = point.Mid(6, f - 6);
        mysounding.lon = s0;

        s0.ToDouble(&lon);
        mysounding.lonD = lon;

        s1 = point.Mid(f + 1, l - f - 2);
        mysounding.lat = s1;

        s1.ToDouble(&lat);
        mysounding.latD = lat;

        all_soundingdata.push_back(mysounding);
    }
    // dbFreeResults(result);
    return all_soundingdata;
}

vector<soundingdata> survey_pi::SetEditTable(int i)
{
    char** result;
    int n_rows = 0;
    int n_columns;
    soundingdata mysounding;

    all_soundingdata.clear();

    // wxString myInt = wxString::Format(wxT("%i"), i);

    wxString myString = wxString::Format(
        _T("SELECT sounding_id, depth, measured, AsText(geom), tide FROM ")
        _T("edited_sounding WHERE survey_id = %i"),
        i);
    dbGetTable(myString, &result, n_rows, n_columns);
    wxArrayString soundings;
    wxString s0, s1;
    double lat, lon;
    if (n_rows == 0) {
        return all_soundingdata;
    }
    for (int i = 1; i <= n_rows; i++) {

        char* id = result[(i * n_columns) + 0];

        int sounding_id = atoi(id);
        mysounding.sdgid = sounding_id;

        char* depth = result[(i * n_columns) + 1];

        wxString sounding(depth, wxConvUTF8);
        soundings.Add(sounding);
        mysounding.depth = sounding;

        char* measured = result[(i * n_columns) + 2];

        wxString time_measured(measured, wxConvUTF8);
        mysounding.time = time_measured;

        char* geom = result[(i * n_columns) + 3];

        char* tide = result[(i * n_columns) + 4];
        wxString mt(tide, wxConvUTF8);

        mysounding.tide = mt;

        wxString point(geom, wxConvUTF8);
        // wxMessageBox(point);

        int f = point.Find(_T(" "));
        int l = point.Len();

        s0 = point.Mid(6, f - 6);
        mysounding.lon = s0;

        s0.ToDouble(&lon);
        mysounding.lonD = lon;

        s1 = point.Mid(f + 1, l - f - 2);
        mysounding.lat = s1;

        s1.ToDouble(&lat);
        mysounding.latD = lat;

        all_soundingdata.push_back(mysounding);
    }
    // dbFreeResults(result);
    return all_soundingdata;
}

wxArrayString survey_pi::SetSoundings(int i)
{
    char** result;
    int n_rows;
    int n_columns;

    // wxString myInt = wxString::Format(wxT("%i"),i);
    wxString myString = wxString::Format(
        _T("SELECT * FROM sounding WHERE survey_id = %i"), i);
    dbGetTable(myString, &result, n_rows, n_columns);
    wxArrayString soundings;
    for (int i = 1; i <= n_rows; i++) {
        char* id = result[(i * n_columns) + 0];
        char* depth = result[(i * n_columns) + 1];
        int survey_id = atoi(id);
        wxString sounding(depth, wxConvUTF8);
        soundings.Add(sounding);
    }
    dbFreeResults(result);
    return soundings;
}

wxString survey_pi::dbGetBeginDate(int surv)
{

    char** result;
    int n_rows;
    int n_columns;
    char* measured;

    wxString sql = wxString::Format(
        "SELECT measured FROM sounding WHERE survey_id = %i and sounding_id = "
        "(SELECT min(sounding_id) FROM sounding WHERE survey_id = %i ) ",
        surv, surv);
    wxString s0, s1;
    ret = sqlite3_get_table(
        m_database, sql.mb_str(), &result, &n_rows, &n_columns, &err_msg);
    if (ret != SQLITE_OK) {
        /* some error occurred */
        wxLogMessage(_T("Spatialite SQL error: %s\n"), err_msg);
        sqlite3_free(err_msg);
        return "error";
    }
    for (int i = 1; i <= n_rows; i++) {
        measured = result[(i * n_columns) + 0];
    }

    dbFreeResults(result);

    wxString time_measured(measured, wxConvUTF8);
    return time_measured;
}

wxString survey_pi::dbGetEndDate(int surv)
{
    char** result;
    int n_rows;
    int n_columns;
    char* measured;

    wxString sql = wxString::Format(
        "SELECT measured FROM sounding WHERE survey_id = %i and sounding_id = "
        "(SELECT max(sounding_id) FROM sounding WHERE survey_id = %i )",
        surv, surv);
    wxString s0, s1;
    ret = sqlite3_get_table(
        m_database, sql.mb_str(), &result, &n_rows, &n_columns, &err_msg);
    if (ret != SQLITE_OK) {
        /* some error occurred */
        wxLogMessage(_T("Spatialite SQL error: %s\n"), err_msg);
        sqlite3_free(err_msg);
        return "error";
    }

    // wxString test1 = wxString::Format("%i", n_rows);
    // wxMessageBox(test1);

    // wxString test2 = wxString::Format("%i", n_columns);
    // wxMessageBox(test2);

    for (int i = 1; i <= n_rows; i++) {
        measured = result[(i * n_columns) + 0];
    }

    dbFreeResults(result);

    wxString time_measured(measured, wxConvUTF8);
    return time_measured;
}

wxString survey_pi::dbGetStringValue(wxString sql)
{
    char** result;
    int n_rows;
    int n_columns;
    dbGetTable(sql, &result, n_rows, n_columns);
    wxArrayString surveys;
    if (result) {
        wxString ret = wxString::FromUTF8(result[1]);
        dbFreeResults(result);
        if (n_rows == 1)
            return ret;
    }
    return wxEmptyString;
}

int survey_pi::dbGetIntNotNullValue(wxString sql)
{
    char** result;
    int n_rows;
    int n_columns;
    dbGetTable(sql, &result, n_rows, n_columns);
    wxArrayString surveys;
    if (result) {
        int ret = atoi(result[1]);
        dbFreeResults(result);
        if (n_rows == 1)
            return ret;
    }
    return 0;
}

wxString survey_pi::GetSurveyName(int survey_id)
{
    return dbGetStringValue(wxString::Format(
        _T("SELECT survey_name FROM survey WHERE survey_id=%i"), survey_id));
}

wxString survey_pi::GetSurveyMaxDepth(int survey_id)
{
    return dbGetStringValue(wxString::Format(
        _T("SELECT max(depth) FROM sounding  WHERE survey_id=%i"), survey_id));
}

wxString survey_pi::GetSurveyMinDepth(int survey_id)
{
    return dbGetStringValue(wxString::Format(
        _T("SELECT min(depth) FROM sounding WHERE survey_id=%i"), survey_id));
}

wxString survey_pi::GetSurveyNumSoundings(int survey_id)
{
    return dbGetStringValue(wxString::Format(
        _T("SELECT count(depth) FROM sounding WHERE survey_id=%i"), survey_id));
}

wxString survey_pi::GetSurveyAreaSoundings(int survey_id)
{

    return dbGetStringValue(wxString::Format(
        _T("SELECT SUM(AREA(geom)) FROM sounding WHERE survey_id=%i"),
        survey_id));
}

wxString survey_pi::GetEditSurveyMaxDepth(int survey_id)
{
    return dbGetStringValue(wxString::Format(
        _T("SELECT max(depth) FROM edited_sounding  WHERE survey_id=%i"),
        survey_id));
}

wxString survey_pi::GetEditSurveyMinDepth(int survey_id)
{
    return dbGetStringValue(wxString::Format(
        _T("SELECT min(depth) FROM edited_sounding WHERE survey_id=%i"),
        survey_id));
}

wxString survey_pi::GetEditSurveyNumSoundings(int survey_id)
{
    return dbGetStringValue(wxString::Format(
        _T("SELECT count(depth) FROM edited_sounding WHERE survey_id=%i"),
        survey_id));
}

wxString survey_pi::GetEditSurveyAreaSoundings(int survey_id)
{

    return dbGetStringValue(wxString::Format(
        _T("SELECT SUM(AREA(geom)) FROM edited_sounding WHERE survey_id=%i"),
        survey_id));
}

int survey_pi::GetSurveyId(wxString survey_name)
{
    return dbGetIntNotNullValue(wxString::Format(
        _T("SELECT survey_id FROM survey WHERE survey_name='%s'"),
        survey_name.c_str()));
}

void survey_pi::dbFreeResults(char** results) { sqlite3_free_table(results); }

int survey_pi::CreateSurvey(wxString name)
{
    wxString sql
        = wxString::Format(_T("INSERT INTO survey (survey_name, created, ")
                           _T("submitted) VALUES ('%s', current_timestamp, 0)"),
            name.c_str());
    dbQuery(sql);
    return sqlite3_last_insert_rowid(m_database);
}

void survey_pi::DeleteSurvey(int id)
{
    wxString s = m_pSurveyDialog->m_chSurvey->GetStringSelection();
    int c = m_pSurveyDialog->m_chSurvey->GetCount();
    if (c != 0) {
        wxString testEdit = s.Mid(0, 4);
        if (testEdit == _T("Edt.")) {
            wxString sql = wxString::Format(
                _T("DELETE FROM edited_sounding WHERE survey_id=%i"), id);
            dbQuery(sql);
            sql = wxString::Format(
                _T("DELETE FROM survey WHERE survey_id=%i"), id);
            dbQuery(sql);
        } else {
            wxString sql = wxString::Format(
                _T("DELETE FROM sounding WHERE survey_id=%i"), id);
            dbQuery(sql);
            sql = wxString::Format(
                _T("DELETE FROM survey WHERE survey_id=%i"), id);
            dbQuery(sql);
        }
        m_activesurvey = 0;
        m_activesurveyname = wxEmptyString;
        m_pSurveyDialog->mysoundings.clear();
        FillSurveyDropdown();
        FillSurveyGrid();
        FillSurveyInfo();

        return;
    } else {
        wxMessageBox(_("No surveys have been made"));
    }
}

void survey_pi::DeleteSounding(int id)
{
    wxString s = m_pSurveyDialog->m_chSurvey->GetStringSelection();

    int t = GetSurveyId(s);
    SetActiveSurvey(s);

    if (m_activesurvey != 0) {
        wxString surveyName = GetSurveyName(m_activesurvey);
        if (s.length() > 4) {
            wxString testEdit = surveyName.Mid(0, 4);
            if (testEdit == _T("Edt.")) {
                wxString sql = wxString::Format(
                    _T("DELETE FROM edited_sounding WHERE sounding_id=%i"), id);
                dbQuery(sql);
            } else {
                wxString sql = wxString::Format(
                    _T("DELETE FROM sounding WHERE sounding_id=%i"), id);
                dbQuery(sql);
            }
        } else {
            wxString sql = wxString::Format(
                _T("DELETE FROM sounding WHERE sounding_id=%i"), id);
            dbQuery(sql);
        }
    }
}

int survey_pi::InsertSounding(double depth, double lat, double lon, double tide,
    time_t timestamp, int projection)
{

    int t = m_pSurveyDialog->m_chSurvey->GetSelection();

    if (t == -1) {
        wxMessageBox(_(
            "No survey selected, please select or create/select a new survey"));
        return 0;
    }
    wxString s = m_pSurveyDialog->m_chSurvey->GetStringSelection();
    int i = GetSurveyId(s);
    wxString time; // = _T("current_timestamp");
    wxDateTime dt = wxDateTime::Now();

    dt.MakeGMT();
    dt.ParseDateTime(time);

    time = _T("'") + dt.Format(wxT("%Y-%m-%d %H:%M:%S")) + _T("'");

    wxDateTime wdtDate(timestamp);

    if (wdtDate.IsValid()) {
        time = _T("'")
            + wxDateTime(timestamp).FormatISODate().Append(_T(" ")).Append(
                wxDateTime(timestamp).FormatISOTime())
            + _T("'");
    }

    wxString sql = wxString::Format(
        _T("INSERT INTO \"sounding\" (\"depth\", \"measured\", \"survey_id\", ")
        _T("\"geom\", \"tide\") VALUES (%f , %s, %i, GeomFromText('POINT(%f ")
        _T("%f)', %i), %f)"),
        depth, time, i, lon, lat, projection, tide);
    numsoundings++; // For loading NMEA file
    dbQuery(sql);
    m_pSurveyDialog->soundingCount++;
    return sqlite3_last_insert_rowid(m_database);
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int survey_pi::Init(void)
{
    m_projection = PROJECTION;
    m_activesurvey = 0;
    int db_ver = 1;
    mPriPosition = 99;
    mPriDepth = 99;
    m_lat = 999.0;
    m_lon = 999.0;
    mLastX = -1;
    mLastY = -1;
    mLastSdgId = -1;
    mLastSurveyId = -1;
    m_lastPosReport = wxDateTime::Now();
    AddLocaleCatalog(_T("opencpn-survey_pi"));

    // spatialite_init(0);
    err_msg = NULL;
    wxString sql;
    m_recording = false;
    m_survey_trace = false;
    // Set some default private member parameters
    m_survey_dialog_x = 0;
    m_survey_dialog_y = 0;

    m_trimcount = 0;
    m_latprev = 0;
    m_lonprev = 0;

    g_pFontTitle = new wxFont(
        10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);
    g_pFontData = new wxFont(
        14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    g_pFontLabel = new wxFont(
        8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    g_pFontSmall = new wxFont(
        8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    ::wxDisplaySize(&m_display_width, &m_display_height);

    //    Get a pointer to the opencpn display canvas, to use as a parent for
    //    the POI Manager dialog
    m_parent_window = GetOCPNCanvasWindow();

    //    Get a pointer to the opencpn configuration object
    m_pconfig = GetOCPNConfigObject();

    //    And load the configuration items
    LoadConfig();

    //      Establish the location of the database file
    wxString dbpath;
    dbpath = survey_pi::StandardPath() + _T(DATABASE_NAME);

    bool newDB = !wxFileExists(dbpath);
    b_dbUsable = true;

    void* cache; // SOLUTION

    ret = sqlite3_open_v2(dbpath.mb_str(), &m_database,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (ret != SQLITE_OK) {
        wxLogMessage(_T("SURVEY_PI: cannot open '%s': %s\n"), DATABASE_NAME,
            sqlite3_errmsg(m_database));
        sqlite3_close(m_database);
        b_dbUsable = false;
    }

    sqlite3_enable_load_extension(m_database, 1);
    sql = "SELECT load_extension('mod_spatialite')";

    ret = sqlite3_exec(m_database, sql.c_str(), nullptr, nullptr, &err_msg);
    if (ret != SQLITE_OK) {

        sqlite3_free(err_msg);
        sqlite3_close(m_database);
        wxMessageBox(
            "Error loading the mod_spatialite extension, refer to the manual.");
        return 0;
    }

    if (newDB && b_dbUsable) {
        // create empty db
        dbQuery(_T("SELECT InitSpatialMetadata(1)"));
        // dbQuery(_T("INSERT INTO spatial_ref_sys (srid, auth_name, auth_srid,
        // ref_sys_name, proj4text) VALUES(32632, 'epsg', 32632, 'WGS 84 / UTM
        // zone 32N', '+proj=utm +zone=32 +ellps=WGS84 +datum=WGS84 +units=m
        // +no_defs ')")); CREATE OUR TABLES
        sql = _T("CREATE TABLE survey (")
              _T("survey_id INTEGER PRIMARY KEY AUTOINCREMENT,")
              _T("survey_name TEXT,")
              _T("created INTEGER,")
              _T("submitted INTEGER)");
        dbQuery(sql);

        sql = _T("CREATE TABLE sounding (")
              _T("sounding_id INTEGER PRIMARY KEY AUTOINCREMENT,")
              _T("depth REAL NOT NULL,")
              _T("measured INTEGER NOT NULL,")
              _T("survey_id INTEGER NOT NULL,")
              _T("FOREIGN KEY(survey_id) REFERENCES survey(survey_id))");
        dbQuery(sql);
        dbQuery(_T("SELECT AddGeometryColumn('sounding', 'geom', 32632, ")
                _T("'POINT', 2)"));
    }

    // Update DB structure and contents
    if (b_dbUsable) {
        char** results;
        int n_rows;
        int n_columns;
        sql = _T("SELECT value FROM settings WHERE key = 'DBVersion'");
        ret = sqlite3_get_table(
            m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
        if (ret != SQLITE_OK) {
            sqlite3_free(err_msg);
            sql = _T("CREATE TABLE settings (")
                  _T("key TEXT NOT NULL UNIQUE,")
                  _T("value TEXT)");
            dbQuery(sql);
            dbQuery(_T("INSERT INTO settings (key, value) VALUES ")
                    _T("('DBVersion', '2')"));
            db_ver = 2;
        } else {
            db_ver = atoi(results[1]);
        }
        sqlite3_free_table(results);
        wxLogMessage(_T("SURVEY_PI: Database version: %i\n"), db_ver);
    }

    if (b_dbUsable && db_ver == 2) {
        sql = _T("ALTER TABLE sounding ")
              _T("ADD COLUMN tide REAL NOT NULL DEFAULT 0.0");
        dbQuery(sql);

        dbQuery(_T("UPDATE settings SET value = 3 WHERE key = 'DBVersion'"));
        db_ver = 3;
    }
    if (b_dbUsable && db_ver == 3) {
        sql = _T("CREATE TABLE new_sounding (")
              _T("sounding_id INTEGER PRIMARY KEY AUTOINCREMENT,")
              _T("depth REAL NOT NULL,")
              _T("measured INTEGER NOT NULL,")
              _T("survey_id INTEGER NOT NULL,")
              _T("tide REAL NOT NULL DEFAULT 0.0,")
              _T("FOREIGN KEY(survey_id) REFERENCES survey(survey_id))");
        dbQuery(sql);
        dbQuery(_T("SELECT AddGeometryColumn('new_sounding', 'geom', 3395, ")
                _T("'POINT', 2)"));
        dbQuery(
            _T("INSERT INTO new_sounding SELECT sounding_id, depth, measured, ")
            _T("survey_id, tide, SetSRID('geom', 3395) FROM sounding"));
        dbQuery(_T("DROP TABLE sounding"));
        dbQuery(_T("SELECT DiscardGeometryColumn('sounding', 'geom')"));
        sql = _T("CREATE TABLE sounding (")
              _T("sounding_id INTEGER PRIMARY KEY AUTOINCREMENT,")
              _T("depth REAL NOT NULL,")
              _T("measured INTEGER NOT NULL,")
              _T("survey_id INTEGER NOT NULL,")
              _T("tide REAL NOT NULL DEFAULT 0.0,")
              _T("FOREIGN KEY(survey_id) REFERENCES survey(survey_id))");
        dbQuery(sql);
        dbQuery(_T("SELECT AddGeometryColumn('sounding', 'geom', 3395, ")
                _T("'POINT', 2)"));
        dbQuery(_T("INSERT INTO sounding SELECT sounding_id, depth, measured, ")
                _T("survey_id, tide, SetSRID('geom', 3395) FROM new_sounding"));
        dbQuery(_T("DROP TABLE new_sounding"));
        dbQuery(_T("SELECT DiscardGeometryColumn('new_sounding', 'geom')"));
        dbQuery(_T("UPDATE settings SET value = 4 WHERE key = 'DBVersion'"));
        db_ver = 4;
    }

    if (b_dbUsable && db_ver == 4 && newDB) {
        sql = _T("CREATE TABLE edited_sounding (")
              _T("sounding_id INTEGER PRIMARY KEY AUTOINCREMENT,")
              _T("depth REAL NOT NULL,")
              _T("measured INTEGER NOT NULL,")
              _T("survey_id INTEGER NOT NULL,")
              _T("tide REAL NOT NULL DEFAULT 0.0,")
              _T("FOREIGN KEY(survey_id) REFERENCES survey(survey_id))");
        dbQuery(sql);
        dbQuery(_T("SELECT AddGeometryColumn('edited_sounding', 'geom', 3395, ")
                _T("'POINT', 2)"));

        sql = _T("CREATE TABLE tide_height (")
              _T("measured INTEGER NOT NULL,")
              _T("height REAL NOT NULL,")
              _T("survey_id INTEGER NOT NULL,")
              _T("FOREIGN KEY(survey_id) REFERENCES survey(survey_id))");
        dbQuery(sql);
    }

    wxMenu dummy_menu;
    m_delete_menu_id = AddCanvasContextMenuItem

        (new wxMenuItem(&dummy_menu, -1, _("Delete this Sounding")), this);
    SetCanvasContextMenuItemViz(m_delete_menu_id, true);

    //    This PlugIn needs a toolbar icon, so request its insertion
    if (m_bSurveyShowIcon)

#ifdef SURVEY_USE_SVG
        m_leftclick_tool_id = InsertPlugInToolSVG(_T("Survey"), _svg_survey,
            _svg_survey, _svg_survey_toggled, wxITEM_CHECK, _("Survey"), _T(""),
            NULL, SURVEY_TOOL_POSITION, 0, this);
#else
        m_leftclick_tool_id
            = InsertPlugInTool(_T(""), _img_survey, _img_survey, wxITEM_CHECK,
                _("Survey"), _T(""), NULL, SURVEY_TOOL_POSITION, 0, this);
#endif

    m_pSurveyDialog = NULL;
    m_pSurveyOverlayFactory = NULL;

    // DoRenderSurveyOverlay();

    return (WANTS_TOOLBAR_CALLBACK | INSTALLS_TOOLBAR_TOOL | WANTS_CURSOR_LATLON
        | WANTS_PREFERENCES | WANTS_NMEA_SENTENCES | WANTS_NMEA_EVENTS
        | WANTS_OVERLAY_CALLBACK | WANTS_ONPAINT_VIEWPORT
        | WANTS_OPENGL_OVERLAY_CALLBACK | WANTS_CONFIG);
}

bool survey_pi::DeInit(void)
{
    RemovePlugInTool(m_leftclick_tool_id);
    //    Record the dialog position

    if (NULL != m_pSurveyDialog) {

        wxPoint p = m_pSurveyDialog->GetPosition();
        SetSurveyDialogX(p.x);
        SetSurveyDialogY(p.y);

        m_pSurveyDialog->Close();
        delete m_pSurveyDialog;
        delete m_pSurveyOverlayFactory;
        m_pSurveyDialog = NULL;
        m_pSurveyOverlayFactory = NULL;
    }

    SaveConfig();
    // spatialite_cleanup();
    int m = sqlite3_close(m_database);

    wxLogMessage(_T("SURVEY_PI: Close Msg: %i\n"), m);
    return true;
}

int survey_pi::GetAPIVersionMajor() { return atoi(API_VERSION); }

int survey_pi::GetAPIVersionMinor()
{
    std::string v(API_VERSION);
    size_t dotpos = v.find('.');
    return atoi(v.substr(dotpos + 1).c_str());
}

int survey_pi::GetPlugInVersionMajor() { return PLUGIN_VERSION_MAJOR; }

int survey_pi::GetPlugInVersionMinor() { return PLUGIN_VERSION_MINOR; }

wxBitmap* survey_pi::GetPlugInBitmap() { return &m_panelBitmap; }

wxString survey_pi::GetCommonName() { return _("Survey"); }

wxString survey_pi::GetShortDescription()
{
    return _("Survey PlugIn for OpenCPN");
}

wxString survey_pi::GetLongDescription()
{
    return _("Records depth from the NMEA stream\n\
And let's you do various other tricks.");
}

int survey_pi::GetToolbarToolCount(void) { return 1; }

void survey_pi::SetColorScheme(PI_ColorScheme cs)
{
    if (NULL == m_pSurveyDialog)
        return;

    DimeWindow(m_pSurveyDialog);
}

void survey_pi::FillSurveyDropdown()
{
    m_pSurveyDialog->m_chSurvey->Clear();
    m_pSurveyDialog->m_chSurvey->Append(GetSurveyList());
    for (int i = 0; i < GetSurveyList().Count(); i++) {
        if (GetSurveyList()[i] == m_activesurveyname) {
            m_pSurveyDialog->m_chSurvey->SetSelection(i);
            int st = GetSurveyId(m_pSurveyDialog->m_chSurvey->GetString(0));
            SetActiveSurveyId(st);
        } else {
            m_pSurveyDialog->m_chSurvey->SetSelection(0);
            int s = GetSurveyId(m_pSurveyDialog->m_chSurvey->GetString(0));
            SetActiveSurveyId(s);
        }
    }
}

void survey_pi::FillSurveyGrid()
{

    m_pSurveyDialog->m_gdSoundings->ClearGrid();

    if (m_pSurveyDialog->m_chSurvey->GetCount() > 0) {

        int t = m_pSurveyDialog->m_chSurvey->GetSelection();
        wxString s = m_pSurveyDialog->m_chSurvey->GetString(t);

        int id = GetSurveyId(s);
        SetActiveSurveyId(id);

        m_pSurveyDialog->mysoundings = SetTable(id);

        int i = 0;

        for (std::vector<soundingdata>::iterator it = all_soundingdata.begin();
             it != all_soundingdata.end(); it++) {
            wxDateTime dt;
            dt.ParseDateTime(it->time);
            wxString str = dt.Format(wxT("%Y-%m-%d %H:%M:%S"));

            m_pSurveyDialog->m_gdSoundings->SetCellValue(i, 0, it->depth);
            m_pSurveyDialog->m_gdSoundings->SetCellValue(i, 1, str);
            m_pSurveyDialog->m_gdSoundings->SetCellValue(i, 2, it->lat);
            m_pSurveyDialog->m_gdSoundings->SetCellValue(i, 3, it->lon);

            wxString sid = wxString::Format(wxT("%i"), it->sdgid);
            m_pSurveyDialog->m_gdSoundings->SetCellValue(i, 4, sid);
            m_pSurveyDialog->m_gdSoundings->HideCol(5);

            m_pSurveyDialog->m_gdSoundings->AppendRows(1);
            i++;
        }
    }
}

void survey_pi::FillEditSurveyGrid(int surv_id)
{

    m_pSurveyDialog->m_gdSoundings->ClearGrid();

    if (m_pSurveyDialog->m_chSurvey->GetCount() > 0) {

        int id = surv_id;
        SetActiveSurveyId(id);

        m_pSurveyDialog->mysoundings = SetEditTable(id);

        int i = 0;

        for (std::vector<soundingdata>::iterator it = all_soundingdata.begin();
             it != all_soundingdata.end(); it++) {
            wxDateTime dt;
            dt.ParseDateTime(it->time);
            wxString str = dt.Format(wxT("%Y-%m-%d %H:%M:%S"));

            m_pSurveyDialog->m_gdSoundings->SetCellValue(i, 0, it->depth);
            m_pSurveyDialog->m_gdSoundings->SetCellValue(i, 1, str);
            m_pSurveyDialog->m_gdSoundings->SetCellValue(i, 2, it->lat);
            m_pSurveyDialog->m_gdSoundings->SetCellValue(i, 3, it->lon);

            wxString sid = wxString::Format(wxT("%i"), it->sdgid);
            m_pSurveyDialog->m_gdSoundings->SetCellValue(i, 4, sid);
            m_pSurveyDialog->m_gdSoundings->ShowCol(5);
            m_pSurveyDialog->m_gdSoundings->SetCellValue(i, 5, it->tide);

            m_pSurveyDialog->m_gdSoundings->AppendRows(1);
            i++;
        }
    }
}

void survey_pi::FillSurveyInfo()
{
    int i = GetActiveSurveyId();
    wxString maxD, minD, numD, areaD;
    maxD = _("Maximum depth: ") + GetSurveyMaxDepth(i) + _("m");
    minD = _("Minimum depth: ") + GetSurveyMinDepth(i) + _("m");
    numD = _("Soundings: ") + GetSurveyNumSoundings(i);
    areaD = GetSurveyAreaSoundings(i);

    m_pSurveyDialog->m_tMaxDepth->SetLabel(maxD);
    m_pSurveyDialog->m_tMinDepth->SetLabel(minD);
    m_pSurveyDialog->m_tNrSoundings->SetLabel(numD);

    RequestRefresh(m_parent_window); // refresh main window
}

void survey_pi::FillEditSurveyInfo()
{
    int i = GetActiveSurveyId();
    wxString maxD, minD, numD, areaD;
    maxD = _("Maximum depth: ") + GetEditSurveyMaxDepth(i) + _("m");
    minD = _("Minimum depth: ") + GetEditSurveyMinDepth(i) + _("m");
    numD = _("Soundings: ") + GetEditSurveyNumSoundings(i);
    areaD = GetEditSurveyAreaSoundings(i);

    m_pSurveyDialog->m_tMaxDepth->SetLabel(maxD);
    m_pSurveyDialog->m_tMinDepth->SetLabel(minD);
    m_pSurveyDialog->m_tNrSoundings->SetLabel(numD);

    RequestRefresh(m_parent_window); // refresh main window
}

void survey_pi::OnToolbarToolCallback(int id)
{
    if (NULL == m_pSurveyDialog) {
        m_pSurveyDialog = new SurveyDlg(m_parent_window);
        m_pSurveyOverlayFactory = new SurveyOverlayFactory(*m_pSurveyDialog);
        m_pSurveyDialog->plugin = this;
        m_pSurveyDialog->Move(wxPoint(m_survey_dialog_x, m_survey_dialog_y));
        FillSurveyDropdown();
        FillSurveyInfo();
        FillSurveyGrid();
        SetSettings();
    }

    // Toggle otcurrent overlay display
    m_bShowSurvey = !m_bShowSurvey;

    //    Toggle dialog?
    if (m_bShowSurvey) {
        m_pSurveyDialog->Show();
    } else {
        m_pSurveyDialog->Hide();
    }

    // Toggle is handled by the toolbar but we must keep plugin manager b_toggle
    // updated to actual status to ensure correct status upon toolbar rebuild
    SetToolbarItemState(m_leftclick_tool_id, m_bShowSurvey);
    RequestRefresh(m_parent_window); // refresh main window
}

void survey_pi::SetSettings()
{

    if (NULL == m_pSurveyDialog) {
        m_pSurveyDialog = new SurveyDlg(m_parent_window);
        m_pSurveyOverlayFactory = new SurveyOverlayFactory(*m_pSurveyDialog);
        m_pSurveyDialog->plugin = this;
        m_pSurveyDialog->Move(wxPoint(m_survey_dialog_x, m_survey_dialog_y));
        FillSurveyDropdown();
        FillSurveyInfo();
        FillSurveyGrid();
    }

    m_pSurveyDialog->mySettings.m_iOpacity = m_iOpacity;
    m_pSurveyDialog->mySettings.m_iUnits = PI_GetPLIBDepthUnitInt();
    m_pSurveyDialog->mySettings.m_bCalcTide = m_bCalcTide;
    m_pSurveyDialog->mySettings.m_sCorrection = m_sCorrection;
    m_pSurveyDialog->mySettings.m_bRenderAllSurveys = m_bRenderAllSurveys;
    m_pSurveyDialog->mySettings.m_bConnectSoundings = m_bConnectSoundings;
    m_pSurveyDialog->mySettings.m_bRenderSoundingText = m_bRenderSoundingText;
    m_pSurveyDialog->mySettings.m_bRenderWithCorrn = m_bRenderWithCorrn;
    m_pSurveyDialog->mySettings.m_iSoundingShape = m_iSoundingShape;
    m_pSurveyDialog->mySettings.m_bUseSymbol = m_bUseSymbol;
    m_pSurveyDialog->mySettings.m_bUseDepthColours = m_bUseDepthColours;
    m_pSurveyDialog->mySettings.m_sSoundingColor = m_sSoundingColor;
    m_pSurveyDialog->mySettings.m_sConnectorColor = m_sConnectorColor;
    m_pSurveyDialog->mySettings.m_sFont = m_sFont;
    m_pSurveyDialog->mySettings.m_sFontColor = m_sFontColor;
    m_pSurveyDialog->mySettings.m_fLOA = m_fLOA;
    m_pSurveyDialog->mySettings.m_fBeam = m_fBeam;
    m_pSurveyDialog->mySettings.m_fSounderBow = m_fSounderBow;
    m_pSurveyDialog->mySettings.m_fWaterlineOffset = m_fWaterlineOffset;
    m_pSurveyDialog->mySettings.m_fGPSBow = m_fGPSBow;
    m_pSurveyDialog->mySettings.m_fGPSPort = m_fGPSPort;
    m_pSurveyDialog->mySettings.m_fMinDistance = m_fMinDistance;
    // m_pSurveyDialog->mySettings.m_fAutoNewDistance = m_fAutoNewDistance;

    m_pSurveyDialog->mySettings.mLastSdgId = mLastSdgId;
    m_pSurveyDialog->mySettings.mLastSurveyId = mLastSurveyId;

    m_pSurveyDialog->mySettings.mLastX = mLastX;
    m_pSurveyDialog->mySettings.mLastY = mLastY;

    int gl = wxUSE_GLCANVAS;

    if (!m_bUseSymbol && (gl == 0)) {
        wxMessageBox(_("WITHOUT OpenGL the display must still use a symbol"));
    }
}

bool survey_pi::LoadConfig(void)
{
    wxFileConfig* pConf = (wxFileConfig*)m_pconfig;

    if (pConf) {
        pConf->SetPath(_T( "/Settings/Survey" ));

        pConf->Read(_T ( "Opacity" ), &m_iOpacity, 255);
        pConf->Read(_T ( "Units" ), &m_iUnits, PI_GetPLIBDepthUnitInt());
        pConf->Read(_T ( "CalculateTide" ), &m_bCalcTide, false);
        pConf->Read(_T ( "Correction" ), &m_sCorrection);

        pConf->Read(_T ( "RenderOverlay" ), &m_bRenderOverlay, true);
        pConf->Read(_T ( "RenderAll" ), &m_bRenderAllSurveys, true);
        pConf->Read(_T ( "ConnectSoundings" ), &m_bConnectSoundings, true);
        pConf->Read(_T ( "RenderSoundingText" ), &m_bRenderSoundingText, true);
        pConf->Read(_T ( "RenderWithTide" ), &m_bRenderWithCorrn, false);

        pConf->Read(_T ( "SoundingColor" ), &m_sSoundingColor);
        pConf->Read(_T ( "ConnectorColor" ), &m_sConnectorColor);
        pConf->Read(_T ( "SoundingFont" ), &m_sFont);
        pConf->Read(_T ( "SoundingFontColor" ), &m_sFontColor);
        pConf->Read(_T ( "SoundingSymbol" ), &m_iSoundingShape, 0);
        pConf->Read(_T ( "UseSymbol" ), &m_bUseSymbol, false);
        pConf->Read(_T ( "UseDepthColours"), &m_bUseDepthColours, false);

        pConf->Read(_T ( "ActiveSurvey" ), &m_activesurvey, 0);

        pConf->Read(_T ( "LOA" ), &m_fLOA, 0.0);
        pConf->Read(_T ( "Beam" ), &m_fBeam, 0.0);
        pConf->Read(_T ( "SounderBow" ), &m_fSounderBow, 0.0);
        pConf->Read(_T ( "WaterlineOffset" ), &m_fWaterlineOffset, 0.0);
        pConf->Read(_T ( "GPSBow" ), &m_fGPSBow, 0.0);
        pConf->Read(_T ( "GPSPort" ), &m_fGPSPort, 0.0);
        pConf->Read(_T ( "MinSoundingDist" ), &m_fMinDistance, 0.0);
        // pConf->Read ( _T ( "AutoNewDistance" ), &m_fAutoNewDistance, 0.0 );

        pConf->Read(_T("ShowSurveyIcon"), &m_bSurveyShowIcon, 1);

        m_survey_dialog_x = pConf->Read(_T ( "DialogPosX" ), 20L);
        m_survey_dialog_y = pConf->Read(_T ( "DialogPosY" ), 20L);

        if ((m_survey_dialog_x < 0) || (m_survey_dialog_x > m_display_width))
            m_survey_dialog_x = 5;
        if ((m_survey_dialog_y < 0) || (m_survey_dialog_y > m_display_height))
            m_survey_dialog_y = 5;
        return true;
    } else
        return false;
}

bool survey_pi::SaveConfig(void)
{
    wxFileConfig* pConf = (wxFileConfig*)m_pconfig;

    if (pConf) {
        pConf->SetPath(_T ( "/Settings/Survey" ));
        pConf->Write(_T ( "Opacity" ), m_iOpacity);
        pConf->Write(_T ( "Units" ), PI_GetPLIBDepthUnitInt());
        pConf->Write(_T ( "CalculateTide"), m_bCalcTide);
        pConf->Write(_T ( "Correction"), m_sCorrection);
        pConf->Write(_T ( "RenderOverlay" ), m_bRenderOverlay);
        pConf->Write(_T ( "RenderAll" ), m_bRenderAllSurveys);
        pConf->Write(_T ( "ConnectSoundings" ), m_bConnectSoundings);
        pConf->Write(_T ( "RenderSoundingText"), m_bRenderSoundingText);
        pConf->Write(_T ( "RenderWithTide"), m_bRenderWithCorrn);
        pConf->Write(_T ( "SoundingColor" ), m_sSoundingColor);
        pConf->Write(_T ( "ConnectorColor" ), m_sConnectorColor);
        pConf->Write(_T ( "SoundingFont" ), m_sFont);
        pConf->Write(_T ( "SoundingFontColor" ), m_sFontColor);
        pConf->Write(_T ( "SoundingSymbol" ), m_iSoundingShape);
        pConf->Write(_T ( "UseSymbol" ), m_bUseSymbol);
        pConf->Write(_T ( "UseDepthColours"), m_bUseDepthColours);

        pConf->Write(_T ( "ActiveSurvey" ), m_activesurvey);

        pConf->Write(_T ( "LOA" ), m_fLOA);
        pConf->Write(_T ( "Beam" ), m_fBeam);
        pConf->Write(_T ( "SounderBow" ), m_fSounderBow);
        pConf->Write(_T ( "WaterlineOffset" ), m_fWaterlineOffset);
        pConf->Write(_T ( "GPSBow" ), m_fGPSBow);
        pConf->Write(_T ( "GPSPort" ), m_fGPSPort);
        pConf->Write(_T ( "MinSoundingDist" ), m_fMinDistance);
        // pConf->Write ( _T ( "AutoNewDistance" ), m_fAutoNewDistance );

        pConf->Write(_T("ShowSurveyIcon"), m_bSurveyShowIcon);

        pConf->Write(_T ( "DialogPosX" ), m_survey_dialog_x);
        pConf->Write(_T ( "DialogPosY" ), m_survey_dialog_y);

        return true;
    } else
        return false;
}

void survey_pi::ShowPreferencesDialog(wxWindow* parent)
{
    SurveyCfgDlg* dialog = new SurveyCfgDlg(parent, wxID_ANY,
        _("Survey Preferences"), wxPoint(m_survey_dialog_x, m_survey_dialog_y),
        wxSize(650, 650), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    dialog->Fit();
    wxColour cl;

    DimeWindow(dialog);

    dialog->m_cbRenderOverlay->SetValue(m_bRenderOverlay);
    dialog->m_sOpacity->SetValue(m_iOpacity);
    switch (m_iUnits) {
    case METERS:
        dialog->m_rbMeters->SetValue(true);
        break;
    case FEET:
        dialog->m_rbFeet->SetValue(true);
        break;
    case FATHOMS:
        dialog->m_rbFathoms->SetValue(true);
        break;
    }

    // dialog->m_cbCalcTide->SetValue(m_bCalcTide);
    dialog->m_tCorrection->SetValue(m_sCorrection);

    dialog->m_rbRendedAllSurveys->SetValue(m_bRenderAllSurveys);
    dialog->m_rbRenderActiveSurvey->SetValue(!m_bRenderAllSurveys);
    dialog->m_cbConnectSoundings->SetValue(m_bConnectSoundings);

    // Render text
    dialog->m_cbRenderDepthValues->SetValue(m_bRenderSoundingText);
    dialog->m_cbSdgsPlusCorrn->SetValue(m_bRenderWithCorrn);

    // Symbols
    dialog->m_lSymbolList->SetSelection(m_iSoundingShape);
    dialog->m_cbSelectedSymbol->SetValue(m_bUseSymbol);
    dialog->m_cbUseDepthColours->SetValue(m_bUseDepthColours);

    dialog->m_cpSoundingColor->SetColour(m_sSoundingColor);
    dialog->m_cpConnectorColor->SetColour(m_sConnectorColor);
    dialog->m_fpFont->SetSelectedFont(wxFont(m_sFont));
    dialog->m_cpFontColor->SetColour(m_sFontColor);
    dialog->m_tLOA->SetValue(wxString::Format(wxT("%f"), m_fLOA));
    dialog->m_tBeam->SetValue(wxString::Format(wxT("%f"), m_fBeam));
    dialog->m_tSounderBow->SetValue(wxString::Format(wxT("%f"), m_fSounderBow));
    dialog->m_tWaterlineOffset->SetValue(
        wxString::Format(wxT("%f"), m_fWaterlineOffset));
    dialog->m_tGPSBow->SetValue(wxString::Format(wxT("%f"), m_fGPSBow));
    dialog->m_tGPSPort->SetValue(wxString::Format(wxT("%f"), m_fGPSPort));
    dialog->m_tMinDistance->SetValue(
        wxString::Format(wxT("%f"), m_fMinDistance));
    // dialog->m_tAutoNewDistance->SetValue(wxString::Format(wxT("%f"),
    // m_fAutoNewDistance));

    if (dialog->ShowModal() == wxID_OK) {
        m_bRenderOverlay = dialog->m_cbRenderOverlay->GetValue();
        m_iOpacity = dialog->m_sOpacity->GetValue();
        if (dialog->m_rbMeters->GetValue())
            m_iUnits = METERS;
        else if (dialog->m_rbFeet->GetValue())
            m_iUnits = FEET;
        else
            m_iUnits = FATHOMS;

        // m_bCalcTide = dialog->m_cbCalcTide->GetValue();
        m_sCorrection = dialog->m_tCorrection->GetValue();

        m_bRenderAllSurveys = dialog->m_rbRendedAllSurveys->GetValue();
        m_bConnectSoundings = dialog->m_cbConnectSoundings->GetValue();

        // Render Text
        m_bRenderSoundingText = dialog->m_cbRenderDepthValues->GetValue();
        m_bRenderWithCorrn = dialog->m_cbSdgsPlusCorrn->GetValue();

        // Symbols
        m_iSoundingShape = dialog->m_lSymbolList->GetSelection();
        m_bUseSymbol = dialog->m_cbSelectedSymbol->GetValue();
        m_bUseDepthColours = dialog->m_cbUseDepthColours->GetValue();

        m_sSoundingColor = dialog->m_cpSoundingColor->GetColour().GetAsString();
        m_sConnectorColor
            = dialog->m_cpConnectorColor->GetColour().GetAsString();
        m_sFont = dialog->m_fpFont->GetSelectedFont().GetNativeFontInfoDesc();
        m_sFontColor = dialog->m_cpFontColor->GetColour().GetAsString();
        double value;
        if (!dialog->m_tLOA->GetValue().ToDouble(&value)) {
            value = 0.0;
        }
        m_fLOA = value;
        if (!dialog->m_tBeam->GetValue().ToDouble(&value)) {
            value = 0.0;
        }
        m_fBeam = value;
        if (!dialog->m_tSounderBow->GetValue().ToDouble(&value)) {
            value = 0.0;
        }
        m_fSounderBow = value;
        if (!dialog->m_tWaterlineOffset->GetValue().ToDouble(&value)) {
            value = 0.0;
        }
        m_fWaterlineOffset = value;
        if (!dialog->m_tGPSBow->GetValue().ToDouble(&value)) {
            value = 0.0;
        }
        m_fGPSBow = value;
        if (!dialog->m_tGPSPort->GetValue().ToDouble(&value)) {
            value = 0.0;
        }
        m_fGPSPort = value;
        if (!dialog->m_tMinDistance->GetValue().ToDouble(&value)) {
            value = 0.0;
        }
        m_fMinDistance = value;
        // if(!dialog->m_tAutoNewDistance->GetValue().ToDouble(&value)){ value =
        // 0.0; } m_fAutoNewDistance = value;

        SaveConfig();
        SetSettings();
        RequestRefresh(m_parent_window); // refresh main window
    }
    delete dialog;
}

bool survey_pi::RenderOverlay(wxDC& dc, PlugIn_ViewPort* vp)
{

    if (!m_pSurveyDialog || !m_pSurveyDialog->IsShown()
        || !m_pSurveyOverlayFactory) {
        return false;
    }

    m_pSurveyDialog->SetViewPort(vp);
    viewscale = vp->view_scale_ppm;

    wxString s;

    if (m_pSurveyDialog->m_chSurvey->GetCount() != 0) {
        s = m_pSurveyDialog->m_chSurvey->GetStringSelection();
        SetActiveSurvey(s);
    }

    if (m_activesurvey != 0) {

        if (s.length() > 4) {
            wxString testEdit = s.Mid(0, 4);
            if (testEdit == _T("Edt.")) {
                GetEditedSoundings(m_activesurvey);
                m_pSurveyOverlayFactory->RenderSurveyOverlay(dc, vp);
            } else {
                GetSurveySoundings(m_activesurvey);
                m_pSurveyOverlayFactory->RenderSurveyOverlay(dc, vp);
            }
        } else {
            GetSurveySoundings(m_activesurvey);
            m_pSurveyOverlayFactory->RenderSurveyOverlay(dc, vp);
        }
    }

    return true;
}

bool survey_pi::RenderGLOverlay(wxGLContext* pcontext, PlugIn_ViewPort* vp)
{
    if (!m_pSurveyDialog || !m_pSurveyDialog->IsShown()
        || !m_pSurveyOverlayFactory)
        return false;

    m_pSurveyDialog->SetViewPort(vp);
    viewscale = vp->view_scale_ppm;
    wxString s;
    if (m_pSurveyDialog->m_chSurvey->GetCount() != 0) {
        s = m_pSurveyDialog->m_chSurvey->GetStringSelection();
        SetActiveSurvey(s);
    }

    if (m_activesurvey != 0) {

        if (s.length() > 4) {

            wxString testEdit = s.Mid(0, 4);
            if (testEdit == _T("Edt.")) {
                GetEditedSoundings(m_activesurvey);
                m_pSurveyOverlayFactory->RenderGLSurveyOverlay(pcontext, vp);
            } else {
                GetSurveySoundings(m_activesurvey);
                m_pSurveyOverlayFactory->RenderGLSurveyOverlay(pcontext, vp);
            }
        } else {
            GetSurveySoundings(m_activesurvey);
            m_pSurveyOverlayFactory->RenderGLSurveyOverlay(pcontext, vp);
        }
    }

    return true;
}

bool survey_pi::GetSurveySoundings(int as)
{
    soundingdata mySounding;
    mySurveySoundings.clear();

    if (!b_dbUsable || !m_bRenderOverlay)
        return false;

    // wxString sql = wxString::Format(_T("SELECT depth, AsText(geom) FROM
    // sounding WHERE Within(geom, PolygonFromText('POLYGON((%f %f, %f %f, %f
    // %f, %f %f))'))"), vp->lat_min, vp->lon_min, vp->lat_min, vp->lon_max,
    // vp->lat_max, vp->lon_max, vp->lat_max, vp->lon_min);
    wxString sql
        = wxString::Format(_T("SELECT depth, AsText(geom), sounding_id, ")
                           _T("survey_id, measured, tide FROM sounding"));
    if (as > 0 && !m_bRenderAllSurveys) {
        sql = wxString::Format(
            _T("SELECT depth, AsText(geom), sounding_id, survey_id, measured, ")
            _T("tide FROM sounding WHERE survey_id = %i"),
            as);
    }

    char** results;
    int n_rows;
    int n_columns;
    char* dpt;
    char* pos;
    char* sdgid;
    char* surid;
    char* measured;
    char* tide;

    wxString s0, s1;
    ret = sqlite3_get_table(
        m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
    if (ret != SQLITE_OK) {
        /* some error occurred */
        wxLogMessage(_T("Spatialite SQL error: %s\n"), err_msg);
        sqlite3_free(err_msg);
        return false;
    }

    for (int i = 1; i <= n_rows; i++) {
        dpt = results[(i * n_columns) + 0];
        pos = results[(i * n_columns) + 1];
        sdgid = results[(i * n_columns) + 2];
        surid = results[(i * n_columns) + 3];
        measured = results[(i * n_columns) + 4];
        tide = results[(i * n_columns) + 5];
        double depth = atof(dpt);
        wxString s(pos, wxConvUTF8);

        wxString d(dpt, wxConvUTF8);

        double latl;
        double lonl;

        int f = s.First(_T(" "));
        int l = s.Len();

        s0 = s.Mid(6, f - 6);
        s0.ToDouble(&lonl);

        s1 = s.Mid(f + 1, l - f - 2);
        s1.ToDouble(&latl);

        mySounding.latD = latl;
        mySounding.lonD = lonl;

        wxString ms(tide, wxConvUTF8);
        mySounding.tide = ms;

        wxString dt(dpt, wxConvUTF8);
        mySounding.depth = dt;
        mySounding.surid = atoi(surid);
        mySounding.sdgid = atoi(sdgid);
        mySurveySoundings.push_back(mySounding);
    }

    m_pSurveyDialog->mysoundings = mySurveySoundings;

    mLastX = -1;
    mLastY = -1;

    sqlite3_free_table(results);

    return true;
}

bool survey_pi::GetEditedSoundings(int as)
{
    soundingdata mySounding;
    mySurveySoundings.clear();

    if (!b_dbUsable || !m_bRenderOverlay)
        return false;

    // wxString sql = wxString::Format(_T("SELECT depth, AsText(geom) FROM
    // sounding WHERE Within(geom, PolygonFromText('POLYGON((%f %f, %f %f, %f
    // %f, %f %f))'))"), vp->lat_min, vp->lon_min, vp->lat_min, vp->lon_max,
    // vp->lat_max, vp->lon_max, vp->lat_max, vp->lon_min);
    wxString sql = wxString::Format(
        _T("SELECT depth, AsText(geom), sounding_id, survey_id, measured, ")
        _T("tide FROM edited_sounding WHERE survey_id = %i"),
        as);

    char** results;
    int n_rows;
    int n_columns;
    char* dpt;
    char* pos;
    char* sdgid;
    char* surid;
    char* measured;
    char* tide;

    wxString s0, s1;
    ret = sqlite3_get_table(
        m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
    if (ret != SQLITE_OK) {
        /* some error occurred */
        wxLogMessage(_T("Spatialite SQL error: %s\n"), err_msg);
        sqlite3_free(err_msg);
        return false;
    }

    for (int i = 1; i <= n_rows; i++) {
        dpt = results[(i * n_columns) + 0];
        pos = results[(i * n_columns) + 1];
        sdgid = results[(i * n_columns) + 2];
        surid = results[(i * n_columns) + 3];
        measured = results[(i * n_columns) + 4];
        tide = results[(i * n_columns) + 5];
        double depth = atof(dpt);
        wxString s(pos, wxConvUTF8);

        wxString d(dpt, wxConvUTF8);

        double latl;
        double lonl;

        int f = s.First(_T(" "));
        int l = s.Len();

        s0 = s.Mid(6, f - 6);
        s0.ToDouble(&lonl);

        s1 = s.Mid(f + 1, l - f - 2);
        s1.ToDouble(&latl);

        mySounding.latD = latl;
        mySounding.lonD = lonl;

        wxString ms(tide, wxConvUTF8);
        mySounding.tide = ms;

        wxString dt(dpt, wxConvUTF8);
        // wxMessageBox(dt);
        mySounding.depth = dt;
        mySounding.surid = atoi(surid);
        mySounding.sdgid = atoi(sdgid);
        mySurveySoundings.push_back(mySounding);
    }

    m_pSurveyDialog->mysoundings = mySurveySoundings;
    // int c = m_pSurveyDialog->mysoundings.size();

    // wxMessageBox(m_pSurveyDialog->mysoundings.begin()->depth);
    // wxMessageBox(m_pSurveyDialog->mysoundings.begin()->lat);
    // wxString cs = wxString::Format("%f",
    // m_pSurveyDialog->mysoundings.begin()->latD); wxMessageBox(cs);
    // wxMessageBox(m_pSurveyDialog->mysoundings.begin()->lon);

    // cs = wxString::Format("%f", m_pSurveyDialog->mysoundings.begin()->lonD);
    // wxMessageBox(cs);
    // cs = GetSurveyName(m_pSurveyDialog->mysoundings.begin()->surid);
    // wxMessageBox(cs);

    mLastX = -1;
    mLastY = -1;

    sqlite3_free_table(results);

    return true;
}
void survey_pi::StoreSounding(double depth)
{
    if (m_lastPosReport.Subtract(wxDateTime::Now()).GetSeconds() > 2
        || m_lat == 999.0 || m_lon == 999.0)
        return;
    char number[24]; // dummy size, you should take care of the size!
    sprintf(number, "%.2f", depth);
    depth = atof(number);
    double brg, dist;
    double tide;
    wxDateTime dt = wxDateTime::Now();

    dist = 0;
    if (m_latprev == 0) {
        tide = 0;
        depth += m_fWaterlineOffset;
        InsertSounding(depth, m_lat, m_lon, tide, 00.0, 3395);
        m_pSurveyDialog->mySurveyTrace->SetData(
            OCPN_DBP_STC_MDA, depth, _("meters"));
    }
    if (!m_latprev == 0) {
        DistanceBearingMercator_Plugin(
            m_latprev, m_lonprev, m_lat, m_lon, &brg, &dist);
        if (dist * 1852 >= m_fMinDistance) {
            tide = 0;
            depth += m_fWaterlineOffset;
            InsertSounding(depth, m_lat, m_lon, tide, 00.0, 3395);
            m_pSurveyDialog->mySurveyTrace->SetData(
                OCPN_DBP_STC_MDA, depth, _("meters"));
            m_latprev = m_lat;
            m_lonprev = m_lon;
        }
    } else {
        m_latprev = m_lat;
        m_lonprev = m_lon;
        return;
    }
}

void survey_pi::SetNMEASentence(wxString& sentence)
{
    if (m_recording) {
        if (!m_survey_trace) {
            m_pSurveyDialog->SetTrace();
        }
        m_pSurveyDialog->Refresh();

        m_NMEA0183 << sentence;

        if (m_NMEA0183.PreParse()) {
            if (m_NMEA0183.LastSentenceIDReceived == _T("DBT")) {
                if (m_NMEA0183.Parse()) {
                    if (mPriDepth >= 1) {
                        mPriDepth = 1;

                        StoreSounding(m_NMEA0183.Dbt.DepthMeters);
                    }
                }
            }

            else if (m_NMEA0183.LastSentenceIDReceived == _T("DPT")) {
                if (m_NMEA0183.Parse()) {
                    if (mPriDepth >= 2) {
                        mPriDepth = 2;

                        /*
                        double m_NMEA0183.Dpt.DepthMeters
                        double m_NMEA0183.Dpt.OffsetFromTransducerMeters
                        */
                        StoreSounding(m_NMEA0183.Dpt.DepthMeters);
                    }
                }
            }
            // TODO: GBS - GPS Satellite fault detection
            else if (m_NMEA0183.LastSentenceIDReceived == _T("GGA")) {
                if (m_NMEA0183.Parse()) {
                    if (m_NMEA0183.Gga.GPSQuality > 0) {
                        if (mPriPosition >= 3) {
                            mPriPosition = 3;
                            float llt
                                = m_NMEA0183.Gga.Position.Latitude.Latitude;
                            int lat_deg_int = (int)(llt / 100);
                            float lat_deg = lat_deg_int;
                            float lat_min = llt - (lat_deg * 100);
                            m_lat = lat_deg + (lat_min / 60.);
                            if (m_NMEA0183.Gga.Position.Latitude.Northing
                                == South)
                                m_lat = -m_lat;

                            float lln
                                = m_NMEA0183.Gga.Position.Longitude.Longitude;
                            int lon_deg_int = (int)(lln / 100);
                            float lon_deg = lon_deg_int;
                            float lon_min = lln - (lon_deg * 100);
                            m_lon = lon_deg + (lon_min / 60.);
                            if (m_NMEA0183.Gga.Position.Longitude.Easting
                                == West)
                                m_lon = -m_lon;
                            m_lastPosReport = wxDateTime::Now();
                        }
                    }
                }
            }

            else if (m_NMEA0183.LastSentenceIDReceived == _T("GLL")) {
                if (m_NMEA0183.Parse()) {
                    if (m_NMEA0183.Gll.IsDataValid == NTrue) {
                        if (mPriPosition >= 2) {
                            mPriPosition = 2;
                            float llt
                                = m_NMEA0183.Gll.Position.Latitude.Latitude;
                            int lat_deg_int = (int)(llt / 100);
                            float lat_deg = lat_deg_int;
                            float lat_min = llt - (lat_deg * 100);
                            m_lat = lat_deg + (lat_min / 60.);
                            if (m_NMEA0183.Gll.Position.Latitude.Northing
                                == South)
                                m_lat = -m_lat;

                            float lln
                                = m_NMEA0183.Gll.Position.Longitude.Longitude;
                            int lon_deg_int = (int)(lln / 100);
                            float lon_deg = lon_deg_int;
                            float lon_min = lln - (lon_deg * 100);
                            m_lon = lon_deg + (lon_min / 60.);
                            if (m_NMEA0183.Gll.Position.Longitude.Easting
                                == West)
                                m_lon = -m_lon;
                            m_lastPosReport = wxDateTime::Now();
                        }
                    }
                }
            }

            else if (m_NMEA0183.LastSentenceIDReceived == _T("RMC")) {
                if (m_NMEA0183.Parse()) {
                    if (m_NMEA0183.Rmc.IsDataValid == NTrue) {
                        if (mPriPosition >= 4) {
                            mPriPosition = 4;
                            float llt
                                = m_NMEA0183.Rmc.Position.Latitude.Latitude;
                            int lat_deg_int = (int)(llt / 100);
                            float lat_deg = lat_deg_int;
                            float lat_min = llt - (lat_deg * 100);
                            m_lat = lat_deg + (lat_min / 60.);
                            if (m_NMEA0183.Rmc.Position.Latitude.Northing
                                == South)
                                m_lat = -m_lat;

                            float lln
                                = m_NMEA0183.Rmc.Position.Longitude.Longitude;
                            int lon_deg_int = (int)(lln / 100);
                            float lon_deg = lon_deg_int;
                            float lon_min = lln - (lon_deg * 100);
                            m_lon = lon_deg + (lon_min / 60.);
                            if (m_NMEA0183.Rmc.Position.Longitude.Easting
                                == West)
                                m_lon = -m_lon;

                            m_cog = m_NMEA0183.Rmc.TrackMadeGoodDegreesTrue;
                            m_lastPosReport = wxDateTime::Now();
                        }
                    }
                }
            }
        }
    }
}

void survey_pi::ParseNMEASentence(wxString sentence)
{

    wxString m_depth;
    wxString token[40];
    wxString s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    token[0] = _T("");

    wxStringTokenizer tokenizer(sentence, wxT(","));
    int i = 0;

    while (tokenizer.HasMoreTokens()) {
        token[i] = tokenizer.GetNextToken();
        i++;
    }
    if (token[0].Right(3) == _T("GLL")) {
        b_gotdepth = false;
        s0 = token[1] + token[2];
        s1 = token[3] + token[4];
        mydata.lat = DDMLatToDecimal(s0);
        mydata.lon = DDMLonToDecimal(s1);

    } else {
        if (token[0].Right(3) == _T("ZDA")) {
            b_gotdepth = false;
            s0 = token[1];
            s1 = s0.Mid(0, 2);
            s2 = s0.Mid(2, 2);
            s3 = s0.Mid(4, 2);
            s4 = s1 + _T(":") + s2 + _T(":") + s3;
            s5 = token[2];
            s6 = token[3];
            s7 = token[4];
            s9 = s7 + _T("-") + s6 + _T("-") + s5 + _T(" ") + s4;

            mydata.time = s9; // _T("$IIZDA,055842,15,09,2012,,*5D");  //
        } else {
            if (token[0].Right(3) == _T("RMC")) {
                //$GPRMC, 110858.989, A, 4549.9135, N, 00612.2671, E, 003.7,
                // 207.5, 050513, , , A * 60
                b_gotdepth = false;

                s10 = token[3] + token[4];
                s11 = token[5] + token[6];
                mydata.lat = DDMLatToDecimal(s10);
                mydata.lon = DDMLonToDecimal(s11);

                s10 = token[8];
                mydata.cog = s10;

                s0 = token[1];
                s1 = s0.Mid(0, 2);
                s2 = s0.Mid(2, 2);
                s3 = s0.Mid(4, 2);
                s4 = s1 + _T(":") + s2 + _T(":") + s3;
                s5 = token[9];
                s6 = s5.Mid(0, 2);
                s7 = s5.Mid(2, 2);
                s8 = _T("20") + s5.Mid(4, 2);

                s9 = s8 + _T("-") + s7 + _T("-") + s6 + _T(" ") + s4;

                mydata.time = s9;
            } else {
                if (token[0].Right(3) == _T("DBT")) {
                    // wxMessageBox(sentence);
                    mydata.depth = token[3];
                    b_gotdepth = true;
                } else {
                    return;
                }
            }
        }
    }

    if (b_gotdepth == true) {

        if (mydata.lat == _T("999") || mydata.lon == _T("999")) {
            //	b_gotdepth = false;
            return;
        }

        double value, valuela, valuelo;
        mydata.depth.ToDouble(&value);
        double depth2 = value;
        mydata.lat.ToDouble(&valuela);
        // wxMessageBox(mydata.lat);

        double m_mlat = valuela;
        mydata.lon.ToDouble(&valuelo);
        double m_mlon = valuelo;

        wxDateTime dt;

        // dt = wxDateTime::Now();		// For testing using nmea
        // without time

        dt.ParseDateTime(mydata.time); // When not testing

        time_t t = dt.GetTicks();
        int64_t i = *((int64_t*)&t);

        char number[24]; // dummy size, you should take care of the size!
        sprintf(number, "%.2f", depth2);
        depth2 = atof(number);
        double brg, dist;
        double tide;

        if (m_latprev == 0) {

            tide = 0;
            InsertSounding(depth2, m_mlat, m_mlon, tide, t, 3395);
            m_latprev = m_mlat;
            m_lonprev = m_mlon;
            b_gotposn = true;
            return;
        }

        if (b_gotposn) {
            DistanceBearingMercator_Plugin(
                m_latprev, m_lonprev, m_mlat, m_mlon, &brg, &dist);
            wxString str = wxString::Format(wxT("%6.6f"), (double)dist * 1852);
            // wxMessageBox(str);

            if (dist * 1852 >= m_fMinDistance) {
                tide = 0;
                InsertSounding(depth2, m_mlat, m_mlon, tide, t, 3395);
                m_pSurveyDialog->soundingCount++;

                m_latprev = m_mlat;
                m_lonprev = m_mlon;
            } else {
                b_gotdepth = false;
                return;
            }
        }
    }
}

void survey_pi::timeToZDA(wxString myGPXTime)
{
    wxString s = myGPXTime;
    // 2015-07-09T10:37:35.000Z
    //$IIZDA,055842,15,09,2012,,*5D
    wxString s0 = _T("$IIZDA,");
    wxString s1 = s.Mid(0, 4);
    wxString s2 = s.Mid(5, 2);
    wxString s3 = s.Mid(8, 2);
    wxString s4 = s.Mid(11, 2);
    wxString s5 = s.Mid(14, 2);
    wxString s6 = s.Mid(17, 2);
    wxString s999 = _T(",,*\n");

    wxString s_c = _T(",");
    myTZDA.m_time = s4 + s5 + s6;

    myTZDA.m_ZDA = s0 + s4 + s5 + s6 + s_c + s3 + s_c + s2 + s_c + s1 + s999;
}

wxString survey_pi::DDMLatToDecimal(wxString myString)
{

    wxString s0, s1, s2, s3;
    double d0, d1, d2;
    double value;

    int i = myString.Find(_T("."));
    int l = myString.Len();

    s0 = myString.Mid(0, i - 2);
    s1 = myString.Mid(i - 2, l - i);
    s2 = myString.Mid(l - 1, 1);

    s0.ToDouble(&value);
    d0 = value;

    s1.ToDouble(&value);
    d1 = value / 60;

    d2 = d0 + d1;
    wxString str = wxString::Format(wxT("%6.6f"), (double)d2);

    if (s2 == _T("N")) {
        s3 = wxString::Format(wxT("%6.6f"), (double)d2);
        return s3;
    }
    if (s2 == _T("S")) {
        s3 = wxString::Format(wxT("%6.6f"), (double)d2);
        return _T("-") + s3;
    }
    return _T("");
}

wxString survey_pi::DDMLonToDecimal(wxString myString)
{

    wxString s0, s1, s2, s3;
    double d0, d1, d2;
    double value;

    int i = myString.Find(_T("."));
    int l = myString.Len();

    s0 = myString.Mid(0, i - 2);
    s1 = myString.Mid(i - 2, l - i);
    s2 = myString.Mid(l - 1, 1);

    s0.ToDouble(&value);
    d0 = value;

    s1.ToDouble(&value);
    d1 = value / 60;

    d2 = d0 + d1;

    if (s2 == _T("E")) {
        s3 = wxString::Format(wxT("%6.6f"), (double)d2);
        return s3;
    }
    if (s2 == _T("W")) {
        s3 = wxString::Format(wxT("%6.6f"), (double)d2);
        return _T("-") + s3;
    }
    return _T("");
}

int survey_pi::GetSoundingID(wxString lat, wxString lon)
{
    /*
    SELECT geonameid, name, country,
    Min(ST_Distance(MakePoint(10, 43), geom, 1)) / 1000.0 AS dist_km
FROM airports;

    wxString sql = wxString::Format(_T("SELECT depth, AsText(geom), sounding_id,
survey_id, measured, tide,	Min(ST_Distance(MakePoint(-4.2, 50.5), geom,
1))/ 1000 AS dist_km	FROM sounding"));



    wxString sql = wxString::Format(_T("SELECT depth, AsText(geom), sounding_id,
survey_id, measured, tide FROM sounding"));

    SELECT depth, AsText(geom), sounding_id, survey_id, measured, tide,
    ((X(Geometry)-X(loc))*(X(Geometry)-X(loc))) +
((Y(Geometry)-Y(loc))*(Y(Geometry)-Y(loc))) as distance FROM sounding, (SELECT
MakePoint(-4.2, 50.5) as loc)

    select distance(makePOINT(-4.2, 50.5), geom,1)/1852 from sounding ...
correct!
    //wxString sql = wxString::Format(_T("SELECT depth, AsText(geom),
sounding_id, survey_id, measured, tide, ST_Distance(MakePoint(-4.2, 50.5),
geom)*60 AS dist_nM	FROM sounding"));

    */

    // wxMessageBox(lat);

    string sLat = std::string(lat.mb_str());
    string sLon = std::string(lon.mb_str());

    wxString sql = wxString::Format(
        _T("SELECT depth, AsText(geom), sounding_id, survey_id, measured, ")
        _T("tide,	Min(ST_Distance(MakePoint(%s, %s), geom))*60 AS ")
        _T("dist_nM FROM sounding"),
        sLon, sLat);
    // wxMessageBox(sql);

    dbQuery(sql);

    char** results;
    int n_rows;
    int n_columns;
    char* dpt;
    char* pos;
    char* sdgid;
    char* surid;
    char* measured;
    char* tide;
    char* distance;

    wxString s0, s1;
    ret = sqlite3_get_table(
        m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
    if (ret != SQLITE_OK) {
        /* some error occurred */
        wxLogMessage(_T("Spatialite SQL error: %s\n"), err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    // wxString soundid = wxString::Format(wxT("%i"), n_rows);
    // wxMessageBox(soundid);
    for (int i = 1; i <= n_rows; i++) {
        dpt = results[(i * n_columns) + 0];
        pos = results[(i * n_columns) + 1];
        sdgid = results[(i * n_columns) + 2];
        surid = results[(i * n_columns) + 3];
        measured = results[(i * n_columns) + 4];
        tide = results[(i * n_columns) + 5];
        distance = results[(i * n_columns) + 6];

        double depth = atof(dpt);
        int sounding_id = atoi(sdgid);

        double distance_calc = atof(distance);

        wxString s(pos, wxConvUTF8);

        wxString d(dpt, wxConvUTF8);

        double latl;
        double lonl;

        int f = s.First(_T(" "));
        int l = s.Len();

        s0 = s.Mid(6, f - 6);
        s0.ToDouble(&lonl);

        wxString slat = wxString::Format(wxT("%f"), latl);
        // wxMessageBox(slat);

        s1 = s.Mid(f + 1, l - f - 2);
        s1.ToDouble(&latl);

        wxString slon = wxString::Format(wxT("%f"), lonl);
        // wxMessageBox(slon);

        wxString sid = wxString::Format(wxT("%f"), distance_calc);
        // wxMessageBox(sid);

        sqlite3_free(err_msg);
        DeleteSounding(sounding_id);
    }
    return 1;
}

void survey_pi::OnContextMenuItemCallback(int id)
{
    if (!m_pSurveyDialog)
        return;

    if (id == m_delete_menu_id) {
        m_cursor_lat = GetCursorLat();
        m_cursor_lon = GetCursorLon();

        wxString wLat, wLon;
        wLat = wxString::Format(wxT("%f"), m_cursor_lat);
        wLon = wxString::Format(wxT("%f"), m_cursor_lon);

        GetSoundingID(wLat, wLon);
    }
}

void survey_pi::SetCursorLatLon(double lat, double lon)
{
    m_cursor_lat = lat;
    m_cursor_lon = lon;
}

void survey_pi::OnSurveyDialogClose()
{
    m_bShowSurvey = false;
    SetToolbarItemState(m_leftclick_tool_id, m_bShowSurvey);
    m_pSurveyDialog->Hide();
    SaveConfig();

    // RequestRefresh(m_parent_window); // refresh main window
}

wxString survey_pi::StandardPath()
{
    wxStandardPathsBase& std_path = wxStandardPathsBase::Get();
    wxString s = wxFileName::GetPathSeparator();

#if defined(__WXMSW__)
    wxString stdPath = std_path.GetConfigDir();
#elif defined(__WXGTK__) || defined(__WXQT__)
    wxString stdPath = std_path.GetUserDataDir();
#elif defined(__WXOSX__)
    wxString stdPath = (std_path.GetUserConfigDir() + s + "opencpn");
#endif

    stdPath += s + "plugins";
    if (!wxDirExists(stdPath))
        wxMkdir(stdPath);

    stdPath += s + "survey";

#ifdef __WXOSX__
    // Compatibility with pre-OCPN-4.2; move config dir to
    // ~/Library/Preferences/opencpn if it exists
    wxString oldPath
        = (std_path.GetUserConfigDir() + s + "plugins" + s + "survey");
    if (wxDirExists(oldPath) && !wxDirExists(stdPath)) {
        wxLogMessage("survey_pi: moving config dir %s to %s", oldPath, stdPath);
        wxRenameFile(oldPath, stdPath);
    }
#endif

    if (!wxDirExists(stdPath))
        wxMkdir(stdPath);

    stdPath += s; // is this necessary?
    return stdPath;
}
