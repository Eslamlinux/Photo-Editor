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

void CanvasPanel::onMouseUp(wxMouseEvent& event)
{
    // إلغاء وضع التحريك
    m_isPanning = false;
    
    event.Skip();
}

void CanvasPanel::drawImage(wxDC& dc)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor || !m_imageProcessor->hasImage()) {
        return;
    }
    
    // الحصول على الصورة
    cv::Mat image = m_imageProcessor->getImage();
    
    // تحويل الصورة من OpenCV إلى wxWidgets
    wxImage wxImg;
    
    // التحقق من نوع الصورة
    if (image.channels() == 3) {
        // صورة RGB
        wxImg = wxImage(image.cols, image.rows);
        
        // نسخ بيانات الصورة
        unsigned char* imgData = wxImg.GetData();
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
                int index = (y * image.cols + x) * 3;
                imgData[index] = pixel[2];     // R
                imgData[index + 1] = pixel[1]; // G
                imgData[index + 2] = pixel[0]; // B
            }
        }
    } else if (image.channels() == 4) {
        // صورة RGBA
        wxImg = wxImage(image.cols, image.rows, true);
        
        // نسخ بيانات الصورة
        unsigned char* imgData = wxImg.GetData();
        unsigned char* alphaData = wxImg.GetAlpha();
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                cv::Vec4b pixel = image.at<cv::Vec4b>(y, x);
                int index = (y * image.cols + x) * 3;
                imgData[index] = pixel[2];     // R
                imgData[index + 1] = pixel[1]; // G
                imgData[index + 2] = pixel[0]; // B
                alphaData[y * image.cols + x] = pixel[3]; // Alpha
            }
        }
    } else if (image.channels() == 1) {
        // صورة رمادية
        wxImg = wxImage(image.cols, image.rows);
        
        // نسخ بيانات الصورة
        unsigned char* imgData = wxImg.GetData();
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                unsigned char pixel = image.at<unsigned char>(y, x);
                int index = (y * image.cols + x) * 3;
                imgData[index] = pixel;     // R
                imgData[index + 1] = pixel; // G
                imgData[index + 2] = pixel; // B
            }
        }
    } else {
        // نوع غير مدعوم
        return;
    }
    
    // تحويل الصورة إلى bitmap
    wxBitmap bitmap(wxImg);
    
    // حساب حجم الصورة المقياسة
    wxSize scaledSize = getScaledImageSize();
    
    // حساب موضع الصورة
    wxPoint origin = getImageOrigin();
    
    // رسم الصورة
    dc.SetUserScale(m_zoomFactor, m_zoomFactor);
    dc.DrawBitmap(bitmap, origin.x / m_zoomFactor, origin.y / m_zoomFactor, true);
    dc.SetUserScale(1.0, 1.0);
}

void CanvasPanel::drawGrid(wxDC& dc)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor || !m_imageProcessor->hasImage()) {
        return;
    }
    
    // الحصول على مستطيل الصورة
    wxRect imageRect = getImageRect();
    
    // تعيين قلم الشبكة
    dc.SetPen(wxPen(wxColour(200, 200, 200), 1, wxPENSTYLE_DOT));
    
    // رسم الشبكة الأفقية
    int gridSize = 50;
    for (int y = imageRect.GetTop(); y <= imageRect.GetBottom(); y += gridSize) {
        dc.DrawLine(imageRect.GetLeft(), y, imageRect.GetRight(), y);
    }
    
    // رسم الشبكة الرأسية
    for (int x = imageRect.GetLeft(); x <= imageRect.GetRight(); x += gridSize) {
        dc.DrawLine(x, imageRect.GetTop(), x, imageRect.GetBottom());
    }
}

void CanvasPanel::drawInfo(wxDC& dc)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor || !m_imageProcessor->hasImage()) {
        return;
    }
    
    // تعيين الخط
    dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.SetTextForeground(wxColour(0, 0, 0));
    
    // إنشاء نص المعلومات
    wxString info;
    info << "Size: " << m_imageProcessor->getWidth() << " x " << m_imageProcessor->getHeight();
    info << "  |  Zoom: " << static_cast<int>(m_zoomFactor * 100) << "%";
    
    // رسم النص
    dc.DrawText(info, 10, 10);
}

