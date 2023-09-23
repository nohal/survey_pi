/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Tide and Current Manager
 * Author:   David Register
 * Todo add original author
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
 *   bdbcat@yahoo.com   *
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
 *
 **
 *
 */

#ifndef __TCMGR_H__
#define __TCMGR_H__

// ----------------------------------------------------------------------------
// external C linkages
// ----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//   constants
//----------------------------------------------------------------------------
typedef enum { LENGTH, VELOCITY, BOGUS } unit_type;
typedef struct {
    char* name;
    char* abbrv;
    unit_type type;
    double conv_factor;
} unit;

#define NUMUNITS 4

#define MAXNAMELEN 90
#define USF_REMOVE 1
#define USF_UPDATE 2
#define USF_WRITE 3

#ifndef M_PI
#define M_PI 3.141592654
#endif

#define linelen 300

#define TIDE_MAX_DERIV (2) // Maximum derivative supported
/* TIDE_TIME_PREC
 *   Precision (in seconds) to which we will find roots
 */
#define TIDE_TIME_PREC (15)

/* TIDE_TIME_BLEND
 *   Half the number of seconds over which to blend the tides from
 *   one epoch to the next.
 */
#define TIDE_BLEND_TIME (3600)

/* TIDE_TIME_STEP
 *   We are guaranteed to find all high and low tides as long as their
 * spacing is greater than this value (in seconds).
 */
#define TIDE_TIME_STEP (TIDE_TIME_PREC)
#define TIDE_BAD_TIME ((time_t)-1)

//    class/struct declarations

class Station_Data;

typedef struct {
    int type;
    char* short_s;
    char* long_s;
} abbreviation_entry;

#define REGION 1
#define COUNTRY 2
#define STATE 3

/*
typedef struct {
   void     *IDX_next;                      // Points to next linked item
   int       IDX_rec_num;                   // Keeps track of multiple entries
w/same name char      IDX_type;                      // Entry "TCtcIUu"
identifier char      IDX_zone[40];                  // Alpha
region/country/state ID char      IDX_station_name[MAXNAMELEN];  // Name of
station double    IDX_lon;                       // Longitude (+East) double
IDX_lat;                       // Latitude (+North) int       IDX_time_zone; //
Minutes offset from UTC int       IDX_ht_time_off;               // High tide
offset in minutes float     IDX_ht_mpy;                    // High tide
multiplier (nom 1.0) float     IDX_ht_off;                    // High tide level
offset (feet?) int       IDX_lt_time_off;               // Low tide offset in
minutes float     IDX_lt_mpy;                    // Low tide multiplier
(nom 1.0) float     IDX_lt_off;                    // Low tide level offset
(feet?) int       IDX_sta_num;                   // Subordinate station number
   int       IDX_flood_dir;                 // Added DSR opencpn
   int       IDX_ebb_dir;
   int       IDX_tried_once;                // Master station search control
   int       IDX_Useable;
   int       Valid15;
   float     Value15;
   float     Dir15;
   bool      Ret15;
   char     *IDX_tzname;                    // Timezone name
   int       IDX_ref_file_num;              // # of reference file where
reference station is char      IDX_reference_name[MAXNAMELEN];// Name of
reference station Station_Data   *pref_sta_data;           // Pointer to the
Reference Station Data } IDX_entry;
*/

class IDX_entry {
public:
    void* IDX_next; // Points to next linked item
    int IDX_rec_num; // Keeps track of multiple entries w/same name
    char IDX_type; // Entry "TCtcIUu" identifier
    char IDX_zone[40]; // Alpha region/country/state ID
    char IDX_station_name[MAXNAMELEN]; // Name of station
    double IDX_lon; // Longitude (+East)
    double IDX_lat; // Latitude (+North)
    int IDX_time_zone; // Minutes offset from UTC
    int IDX_ht_time_off; // High tide offset in minutes
    float IDX_ht_mpy; // High tide multiplier (nom 1.0)
    float IDX_ht_off; // High tide level offset (feet?)
    int IDX_lt_time_off; // Low tide offset in minutes
    float IDX_lt_mpy; // Low tide multiplier (nom 1.0)
    float IDX_lt_off; // Low tide level offset (feet?)
    int IDX_sta_num; // Subordinate station number
    int IDX_flood_dir; // Added DSR opencpn
    int IDX_ebb_dir;
    int IDX_tried_once; // Master station search control
    int IDX_Useable;
    int Valid15;
    float Value15;
    float Dir15;
    bool Ret15;
    bool b_is_secondary;
    char* IDX_tzname; // Timezone name
    int IDX_ref_file_num; // # of reference file where reference station is
    char IDX_reference_name[MAXNAMELEN]; // Name of reference station
    int IDX_ref_dbIndex; // tcd index of reference station
    Station_Data* pref_sta_data; // Pointer to the Reference Station Data
};

typedef struct {
    void* next;
    short int rec_start;
    char* name;
} harmonic_file_entry;

//----------------------------------------------------------------------------
//   Reference Station Data
//----------------------------------------------------------------------------

