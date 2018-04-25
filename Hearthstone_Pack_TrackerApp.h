/***************************************************************
 * Name:      Hearthstone_Pack_TrackerApp.h
 * Purpose:   Defines Application Class
 * Author:     ()
 * Created:   2018-04-24
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef HEARTHSTONE_PACK_TRACKERAPP_H
#define HEARTHSTONE_PACK_TRACKERAPP_H

#include <wx/app.h>
class MainFrame;
class Hearthstone_Pack_TrackerApp : public wxApp
{
    public:

        virtual bool OnInit();
    private:
        MainFrame* m_frame;
};

#endif // HEARTHSTONE_PACK_TRACKERAPP_H
