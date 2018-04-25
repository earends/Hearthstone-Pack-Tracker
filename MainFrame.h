/***************************************************************
 * Name:      MainFrame.h
 * Purpose:   Defines Application Frame class
 * Author:     ()
 * Created:   2018-04-24
 * Copyright:  ()
 * License:
 **************************************************************/
#ifndef MAINFRAME_H_INCLUDED
#define MAINFRAME_H_INCLUDED

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class MainFrame : public wxFrame
{
    public:
        MainFrame(wxWindow *parent,wxString title,wxSize sze);
        ~MainFrame();
    protected:

    private:
        void OnClose(wxCloseEvent& event);
        DECLARE_EVENT_TABLE()
};

#endif // MAINFRAME_H_INCLUDED
