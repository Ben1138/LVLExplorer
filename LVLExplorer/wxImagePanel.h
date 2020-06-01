#include <wx/wx.h>
#include <wx/sizer.h>


/*
 * Originally copied from: https://wiki.wxwidgets.org/An_image_panel
 * Adapted to our needs.
 */
class wxImagePanel : public wxPanel
{
public:
    wxImagePanel(wxWindow* parent);

    /*
     * According to the docs, the data must have been allocated with 
     * malloc(), NOT new!
     */
    void SetImageData(int width, int height, unsigned char* data);

private:
    wxImage m_image;
    wxBitmap m_bitmap;

    void paintEvent(wxPaintEvent& evt);
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);

    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */

    DECLARE_EVENT_TABLE()
};