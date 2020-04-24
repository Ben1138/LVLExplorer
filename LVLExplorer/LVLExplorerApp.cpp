#include "LVLExplorerApp.h"

wxIMPLEMENT_APP(LVLExplorerApp);

using LibSWBF2::Logging::Logger;
using LibSWBF2::Logging::LogCallback;
using LibSWBF2::Logging::ELogType;


LVLExplorerFrame* LVLExplorerApp::m_frame;

bool LVLExplorerApp::OnInit()
{
	Logger::SetLogCallback(&SWBF2LogCallback);

	m_frame = new LVLExplorerFrame();
	m_frame->Show();
	return true;
}

void LVLExplorerApp::SWBF2LogCallback(const LoggerEntry* loggerEntry)
{
	m_frame->AddLogLine(loggerEntry->ToString().c_str());
}