//
//  DNAFrame.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNAFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/busyinfo.h>
#include "DNADrawPanel.h"
#include "Exceptions.h"
#include "DNAAlignDlg.h"
#include "FASTALoad.h"
#include "translator.h"
#include <iostream>
#include "NeedlemanWunsch.h"
#include "timer.h"

enum
{
	ID_AMINO_HIST=1000,
    ID_NEEDLE=1100,
};

wxBEGIN_EVENT_TABLE(DNAFrame, wxFrame)
	EVT_MENU(wxID_EXIT, DNAFrame::onExit)
	EVT_MENU(wxID_NEW, DNAFrame::onNew)
	EVT_MENU(ID_AMINO_HIST, DNAFrame::onAminoHist)
    EVT_MENU(ID_NEEDLE, DNAFrame::onNeed)
wxEND_EVENT_TABLE()

DNAFrame::DNAFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(ID_AMINO_HIST, "Amino Acid Histogram...",
					 "Create a histogram from a FASTA file.");
	menuFile->Append(ID_NEEDLE, "Needleman-Wunsch...", "Find optimum comparison of two FASTA files.");
    menuFile->Append(wxID_EXIT);
    
	
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	SetMenuBar(menuBar);
	CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new DNADrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);
	
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}

void DNAFrame::onExit(wxCommandEvent& event)
{
	Close(true);
}

void DNAFrame::onNew(wxCommandEvent& event)
{
	// Doesn't do anything...
}

void DNAFrame::onAminoHist(wxCommandEvent& event)//(File>Amino Acid Histogram...)
{
    
    wxFileDialog fileDlg(this, "Load a FASTA file...", "./data", "", "FASTA Files|*.fasta", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    std::string pathName;
    
    if (fileDlg.ShowModal() == wxID_OK) {
        
        pathName = fileDlg.GetPath();
        
        try{
            fileParse DNAchecker(pathName);//Makes sure that it is a valid FASTA file
            
            translateInst.stateMachine(DNAchecker.showContent());
            translateInst.calculatePercent();
            mPanel->setHistogram(translateInst.getPercent());
            mPanel->setCounts(translateInst.getCount());
            mPanel->setTitle(DNAchecker.getHeader());
            mPanel->paintNow();
            
        }
        
        catch(FileLoadExcept& e){
            
            std::cout << "File Load Exception!" << std::endl;
            e.what();
            
        }
        
    }
    
}

void DNAFrame::onNeed(wxCommandEvent &event)////(File>Needleman-Wunsch...)
{
    DNAAlignDlg NWCompare;
    
    std::string inpath1;
    std::string inpath2;
    std::string outpath;
    
    if (NWCompare.ShowModal() == wxID_OK){
        
        inpath1 = NWCompare.GetInputAPath();
        inpath2 = NWCompare.GetInputBPath();
        outpath = NWCompare.GetOutputPath();
        
        try{
            
            wxBusyInfo info("Calulating pairwise match...", this);
            NeedWun One(inpath1, inpath2);
            
            Timer time;
            time.start();
            One.FindWun();
            std::cout << "Time: " << time.getElapsed() << std::endl;
            One.WriteWun(outpath);
            
        }
        
        catch(FileLoadExcept& e){
            
            std::cout << "File Load Exception!" << std::endl;
            e.what();
            
        }
        
    }
    
};













