#pragma once
#include <map>
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/menu.h>
#include "wxImagePanel.h"
#include "LibSWBF2.h"

using std::map;
using LibSWBF2::Chunks::LVL::LVL;
using LibSWBF2::Chunks::GenericChunk;
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
	map<wxTreeItemId, const GenericChunk*> m_treeToChunk;
	unsigned char* m_imageData;


private:
	void DisplayText();
	void DisplayImage();
	void HideCurrentDisplay();
	void ParseChunk(const GenericChunk& chunk, wxTreeItemId parent);

	// events
	void OnMenuOpenFile(wxCommandEvent& event);
	void OnMenuExit(wxCommandEvent& event);
	void OnTreeSelectionChanges(wxTreeEvent& event);

	wxDECLARE_EVENT_TABLE();

public:
	void AddLogLine(wxString msg);
};