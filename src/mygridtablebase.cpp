#include "mygridtablebase.h"

myGridTableBase::myGridTableBase(int rows, int cols)
    : rows(rows)
    , cols(cols)
{
}

myGridTableBase::~myGridTableBase() { }

myGridTableBase::myGridTableBase(const myGridTableBase& copy)
{
    column_labels = copy.column_labels;
    rows = copy.rows;
    cols = copy.cols;
}

myGridTableBase& myGridTableBase::operator=(const myGridTableBase& copy)
{
    return *this;
}

int myGridTableBase::GetNumberRows() { return rows; }

int myGridTableBase::GetNumberCols() { return cols; }

bool myGridTableBase::IsEmptyCell(int x, int y) { return false; }

void myGridTableBase::SetColLabelValue(int numcol, const wxString& label)
{
    if (numcol < column_labels.size())
        column_labels[numcol] = label;

    else {
        column_labels.push_back(label);
        wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_COLS_APPENDED, numcol);
        GetView()->ProcessTableMessage(msg);
        cols++;
    }
}

wxString myGridTableBase::GetColLabelValue(int numcol)
{
    return column_labels[numcol];
}

bool myGridTableBase::AppendRows(size_t nrows)
{
    rows += nrows;
    wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_APPENDED, nrows);
    return GetView()->ProcessTableMessage(msg);
}

bool myGridTableBase::DeleteRows(size_t pos, size_t numRows)
{
    rows -= numRows;
    wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_DELETED, pos, numRows);
    return GetView()->ProcessTableMessage(msg);
}

myGridTableBase::myGridTableBase()
{
    rows = 0;
    cols = 0;
}

bool myGridTableBase::AppendCols(size_t numcol)
{
    for (int i = 0; i < numcol; i++)
        column_labels.push_back(wxEmptyString);
    cols += numcol;
    wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_COLS_APPENDED, numcol);
    return GetView()->ProcessTableMessage(msg);
}

bool myGridTableBase::InsertCols(size_t pos, size_t numcol)
{
    std::vector<wxString>::iterator it = column_labels.begin();
    for (size_t i = 0; i < pos; ++i)
        it++;
    cols += numcol;
    column_labels.insert(it, numcol, wxEmptyString);
    wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_COLS_INSERTED, pos, numcol);
    GetView()->ProcessTableMessage(msg);
    return true;
}

bool myGridTableBase::InsertRows(size_t pos, size_t numRows)
{
    rows += numRows;
    wxGridTableMessage msg(
        this, wxGRIDTABLE_NOTIFY_ROWS_INSERTED, pos, numRows);
    return GetView()->ProcessTableMessage(msg);
}
