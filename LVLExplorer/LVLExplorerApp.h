#pragma once
#include <wx/wx.h>
#include "LVLExplorerFrame.h"
#include "LibSWBF2.h"

using LibSWBF2::Logging::LoggerEntry;


class LVLExplorerApp : public wxApp
{
private:
	static LVLExplorerFrame* m_frame;

public:
	virtual bool OnInit();
};

/* ---- CREDITS ----
 * APPLICATION ICON: adrenochromedream, http://www.rw-designer.com/user/adrenochromedream
 */