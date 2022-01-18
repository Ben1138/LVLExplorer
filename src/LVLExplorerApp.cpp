#include "LVLExplorerApp.h"

wxIMPLEMENT_APP(LVLExplorerApp);


LVLExplorerFrame* LVLExplorerApp::m_frame;

bool LVLExplorerApp::OnInit()
{
	m_frame = new LVLExplorerFrame();
	m_frame->Show();

	return true;
}