class Station_Data {
public:
    char* station_name;
    wxChar station_type; // T or C
    double* amplitude;
    double* epoch;
    double DATUM;
    int meridian;
    char tzfile[40];
    char unit[40];
    char units_conv[40]; // printable converted units
    char units_abbrv[20]; // and abbreviation
    int have_BOGUS;
};

typedef struct {
    Station_Data* sta_data;
    void* next;
} mru_entry;

//----------------------------------------------------------------------------
//   TCMgr
//----------------------------------------------------------------------------

class TCMgr {
public:
    TCMgr(const wxString& data_dir, const wxString& home_dir);
    ~TCMgr();
    bool IsReady(void) { return bTCMReady; }
    bool GetTideOrCurrent(time_t t, int idx, float& value, float& dir);
    bool GetTideOrCurrent15(
        time_t t, int idx, float& tcvalue, float& dir, bool& bnew_val);
    bool GetTideFlowSens(time_t t, int sch_step, int idx, float& tcvalue_now,
        float& tcvalue_prev, bool& w_t);
    void GetHightOrLowTide(time_t t, int sch_step_1, int sch_step_2,
        float tide_val, bool w_t, int idx, float& tcvalue, time_t& tctime);
    int GetStationTimeOffset(IDX_entry* pIDX);
    int GetStationIDXbyName(
        wxString prefix, double xlat, double xlong, TCMgr* ptcmgr);
    int GetNextBigEvent(time_t* tm, int idx);

    int Get_max_IDX() { return max_IDX; }
    IDX_entry* GetIDX_entry(int i) { return paIDX[i]; }

    wxString GetHarmonicFilename() { return wxString::FromUTF8(hfile_name); }

private:
    bool init_idx_array(void);

    void LoadMRU(void);
    void SaveMRU(void);
    void AddMRU(Station_Data* psd);
    void FreeMRU(void);

    int build_IDX_entry(IDX_entry* pIDX);
    int init_index_file(int load_index, int hwnd);
    IDX_entry* get_index_data(short int rec_num);
    Station_Data* find_or_load_harm_data(IDX_entry* pIDX);

    long IndexFileIO(int func, long value);
    void UserStationFuncs(int func, char* custom_name);
    void free_station_index();
    void free_harmonic_file_list();
    void free_abbreviation_list();
    void clean_string(char* str);
    int allocate_copy_string(char** dst, const char* string);
    int skipnl(FILE* fp);
    int next_line(FILE* fp, char linrec[linelen], int end_ok);
    int slackcmp(char* a, char* b);
    char* nojunk(char* linrec);
    void free_data();
    void free_epochs();
    void free_nodes();
    void free_cst();
    void allocate_epochs();
    void allocate_nodes();
    void allocate_cst();
    void fudge_constituents(Station_Data* psd, IDX_entry* pIDX);
    int findunit(const char* unit);
    void figure_amplitude();
    void figure_multipliers();
    void happy_new_year(int new_year);
    void set_epoch(int year, int num_epochs, int first_year);
    int compare_tm(struct tm* a, struct tm* b);

    //    TideLib
    double _time2dt_tide(time_t t, int deriv);
    double blend_tide(
        time_t t, unsigned int deriv, int first_year, double blend);
    double time2dt_tide(time_t t, int deriv);
    int next_big_event(time_t* tm, IDX_entry* pIDX);
    double time2atide(time_t t, IDX_entry* pIDX);
    double BOGUS_amplitude(double mpy, IDX_entry* pIDX);
    double time2tide(time_t t);
    double time2mean(time_t t);
    double time2asecondary(time_t t, IDX_entry* pIDX);

    //    TimeLib
    int yearoftimet(time_t t);
    time_t tm2gmt(struct tm* ht);

    IDX_entry** paIDX;
    int max_IDX;

    mru_entry* pmru_head;
    mru_entry* pmru_last;
    mru_entry* pmru_next;

    double amplitude;
    int year;
    time_t epoch;

    abbreviation_entry** abbreviation_list;
    IDX_entry* pIDX_first;
    IDX_entry IDX;
    harmonic_file_entry* harmonic_file_list;
    FILE* IndexFile;
    char* hfile_name;
    char location[200];
    char* indexfile_name;
    char* userfile_name;
    char* IDX_reference_name;
    char* Izone;

    char index_line[1024];
    int index_in_memory;
    int iscurrent;

    int num_csts;
    double* cst_speeds;
    double* work;
    int num_nodes;
    double** cst_nodes;
    double** cst_epochs;
    int num_epochs;
    int first_year;

    Station_Data* pmsd;

    int have_offsets;

    char tzfile[80];

    //    Used by Index Entry Builder
    IDX_entry* pIDXs;
    int looking_end;
    int rewound;
    int IDX_rec_num;
    int have_index;
    wxString* plast_reference_not_found;

    bool bTCMReady; // set by ctor if all is well

    //    Data file name strings
    wxString* pmru_file_name;

    unit known_units[NUMUNITS];
};

#endif
