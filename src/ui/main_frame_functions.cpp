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
wxMenu* filterMenu = new wxMenu();
    filterMenu->Append(ID_GRAYSCALE, _("&Grayscale"));
    filterMenu->Append(ID_SEPIA, _("S&epia"));
    filterMenu->Append(ID_NEGATIVE, _("&Negative"));
    filterMenu->Append(ID_BLUR, _("&Blur..."));
    filterMenu->Append(ID_SHARPEN, _("&Sharpen"));
    filterMenu->Append(ID_EDGE_DETECTION, _("&Edge Detection"));
    filterMenu->Append(ID_EMBOSS, _("E&mboss"));
    filterMenu->Append(ID_CARTOON, _("&Cartoon"));
    filterMenu->Append(ID_WATERCOLOR, _("&Watercolor"));
    filterMenu->Append(ID_MOSAIC, _("&Mosaic..."));
    filterMenu->Append(wxID_NEW + 101, _("&Oil Painting"));
    filterMenu->Append(wxID_NEW + 102, _("&Pencil Sketch"));
    m_menuBar->Append(filterMenu, _("&Filter"));
    
    // إنشاء قائمة التعديل
wxMenu* adjustMenu = new wxMenu();
    adjustMenu->Append(ID_BRIGHTNESS, _("&Brightness..."));
    adjustMenu->Append(ID_CONTRAST, _("&Contrast..."));
    adjustMenu->Append(ID_SATURATION, _("&Saturation..."));
    adjustMenu->Append(ID_HUE, _("&Hue..."));
    adjustMenu->Append(ID_GAMMA, _("&Gamma..."));
    adjustMenu->Append(ID_TEMPERATURE, _("&Temperature..."));
    adjustMenu->Append(ID_SHADOWS_HIGHLIGHTS, _("&Shadows/Highlights..."));
    adjustMenu->AppendSeparator();
    adjustMenu->Append(wxID_NEW + 103, _("&Auto White Balance"));
    adjustMenu->Append(wxID_NEW + 104, _("Auto &Contrast"));
    adjustMenu->Append(wxID_NEW + 105, _("Auto Color &Enhance"));
    adjustMenu->Append(wxID_NEW + 106, _("Reduce &Noise"));
    adjustMenu->Append(wxID_NEW + 107, _("Adaptive &Sharpen"));
    m_menuBar->Append(adjustMenu, _("&Adjust"));
    
