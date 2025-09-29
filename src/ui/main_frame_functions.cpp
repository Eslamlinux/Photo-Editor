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
    

    // إنشاء قائمة العرض
    wxMenu* viewMenu = new wxMenu();
    viewMenu->Append(ID_ZOOM_IN, _("Zoom &In\t+"));
    viewMenu->Append(ID_ZOOM_OUT, _("Zoom &Out\t-"));
    viewMenu->Append(ID_ZOOM_FIT, _("Zoom to &Fit\tF"));
    viewMenu->Append(ID_ZOOM_RESET, _("Zoom &Reset\t0"));
    viewMenu->AppendSeparator();
    viewMenu->Append(ID_TOGGLE_GRID, _("Show &Grid\tG"));
    viewMenu->Append(ID_TOGGLE_INFO, _("Show &Info\tI"));
    m_menuBar->Append(viewMenu, _("&View"));
    

    // إنشاء قائمة الأدوات
    wxMenu* toolsMenu = new wxMenu();
    toolsMenu->Append(wxID_NEW + 108, _("Crop to Aspect &Ratio..."));
    toolsMenu->Append(wxID_NEW + 109, _("Add &Border..."));
    toolsMenu->Append(wxID_NEW + 110, _("Add &Vignette..."));
    toolsMenu->Append(wxID_NEW + 111, _("Add &Watermark..."));
    toolsMenu->Append(wxID_NEW + 112, _("Add &Text..."));
    m_menuBar->Append(toolsMenu, _("&Tools"));
 
    // إنشاء قائمة المساعدة
    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, _("&About..."));
    m_menuBar->Append(helpMenu, _("&Help"));
  
 
    // تعيين شريط القوائم
    SetMenuBar(m_menuBar);
}

