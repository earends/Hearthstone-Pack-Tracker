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
        //fUNCTIONS
        void OnClose(wxCloseEvent& event);
        void CreateMenuStatus();
        void CreateFilters();
        void CreateCardList();
        void PopulateLists();
        void OnSize(wxSizeEvent& event);
        void OnMouseUp(wxMouseEvent& event);

        //varaibles
        wxSize m_win_size;
        //sIZERS
        wxBoxSizer * m_cardList_sizer; // holds cardlist label plus list (vertical)
        wxBoxSizer * m_filter_sizer; // holds filter and label (vertical)
        wxBoxSizer * m_stats_sizer; // holds stats (horizontal)
        wxBoxSizer * m_sizer; // sizer that holds everything up and down
        wxBoxSizer * m_top_sizer; // holds filter and card list sizer (HORIZONTAL)
        //Labels
        wxStaticText *m_card_list_label;
        wxStaticText *m_filter_label;
            //labels for filters
        wxStaticText *m_class_label; // rogue ..druid ..etc
        wxStaticText *m_card_type_label; // minion..weapons..spell
        wxStaticText *m_cost_label; // 0 mana, 2 mana...
        wxStaticText *m_rarity_label; // legendary .. epic..
        wxStaticText *m_rarity_golden_label; // Golden or not
        //DROP DOWN CHOICES
        wxChoice *m_class_choice;
        wxArrayString m_class_list;
        wxChoice *m_card_type_choice;
        wxArrayString m_type_list;
        wxChoice *m_cost_choice;
        wxArrayString m_cost_list;
        wxChoice *m_rarity_choice;
        wxArrayString m_rarity_list;
        wxChoice *m_rarity_golden_choice;
        wxArrayString m_rarity_golden_list;

        //OTHER
        wxListBox * m_card_list;

        DECLARE_EVENT_TABLE()
};

#endif // MAINFRAME_H_INCLUDED
