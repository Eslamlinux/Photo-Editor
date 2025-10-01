#include "ui/canvas_panel.h"
#include "core/image_processor.h"

#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/dcmemory.h>

namespace pme {
namespace ui {

BEGIN_EVENT_TABLE(CanvasPanel, wxPanel)
    EVT_PAINT(CanvasPanel::onPaint)
    EVT_SIZE(CanvasPanel::onSize)
    EVT_MOUSEWHEEL(CanvasPanel::onMouseWheel)
    EVT_MOTION(CanvasPanel::onMouseMove)
    EVT_LEFT_DOWN(CanvasPanel::onMouseDown)
    EVT_LEFT_UP(CanvasPanel::onMouseUp)
END_EVENT_TABLE()

CanvasPanel::CanvasPanel(wxWindow* parent, core::ImageProcessor* imageProcessor)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
      m_imageProcessor(imageProcessor),
      m_zoomFactor(1.0),
      m_isGridVisible(false),
      m_isInfoVisible(true),
      m_isPanning(false)
{
    // تعيين الخلفية
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetBackgroundColour(wxColour(240, 240, 240));
}

CanvasPanel::~CanvasPanel()
{
}


void CanvasPanel::onPaint(wxPaintEvent& event)
{
    // إنشاء سياق الرسم المخزن مؤقتًا
    wxBufferedPaintDC dc(this);
    
    // مسح الخلفية
    dc.SetBackground(wxBrush(GetBackgroundColour()));
    dc.Clear();
    
    // رسم الصورة
    drawImage(dc);
    
    // رسم الشبكة إذا كانت مرئية
    if (m_isGridVisible) {
        drawGrid(dc);
    }
    
    // رسم المعلومات إذا كانت مرئية
    if (m_isInfoVisible) {
        drawInfo(dc);
    }
}
void CanvasPanel::onSize(wxSizeEvent& event)
{
    // إعادة رسم اللوحة
    Refresh();
    event.Skip();
}

void CanvasPanel::onMouseWheel(wxMouseEvent& event)
{
    // تغيير عامل التكبير/التصغير
    double delta = event.GetWheelRotation() / event.GetWheelDelta();
    double factor = 1.1;
    
    if (delta > 0) {
        m_zoomFactor *= factor;
    } else if (delta < 0) {
        m_zoomFactor /= factor;
    }
    
    // تقييد عامل التكبير/التصغير
    if (m_zoomFactor < 0.1) {
        m_zoomFactor = 0.1;
    } else if (m_zoomFactor > 10.0) {
        m_zoomFactor = 10.0;
    }
    
    // إعادة رسم اللوحة
    Refresh();
}

void CanvasPanel::onMouseMove(wxMouseEvent& event)
{
    // تحديث موضع الماوس الأخير
    wxPoint mousePos = event.GetPosition();
    
    // التحقق من وضع التحريك
    if (m_isPanning) {
        // حساب الإزاحة
        wxPoint delta = mousePos - m_lastMousePos;
        
        // تحديث إزاحة التحريك
        m_panOffset.x += delta.x;
        m_panOffset.y += delta.y;
        
        // إعادة رسم اللوحة
        Refresh();
    }
    
    // تحديث موضع الماوس الأخير
    m_lastMousePos = mousePos;
    
    event.Skip();
}

void CanvasPanel::onMouseDown(wxMouseEvent& event)
{
    // تعيين وضع التحريك
    m_isPanning = true;
    
    // تحديث موضع الماوس الأخير
    m_lastMousePos = event.GetPosition();
    
    event.Skip();
}
