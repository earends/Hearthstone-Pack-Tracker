/***************************************************************
 * Name:      Hearthstone_Pack_TrackerApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2018-04-24
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
    #include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__
#include <wx/wx.h>
#include <wx/sizer.h>

#include "Hearthstone_Pack_TrackerApp.h"
#include "MainFrame.h"

IMPLEMENT_APP(Hearthstone_Pack_TrackerApp);

bool Hearthstone_Pack_TrackerApp::OnInit()
{
    wxInitAllImageHandlers();
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    m_frame = new MainFrame(NULL,wxT("Hearthstone"),wxSize(600,800));
    m_frame->Show();
    return true;
}
