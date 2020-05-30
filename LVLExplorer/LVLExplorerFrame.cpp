#include "LVLExplorerFrame.h"
#include <wx/msgdlg.h>
#include <wx/wfstream.h>
#include <wx/sizer.h>

#define ID_MENU_FILE_OPEN 1138
#define ID_MENU_EXIT 1139
#define ID_TREE_VIEW 1140

wxBEGIN_EVENT_TABLE(LVLExplorerFrame, wxFrame)
	EVT_MENU(ID_MENU_FILE_OPEN, OnMenuOpenFile)
	EVT_MENU(ID_MENU_EXIT, OnMenuExit)
	EVT_TREE_SEL_CHANGED(ID_TREE_VIEW, OnTreeSelectionChanges)
wxEND_EVENT_TABLE()

LVLExplorerFrame::LVLExplorerFrame() : wxFrame(
	nullptr,
	wxID_ANY,
	"LVLExplorer", 
	wxDefaultPosition, 
	wxSize(1024, 768))
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
		ID_TREE_VIEW,
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

	m_infoText = new wxStaticText(
		m_panelMain,
		wxID_ANY,
		"Chunk Position:\n"
		"Chunk Data Size:\n"
		"Chunk Full Size:",
		wxDefaultPosition,
		wxDefaultSize
	);
	m_infoText->SetMinSize(wxSize(400, 50));
	m_infoText->SetMaxSize(wxSize(400, 50));

	m_sizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_sizer1->Add(m_lvlTreeCtrl, wxSizerFlags().Expand().Proportion(1).Border(wxALL, 10));
	m_panelMain->SetSizer(m_sizer1);
	m_panelMain->SetAutoLayout(true);

	m_sizer2 = new wxBoxSizer(wxVERTICAL);
	m_sizer2->Add(m_infoText, wxSizerFlags().Proportion(1).Border(wxTOP, 10));
	m_sizer1->Add(m_sizer2, wxSizerFlags().Expand().Proportion(2));

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
	if (m_displayStatus == EDisplayStatus::TEXT)
		return;

	if (m_displayStatus != EDisplayStatus::NONE)
		HideCurrentDisplay();

	m_textDisplay->Show();
	m_sizer2->Add(m_textDisplay, m_rightHandSideFlags);
	m_sizer2->Layout();
	m_panelMain->Layout();
	m_displayStatus = EDisplayStatus::TEXT;
}

void LVLExplorerFrame::DisplayImage()
{
	if (m_displayStatus == EDisplayStatus::IMAGE)
		return;

	if (m_displayStatus != EDisplayStatus::NONE)
		HideCurrentDisplay();

	if (m_imageData == nullptr)
	{
		m_imageWidth = 256;
		m_imageHeight = 256;
		size_t size = m_imageWidth * m_imageHeight * 3;
		m_imageData = (unsigned char*)malloc(size);

		for (int i = 0; i < size; ++i)
		{
			// black image
			m_imageData[i] = 0;
		}
	}

	m_imageDisplay->SetImageData(m_imageWidth, m_imageHeight, m_imageData);
	m_imageDisplay->Show();
	m_sizer2->Add(m_imageDisplay, m_rightHandSideFlags);
	m_sizer2->Layout();
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
			m_sizer2->Remove(1);
			m_textDisplay->Hide();
			break;
		case EDisplayStatus::IMAGE:
			m_sizer2->Remove(1);
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
	m_treeToChunk.clear();

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
		ParseChunk(m_currentLVL, m_treeRoot);
	}
}

void LVLExplorerFrame::OnMenuExit(wxCommandEvent& event)
{
	Close();
}

void LVLExplorerFrame::OnTreeSelectionChanges(wxTreeEvent& event)
{
	wxTreeItemId item = event.GetItem();
	if (item == m_treeRoot)
	{
		m_infoText->SetLabel(
			"Chunk Position:\n"
			"Chunk Data Size:\n"
			"Chunk Full Size:"
		);
		return;
	}

	auto it = m_treeToChunk.find(item);
	if (it == m_treeToChunk.end())
	{
		wxLogError("Could not find corresponding chunk for tree item ID %i!", item.GetID());
		return;
	}

	GenericChunk* chunk = it->second;
	m_infoText->SetLabel(wxString::Format(
		"Chunk Position:\t%i\n"
		"Chunk Data Size:\t%i\n"
		"Chunk Full Size:\t%i",
		(uint32_t)chunk->GetPosition(),
		(uint32_t)chunk->GetDataSize(),
		(uint32_t)chunk->GetFullSize()
	));

	BODY* textureBodyChunk = dynamic_cast<BODY*>(chunk);
	if (textureBodyChunk != nullptr)
	{
		uint8_t* data;

		// this delivers R8 G8 B8 A8
		textureBodyChunk->GetImageData(m_imageWidth, m_imageHeight, data);

		size_t numPixels = m_imageWidth * m_imageHeight;
		if (m_imageData != nullptr)
		{
			delete[] m_imageData;
		}

		m_imageData = new unsigned char[numPixels * 3];
		for (size_t i = 0; i < numPixels; ++i)
		{
			//let's get rid of the alpha channel
			m_imageData[(i * 3) + 0] = data[(i * 4) + 0];
			m_imageData[(i * 3) + 1] = data[(i * 4) + 1];
			m_imageData[(i * 3) + 2] = data[(i * 4) + 2];
		}

		DisplayImage();
	}
	else
	{
		m_textDisplay->Clear();
		m_textDisplay->WriteText(chunk->ToString().Buffer());
		DisplayText();
	}
}

void LVLExplorerFrame::ParseChunk(GenericChunk* chunk, wxTreeItemId parent)
{
	static int count = 0;
	if (count > 1000) return;
	count++;

	wxTreeItemId current = m_lvlTreeCtrl->AppendItem(parent, chunk->GetHeaderName().c_str());
	m_treeToChunk.emplace(current, chunk);

	const List<GenericChunk*>& children = chunk->GetChildren();
	for (size_t i = 0; i < children.Size(); ++i)
	{
		ParseChunk(children[i], current);
	}
}

void LVLExplorerFrame::AddLogLine(wxString msg)
{
	if (m_textDisplay != nullptr)
		m_textDisplay->AppendText(msg + "\n");
}