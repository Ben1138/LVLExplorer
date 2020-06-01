#pragma once
#include <map>
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/menu.h>
#include "wxImagePanel.h"
#include "LibSWBF2.h"

using std::map;
using LibSWBF2::Chunks::LVL::LVL;
using LibSWBF2::Chunks::LVL::BODY;
using LibSWBF2::Chunks::GenericBaseChunk;
using LibSWBF2::Types::List;

class LVLExplorerFrame : public wxFrame
{
public:
	LVLExplorerFrame();
	~LVLExplorerFrame();

private:
	enum class EDisplayStatus
	{
		NONE,	// this should never be used, except for initialization
		TEXT,
		IMAGE
	};

private:
	wxMenuBar* m_menuMain;
	wxMenu* m_fileMenu;
	wxPanel* m_panelMain;
	wxBoxSizer* m_sizer1;
	wxBoxSizer* m_sizer2;
	wxTreeCtrl* m_lvlTreeCtrl;
	wxStaticText* m_infoText;
	wxTextCtrl* m_textDisplay;
	wxImagePanel* m_imageDisplay;

	wxTreeItemId m_treeRoot;
	wxSizerFlags m_rightHandSideFlags;
	EDisplayStatus m_displayStatus;

	LVL* m_currentLVL;
	map<wxTreeItemId, GenericBaseChunk*> m_treeToChunk;

	uint16_t m_imageWidth;
	uint16_t m_imageHeight;
	unsigned char* m_imageData = nullptr;

private:
	void DisplayText();
	void DisplayImage();
	void HideCurrentDisplay();
	void ParseChunk(GenericBaseChunk* chunk, wxTreeItemId parent);

	// events
	void OnMenuOpenFile(wxCommandEvent& event);
	void OnMenuExit(wxCommandEvent& event);
	void OnTreeSelectionChanges(wxTreeEvent& event);

	//wxString m_chunkNames;

	wxDECLARE_EVENT_TABLE();

public:
	void AddLogLine(wxString msg);
};