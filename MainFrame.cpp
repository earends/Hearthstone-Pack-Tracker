/***************************************************************
 * Name:     MainFrame.cpp
 * Purpose:   Code for MainFrame class
 * Author:     ()
 * Created:   2018-04-24
 * Copyright:  ()
 * License:
 **************************************************************/

#include "MainFrame.h"
#ifdef WX_PRECOMP
    #include "wx_pch.h"
#endif
#include <wx/wx.h>
#include <wx/sizer.h>

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_SIZE(MainFrame::OnSize)
    EVT_MOTION(MainFrame::OnMouseUp)
END_EVENT_TABLE()

MainFrame::MainFrame (wxWindow *parent,wxString title,wxSize sze)
    :wxFrame(parent, wxID_ANY, title, wxDefaultPosition, sze)
{
    //Initialize lists
    PopulateLists();

    CreateMenuStatus();
    /**
    Call setup functions
    **/
    m_sizer = new wxBoxSizer(wxVERTICAL);
    m_top_sizer = new wxBoxSizer(wxHORIZONTAL);
    CreateCardList();
    CreateFilters();
    m_sizer->Add(m_top_sizer, 1, wxEXPAND);

    /**
    Layout sizers
    **/
    this->Layout();
    this->SetSizer(m_sizer);
    m_sizer->Fit(this);

}

MainFrame::~MainFrame() {
    Destroy();
}

/**
    Creates menu bar
**/
void MainFrame::CreateMenuStatus() {
    // Create a menu bar
    wxMenu *fileMenu = new wxMenu;
    // The “About” item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"),wxT("Show about dialog"));
    fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"),wxT("Quit this program"));
    // Now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));
    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
    // Create a status bar just for fun
    CreateStatusBar(2);
    SetStatusText(wxT("Welcome to Hearthstone Pack Tracker"));
}


/**
    Creates sizer with list of filters for user
    includes label at top
**/
void MainFrame::CreateFilters() {
    m_filter_sizer = new wxBoxSizer(wxVERTICAL);
    //labels
    m_filter_label = new wxStaticText(this,wxID_ANY,wxT("Filters"),wxDefaultPosition,wxDefaultSize);
    m_class_label = new  wxStaticText(this,wxID_ANY,wxT("Class"));
    m_cost_label =  new  wxStaticText(this,wxID_ANY,wxT("Mana"));
    m_card_type_label =  new  wxStaticText(this,wxID_ANY,wxT("Card Type"));
    m_rarity_label =  new  wxStaticText(this,wxID_ANY,wxT("Rarity"));
    m_rarity_golden_label =  new  wxStaticText(this,wxID_ANY,wxT("Golden?"));
    //choices
    m_class_choice = new wxChoice(this, wxID_ANY,  wxDefaultPosition, wxDefaultSize, m_class_list);
    m_cost_choice = new wxChoice(this, wxID_ANY,  wxDefaultPosition, wxDefaultSize, m_cost_list);
    m_card_type_choice = new wxChoice(this, wxID_ANY,  wxDefaultPosition, wxDefaultSize, m_type_list);
    m_rarity_choice = new wxChoice(this, wxID_ANY,  wxDefaultPosition, wxDefaultSize, m_rarity_list);
    m_rarity_golden_choice = new wxChoice(this, wxID_ANY,  wxDefaultPosition, wxDefaultSize, m_rarity_golden_list);
    //add to filter sizer
    m_filter_sizer->Add(m_filter_label);
    m_filter_sizer->Add(m_rarity_label, 1, wxEXPAND);
    m_filter_sizer->Add(m_rarity_choice, 1, wxEXPAND);
    m_filter_sizer->Add(m_class_label, 1, wxEXPAND);
    m_filter_sizer->Add(m_class_choice, 1, wxEXPAND);
    m_filter_sizer->Add(m_cost_label, 1, wxEXPAND);
    m_filter_sizer->Add(m_cost_choice, 1, wxEXPAND);
    m_filter_sizer->Add(m_card_type_label, 1, wxEXPAND);
    m_filter_sizer->Add(m_card_type_choice, 1, wxEXPAND);
    m_filter_sizer->Add(m_rarity_golden_label, 1, wxEXPAND);
    m_filter_sizer->Add(m_rarity_golden_choice, 1, wxEXPAND);

    m_top_sizer->Add(m_filter_sizer, 1, wxEXPAND);
}


/**
    Creates table with list of cards
    includes label at top
**/
void MainFrame::CreateCardList() {
    m_cardList_sizer = new wxBoxSizer(wxVERTICAL);
    m_card_list_label = new wxStaticText(this,wxID_ANY,wxT("Cards"),wxDefaultPosition,wxDefaultSize);
    m_cardList_sizer->Add(m_card_list_label);
    wxArrayString strings;
    strings.Add(wxT("First string"));
    strings.Add(wxT("Second string"));
    m_card_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, strings, wxLB_SINGLE|wxEXPAND);
    m_cardList_sizer->Add(m_card_list, 1, wxEXPAND);
    m_top_sizer->Add(m_cardList_sizer, 1, wxEXPAND);
}

void MainFrame::CreateStats() {

}

void MainFrame::OnSize(wxSizeEvent& event) {

    this->Layout();
}

void MainFrame::OnMouseUp(wxMouseEvent& event) {
    wxClientDC dc(this);
    m_win_size = dc.GetSize();
}

void MainFrame::PopulateLists() {
    m_class_list = wxArrayString();
    m_class_list.Add("Class");
    m_class_list.Add("Warrior");
    m_class_list.Add("Shaman");
    m_class_list.Add("Rogue");
    m_class_list.Add("Paladin");
    m_class_list.Add("Hunter");
    m_class_list.Add("Druid");
    m_class_list.Add("Warlock");
    m_class_list.Add("Mage");
    m_class_list.Add("Priest");

    m_type_list = wxArrayString();
    m_type_list.Add("Type");
    m_type_list.Add("Minion");
    m_type_list.Add("Spell");
    m_type_list.Add("Weapon");

    m_cost_list = wxArrayString();
    m_cost_list.Add("0");
    m_cost_list.Add("1");
    m_cost_list.Add("2");
    m_cost_list.Add("3");
    m_cost_list.Add("4");
    m_cost_list.Add("5");
    m_cost_list.Add("6");
    m_cost_list.Add("7");
    m_cost_list.Add("8");
    m_cost_list.Add("9");
    m_cost_list.Add("10");

    m_rarity_list = wxArrayString();
    m_rarity_list.Add("Rarity");
    m_rarity_list.Add("Legendary");
    m_rarity_list.Add("Epic");
    m_rarity_list.Add("Rare");
    m_rarity_list.Add("Common");
    m_rarity_list.Add("Hunter");

    m_rarity_golden_list = wxArrayString();
    m_rarity_golden_list.Add("Golden");
    m_rarity_golden_list.Add("Yes");
    m_rarity_golden_list.Add("No");
}




