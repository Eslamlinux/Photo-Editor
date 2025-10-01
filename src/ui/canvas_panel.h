#ifndef PME_UI_CANVAS_PANEL_H
#define PME_UI_CANVAS_PANEL_H

#include <wx/panel.h>

namespace pme {

namespace core {
class ImageProcessor;
}

namespace ui {

class CanvasPanel : public wxPanel {
public:
    CanvasPanel(wxWindow* parent, core::ImageProcessor* imageProcessor);
    virtual ~CanvasPanel();
    
    // عمليات التكبير/التصغير
    void zoomIn();
    void zoomOut();
    void zoomFit();
    void zoomReset();
    
    // عمليات العرض
    void toggleGrid();
    void toggleInfo();
    
    // الوصول إلى الحالة
    double getZoomFactor() const;
    bool isGridVisible() const;
    bool isInfoVisible() const;
    
private:
    // معالج الأحداث
    void onPaint(wxPaintEvent& event);
    void onSize(wxSizeEvent& event);
    void onMouseWheel(wxMouseEvent& event);
    void onMouseMove(wxMouseEvent& event);
    void onMouseDown(wxMouseEvent& event);
    void onMouseUp(wxMouseEvent& event);
    
    // وظائف الرسم
    void drawImage(wxDC& dc);
    void drawGrid(wxDC& dc);
    void drawInfo(wxDC& dc);
    
    // وظائف مساعدة
    wxPoint getImageOrigin() const;
    wxSize getScaledImageSize() const;
    wxRect getImageRect() const;
    
    // المتغيرات الخاصة
    core::ImageProcessor* m_imageProcessor;
    double m_zoomFactor;
    bool m_isGridVisible;
    bool m_isInfoVisible;
    wxPoint m_lastMousePos;
    wxPoint m_panOffset;
    bool m_isPanning;
    
    DECLARE_EVENT_TABLE()
};

} // namespace ui
} // namespace pme

#endif // PME_UI_CANVAS_PANEL_H
