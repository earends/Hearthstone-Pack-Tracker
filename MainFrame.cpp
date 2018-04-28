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
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include "sstream"
#include "string"
#include "fstream"

using namespace rapidjson;

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_SIZE(MainFrame::OnSize)
    EVT_MOTION(MainFrame::OnMouseUp)
    EVT_MENU(idBtnLoad,MainFrame::OnLoad)
    EVT_MENU(idBtnCollect,MainFrame::OnCollect)
    EVT_CHOICE(idChoiceRarity,MainFrame::OnSelectRarity)
    EVT_CHOICE(idChoiceClass,MainFrame::OnSelectClass)
    EVT_CHOICE(idChoiceMana,MainFrame::OnSelectMana)
    EVT_CHOICE(idChoiceCardType,MainFrame::OnSelectCardType)
    EVT_CHOICE(idChoiceGolden,MainFrame::OnSelectGolden)
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
    GetCollection();
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
    fileMenu->Append(idBtnLoad,wxT("Load Log File"),wxT("Show something"));
    fileMenu->Append(idBtnCollect,wxT("Collect new cards"),wxT("Show something"));
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
    m_class_choice = new wxChoice(this, idChoiceClass,  wxDefaultPosition, wxDefaultSize, m_class_list);
    m_class_choice->SetSelection(0);
    m_cost_choice = new wxChoice(this, idChoiceMana,  wxDefaultPosition, wxDefaultSize, m_cost_list);
    m_cost_choice->SetSelection(0);
    m_card_type_choice = new wxChoice(this, idChoiceCardType,  wxDefaultPosition, wxDefaultSize, m_type_list);
    m_card_type_choice->SetSelection(0);
    m_rarity_choice = new wxChoice(this, idChoiceRarity,  wxDefaultPosition, wxDefaultSize, m_rarity_list);
    m_rarity_choice->SetSelection(0);
    m_rarity_golden_choice = new wxChoice(this, idChoiceGolden,  wxDefaultPosition, wxDefaultSize, m_rarity_golden_list);
    m_rarity_golden_choice->SetSelection(0);
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

void MainFrame::OnCollect(wxCommandEvent& event) {

    if (m_log_path == "") {
        wxMessageBox(wxT("Please Load last modified Log file first"));
        OnLoad(event);
    } else {
        /**
        Read file for id numbers of cards unpacked
        add each id number to collection.txt
        re call function that populates our card set \
        refresh view to reload list
        **/
        SweepLog();

    }

}

void MainFrame::SweepLog() {
    std::ifstream infile(m_log_path);
    std::string line;
    while (infile >> line)
    {
        if (line.find("cardId=") != -1) {
            std::string id = line.erase(0,7);
            modify_collection(id);
            m_collection.Add(ParseFor("name",id));


        }
    }
    infile.close();
    rename(m_log_path,"use_last_modified_log_file.log");
    m_log_path = "";
    m_card_list->SetStrings(m_collection);



}

void MainFrame::OnLoad(wxCommandEvent& event) {
    wxString caption = wxT("Find your log file!");
    wxString wildcard = wxT("Log files (*.log)|*.log|LOG files (*.log)|*.log");
    wxString defaultDir = wxT("c:\\temp");
    wxString defaultFilename = wxEmptyString;
    wxFileDialog dialog(this, caption, defaultDir, defaultFilename,wildcard);
    if (dialog.ShowModal() == wxID_OK)
    {
    m_log_path = dialog.GetPath();
    }
    if (m_log_path == "") {
        wxMessageBox(wxT("No Path Selected"));
    }
}


/**
    Creates table with list of cards
    includes label at top
**/
void MainFrame::CreateCardList() {
    m_cardList_sizer = new wxBoxSizer(wxVERTICAL);
    //m_card_list_label = new wxStaticText(this,wxID_ANY,wxT("Cards"),wxDefaultPosition,wxDefaultSize);
    //m_cardList_sizer->Add(m_card_list_label);
    m_card_list = new wxEditableListBox(this,wxID_ANY,wxT("List of cards"),wxDefaultPosition, wxDefaultSize, wxLB_SINGLE|wxEXPAND);
    m_card_list->SetStrings(m_collection);
    m_cardList_sizer->Add(m_card_list, 1, wxEXPAND);
    m_top_sizer->Add(m_cardList_sizer, 1, wxEXPAND);
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
    m_class_list.Add("None");
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
    m_type_list.Add("None");
    m_type_list.Add("Minion");
    m_type_list.Add("Spell");
    m_type_list.Add("Weapon");

    m_cost_list = wxArrayString();
    m_cost_list.Add("None");
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
    m_rarity_list.Add("None");
    m_rarity_list.Add("Legendary");
    m_rarity_list.Add("Epic");
    m_rarity_list.Add("Rare");
    m_rarity_list.Add("Common");
    m_rarity_list.Add("Hunter");

    m_rarity_golden_list = wxArrayString();
    m_rarity_golden_list.Add("None");
    m_rarity_golden_list.Add("Yes");
    m_rarity_golden_list.Add("No");
}



