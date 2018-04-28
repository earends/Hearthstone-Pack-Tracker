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

#include "wx/editlbox.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class MainFrame : public wxFrame
{

    public:
        MainFrame(wxWindow *parent,wxString title,wxSize sze);
        ~MainFrame();
    protected:
        enum {

                idBtnCollect,
                idBtnLoad,
                idChoiceRarity,
                idChoiceClass,
                idChoiceMana,
                idChoiceCardType,
                idChoiceGolden,
            };
    private:
        //fUNCTIONS
        void OnClose(wxCloseEvent& event);
        void CreateMenuStatus();
        void CreateFilters();
        void CreateCardList();
        void PopulateLists();
        void OnSize(wxSizeEvent& event);
        void OnMouseUp(wxMouseEvent& event);
        void GetCollection();
        void OnLoad(wxCommandEvent& event);
        void OnCollect(wxCommandEvent& event);
        void OnSelectRarity(wxCommandEvent& event);
        void OnSelectClass(wxCommandEvent& event);
        void OnSelectMana(wxCommandEvent& event);
        void OnSelectCardType(wxCommandEvent& event);
        void OnSelectGolden(wxCommandEvent& event);
        wxString ParseFor(std::string id,std::string fileName);
        std::string IntToStr(int n);
        void modify_collection(std::string str);
        void SweepLog();
        void FilterCollection(std::string stat,std::string stat_val,int cost_val);

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
        wxEditableListBox* m_card_list;
        wxArrayString m_collection;
        wxArrayString m_filtered_cards;
        wxString m_log_path;


        DECLARE_EVENT_TABLE()
};

#endif // MAINFRAME_H_INCLUDED
