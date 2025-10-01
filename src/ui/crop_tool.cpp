#include "ui/crop_tool.h"
#include "ui/canvas_panel.h"

#include <wx/dcclient.h>
#include <wx/button.h>
#include <wx/sizer.h>

namespace pme {
namespace ui {

BEGIN_EVENT_TABLE(CropTool, wxPanel)
    EVT_PAINT(CropTool::onPaint)
END_EVENT_TABLE()

CropTool::CropTool(wxWindow* parent, CanvasPanel* canvasPanel)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
      m_canvasPanel(canvasPanel),
      m_isActive(false)
{
    // إنشاء عناصر واجهة المستخدم
    wxButton* applyButton = new wxButton(this, wxID_APPLY, "Apply");
    wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
    
    // إنشاء تخطيط اللوحة
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(applyButton, 0, wxALL, 5);
    sizer->Add(cancelButton, 0, wxALL, 5);
    SetSizer(sizer);
    
    // ربط معالجات الأحداث
    applyButton->Bind(wxEVT_BUTTON, &CropTool::onApply, this);
    cancelButton->Bind(wxEVT_BUTTON, &CropTool::onCancel, this);
    
    // إخفاء اللوحة
    Hide();
}

CropTool::~CropTool()
{
}

void CropTool::activate()
{
    // تنشيط الأداة
    m_isActive = true;
    
    // إظهار اللوحة
    Show();
    
    // إعادة تخطيط الإطار الأصل
    GetParent()->Layout();
}

void CropTool::deactivate()
{
    // إلغاء تنشيط الأداة
    m_isActive = false;
    
    // إخفاء اللوحة
    Hide();
    
    // إعادة تخطيط الإطار الأصل
    GetParent()->Layout();
}

bool CropTool::isActive() const
{
    return m_isActive;
}

void CropTool::onPaint(wxPaintEvent& event)
{
    // إنشاء سياق الرسم
    wxPaintDC dc(this);
    
    // رسم خلفية اللوحة
    dc.SetBackground(wxBrush(GetBackgroundColour()));
    dc.Clear();
}

void CropTool::onApply(wxCommandEvent& event)
{
    // تطبيق عملية القص
    // (سيتم تنفيذها لاحقًا)
    
    // إلغاء تنشيط الأداة
    deactivate();
}

void CropTool::onCancel(wxCommandEvent& event)
{
    // إلغاء عملية القص
    
    // إلغاء تنشيط الأداة
    deactivate();
}

} // namespace ui
} // namespace pme
