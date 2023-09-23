#ifndef myGridTableBase_h
#define myGridTableBase_h

#include <vector>
#include <wx/grid.h>

class myGridTableBase : public wxGridTableBase {
    std::vector<wxString> column_labels;

protected:
    int rows;
    int cols;

public:
    myGridTableBase(int rows, int cols);
    virtual ~myGridTableBase();
    myGridTableBase(const myGridTableBase& copy);
    myGridTableBase& operator=(const myGridTableBase& copy);
    virtual int GetNumberRows();
    virtual int GetNumberCols();
    virtual bool IsEmptyCell(int x, int y);
    virtual void SetColLabelValue(int numcol, const wxString& label);
    virtual wxString GetColLabelValue(int numcol);
    virtual bool AppendRows(size_t nrows);
    virtual bool DeleteRows(size_t pos = 0, size_t numRows = 1);
    myGridTableBase();
    bool AppendCols(size_t numcol);
    bool InsertCols(size_t pos = 0, size_t numcol = 1);
    bool InsertRows(size_t pos = 0, size_t numRows = 1);
};

#endif
