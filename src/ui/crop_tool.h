#ifndef PME_UI_CROP_TOOL_H
#define PME_UI_CROP_TOOL_H

#include <wx/panel.h>

namespace pme {
namespace ui {

class CanvasPanel;

class CropTool : public wxPanel {
public:
    CropTool(wxWindow* parent, CanvasPanel* canvasPanel);
    virtual ~CropTool();
    
    // عمليات الأداة
    void activate();
    void deactivate();
    bool isActive() const;
    
private:
    // معالج الأحداث
    void onPaint(wxPaintEvent& event);
    void onApply(wxCommandEvent& event);
    void onCancel(wxCommandEvent& event);
    
    // المتغيرات الخاصة
    CanvasPanel* m_canvasPanel;
    bool m_isActive;
    
    DECLARE_EVENT_TABLE()
};

} // namespace ui
} // namespace pme

#endif // PME_UI_CROP_TOOL_H
