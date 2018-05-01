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
    EVT_MENU(idBtnReset,MainFrame::OnResetFilter)
    EVT_MENU(idBtnLoadPowerLog,MainFrame::OnConfigurePowerLog)
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
    fileMenu->Append(idBtnLoadPowerLog,wxT("Configure Power Log File"),wxT("Show something"));
    fileMenu->Append(idBtnLoad,wxT("Load Log File"),wxT("Show something"));

    fileMenu->Append(idBtnCollect,wxT("Collect new cards"),wxT("Show something"));
    fileMenu->Append(idBtnReset,wxT("Reset Filter"),wxT("Show something"));
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
    m_class_list.Add("Neutral");

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
        FilterCollection("rarity","LEGENDARY",-1);
        break;
    case 2:
        FilterCollection("rarity","EPIC",-1);
        break;
    case 3:
        FilterCollection("rarity","RARE",-1);
        break;
    case 4:
        FilterCollection("rarity","COMMON",-1);
        break;
    default:
        break;
    }
}

void MainFrame::OnSelectClass(wxCommandEvent& event) {
    switch (event.GetSelection()) {
    case 1:
        FilterCollection("cardClass","WARRIOR",-1);
        break;
    case 2:
        FilterCollection("cardClass","SHAMAN",-1);
        break;
    case 3:
        FilterCollection("cardClass","ROGUE",-1);
        break;
    case 4:
        FilterCollection("cardClass","PALADIN",-1);
        break;
    case 5:
        FilterCollection("cardClass","HUNTER",-1);
        break;
    case 6:
        FilterCollection("cardClass","DRUID",-1);
        break;
    case 7:
        FilterCollection("cardClass","WARLOCK",-1);
        break;
    case 8:
        FilterCollection("cardClass","MAGE",-1);
        break;
    case 9:
        FilterCollection("cardClass","PRIEST",-1);
        break;
    case 10:
        FilterCollection("cardClass","NEUTRAL",-1);
        break;
    default:
        break;
    }
}

void MainFrame::OnSelectMana(wxCommandEvent& event) {
    switch (event.GetSelection()) {
    case 1: // 0 mana
        FilterCollection("cost","",0);
        break;
    case 2:
        FilterCollection("cost","",1);
        break;
    case 3:
        FilterCollection("cost","",2);
        break;
    case 4:
        FilterCollection("cost","",3);
        break;
    case 5:
        FilterCollection("cost","",4);
        break;
    case 6:
        FilterCollection("cost","",5);
        break;
    case 7:
        FilterCollection("cost","",6);
        break;
    case 8:
        FilterCollection("cost","",7);
        break;
    case 9:
        FilterCollection("cost","",8);
        break;
    case 10:
        FilterCollection("cost","",9);
        break;
    case 11:
        FilterCollection("cost","",10);
        break;
    default:
        break;
    }
}

void MainFrame::OnSelectCardType(wxCommandEvent& event) {
    switch (event.GetSelection()) {
    case 1:
        case 9:
        FilterCollection("type","MINION",-1);
        break;
        break;
    case 2:
        FilterCollection("type","SPELL",-1);
        break;
    case 3:
        FilterCollection("type","WEAPON",-1);
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


bool MainFrame::CardHasStats(std::string stat,std::string stat_val,int cost_val,std::string cardId) {

    std::string str = "./cards/" + cardId + ".json";
    const char * c = str.c_str();
    FILE* fp = fopen(c, "rb"); // non-Windows use "r"
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document d;
    d.ParseStream(is);

    if (cost_val == -1) { // if we are not looking for a cards mana cost
        if (d[stat.c_str()].GetString() == stat_val) {
            return true;
        } else {
            return false;
        }
    } else { // if we ARE looking for a cards mana cost
        if (d[stat.c_str()].GetInt() == cost_val) {
            return true;
        } else {
            return false;
        }
    }

    fclose(fp);
}

void MainFrame::FilterCollection(std::string stat,std::string stat_val,int cost_val) {
    std::ifstream infile("collection.txt");
    std::string line;
    while (infile >> line)
    {
        if (CardHasStats(stat,stat_val,cost_val,line)) {
            m_filtered_cards.Add(ParseFor("name",line));
        }
    }
    m_card_list->SetStrings(m_filtered_cards);


    infile.close();
}

void MainFrame::OnResetFilter(wxCommandEvent& event) {
    m_card_list->SetStrings(m_collection);
    m_filtered_cards.Clear();
    m_rarity_choice->SetSelection(0);
    m_class_choice->SetSelection(0);
    m_cost_choice->SetSelection(0);
    m_card_type_choice->SetSelection(0);
    m_rarity_golden_choice->SetSelection(0);
}

void MainFrame::OnConfigurePowerLog(wxCommandEvent& event) {

    wxMessageBox("Go to your hearthstone folder via AppData, something like C:/Users/<user>/Local/Blizzard/Hearthstone to configure power log file");

    wxString defaultDir = wxT("c:\\temp");

    wxString defaultFilename = wxEmptyString;
    wxDirDialog dialog(NULL, "Choose input directory", defaultDir,wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    //wxFileDialog dialog(this, caption, defaultDir, defaultFilename,wildcard);
    if (dialog.ShowModal() == wxID_OK)
    {
    m_power_log_path = dialog.GetPath();
    }
    if (dialog.GetPath() == "") {
        wxMessageBox(wxT("No Path Selected"));
        return;
    }


    std::ofstream myfile;
    myfile.open (dialog.GetPath() + "/log.config");
    myfile << "[Achievements]\nLogLevel=1\nFilePrinting=true\nConsolePrinting=true\nScreenPrinting=false\n\n[Power]\nLogLevel=1\nFilePrinting=true\nConsolePrinting=true\nScreenPrinting=false\n";
    myfile.close();

}

/**
Functionality that is still needed
Filters need to be stacked
add if card is golden to collection
    - adapt read settings or make seprate file to set gold
add stats at bottom of screen
preferably drop down menu

**/











