// هذا الملف يحتوي على تنفيذ الوظائف المتبقية في MainFrame

#include "ui/main_frame.h"
#include "ui/canvas_panel.h"
#include "ui/crop_tool.h"
#include "core/image_processor.h"

#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <wx/colordlg.h>
#include <wx/filedlg.h>
#include <wx/textctrl.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <opencv2/imgcodecs.hpp>


namespace pme {
namespace ui {

// إضافة عناصر القائمة الجديدة إلى دالة createMenuBar
void MainFrame::createMenuBar()
{
    // إنشاء شريط القوائم
    m_menuBar = new wxMenuBar();
    
    // إنشاء قائمة الملف
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_OPEN, _("&Open...\tCtrl+O"));
    fileMenu->Append(wxID_SAVE, _("&Save\tCtrl+S"));
    fileMenu->Append(wxID_SAVEAS, _("Save &As...\tCtrl+Shift+S"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, _("E&xit\tAlt+F4"));
    m_menuBar->Append(fileMenu, _("&File"));
    
    // إنشاء قائمة التحرير

    wxMenu* editMenu = new wxMenu();
    editMenu->Append(wxID_UNDO, _("&Undo\tCtrl+Z"));
    editMenu->Append(wxID_REDO, _("&Redo\tCtrl+Y"));
    editMenu->AppendSeparator();
    editMenu->Append(ID_CROP, _("&Crop\tC"));
    editMenu->Append(ID_ROTATE_90_CW, _("Rotate 90° &Clockwise\tR"));
    editMenu->Append(ID_ROTATE_90_CCW, _("Rotate 90° &Counter-Clockwise\tShift+R"));
    editMenu->Append(ID_ROTATE_180, _("Rotate &180°\tCtrl+R"));
    editMenu->Append(ID_FLIP_HORIZONTAL, _("Flip &Horizontal\tH"));
    editMenu->Append(ID_FLIP_VERTICAL, _("Flip &Vertical\tV"));
    editMenu->AppendSeparator();
    editMenu->Append(wxID_NEW + 100, _("&Resize...\tCtrl+Alt+R"));
    m_menuBar->Append(editMenu, _("&Edit"));
    
    // إنشاء قائمة المرشحات
