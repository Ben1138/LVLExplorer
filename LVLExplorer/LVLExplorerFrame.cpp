#include "LVLExplorerFrame.h"
#include <wx/msgdlg.h>
#include <wx/wfstream.h>
#include <wx/sizer.h>

#define ID_MENU_FILE_OPEN 1138
#define ID_MENU_EXIT 1139

wxBEGIN_EVENT_TABLE(LVLExplorerFrame, wxFrame)
	EVT_MENU(ID_MENU_FILE_OPEN, OnMenuOpenFile)
	EVT_MENU(ID_MENU_EXIT, OnMenuExit)
wxEND_EVENT_TABLE()

LVLExplorerFrame::LVLExplorerFrame() : wxFrame(
	nullptr,
	wxID_ANY,
	"LVLExplorer", 
	wxDefaultPosition, 
	wxSize(600, 400))
{
	this->CenterOnScreen();
	SetMinSize(wxSize(600, 400));

	m_menuMain = new wxMenuBar();
	m_fileMenu = new wxMenu();
	m_fileMenu->Append(ID_MENU_FILE_OPEN, "Open *.lvl File");
	m_fileMenu->Append(ID_MENU_EXIT, "Exit");
	m_menuMain->Append(m_fileMenu, "File");
	SetMenuBar(m_menuMain);

	m_panelMain = new wxPanel(this, wxID_ANY);

	m_lvlTreeCtrl = new wxTreeCtrl(
		m_panelMain,
		wxID_ANY,
		wxDefaultPosition,
		wxSize(200, 400)
	);

	m_textDisplay = new wxTextCtrl(
		m_panelMain,
		wxID_ANY,
		"",
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_MULTILINE);
	m_textDisplay->Hide();

	m_imageDisplay = new wxImagePanel(m_panelMain);
	m_imageDisplay->Hide();
	m_imageData = nullptr;
	m_currentLVL = nullptr;

	m_sizer = new wxBoxSizer(wxHORIZONTAL);
	m_sizer->Add(m_lvlTreeCtrl, wxSizerFlags().Expand().Proportion(1).Border(wxALL, 10));
	m_panelMain->SetSizer(m_sizer);
	
	m_rightHandSideFlags = wxSizerFlags().Expand().Proportion(2).Border(wxTOP | wxRIGHT | wxBOTTOM, 10);
	m_displayStatus = EDisplayStatus::NONE;
	DisplayText();
}

LVLExplorerFrame::~LVLExplorerFrame()
{
	if (m_imageData != nullptr)
	{
		free(m_imageData);
		m_imageData = nullptr;
	}
}

void LVLExplorerFrame::DisplayText()
{
	if (m_displayStatus != EDisplayStatus::TEXT)
		HideCurrentDisplay();

	m_textDisplay->Show();
	m_sizer->Add(m_textDisplay, m_rightHandSideFlags);
	m_sizer->Layout();
	m_panelMain->Layout();
	m_displayStatus = EDisplayStatus::TEXT;
}

void LVLExplorerFrame::DisplayImage()
{
	if (m_displayStatus != EDisplayStatus::IMAGE)
		HideCurrentDisplay();

	static size_t width, height;
	if (m_imageData == nullptr)
	{
		width = 512;
		height = 512;
		size_t size = width * height * 3;
		m_imageData = (unsigned char*)malloc(size);

		for (int i = 0; i < size; ++i)
		{
			// black image
			m_imageData[i] = 0;
		}
	}

	m_imageDisplay->SetImageData(width, height, m_imageData);
	m_imageDisplay->Show();
	m_sizer->Add(m_imageDisplay, m_rightHandSideFlags);
	m_sizer->Layout();
	m_panelMain->Layout();
	m_displayStatus = EDisplayStatus::IMAGE;
}

void LVLExplorerFrame::HideCurrentDisplay()
{
	switch (m_displayStatus)
	{
		case EDisplayStatus::NONE:
			return;
		case EDisplayStatus::TEXT:
			m_sizer->Remove(1);
			m_textDisplay->Hide();
			break;
		case EDisplayStatus::IMAGE:
			m_sizer->Remove(1);
			m_imageDisplay->Hide();
			break;
		default:
			wxLogError("Unknown EDisplayStatus %i", (int)m_displayStatus);
			return;
	}
}

void LVLExplorerFrame::OnMenuOpenFile(wxCommandEvent& event)
{
	wxFileDialog dialog(this, "Open *.lvl file", "", "",
		"LVL file (*.lvl)|*.lvl", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (dialog.ShowModal() == wxID_CANCEL)
		return;


	m_lvlTreeCtrl->DeleteAllItems();
	if (m_currentLVL != nullptr)
	{
		LVL::Destroy(m_currentLVL);
		m_currentLVL = nullptr;
	}

	m_currentLVL = LVL::Create();
	if (!m_currentLVL->ReadFromFile(dialog.GetPath().c_str().AsChar()))
	{
		wxMessageBox(
			wxString::Format("Errors occoured while opening file '%s'!", dialog.GetPath()),
			"Error",
			wxICON_ERROR);

		//LVL::Destroy(m_currentLVL);
		//m_currentLVL = nullptr;
		//return;
	}

	m_treeRoot = m_lvlTreeCtrl->AddRoot(dialog.GetFilename().c_str().AsChar());
	if (m_currentLVL != nullptr)
	{
		ParseChunk(*m_currentLVL, m_treeRoot);
	}
}

void LVLExplorerFrame::OnMenuExit(wxCommandEvent& event)
{
	Close();
}

void LVLExplorerFrame::ParseChunk(const GenericChunk& chunk, wxTreeItemId parent)
{
	static int count = 0;
	if (count > 1000) return;
	count++;

	wxTreeItemId current = m_lvlTreeCtrl->AppendItem(parent, chunk.GetHeaderName().c_str());

	const std::vector<GenericChunk>& children = chunk.GetChildren();
	for (const GenericChunk& child : children)
	{
		ParseChunk(child, current);
	}
}

void LVLExplorerFrame::AddLogLine(wxString msg)
{
	if (m_textDisplay != nullptr)
		m_textDisplay->AppendText(msg + "\n");
}