#ifndef SoundingGridTable_h
#define SoundingGridTable_h

// #include <vector>

#include "mygridtablebase.h"

class SoundingGridTable : public myGridTableBase {
    std::vector<int*> data;

protected:
public:
    SoundingGridTable();
    virtual ~SoundingGridTable();
    SoundingGridTable(const SoundingGridTable& copy);
    SoundingGridTable& operator=(const SoundingGridTable& copy);
    bool AppendRows(size_t nrows);
    void SetValue(int x, int y, const wxString& str);
    wxString GetValue(int x, int y);
    bool InsertRows(size_t pos = 0, size_t numRows = 1);
};
#endif
