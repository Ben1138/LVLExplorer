#pragma once
#include <wx/wx.h>
#include "LVLExplorerFrame.h"
#include "LibSWBF2.h"

using LibSWBF2::Logging::LoggerEntry;
using LibSWBF2::Logging::LogCallback;

class LVLExplorerApp : public wxApp
{
private:
	static LVLExplorerFrame* m_frame;

private:
	static void SWBF2LogCallback(const LoggerEntry* loggerEntry);

public:
	virtual bool OnInit();
};

/* ---- CREDITS ----
 * ICON: adrenochromedream, http://www.rw-designer.com/user/adrenochromedream
 */