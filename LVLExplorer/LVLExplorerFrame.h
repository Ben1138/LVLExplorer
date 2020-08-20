#pragma once
#include <map>
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/menu.h>
#include "wxImagePanel.h"
#include "LibSWBF2.h"

using std::map;
using LibSWBF2::Chunks::LVL::LVL;
using LibSWBF2::Chunks::BNK::BNK;
using LibSWBF2::Chunks::LVL::texture::tex_;
using LibSWBF2::Chunks::LVL::LVL_texture::BODY;
using LibSWBF2::Chunks::GenericBaseChunk;
using LibSWBF2::Types::List;
using LibSWBF2::ETextureFormat;

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

	const wxColor ITEM_COLOR = wxColor(0, 0, 0);
	const wxColor ITEM_COLOR_BACKGROUND = wxColor(255, 255, 255);
	const wxColor ITEM_COLOR_FOUND = wxColor(0, 220, 0);
	const wxColor ITEM_COLOR_FOUND_IN_INFO = wxColor(200, 100, 100);
	const wxColor ITEM_COLOR_FOUND_BACKGROUND = wxColor(32, 32, 32);
	const wxColor ITEM_COLOR_FOUND_CHILDREN = wxColor(220, 220, 0);

private:
	wxMenuBar* m_menuMain;
	wxMenu* m_fileMenu;
	wxPanel* m_panelMain;
	wxBoxSizer* m_sizerLeft;
	wxBoxSizer* m_sizerHorizontal;
	wxBoxSizer* m_sizerRight;
	wxTextCtrl* m_searchBox;
	wxTreeCtrl* m_lvlTreeCtrl;
	wxStaticText* m_infoText;
	wxTextCtrl* m_textDisplay;
	wxImagePanel* m_imageDisplay;

	wxTreeItemId m_treeRoot;
	wxSizerFlags m_rightHandSideFlags;
	EDisplayStatus m_displayStatus;

	GenericBaseChunk* m_currentContainer;
	map<wxTreeItemId, GenericBaseChunk*> m_treeToChunk;

	uint16_t m_imageWidth;
	uint16_t m_imageHeight;

	// use malloc, NOT new! See wxImage::SetData documentation
	uint8_t* m_imageData = nullptr;

private:
	void DisplayText();
	void DisplayImage();
	void HideCurrentDisplay();
	void ParseChunk(GenericBaseChunk* chunk, wxTreeItemId parent, size_t childIndex=0);
	bool SearchTree(wxTreeItemId parent, const wxString& search);
	void DestroyChunkContainer();

	// events
	void OnMenuOpenFile(wxCommandEvent& event);
	void OnMenuExit(wxCommandEvent& event);
	void OnTreeSelectionChanges(wxTreeEvent& event);
	void OnSearch(wxCommandEvent& event);

	//wxString m_chunkNames;

	wxDECLARE_EVENT_TABLE();

public:
	void AddLogLine(wxString msg);
};