void MainFrame::GetCollection() {
    std::ifstream infile("collection.txt");
    std::string line;
    while (infile >> line)
    {
        m_collection.Add(ParseFor("name",line));
    }
    m_card_list->SetStrings(m_collection);

}

wxString MainFrame::ParseFor(std::string id,std::string fileName) {
    std::string str = "./cards/" + fileName + ".json";
    const char * c = str.c_str();
    FILE* fp = fopen(c, "rb"); // non-Windows use "r"
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document d;
    d.ParseStream(is);
    return d[id.c_str()].GetString();
    fclose(fp);
}

std::string MainFrame::IntToStr(int n) {
    std::stringstream ss;
    ss << n;
    std::string str = ss.str();
    return str;
}

void MainFrame::modify_collection(std::string str) {
    std::ifstream infile("collection.txt");
    std::ofstream outfile("temp.txt");
    std::string line;
    while (infile >> line)
    {
        outfile << line <<std::endl;
    }
    outfile << str <<std::endl;
    infile.close();
    outfile.close();
    remove("collection.txt");
    rename("temp.txt","collection.txt");
}

void MainFrame::OnSelectRarity(wxCommandEvent& event) {
    switch (event.GetSelection()) {
    case 1:
        wxMessageBox("1");
        break;
    case 2:
        wxMessageBox("2");
        break;
    case 3:
        wxMessageBox("3");
        break;
    case 4:
        wxMessageBox("4");
        break;
    case 5:
        wxMessageBox("5");
        break;
    default:
        break;
    }
}

void MainFrame::OnSelectClass(wxCommandEvent& event) {
    switch (event.GetSelection()) {
    case 1:
        wxMessageBox("1");
        break;
    case 2:
        wxMessageBox("2");
        break;
    case 3:
        wxMessageBox("3");
        break;
    case 4:
        wxMessageBox("4");
        break;
    case 5:
        wxMessageBox("5");
        break;
    case 6:
        wxMessageBox("6");
        break;
    case 7:
        wxMessageBox("7");
        break;
    case 8:
        wxMessageBox("8");
        break;
    case 9:
        wxMessageBox("9");
        break;
    default:
        break;
    }
}

void MainFrame::OnSelectMana(wxCommandEvent& event) {
    switch (event.GetSelection()) {
    case 1: // 0 mana
        wxMessageBox("1");
        break;
    case 2:
        wxMessageBox("2");
        break;
    case 3:
        wxMessageBox("3");
        break;
    case 4:
        wxMessageBox("4");
        break;
    case 5:
        wxMessageBox("5");
        break;
    case 6:

        break;
    case 7:
        wxMessageBox("7");
        break;
    case 8:
        wxMessageBox("8");
        break;
    case 9:
        wxMessageBox("9");
        break;
    case 10:
        wxMessageBox("10");
        break;
    case 11:
        wxMessageBox("11");
        break;
    default:
        break;
    }
}

void MainFrame::OnSelectCardType(wxCommandEvent& event) {
    switch (event.GetSelection()) {
    case 1:
        wxMessageBox("1");
        break;
    case 2:
        wxMessageBox("2");
        break;
    case 3:
        wxMessageBox("3");
        break;
    default:
        break;
    }
}

void MainFrame::OnSelectGolden(wxCommandEvent& event) {
    switch (event.GetSelection()) {
    case 1:
        wxMessageBox("1");
        break;
    case 2:
        wxMessageBox("2");
        break;
    default:
        break;
    }
}

/**
This function needs still to...
    - loop through card collection
    - check to see if card being read has stats
    - if it has stats
        - add to filter buffer list
    - if it doesent have stats
        - dont add
    - set strings to list
    - make reset filter buffer function
**/
void MainFrame::FilterCollection(std::string stat,std::string stat_val,int cost_val) {

    std::string str;// = "./cards/" + id + ".json";
    const char * c = str.c_str();
    FILE* fp = fopen(c, "rb"); // non-Windows use "r"
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document d;
    d.ParseStream(is);

    if (cost_val == -1) { // if we are not looking for a cards mana cost
        if (d[stat.c_str()].GetString() == stat_val) {
            wxMessageBox("equals");
        } else {
            wxMessageBox("not equals");
        }
    } else { // if we ARE looking for a cards mana cost
        if (d[stat.c_str()].GetInt() == cost_val) {
            wxMessageBox("equals");
        } else {
            wxMessageBox("not equals");
        }
    }

    fclose(fp);
}