void MainFrame::onBrightness(wxCommandEvent& event) {
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Brightness"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    
    // إنشاء عناصر مربع الحوار
    wxSlider* slider = new wxSlider(&dialog, wxID_ANY, 0, -100, 100, wxDefaultPosition, wxSize(300, -1));
    wxStaticText* valueText = new wxStaticText(&dialog, wxID_ANY, "0");
    wxButton* okButton = new wxButton(&dialog, wxID_OK, _("OK"));
    wxButton* cancelButton = new wxButton(&dialog, wxID_CANCEL, _("Cancel"));
    
    // ربط حدث تغيير المنزلق
    slider->Bind(wxEVT_SLIDER, [slider, valueText](wxCommandEvent&) {
        valueText->SetLabel(wxString::Format("%d", slider->GetValue()));
    });
    
    // إنشاء السايزر
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    
 
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Adjust brightness:")), 0, wxALL, 5);
    mainSizer->Add(slider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(valueText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);
    
    // عرض مربع الحوار
    if (dialog.ShowModal() == wxID_OK) {
        // تطبيق السطوع
        m_imageProcessor->adjustBrightness(slider->GetValue());
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}

void MainFrame::onResize(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // الحصول على أبعاد الصورة الحالية
    int currentWidth = m_imageProcessor->getWidth();
    int currentHeight = m_imageProcessor->getHeight();
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Resize Image"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    
    // إنشاء عناصر مربع الحوار
    wxSpinCtrl* widthCtrl = new wxSpinCtrl(&dialog, wxID_ANY, wxString::Format("%d", currentWidth), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, currentWidth);
    wxSpinCtrl* heightCtrl = new wxSpinCtrl(&dialog, wxID_ANY, wxString::Format("%d", currentHeight), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, currentHeight);
    wxCheckBox* aspectRatioCheck = new wxCheckBox(&dialog, wxID_ANY, _("Maintain aspect ratio"));
    wxButton* okButton = new wxButton(&dialog, wxID_OK, _("OK"));
    wxButton* cancelButton = new wxButton(&dialog, wxID_CANCEL, _("Cancel"));
    
    // تعيين حالة الاختيار الافتراضية
    aspectRatioCheck->SetValue(true);
    
    // حساب نسبة العرض إلى الارتفاع
    double aspectRatio = static_cast<double>(currentWidth) / currentHeight;
    
    // ربط حدث تغيير العرض
 
    // ربط حدث تغيير العرض
    widthCtrl->Bind(wxEVT_SPINCTRL, [widthCtrl, heightCtrl, aspectRatioCheck, aspectRatio](wxCommandEvent&) {
        if (aspectRatioCheck->IsChecked()) {
            int newWidth = widthCtrl->GetValue();
            int newHeight = static_cast<int>(newWidth / aspectRatio);
            heightCtrl->SetValue(newHeight);
        }
    });
    
    // ربط حدث تغيير الارتفاع
    heightCtrl->Bind(wxEVT_SPINCTRL, [widthCtrl, heightCtrl, aspectRatioCheck, aspectRatio](wxCommandEvent&) {
        if (aspectRatioCheck->IsChecked()) {
            int newHeight = heightCtrl->GetValue();
            int newWidth = static_cast<int>(newHeight * aspectRatio);
            widthCtrl->SetValue(newWidth);
        }
    });
    
    // إنشاء السايزر



    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 2, 5, 5);
    gridSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Width:")), 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(widthCtrl, 0, wxEXPAND);
    gridSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Height:")), 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(heightCtrl, 0, wxEXPAND);
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(gridSizer, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(aspectRatioCheck, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);
    
    // عرض مربع الحوار

    if (dialog.ShowModal() == wxID_OK) {
        // تغيير حجم الصورة
        m_imageProcessor->resize(widthCtrl->GetValue(), heightCtrl->GetValue());
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}

void MainFrame::onOilPainting(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Oil Painting"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    
    // إنشاء عناصر مربع الحوار
    wxSlider* sizeSlider = new wxSlider(&dialog, wxID_ANY, 5, 1, 10, wxDefaultPosition, wxSize(300, -1));
    wxStaticText* sizeText = new wxStaticText(&dialog, wxID_ANY, "5");
    wxSlider* dynRatioSlider = new wxSlider(&dialog, wxID_ANY, 1, 1, 5, wxDefaultPosition, wxSize(300, -1));
    wxStaticText* dynRatioText = new wxStaticText(&dialog, wxID_ANY, "1");
    wxButton* okButton = new wxButton(&dialog, wxID_OK, _("OK"));
    wxButton* cancelButton = new wxButton(&dialog, wxID_CANCEL, _("Cancel"));
    
    // ربط حدث تغيير المنزلق
    sizeSlider->Bind(wxEVT_SLIDER, [sizeSlider, sizeText](wxCommandEvent&) {
        sizeText->SetLabel(wxString::Format("%d", sizeSlider->GetValue()));
    });
    
    dynRatioSlider->Bind(wxEVT_SLIDER, [dynRatioSlider, dynRatioText](wxCommandEvent&) {
        dynRatioText->SetLabel(wxString::Format("%d", dynRatioSlider->GetValue()));
    });


    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Size:")), 0, wxALL, 5);
    mainSizer->Add(sizeSlider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(sizeText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Dynamic Ratio:")), 0, wxALL, 5);
    mainSizer->Add(dynRatioSlider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(dynRatioText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);


    if (dialog.ShowModal() == wxID_OK) {
        // تطبيق تأثير الرسم الزيتي
        m_imageProcessor->oilPainting(sizeSlider->GetValue(), dynRatioSlider->GetValue());
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}

void MainFrame::onPencilSketch(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Pencil Sketch"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    


    // إنشاء عناصر مربع الحوار
    wxCheckBox* colorCheck = new wxCheckBox(&dialog, wxID_ANY, _("Color output"));
    wxButton* okButton = new wxButton(&dialog, wxID_OK, _("OK"));
    wxButton* cancelButton = new wxButton(&dialog, wxID_CANCEL, _("Cancel"));
    
    // إنشاء السايزر
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(colorCheck, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);
    
    // عرض مربع الحوار
    if (dialog.ShowModal() == wxID_OK) {
        // تطبيق تأثير الرسم بالقلم الرصاص
        m_imageProcessor->pencilSketch(colorCheck->IsChecked());
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}


