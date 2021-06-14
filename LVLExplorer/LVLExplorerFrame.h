#pragma once
#include <map>
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/menu.h>
#include <wx/progdlg.h>
#include "wxImagePanel.h"
#include "LibSWBF2.h"
#include "Chunks/LVL/tex_/tex_.h"
#include "Chunks/LVL/tex_/BODY.h"

using std::map;
using LibSWBF2::Chunks::LVL::LVL;
using LibSWBF2::Chunks::BNK::BNK;
using LibSWBF2::Chunks::LVL::texture::tex_;
using LibSWBF2::Chunks::LVL::LVL_texture::BODY;
using LibSWBF2::Chunks::GenericBaseChunk;
using LibSWBF2::Types::List;
using LibSWBF2::ETextureFormat;
using LibSWBF2::ELogType;
using LibSWBF2::ELoadStatus;
using LibSWBF2::Container;
using LibSWBF2::SWBF2Handle;
using LibSWBF2::Wrappers::Level;
using LibSWBF2::Logging::Logger;
using LibSWBF2::Logging::LoggerEntry;

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
	//wxTimer m_timer;
	wxProgressDialog* m_progress;

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

	Container* m_currentContainer;
	map<wxTreeItemId, const GenericBaseChunk*> m_treeToChunk;

	uint16_t m_imageWidth;
	uint16_t m_imageHeight;

	// use malloc, NOT new! See wxImage::SetData documentation
	uint8_t* m_imageData = nullptr;

private:
	void DisplayText();
	void DisplayImage();
	void HideCurrentDisplay();
	void ParseChunk(const GenericBaseChunk* chunk, wxTreeItemId parent, size_t childIndex=0);
	bool SearchTree(wxTreeItemId parent, const wxString& search);
	void DestroyLibContainer();
	void AddLogLine(wxString msg);

	// events
	void OnMenuOpenFile(wxCommandEvent& event);
	void OnMenuExit(wxCommandEvent& event);
	void OnTreeSelectionChanges(wxTreeEvent& event);
	void OnSearch(wxCommandEvent& event);
	void OnIdle(wxIdleEvent& event);

	wxDECLARE_EVENT_TABLE();
};