wxPoint CanvasPanel::getImageOrigin() const
{
    // التحقق من وجود صورة
    if (!m_imageProcessor || !m_imageProcessor->hasImage()) {
        return wxPoint(0, 0);
    }
    
    // حساب حجم الصورة المقياسة
    wxSize scaledSize = getScaledImageSize();
    
    // حساب موضع الصورة
    wxSize clientSize = GetClientSize();
    wxPoint origin;
    
    if (scaledSize.GetWidth() < clientSize.GetWidth()) {
        origin.x = (clientSize.GetWidth() - scaledSize.GetWidth()) / 2;
    } else {
        origin.x = 0;
    }
    
    if (scaledSize.GetHeight() < clientSize.GetHeight()) {
        origin.y = (clientSize.GetHeight() - scaledSize.GetHeight()) / 2;
    } else {
        origin.y = 0;
    }
    
    // إضافة إزاحة التحريك
    origin.x += m_panOffset.x;
    origin.y += m_panOffset.y;
    
    return origin;
}

wxSize CanvasPanel::getScaledImageSize() const
{
    // التحقق من وجود صورة
    if (!m_imageProcessor || !m_imageProcessor->hasImage()) {
        return wxSize(0, 0);
    }
    
    // حساب حجم الصورة المقياسة
    int width = static_cast<int>(m_imageProcessor->getWidth() * m_zoomFactor);
    int height = static_cast<int>(m_imageProcessor->getHeight() * m_zoomFactor);
    
    return wxSize(width, height);
}

wxRect CanvasPanel::getImageRect() const
{
    // الحصول على موضع الصورة
    wxPoint origin = getImageOrigin();
    
    // الحصول على حجم الصورة المقياسة
    wxSize scaledSize = getScaledImageSize();
    
    return wxRect(origin, scaledSize);
}

void CanvasPanel::zoomIn()
{
    // زيادة عامل التكبير/التصغير
    m_zoomFactor *= 1.1;
    
    // تقييد عامل التكبير/التصغير
    if (m_zoomFactor > 10.0) {
        m_zoomFactor = 10.0;
    }
    
    // إعادة رسم اللوحة
    Refresh();
}

void CanvasPanel::zoomOut()
{
    // تقليل عامل التكبير/التصغير
    m_zoomFactor /= 1.1;
    
    // تقييد عامل التكبير/التصغير
    if (m_zoomFactor < 0.1) {
        m_zoomFactor = 0.1;
    }
    
    // إعادة رسم اللوحة
    Refresh();
}

void CanvasPanel::zoomFit()
{
    // التحقق من وجود صورة
    if (!m_imageProcessor || !m_imageProcessor->hasImage()) {
        return;
    }
    
    // حساب عامل التكبير/التصغير المناسب
    wxSize clientSize = GetClientSize();
    double zoomX = static_cast<double>(clientSize.GetWidth()) / m_imageProcessor->getWidth();
    double zoomY = static_cast<double>(clientSize.GetHeight()) / m_imageProcessor->getHeight();
    
    // استخدام العامل الأصغر
    m_zoomFactor = std::min(zoomX, zoomY);
    
    // إعادة تعيين إزاحة التحريك
    m_panOffset = wxPoint(0, 0);
    
    // إعادة رسم اللوحة
    Refresh();
}

void CanvasPanel::zoomReset()
{
    // إعادة تعيين عامل التكبير/التصغير
    m_zoomFactor = 1.0;
    
    // إعادة تعيين إزاحة التحريك
    m_panOffset = wxPoint(0, 0);
    
    // إعادة رسم اللوحة
    Refresh();
}

void CanvasPanel::toggleGrid()
{
    // تبديل حالة رؤية الشبكة
    m_isGridVisible = !m_isGridVisible;
    
    // إعادة رسم اللوحة
    Refresh();
}

void CanvasPanel::toggleInfo()
{
    // تبديل حالة رؤية المعلومات
    m_isInfoVisible = !m_isInfoVisible;
    
    // إعادة رسم اللوحة
    Refresh();
}

double CanvasPanel::getZoomFactor() const
{
    return m_zoomFactor;
}

bool CanvasPanel::isGridVisible() const
{
    return m_isGridVisible;
}

bool CanvasPanel::isInfoVisible() const
{
    return m_isInfoVisible;
}

} // namespace ui
} // namespace pme
