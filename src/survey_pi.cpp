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

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/stdpaths.h>
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

survey_pi::survey_pi(void *ppimgr)
      :opencpn_plugin_18(ppimgr)
{
      // Create the PlugIn icons
      initialize_images();
}

void survey_pi::ImportHydromagicTrack(TiXmlElement *track)
{
      TiXmlElement *parameter = track->FirstChildElement();
      while (parameter)
      {
            wxString paramname = wxString::FromUTF8(parameter->Value());
            if (paramname == _T("name"))
            {
                  wxString trackname = wxString::FromUTF8(parameter->GetText());
                  m_activesurvey = CreateSurvey(trackname);
            }
            else if (paramname == _T("desc"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (paramname == _T("visible"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (paramname == _T("lines"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (paramname == _T("values"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (paramname == _T("active"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (paramname == _T("fixes"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (paramname == _T("color"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (paramname == _T("width"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (paramname == _T("textunits"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (paramname == _T("units"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (paramname == _T("textsize"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (paramname == _T("trackpoints"))
            {
                  TiXmlElement *trackpoint = parameter->FirstChildElement();
                  while (trackpoint)
                  {
                        float lat = atof(trackpoint->Attribute("lat"));
                        float lon = atof(trackpoint->Attribute("lon"));
                        float dpt = atof(trackpoint->Attribute("dpt"));
                        float tide = atof(trackpoint->Attribute("tide"));
                        float ele = atof(trackpoint->Attribute("ele"));
                        long time = atol(trackpoint->Attribute("time"));
                        int mark = atoi(trackpoint->Attribute("mark"));
                        InsertSounding(dpt, lat, lon, tide, time);
                        trackpoint = trackpoint->NextSiblingElement();
                  }
            }
            parameter = parameter->NextSiblingElement();
      }
}

void survey_pi::ExportHydromagic(int survey_id, wxString filename)
{
      TiXmlDocument * doc = new TiXmlDocument();
      TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
      TiXmlElement * root = new TiXmlElement("project");

      doc->LinkEndChild(decl);

      TiXmlElement * projection = new TiXmlElement("projection");
      TiXmlElement * grid = new TiXmlElement("grid");
      grid->LinkEndChild(new TiXmlText(wxString::Format(_T("%i"), m_projection).ToUTF8()));
      projection->LinkEndChild(grid);
      root->LinkEndChild(projection);

      TiXmlElement * currentview = new TiXmlElement("currentview");
      root->LinkEndChild(currentview);

      TiXmlElement * colors = new TiXmlElement("colors");
      root->LinkEndChild(colors);

      TiXmlElement * tides = new TiXmlElement("tides");
      root->LinkEndChild(tides);

      TiXmlElement * layers = new TiXmlElement("layers");
      root->LinkEndChild(layers);

      TiXmlElement * comments = new TiXmlElement("comments");
      root->LinkEndChild(comments);

      TiXmlElement * tracks = new TiXmlElement("tracks");
      tracks->SetAttribute("visible", 1);
      TiXmlElement * track = new TiXmlElement("track");

      TiXmlElement * name = new TiXmlElement("name");
      name->LinkEndChild(new TiXmlText(GetSurveyName(survey_id).mb_str()));
      track->LinkEndChild(name);

      TiXmlElement * desc = new TiXmlElement("desc");
      track->LinkEndChild(desc);

      TiXmlElement * visible = new TiXmlElement("visible");
      visible->LinkEndChild(new TiXmlText("1"));
      track->LinkEndChild(visible);

      TiXmlElement * lines = new TiXmlElement("lines");
      lines->LinkEndChild(new TiXmlText("1"));
      track->LinkEndChild(lines);

      TiXmlElement * values = new TiXmlElement("values");
      values->LinkEndChild(new TiXmlText("1"));
      track->LinkEndChild(values);

      TiXmlElement * active = new TiXmlElement("active");
      active->LinkEndChild(new TiXmlText("1"));
      track->LinkEndChild(active);

      TiXmlElement * fixes = new TiXmlElement("fixes");
      fixes->LinkEndChild(new TiXmlText("1"));
      track->LinkEndChild(fixes);

      TiXmlElement * color = new TiXmlElement("color");
      color->LinkEndChild(new TiXmlText("0"));
      track->LinkEndChild(color);

      TiXmlElement * width = new TiXmlElement("width");
      width->LinkEndChild(new TiXmlText("4"));
      track->LinkEndChild(width);

      TiXmlElement * textunits = new TiXmlElement("textunits");
      textunits->LinkEndChild(new TiXmlText("0"));
      track->LinkEndChild(textunits);

      TiXmlElement * units = new TiXmlElement("units");
      units->LinkEndChild(new TiXmlText("0"));
      track->LinkEndChild(units);

      TiXmlElement * textsize = new TiXmlElement("textsize");
      textsize->LinkEndChild(new TiXmlText("12"));
      track->LinkEndChild(textsize);

      TiXmlElement * trackpoints = new TiXmlElement("trackpoints");
      wxString sql = wxString::Format(_T("SELECT depth, AsText(geom), tide, strftime('%%s', measured) FROM sounding WHERE survey_id=%i"), survey_id);
      char **results;
      int n_rows;
      int n_columns;
      char *dpt;
      char *pos;
      char *tid;
      char *tim;

      ret = sqlite3_get_table (m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
	if (ret != SQLITE_OK)
	{
/* some error occurred */
            wxLogMessage (_T("Spatialite SQL error: %s\n"), err_msg);
	      sqlite3_free (err_msg);
	      return;
	}

      for (int i = 1; i <= n_rows; i++)
	{
		dpt = results[(i * n_columns) + 0];
            pos = results[(i * n_columns) + 1];
            tid = results[(i * n_columns) + 2];
            tim = results[(i * n_columns) + 3];
            double tide = atof(tid);
            wxString s(pos, wxConvUTF8);
            wxString latl;
            wxString lonl;
            lonl = s.SubString(6, s.First(_T(" ")));
            latl = s.SubString(s.First(_T(" ")), s.Length() - 1);
            TiXmlElement * trackpoint = new TiXmlElement("trackpoint");
            trackpoint->SetAttribute("lat", latl.ToUTF8());
            trackpoint->SetAttribute("lon", lonl.ToUTF8());
            trackpoint->SetAttribute("dpt", dpt);
            trackpoint->SetAttribute("tide", tid);
            trackpoint->SetAttribute("ele", 0);
            trackpoint->SetAttribute("time", tim);
            trackpoint->SetAttribute("mark", 0);
            trackpoints->LinkEndChild(trackpoint);
      }
      track->LinkEndChild(trackpoints);
      tracks->LinkEndChild(track);
      root->LinkEndChild(tracks);

      TiXmlElement * waypoints = new TiXmlElement("waypoints");
      root->LinkEndChild(waypoints);

      TiXmlElement * routes = new TiXmlElement("routes");
      root->LinkEndChild(routes);

      TiXmlElement * areas = new TiXmlElement("areas");
      root->LinkEndChild(areas);

      TiXmlElement * sections = new TiXmlElement("sections");
      root->LinkEndChild(sections);

      TiXmlElement * soundvelocity = new TiXmlElement("soundvelocity");
      root->LinkEndChild(soundvelocity);

      TiXmlElement * draftcorrections = new TiXmlElement("draftcorrections");
      root->LinkEndChild(draftcorrections);

      doc->LinkEndChild(root);

      doc->SaveFile(filename.mb_str());

      doc->Clear();

      delete doc;
}


bool survey_pi::ImportHydromagic(wxString filename)
{
      TiXmlDocument *doc = new TiXmlDocument();
      doc->LoadFile(filename.mb_str());
      TiXmlElement *project = doc->FirstChildElement();
      if (!project)
      {
            doc->Clear();
            delete doc;
            return false;
      }
      TiXmlElement *section =  project->FirstChildElement();

      while (section)
      {
            wxString secname = wxString::FromUTF8(section->Value());
            if (secname == _T("projection"))
            {
                  const char *val = section->FirstChildElement()->GetText();
                  m_projection = atoi(val);
            }
            else if (secname == _T("currentview"))
            {
                  //TODO: We do not need this, so let's ignore it for now
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
            }
            else if (secname == _T("colors"))
            {
                  //TODO: We do not need this, so let's ignore it for now
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
            }
            else if (secname == _T("tides"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (secname == _T("layers"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (secname == _T("comments"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (secname == _T("tracks"))
            {
                  TiXmlElement *track = section->FirstChildElement();
                  while(track)
                  {
                        ImportHydromagicTrack(track);
                        track = track->NextSiblingElement();
                  }
            }
            else if (secname == _T("waypoints"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (secname == _T("routes"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (secname == _T("areas"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (secname == _T("sections"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (secname == _T("soundvelocity"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            else if (secname == _T("draftcorrections"))
            {
                  //TODO: We do not need this, so let's ignore it for now
            }
            section = section->NextSiblingElement();
      }

      doc->Clear();
      delete doc;
      return true;
}

bool survey_pi::dbQuery(wxString sql)
{
      if (!b_dbUsable)
            return false;
      ret = sqlite3_exec (m_database, sql.mb_str(), NULL, NULL, &err_msg);
      if (ret != SQLITE_OK)
      {
            // some error occurred
            wxLogMessage (_T("Database error: %s in query: %s\n"), *err_msg, sql.c_str());
	      sqlite3_free (err_msg);
            b_dbUsable = false;
      }
      return b_dbUsable;
}

void survey_pi::dbGetTable(wxString sql, char ***results, int &n_rows, int &n_columns)
{
      ret = sqlite3_get_table (m_database, sql.mb_str(), results, &n_rows, &n_columns, &err_msg);
      if (ret != SQLITE_OK)
      {
            wxLogMessage (_T("Database error: %s in query: %s\n"), *err_msg, sql.c_str());
	      sqlite3_free (err_msg);
            b_dbUsable = false;
      } 
}

wxArrayString survey_pi::GetSurveyList()
{
      char **result;
      int n_rows;
      int n_columns;
      dbGetTable(_T("SELECT * FROM survey"), &result, n_rows, n_columns);
      wxArrayString surveys;
      for (int i = 1; i <= n_rows; i++)
	{
		char *id = result[(i * n_columns) + 0];
            char *name = result[(i * n_columns) + 1];
            int survey_id = atoi(id);
            wxString survey_name(name, wxConvUTF8);
            surveys.Add(survey_name);
	}
      dbFreeResults(result);
      return surveys;
}

wxString survey_pi::dbGetStringValue(wxString sql)
{
      char **result;
      int n_rows;
      int n_columns;
      dbGetTable(sql, &result, n_rows, n_columns);
      wxArrayString surveys;
      wxString ret = wxString::FromUTF8(result[1]);
      dbFreeResults(result);
      if(n_rows == 1)
            return ret;
      else
            return wxEmptyString;
}

int survey_pi::dbGetIntNotNullValue(wxString sql)
{
      char **result;
      int n_rows;
      int n_columns;
      dbGetTable(sql, &result, n_rows, n_columns);
      wxArrayString surveys;
      int ret = atoi(result[1]);
      dbFreeResults(result);
      if(n_rows == 1)
            return ret;
      else
            return 0;
}

wxString survey_pi::GetSurveyName(int survey_id)
{
      return dbGetStringValue(wxString::Format(_T("SELECT survey_name FROM survey WHERE survey_id=%i"), survey_id));
}

int survey_pi::GetSurveyId(wxString survey_name)
{
      return dbGetIntNotNullValue(wxString::Format(_T("SELECT survey_id FROM survey WHERE survey_name='%s'"), survey_name.c_str()));
}

void survey_pi::dbFreeResults(char **results)
{
      sqlite3_free_table (results);
}

int survey_pi::CreateSurvey(wxString name)
{
      wxString sql = wxString::Format(_T("INSERT INTO survey (survey_name, created, submitted) VALUES ('%s', current_timestamp, 0)"), name.c_str());
      dbQuery(sql);
      return sqlite3_last_insert_rowid(m_database);
}

void survey_pi::DeleteSurvey(int id)
{
      wxString sql = wxString::Format(_T("DELETE FROM sounding WHERE survey_id=%i"), id);
      dbQuery(sql);
      sql = wxString::Format(_T("DELETE FROM survey WHERE survey_id=%i"), id);
      dbQuery(sql);
      m_activesurvey = 0;
      m_activesurveyname = wxEmptyString;
}

int survey_pi::InsertSounding(double depth, double lat, double lon, double tide, time_t timestamp, int projection)
{
      wxString time = _T("current_timestamp");
      if (timestamp > 0)
            time = wxDateTime(timestamp).FormatISODate().Append(_T(" ")).Append(wxDateTime(timestamp).FormatISOTime()); 
      wxString sql = wxString::Format(_T("INSERT INTO \"sounding\" (\"depth\", \"measured\", \"survey_id\", \"geom\", \"tide\",) VALUES (%f , %s, %i, GeomFromText('POINT(%f %f)', %i), %f)"), depth, time.c_str(), m_activesurvey, lon, lat, projection, tide);
      dbQuery (sql);
      return sqlite3_last_insert_rowid(m_database);
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
      AddLocaleCatalog( _T("opencpn-survey_pi") );
      spatialite_init(0);
      err_msg = NULL;
      wxString sql;

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

      //      Establish the location of the config file
      wxString dbpath;

//      Establish a "home" location
      wxStandardPathsBase& std_path = wxStandardPaths::Get();

      wxString pHome_Locn;
#ifdef __WXMSW__
      pHome_Locn.Append(std_path.GetConfigDir());          // on w98, produces "/windows/Application Data"
#else
      pHome_Locn.Append(std_path.GetUserConfigDir());
#endif
      appendOSDirSlash(&pHome_Locn) ;
#ifdef __WXMSW__
      dbpath = _T(DATABASE_NAME);
      dbpath.Prepend(pHome_Locn);

#elif defined __WXOSX__
      dbpath = std_path.GetUserConfigDir(); // should be ~/Library/Preferences
      appendOSDirSlash(&dbpath) ;
      dbpath.Append(_T(DATABASE_NAME));
#else
      dbpath = std_path.GetUserDataDir(); // should be ~/.opencpn
      appendOSDirSlash(&dbpath) ;
      dbpath.Append(_T(DATABASE_NAME));
#endif
      
      bool newDB = !wxFileExists(dbpath);
      b_dbUsable = true;

      ret = sqlite3_open_v2 (dbpath.mb_str(), &m_database, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
      if (ret != SQLITE_OK)
      {
            wxLogMessage (_T("SURVEY_PI: cannot open '%s': %s\n"), DATABASE_NAME, sqlite3_errmsg (m_database));
	      sqlite3_close (m_database);
	      b_dbUsable = false;
      }
      
      if (newDB && b_dbUsable)
      {
            //create empty db
            dbQuery(_T("SELECT InitSpatialMetadata()"));
            //dbQuery(_T("INSERT INTO spatial_ref_sys (srid, auth_name, auth_srid, ref_sys_name, proj4text) VALUES(32632, 'epsg', 32632, 'WGS 84 / UTM zone 32N', '+proj=utm +zone=32 +ellps=WGS84 +datum=WGS84 +units=m +no_defs ')"));
            //CREATE OUR TABLES
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
            dbQuery(_T("SELECT AddGeometryColumn('sounding', 'geom', 32632, 'POINT', 2)"));
      }

      //Update DB structure and contents
      if (b_dbUsable)
      {
            char **results;
            int n_rows;
            int n_columns;
            sql = _T("SELECT value FROM settings WHERE key = 'DBVersion'");
            ret = sqlite3_get_table (m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
            if (ret != SQLITE_OK)
            {
                  sqlite3_free (err_msg);
                  sql = _T("CREATE TABLE settings (")
                  _T("key TEXT NOT NULL UNIQUE,")
                  _T("value TEXT)");
                  dbQuery(sql);
                  dbQuery(_T("INSERT INTO settings (key, value) VALUES ('DBVersion', '2')"));
                  db_ver = 2;
            } 
            else
            {
                  db_ver = atoi(results[1]);
            }
            sqlite3_free_table (results);
            wxLogMessage (_T("SURVEY_PI: Database version: %i\n"), db_ver);
      }

      if (b_dbUsable && db_ver == 2)
      {
            sql = _T("ALTER TABLE sounding ")
                  _T("ADD COLUMN tide REAL NOT NULL DEFAULT 0.0");
            dbQuery(sql);
            dbQuery(_T("UPDATE settings SET value = 3 WHERE key = 'DBVersion'"));
            db_ver = 3;
      }
      if (b_dbUsable && db_ver == 3)
      {
            sql = _T("CREATE TABLE new_sounding (")
                  _T("sounding_id INTEGER PRIMARY KEY AUTOINCREMENT,")
                  _T("depth REAL NOT NULL,")
                  _T("measured INTEGER NOT NULL,")
                  _T("survey_id INTEGER NOT NULL,")
                  _T("tide REAL NOT NULL DEFAULT 0.0,")
                  _T("FOREIGN KEY(survey_id) REFERENCES survey(survey_id))");
            dbQuery(sql);
            dbQuery(_T("SELECT AddGeometryColumn('new_sounding', 'geom', 3395, 'POINT', 2)"));
            dbQuery(_T("INSERT INTO new_sounding SELECT sounding_id, depth, measured, survey_id, tide, SetSRID('geom', 3395) FROM sounding"));
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
            dbQuery(_T("SELECT AddGeometryColumn('sounding', 'geom', 3395, 'POINT', 2)"));
            dbQuery(_T("INSERT INTO sounding SELECT sounding_id, depth, measured, survey_id, tide, SetSRID('geom', 3395) FROM new_sounding"));
            dbQuery(_T("DROP TABLE new_sounding"));
            dbQuery(_T("SELECT DiscardGeometryColumn('new_sounding', 'geom')"));
            dbQuery(_T("UPDATE settings SET value = 4 WHERE key = 'DBVersion'"));
            db_ver = 4;
      }

      //    This PlugIn needs a toolbar icon, so request its insertion
      m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_survey, _img_survey, wxITEM_NORMAL,
            _("Survey"), _T(""), NULL,
             SURVEY_TOOL_POSITION, 0, this);

      m_pSurveyDialog = NULL;

      return (WANTS_TOOLBAR_CALLBACK    |
              INSTALLS_TOOLBAR_TOOL     |
              WANTS_PREFERENCES         |
              WANTS_NMEA_SENTENCES      |
              WANTS_NMEA_EVENTS         |
              WANTS_OVERLAY_CALLBACK    |
              WANTS_ONPAINT_VIEWPORT    |
              //WANTS_OPENGL_OVERLAY_CALLBACK |
              //WANTS_DYNAMIC_OPENGL_OVERLAY_CALLBACK |
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
      sqlite3_close (m_database);
      spatialite_cleanup();
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

      DimeWindow(m_pSurveyDialog);
}

void survey_pi::FillSurveyDropdown()
{
      m_pSurveyDialog->m_chSurvey->Clear();
      m_pSurveyDialog->m_chSurvey->Append(GetSurveyList());
      for(int i = 0; i < GetSurveyList().Count(); i++)
            if (GetSurveyList()[i] == m_activesurveyname)
                  m_pSurveyDialog->m_chSurvey->SetSelection(i);
}

void survey_pi::OnToolbarToolCallback(int id)
{
      if(NULL == m_pSurveyDialog)
      {
            m_pSurveyDialog = new SurveyDlg(m_parent_window);
            m_pSurveyDialog->plugin = this;
            m_pSurveyDialog->Move(wxPoint(m_survey_dialog_x, m_survey_dialog_y));
            FillSurveyDropdown();
      }

      m_pSurveyDialog->Show(!m_pSurveyDialog->IsShown());
}

bool survey_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings/Survey" ) );

            pConf->Read ( _T ( "Opacity" ),  &m_iOpacity, 255 );
            pConf->Read ( _T ( "Units" ),  &m_iUnits, METERS );
            pConf->Read ( _T ( "RenderOverlay" ), &m_bRenderOverlay, true );
            pConf->Read ( _T ( "RenderAll" ), &m_bRenderAllSurveys, true );
            pConf->Read ( _T ( "ConnectSoundings" ), &m_bConnectSoundings, true );
            pConf->Read ( _T ( "SoundingColor" ), &m_sSoundingColor );
            pConf->Read ( _T ( "ConnectorColor" ), &m_sConnectorColor );
            pConf->Read ( _T ( "SoundingFont" ), &m_sFont );
            pConf->Read ( _T ( "SoundingFontColor" ), &m_sFontColor );
            pConf->Read ( _T ( "ActiveSurvey" ), &m_activesurvey, 0 );

            pConf->Read ( _T ( "LOA" ), &m_fLOA, 0.0 );
            pConf->Read ( _T ( "Beam" ), &m_fBeam, 0.0 );
            pConf->Read ( _T ( "SounderBow" ), &m_fSounderBow, 0.0 );
            pConf->Read ( _T ( "WaterlineOffset" ), &m_fWaterlineOffset, 0.0 );
            pConf->Read ( _T ( "GPSBow" ), &m_fGPSBow, 0.0 );
            pConf->Read ( _T ( "GPSPort" ), &m_fGPSPort, 0.0 );
            pConf->Read ( _T ( "MinSoundingDist" ), &m_fMinDistance, 0.0 );
            pConf->Read ( _T ( "AutoNewDistance" ), &m_fAutoNewDistance, 0.0 );

            m_survey_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
            m_survey_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );

            if((m_survey_dialog_x < 0) || (m_survey_dialog_x > m_display_width))
                  m_survey_dialog_x = 5;
            if((m_survey_dialog_y < 0) || (m_survey_dialog_y > m_display_height))
                  m_survey_dialog_y = 5;
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
            pConf->Write ( _T ( "Opacity" ), m_iOpacity );
            pConf->Write ( _T ( "Units" ), m_iUnits );
            pConf->Write ( _T ( "RenderOverlay" ), m_bRenderOverlay );
            pConf->Write ( _T ( "RenderAll" ), m_bRenderAllSurveys );
            pConf->Write ( _T ( "ConnectSoundings" ), m_bConnectSoundings );
            pConf->Write ( _T ( "SoundingColor" ), m_sSoundingColor );
            pConf->Write ( _T ( "ConnectorColor" ), m_sConnectorColor );
            pConf->Write ( _T ( "SoundingFont" ), m_sFont );
            pConf->Write ( _T ( "SoundingFontColor" ), m_sFontColor );
            pConf->Write ( _T ( "ActiveSurvey" ), m_activesurvey );

            pConf->Write ( _T ( "LOA" ), m_fLOA );
            pConf->Write ( _T ( "Beam" ), m_fBeam );
            pConf->Write ( _T ( "SounderBow" ), m_fSounderBow );
            pConf->Write ( _T ( "WaterlineOffset" ), m_fWaterlineOffset );
            pConf->Write ( _T ( "GPSBow" ), m_fGPSBow );
            pConf->Write ( _T ( "GPSPort" ), m_fGPSPort );
            pConf->Write ( _T ( "MinSoundingDist" ), m_fMinDistance );
            pConf->Write ( _T ( "AutoNewDistance" ), m_fAutoNewDistance );


            pConf->Write ( _T ( "DialogPosX" ),   m_survey_dialog_x );
            pConf->Write ( _T ( "DialogPosY" ),   m_survey_dialog_y );

            return true;
      }
      else
            return false;
}

void survey_pi::ShowPreferencesDialog( wxWindow* parent )
{
      SurveyCfgDlg *dialog = new SurveyCfgDlg( parent, wxID_ANY, _("Survey Preferences"), wxPoint( m_survey_dialog_x, m_survey_dialog_y), wxDefaultSize, wxDEFAULT_DIALOG_STYLE );
      dialog->Fit();
      wxColour cl;
      DimeWindow(dialog);
      dialog->m_cbRenderOverlay->SetValue(m_bRenderOverlay);
      dialog->m_sOpacity->SetValue(m_iOpacity);
      switch(m_iUnits)
      {
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
      dialog->m_rbRendedAllSurveys->SetValue(m_bRenderAllSurveys);
      dialog->m_rbRenderActiveSurvey->SetValue(!m_bRenderAllSurveys);
      dialog->m_cbConnectSoundings->SetValue(m_bConnectSoundings);
      dialog->m_cpSoundingColor->SetColour(m_sSoundingColor);
      dialog->m_cpConnectorColor->SetColour(m_sConnectorColor);
      dialog->m_fpFont->SetSelectedFont(wxFont(m_sFont));
      dialog->m_cpFontColor->SetColour(m_sFontColor);
      dialog->m_tLOA->SetValue(wxString::Format(wxT("%f"), m_fLOA));
      dialog->m_tBeam->SetValue(wxString::Format(wxT("%f"), m_fBeam));
      dialog->m_tSounderBow->SetValue(wxString::Format(wxT("%f"), m_fSounderBow));
      dialog->m_tWaterlineOffset->SetValue(wxString::Format(wxT("%f"), m_fWaterlineOffset));
      dialog->m_tGPSBow->SetValue(wxString::Format(wxT("%f"), m_fGPSBow));
      dialog->m_tGPSPort->SetValue(wxString::Format(wxT("%f"), m_fGPSPort));
      dialog->m_tMinDistance->SetValue(wxString::Format(wxT("%f"), m_fMinDistance));
      dialog->m_tAutoNewDistance->SetValue(wxString::Format(wxT("%f"), m_fAutoNewDistance));

      if(dialog->ShowModal() == wxID_OK)
      {
            m_bRenderOverlay = dialog->m_cbRenderOverlay->GetValue();
            m_iOpacity = dialog->m_sOpacity->GetValue();
            if (dialog->m_rbMeters->GetValue())
                  m_iUnits = METERS;
            else if (dialog->m_rbFeet->GetValue())
                  m_iUnits = FEET;
            else
                  m_iUnits = FATHOMS;
            m_bRenderAllSurveys = dialog->m_rbRendedAllSurveys->GetValue();
            m_bConnectSoundings = dialog->m_cbConnectSoundings->GetValue();
            m_sSoundingColor = dialog->m_cpSoundingColor->GetColour().GetAsString();
            m_sConnectorColor = dialog->m_cpConnectorColor->GetColour().GetAsString();
            m_sFont = dialog->m_fpFont->GetSelectedFont().GetNativeFontInfoDesc();
            m_sFontColor = dialog->m_cpFontColor->GetColour().GetAsString();
            double value;
            if(!dialog->m_tLOA->GetValue().ToDouble(&value)){ value = 0.0; }
            m_fLOA = value;
            if(!dialog->m_tBeam->GetValue().ToDouble(&value)){ value = 0.0; }
            m_fBeam = value;
            if(!dialog->m_tSounderBow->GetValue().ToDouble(&value)){ value = 0.0; }
            m_fSounderBow = value;
            if(!dialog->m_tWaterlineOffset->GetValue().ToDouble(&value)){ value = 0.0; }
            m_fWaterlineOffset = value;
            if(!dialog->m_tGPSBow->GetValue().ToDouble(&value)){ value = 0.0; }
            m_fGPSBow = value;
            if(!dialog->m_tGPSPort->GetValue().ToDouble(&value)){ value = 0.0; }
            m_fGPSPort = value;
            if(!dialog->m_tMinDistance->GetValue().ToDouble(&value)){ value = 0.0; }
            m_fMinDistance = value;
            if(!dialog->m_tAutoNewDistance->GetValue().ToDouble(&value)){ value = 0.0; }
            m_fAutoNewDistance = value;

            SaveConfig();
      }
      delete dialog;
}

void survey_pi::DrawSounding(wxDC &dc, int x, int y, double depth, long sounding_id, long survey_id)
{
      double coef = 1.0;
      if (m_iUnits == FATHOMS)
            coef = 1.8288;
      else if (m_iUnits == FEET)
            coef = 0.3048;
      wxPen p (wxColour(m_sSoundingColor), 2);
      dc.SetPen(p);
      dc.DrawLine(x - 5, y - 5, x + 5, y + 5);
      dc.DrawLine(x - 5, y + 5, x + 5, y - 5);
      dc.SetFont(wxFont(m_sFont));
      dc.SetTextForeground(wxColour(m_sFontColor));
      dc.DrawText(wxString::Format(wxT("%.1f"), depth / coef), x, y);
      if (m_bConnectSoundings)
      {
            if (mLastX != -1 && mLastY != -1 && mLastSdgId == sounding_id -1 && mLastSurveyId == survey_id)
            {
                  wxPen p (wxColour(m_sConnectorColor), 1);
                  dc.SetPen(p);
                  dc.DrawLine(x, y, mLastX, mLastY);
            }
            mLastX = x;
            mLastY = y;
            mLastSdgId = sounding_id;
            mLastSurveyId = survey_id;
      }
}

bool survey_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
      if (!b_dbUsable || !m_bRenderOverlay)
            return false;
      //wxString sql = wxString::Format(_T("SELECT depth, AsText(geom) FROM sounding WHERE Within(geom, PolygonFromText('POLYGON((%f %f, %f %f, %f %f, %f %f))'))"), vp->lat_min, vp->lon_min, vp->lat_min, vp->lon_max, vp->lat_max, vp->lon_max, vp->lat_max, vp->lon_min);
      wxString sql = wxString::Format(_T("SELECT depth, AsText(geom), sounding_id, survey_id FROM sounding WHERE MbrWithin(geom,BuildMbr(%f, %f, %f, %f))"), vp->lon_min, vp->lat_min, vp->lon_max, vp->lat_max);
      char **results;
      int n_rows;
      int n_columns;
      char *dpt;
      char *pos;
      char *sdgid;
      char *surid;

      ret = sqlite3_get_table (m_database, sql.mb_str(), &results, &n_rows, &n_columns, &err_msg);
	if (ret != SQLITE_OK)
	{
/* some error occurred */
            wxLogMessage (_T("Spatialite SQL error: %s\n"), err_msg);
	      sqlite3_free (err_msg);
	      return false;
	}

      for (int i = 1; i <= n_rows; i++)
	{
		dpt = results[(i * n_columns) + 0];
            pos = results[(i * n_columns) + 1];
            sdgid = results[(i * n_columns) + 2];
            surid = results[(i * n_columns) + 3];
            double depth = atof(dpt);
            wxString s(pos, wxConvUTF8);
            double latl;
            double lonl;
            s.SubString(6, s.First(_T(" "))).ToDouble(&lonl);
            s.SubString(s.First(_T(" ")), s.Length() - 1).ToDouble(&latl);
            wxPoint pl;
            GetCanvasPixLL(vp, &pl, latl, lonl);
            if(dc.IsOk())
            {
                  DrawSounding(dc, pl.x, pl.y, depth, atoi(sdgid), atoi(surid));
            }
	}
      
      mLastX = -1;
      mLastY = -1;
      
      //if(PointInLLBox(vp, lon, lat) || PointInLLBox(vp, lon-360., lat))
      sqlite3_free_table (results);
      return true;
}

//bool survey_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
//{
//      return false;
//}

// Is the given point in the vp ??
bool PointInLLBox(PlugIn_ViewPort *vp, double x, double y)
{


    if (  x >= (vp->lon_min) && x <= (vp->lon_max) &&
            y >= (vp->lat_min) && y <= (vp->lat_max) )
            return TRUE;
    return FALSE;
}

void survey_pi::StoreSounding(double depth)
{
      if (m_lastPosReport.Subtract(wxDateTime::Now()).GetSeconds() > 2 || m_lat == 999.0 || m_lon == 999.0)
            return;
      InsertSounding(depth, m_lat, m_lon);
}

void survey_pi::SetNMEASentence(wxString &sentence)
{
      m_NMEA0183 << sentence;

      if(m_NMEA0183.PreParse())
      {
            if(m_NMEA0183.LastSentenceIDReceived == _T("DBT"))
            {
                  if(m_NMEA0183.Parse())
                  {
                        if (mPriDepth >= 1)
                        {
                              mPriDepth = 1;

                              StoreSounding(m_NMEA0183.Dbt.DepthMeters);
                        }
                  }
            }

            else if(m_NMEA0183.LastSentenceIDReceived == _T("DPT"))
            {
                  if(m_NMEA0183.Parse())
                  {
                        if (mPriDepth >= 2)
                        {
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
            else if(m_NMEA0183.LastSentenceIDReceived == _T("GGA"))
            {
                  if(m_NMEA0183.Parse())
                  {
                        if(m_NMEA0183.Gga.GPSQuality > 0)
                        {
                              if (mPriPosition >= 3) {
                                    mPriPosition = 3;
                                    float llt = m_NMEA0183.Gga.Position.Latitude.Latitude;
                                    int lat_deg_int = (int)(llt / 100);
                                    float lat_deg = lat_deg_int;
                                    float lat_min = llt - (lat_deg * 100);
                                    m_lat = lat_deg + (lat_min/60.);
                                    if(m_NMEA0183.Gga.Position.Latitude.Northing == South)
                                          m_lat = -m_lat;

                                    float lln = m_NMEA0183.Gga.Position.Longitude.Longitude;
                                    int lon_deg_int = (int)(lln / 100);
                                    float lon_deg = lon_deg_int;
                                    float lon_min = lln - (lon_deg * 100);
                                    m_lon = lon_deg + (lon_min/60.);
                                    if(m_NMEA0183.Gga.Position.Longitude.Easting == West)
                                          m_lon = -m_lon;
                                    m_lastPosReport = wxDateTime::Now();
                              }
                        }
                  }
            }

            else if(m_NMEA0183.LastSentenceIDReceived == _T("GLL"))
            {
                  if(m_NMEA0183.Parse())
                  {
                        if(m_NMEA0183.Gll.IsDataValid == NTrue)
                        {
                              if (mPriPosition >= 2) {
                                    mPriPosition = 2;
                                    float llt = m_NMEA0183.Gll.Position.Latitude.Latitude;
                                    int lat_deg_int = (int)(llt / 100);
                                    float lat_deg = lat_deg_int;
                                    float lat_min = llt - (lat_deg * 100);
                                    m_lat = lat_deg + (lat_min/60.);
                                    if(m_NMEA0183.Gll.Position.Latitude.Northing == South)
                                          m_lat = -m_lat;

                                    float lln = m_NMEA0183.Gll.Position.Longitude.Longitude;
                                    int lon_deg_int = (int)(lln / 100);
                                    float lon_deg = lon_deg_int;
                                    float lon_min = lln - (lon_deg * 100);
                                    m_lon = lon_deg + (lon_min/60.);
                                    if(m_NMEA0183.Gll.Position.Longitude.Easting == West)
                                          m_lon = -m_lon;
                                    m_lastPosReport = wxDateTime::Now();
                              }

                        }
                  }
            }

            else if(m_NMEA0183.LastSentenceIDReceived == _T("RMC"))
            {
                  if(m_NMEA0183.Parse())
                  {
                        if(m_NMEA0183.Rmc.IsDataValid == NTrue)
                        {
                              if (mPriPosition >= 4) {
                                    mPriPosition = 4;
                                    float llt = m_NMEA0183.Rmc.Position.Latitude.Latitude;
                                    int lat_deg_int = (int)(llt / 100);
                                    float lat_deg = lat_deg_int;
                                    float lat_min = llt - (lat_deg * 100);
                                    m_lat = lat_deg + (lat_min/60.);
                                    if(m_NMEA0183.Rmc.Position.Latitude.Northing == South)
                                          m_lat = -m_lat;

                                    float lln = m_NMEA0183.Rmc.Position.Longitude.Longitude;
                                    int lon_deg_int = (int)(lln / 100);
                                    float lon_deg = lon_deg_int;
                                    float lon_min = lln - (lon_deg * 100);
                                    m_lon = lon_deg + (lon_min/60.);
                                    if(m_NMEA0183.Rmc.Position.Longitude.Easting == West)
                                          m_lon = -m_lon;
                                    m_lastPosReport = wxDateTime::Now();
                              }
                        }
                  }
            }
      }
}