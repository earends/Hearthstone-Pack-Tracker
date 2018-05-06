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

#include "wx/statline.h"
#include "wx/listctrl.h"

class MainFrame : public wxFrame
{

    public:
        MainFrame(wxWindow *parent,wxString title,wxSize sze);
        ~MainFrame();
    protected:
        enum {

                idBtnCollect, //menu
                idBtnLoad,
                idBtnReset,
                idBtnLoadPowerLog,
                idBtnStats,
                idChoiceRarity, // drop down menu
                idChoiceClass,
                idChoiceMana,
                idChoiceCardType,
                idChoiceGolden,
                idListBox, // list of cards
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
        void FilterCollectionWithoutValues();
        wxString ParseFor(std::string id,std::string fileName);
        std::string IntToStr(int n);
        void modify_collection(std::string str);
        void SweepLog();
        bool CardHasStats(std::string stat,std::string stat_val,int cost_val,wxString cardId);
        void FilterCollection(std::string stat,std::string stat_val,int cost_val);
        void OnSelectFromList(wxListEvent& event);
        void OnResetFilter(wxCommandEvent &event);
        void OnConfigurePowerLog(wxCommandEvent& event);
        void RemoveFromFilter(std::string stat);
        void AddStats();
        void GetStats();
        void ResetStats();
        void OnDisplayStats(wxCommandEvent& event);
        void SetStats();

        //varaibles
        wxSize m_win_size;
        //sIZERS
        wxBoxSizer * m_cardList_sizer; // holds cardlist label plus list (vertical)
        wxBoxSizer * m_filter_sizer; // holds filter and label (vertical)
        wxBoxSizer * m_stats_sizer; // holds stats (horizontal) sizer  splits collections vs cards stats
        wxBoxSizer * m_stats_card_sizer; // holds card stats
        wxBoxSizer * m_stats_collection_sizer; // holds collection stats
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

        //DROP DOWN CHOICES
        wxChoice *m_class_choice;
        wxArrayString m_class_list;
        wxChoice *m_card_type_choice;
        wxArrayString m_type_list;
        wxChoice *m_cost_choice;
        wxArrayString m_cost_list;
        wxChoice *m_rarity_choice;
        wxArrayString m_rarity_list;


        //STATS STATIC TEXTS
        wxStaticText* m_total_cards_text;
        wxStaticText* m_total_legendary_text;
        wxStaticText* m_total_epic_text;
        wxStaticText* m_total_rare_text;
        wxStaticText* m_total_dust_text;

        wxStaticText* m_card_name_text;
        wxStaticText* m_card_cost_text;
        wxStaticText* m_card_rarity_text;
        wxStaticText* m_card_cardType_text;
        wxStaticText* m_card_text_text; //description

        int m_total_card_count;
        int m_total_legendary_count;
        int m_total_epic_count;
        int m_total_rare_count;
        int m_total_common_count;
        int m_total_dust_count;



        //OTHER
        wxEditableListBox* m_card_list;
        wxArrayString m_collection;
        wxArrayString m_collection_ids;
        wxArrayString m_filtered_cards;
        wxString m_log_path;
        wxString m_power_log_path;
        wxArrayString m_filtered_ids;
        wxArrayString m_filters_selected;
        wxArrayString m_filters_selected_values;
        int global_cost_value;
        bool show_stats;

        DECLARE_EVENT_TABLE()
};

#endif // MAINFRAME_H_INCLUDED
