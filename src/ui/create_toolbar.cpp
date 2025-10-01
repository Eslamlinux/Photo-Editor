#include "ui/main_frame.h"
#include <wx/artprov.h>
#include <wx/toolbar.h>

namespace pme {
namespace ui {

void MainFrame::createToolBar()
{
    // إنشاء شريط الأدوات
    m_toolBar = CreateToolBar(wxTB_HORIZONTAL | wxTB_FLAT | wxTB_TEXT);
    
    // إضافة أدوات شريط الأدوات
    m_toolBar->AddTool(wxID_OPEN, _("Open"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR), _("Open image"));
    m_toolBar->AddTool(wxID_SAVE, _("Save"), wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR), _("Save image"));
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(wxID_UNDO, _("Undo"), wxArtProvider::GetBitmap(wxART_UNDO, wxART_TOOLBAR), _("Undo"));
    m_toolBar->AddTool(wxID_REDO, _("Redo"), wxArtProvider::GetBitmap(wxART_REDO, wxART_TOOLBAR), _("Redo"));
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(ID_CROP, _("Crop"), wxArtProvider::GetBitmap(wxART_CUT, wxART_TOOLBAR), _("Crop image"));
    m_toolBar->AddTool(ID_ROTATE_90_CW, _("Rotate CW"), wxArtProvider::GetBitmap(wxART_REDO, wxART_TOOLBAR), _("Rotate 90° clockwise"));
    m_toolBar->AddTool(ID_ROTATE_90_CCW, _("Rotate CCW"), wxArtProvider::GetBitmap(wxART_UNDO, wxART_TOOLBAR), _("Rotate 90° counter-clockwise"));
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(ID_ZOOM_IN, _("Zoom In"), wxArtProvider::GetBitmap(wxART_PLUS, wxART_TOOLBAR), _("Zoom in"));
    m_toolBar->AddTool(ID_ZOOM_OUT, _("Zoom Out"), wxArtProvider::GetBitmap(wxART_MINUS, wxART_TOOLBAR), _("Zoom out"));
    m_toolBar->AddTool(ID_ZOOM_FIT, _("Zoom Fit"), wxArtProvider::GetBitmap(wxART_FULL_SCREEN, wxART_TOOLBAR), _("Zoom to fit"));
    m_toolBar->AddTool(ID_ZOOM_RESET, _("Zoom 100%"), wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_TOOLBAR), _("Zoom to 100%"));
    
    // تحديث شريط الأدوات
    m_toolBar->Realize();
}

} // namespace ui
} // namespace pme
