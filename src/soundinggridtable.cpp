#include "soundinggridtable.h"

SoundingGridTable::SoundingGridTable() { }

SoundingGridTable::~SoundingGridTable()
{
    for (int i = 0; i < 2 /*data.size()*/; ++i)
    // delete[] data[i];
    // data.clear();
    { }
}

SoundingGridTable::SoundingGridTable(const SoundingGridTable& copy) { }

SoundingGridTable& SoundingGridTable::operator=(const SoundingGridTable& copy)
{
    return *this;
}

bool SoundingGridTable::AppendRows(size_t nrows)
{
    for (size_t i = 0; i < nrows; ++i)
        // data.push_back(new int[cols]);

        return myGridTableBase::AppendRows(nrows);
}

void SoundingGridTable::SetValue(int x, int y, const wxString& str)
{
    long value;
    if (str.ToLong(&value)) {
        if (x < rows && x > -1 && y < cols && y > -1) {
            // data[y][x]=(int)value;
        }
    }
}

wxString SoundingGridTable::GetValue(int x, int y)
{
    if (x > -1 && x < rows && y > -1 && y < cols)
        return wxString::Format(_T("%i"), 1); // data[y][x]);
    return wxEmptyString;
}

bool SoundingGridTable::InsertRows(size_t pos, size_t numRows)
{
    std::vector<int*>::iterator it = data.begin();
    for (size_t i = 0; i < pos; ++i)
        it++;
    data.insert(it, numRows, new int[cols]);
    return myGridTableBase::InsertRows(pos, numRows);
}
