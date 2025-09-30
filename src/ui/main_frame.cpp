#include "main_frame.h"
#include "canvas_panel.h"
#include "crop_tool.h"
#include "../core/image_processor.h"

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/dcclient.h>
#include <wx/artprov.h>
#include <wx/aboutdlg.h>
#include <wx/colordlg.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <wx/filename.h>

namespace pme {
namespace ui {

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_OPEN, MainFrame::onOpen)
    EVT_MENU(wxID_SAVE, MainFrame::onSave)
    EVT_MENU(wxID_SAVEAS, MainFrame::onSaveAs)
    EVT_MENU(wxID_EXIT, MainFrame::onExit)
    EVT_CLOSE(MainFrame::onClose)
    EVT_MENU(wxID_UNDO, MainFrame::onUndo)
    EVT_MENU(wxID_REDO, MainFrame::onRedo)
    EVT_MENU(wxID_ABOUT, MainFrame::onAbout)
    EVT_MENU(ID_CROP, MainFrame::onCrop)
    EVT_MENU(ID_ROTATE_90_CW, MainFrame::onRotate90CW)
    EVT_MENU(ID_ROTATE_90_CCW, MainFrame::onRotate90CCW)
    EVT_MENU(ID_ROTATE_180, MainFrame::onRotate180)
    EVT_MENU(ID_FLIP_HORIZONTAL, MainFrame::onFlipHorizontal)
    EVT_MENU(ID_FLIP_VERTICAL, MainFrame::onFlipVertical)
    EVT_MENU(ID_GRAYSCALE, MainFrame::onGrayscale)
    EVT_MENU(ID_SEPIA, MainFrame::onSepia)
    EVT_MENU(ID_NEGATIVE, MainFrame::onNegative)
    EVT_MENU(ID_BLUR, MainFrame::onBlur)
    EVT_MENU(ID_SHARPEN, MainFrame::onSharpen)
    EVT_MENU(ID_EDGE_DETECTION, MainFrame::onEdgeDetection)
    EVT_MENU(ID_EMBOSS, MainFrame::onEmboss)
 EVT_MENU(ID_CARTOON, MainFrame::onCartoon)
    EVT_MENU(ID_WATERCOLOR, MainFrame::onWatercolor)
    EVT_MENU(ID_MOSAIC, MainFrame::onMosaic)
    EVT_MENU(ID_BRIGHTNESS, MainFrame::onBrightness)
    EVT_MENU(ID_CONTRAST, MainFrame::onContrast)
    EVT_MENU(ID_SATURATION, MainFrame::onSaturation)
    EVT_MENU(ID_HUE, MainFrame::onHue)
    EVT_MENU(ID_GAMMA, MainFrame::onGamma)
    EVT_MENU(ID_TEMPERATURE, MainFrame::onTemperature)
    EVT_MENU(ID_SHADOWS_HIGHLIGHTS, MainFrame::onShadowsHighlights)
    EVT_MENU(ID_RESET, MainFrame::onReset)
    EVT_MENU(ID_ZOOM_IN, MainFrame::onZoomIn)
    EVT_MENU(ID_ZOOM_OUT, MainFrame::onZoomOut)
    EVT_MENU(ID_ZOOM_FIT, MainFrame::onZoomFit)
    EVT_MENU(ID_ZOOM_RESET, MainFrame::onZoomReset)
    EVT_MENU(ID_TOGGLE_GRID, MainFrame::onToggleGrid)
    EVT_MENU(ID_TOGGLE_INFO, MainFrame::onToggleInfo)
    EVT_MENU(ID_LANGUAGE_ENGLISH, MainFrame::onLanguageEnglish)
    EVT_MENU(ID_LANGUAGE_ARABIC, MainFrame::onLanguageArabic)
EVT_MENU(wxID_NEW + 100, MainFrame::onResize)
    EVT_MENU(wxID_NEW + 101, MainFrame::onOilPainting)
    EVT_MENU(wxID_NEW + 102, MainFrame::onPencilSketch)
    EVT_MENU(wxID_NEW + 103, MainFrame::onAutoWhiteBalance)
    EVT_MENU(wxID_NEW + 104, MainFrame::onAutoContrast)
    EVT_MENU(wxID_NEW + 105, MainFrame::onAutoColorEnhance)
    EVT_MENU(wxID_NEW + 106, MainFrame::onReduceNoise)
    EVT_MENU(wxID_NEW + 107, MainFrame::onAdaptiveSharpen)
    EVT_MENU(wxID_NEW + 108, MainFrame::onCropToAspectRatio)
    EVT_MENU(wxID_NEW + 109, MainFrame::onAddBorder)
    EVT_MENU(wxID_NEW + 110, MainFrame::onAddVignette)
    EVT_MENU(wxID_NEW + 111, MainFrame::onAddWatermark)
    EVT_MENU(wxID_NEW + 112, MainFrame::onAddText)
END_EVENT_TABLE()
MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1024, 768)),
      m_imageProcessor(new core::ImageProcessor()),
      m_isModified(false)
{
    // تعيين أيقونة الإطار
    SetIcon(wxArtProvider::GetIcon(wxART_FRAME_ICON));
    
    // إنشاء شريط القوائم
   createMenuBar();
    
    // إنشاء شريط الأدوات
    createToolBar();
    
    // إنشاء شريط الحالة
    m_statusBar = CreateStatusBar();
    m_statusBar->SetStatusText(_("Ready"));
    
    // إنشاء لوحة الرسم
    m_canvasPanel = new CanvasPanel(this, m_imageProcessor);
    
    // إنشاء أداة القص
    m_cropTool = new CropTool(this, m_canvasPanel);
    
    // إنشاء تخطيط الإطار
  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_canvasPanel, 1, wxEXPAND);
    SetSizer(sizer);
    
    // تعيين وظيفة التحديث
    m_imageProcessor->setUpdateCallback([this]() {
        // تحديث الإطار
        m_canvasPanel->Refresh();
        
        // تعيين حالة التعديل
        m_isModified = true;
        
        // تحديث عناصر القائمة
        updateMenuItems();
        
        // تحديث العنوان
        updateTitle();
    });
    
    // تحديث عناصر القائمة

    updateMenuItems();
    
    // تحديث العنوان
    updateTitle();
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}

MainFrame::~MainFrame()
{
    // تنظيف الموارد
    delete m_imageProcessor;
}

bool MainFrame::OpenFile(const wxString& filePath)
{
    // محاولة تحميل الصورة
    if (m_imageProcessor->loadImage(filePath.ToStdString())) {
        // تعيين مسار الملف الحالي
        m_currentFilePath = filePath;
        
        // إعادة تعيين حالة التعديل
        m_isModified = false;
        
        // تحديث عناصر القائمة
        updateMenuItems();
        
        // تحديث العنوان
        updateTitle();
        
        // تعيين التركيز على لوحة الرسم
        m_canvasPanel->SetFocus();
        
        return true;
    }
    
    return false;
}


void MainFrame::updateMenuItems()
{
    // التحقق من وجود صورة
    bool hasImage = m_imageProcessor->hasImage();
    
    // تحديث عناصر القائمة
    m_menuBar->Enable(wxID_SAVE, hasImage && m_isModified);
    m_menuBar->Enable(wxID_SAVEAS, hasImage);
    m_menuBar->Enable(wxID_UNDO, hasImage && m_imageProcessor->canUndo());
    m_menuBar->Enable(wxID_REDO, hasImage && m_imageProcessor->canRedo());
    m_menuBar->Enable(ID_CROP, hasImage);
    m_menuBar->Enable(ID_ROTATE_90_CW, hasImage);
    m_menuBar->Enable(ID_ROTATE_90_CCW, hasImage);
    m_menuBar->Enable(ID_ROTATE_180, hasImage);
    m_menuBar->Enable(ID_FLIP_HORIZONTAL, hasImage);
    m_menuBar->Enable(ID_FLIP_VERTICAL, hasImage);
    m_menuBar->Enable(ID_GRAYSCALE, hasImage);
    m_menuBar->Enable(ID_SEPIA, hasImage);
    m_menuBar->Enable(ID_NEGATIVE, hasImage);
    m_menuBar->Enable(ID_BLUR, hasImage);
    m_menuBar->Enable(ID_SHARPEN, hasImage);
    m_menuBar->Enable(ID_EDGE_DETECTION, hasImage);
    m_menuBar->Enable(ID_EMBOSS, hasImage);
    m_menuBar->Enable(ID_CARTOON, hasImage);
    m_menuBar->Enable(ID_WATERCOLOR, hasImage);
    m_menuBar->Enable(ID_MOSAIC, hasImage);
    m_menuBar->Enable(ID_BRIGHTNESS, hasImage);
    m_menuBar->Enable(ID_CONTRAST, hasImage);
    m_menuBar->Enable(ID_SATURATION, hasImage);
    m_menuBar->Enable(ID_HUE, hasImage);
    m_menuBar->Enable(ID_GAMMA, hasImage);
    m_menuBar->Enable(ID_TEMPERATURE, hasImage);
    m_menuBar->Enable(ID_SHADOWS_HIGHLIGHTS, hasImage);
    m_menuBar->Enable(ID_RESET, hasImage);
    m_menuBar->Enable(ID_ZOOM_IN, hasImage);
    m_menuBar->Enable(ID_ZOOM_OUT, hasImage);
    m_menuBar->Enable(ID_ZOOM_FIT, hasImage);
    m_menuBar->Enable(ID_ZOOM_RESET, hasImage);
    m_menuBar->Enable(ID_TOGGLE_GRID, hasImage);
    m_menuBar->Enable(ID_TOGGLE_INFO, hasImage);
    

    // تحديث عناصر شريط الأدوات
    m_toolBar->EnableTool(wxID_SAVE, hasImage && m_isModified);
    m_toolBar->EnableTool(wxID_UNDO, hasImage && m_imageProcessor->canUndo());
    m_toolBar->EnableTool(wxID_REDO, hasImage && m_imageProcessor->canRedo());
    m_toolBar->EnableTool(ID_CROP, hasImage);
    m_toolBar->EnableTool(ID_ROTATE_90_CW, hasImage);
    m_toolBar->EnableTool(ID_ROTATE_90_CCW, hasImage);
    m_toolBar->EnableTool(ID_ZOOM_IN, hasImage);
    m_toolBar->EnableTool(ID_ZOOM_OUT, hasImage);
    m_toolBar->EnableTool(ID_ZOOM_FIT, hasImage);
    m_toolBar->EnableTool(ID_ZOOM_RESET, hasImage);
}

void MainFrame::updateTitle()
{
    // إنشاء العنوان
    wxString title = "Photo Editor";
    
    // إضافة اسم الملف إذا كان موجودًا
    if (!m_currentFilePath.empty()) {
        wxFileName fileName(m_currentFilePath);
        title << " - " << fileName.GetFullName();
        
        // إضافة علامة التعديل
        if (m_isModified) {
            title << " *";
        }
    }
    
    // تعيين العنوان
    SetTitle(title);
}


void MainFrame::onOpen(wxCommandEvent& event)
{
    // التحقق من وجود تعديلات غير محفوظة
    if (m_isModified) {
        // سؤال المستخدم عن حفظ التغييرات
        wxMessageDialog dialog(this, _("Do you want to save changes?"), _("Save Changes"), wxYES_NO | wxCANCEL | wxICON_QUESTION);
        int result = dialog.ShowModal();
        
        if (result == wxID_YES) {
            // حفظ التغييرات
            wxCommandEvent saveEvent(wxEVT_COMMAND_MENU_SELECTED, wxID_SAVE);
            ProcessEvent(saveEvent);
            
            // التحقق من نجاح الحفظ
            if (m_isModified) {
                return;
            }
        } else if (result == wxID_CANCEL) {
            // إلغاء العملية
            return;
        }
    }
    
    // إنشاء مربع حوار اختيار الملف
    wxFileDialog openFileDialog(this, _("Open Image"), "", "", 
                              "Image files (*.png;*.jpg;*.jpeg;*.bmp;*.tiff)|*.png;*.jpg;*.jpeg;*.bmp;*.tiff", 
                              wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    // عرض مربع حوار اختيار الملف
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    
    // الحصول على مسار الملف
    wxString filePath = openFileDialog.GetPath();
    
    // فتح الملف
    if (!OpenFile(filePath)) {
        // عرض رسالة خطأ
        wxMessageBox(_("Failed to open image file."), _("Error"), wxICON_ERROR | wxOK);
    }
}


void MainFrame::onSave(wxCommandEvent& event)
{
    // التحقق من وجود مسار ملف
    if (m_currentFilePath.empty()) {
        // استدعاء حدث الحفظ باسم
        wxCommandEvent saveAsEvent(wxEVT_COMMAND_MENU_SELECTED, wxID_SAVEAS);
        ProcessEvent(saveAsEvent);
        return;
    }
    
    // حفظ الصورة
    if (m_imageProcessor->saveImage(m_currentFilePath.ToStdString())) {
        // إعادة تعيين حالة التعديل
        m_isModified = false;
        
        // تحديث عناصر القائمة
        updateMenuItems();
        
        // تحديث العنوان
        updateTitle();
    } else {
        // عرض رسالة خطأ
        wxMessageBox(_("Failed to save image file."), _("Error"), wxICON_ERROR | wxOK);
    }
}


void MainFrame::onSaveAs(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار حفظ الملف
    wxFileDialog saveFileDialog(this, _("Save Image"), "", "", 
                              "PNG files (*.png)|*.png|JPEG files (*.jpg)|*.jpg|BMP files (*.bmp)|*.bmp|TIFF files (*.tiff)|*.tiff", 
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    // تعيين الامتداد الافتراضي
    saveFileDialog.SetFilterIndex(0);
    
    // عرض مربع حوار حفظ الملف
    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    
    // الحصول على مسار الملف
    wxString filePath = saveFileDialog.GetPath();
    
    // التحقق من امتداد الملف
    wxFileName fileName(filePath);
    if (!fileName.HasExt()) {
        // إضافة الامتداد الافتراضي
        switch (saveFileDialog.GetFilterIndex()) {
            case 0:
                filePath << ".png";
                break;
            case 1:
                filePath << ".jpg";
                break;
            case 2:
                filePath << ".bmp";
                break;
            case 3:
                filePath << ".tiff";
                break;
        }
    }
    

    if (m_imageProcessor->saveImage(filePath.ToStdString())) {
        // تعيين مسار الملف الحالي
        m_currentFilePath = filePath;
        
        // إعادة تعيين حالة التعديل
        m_isModified = false;
        
        // تحديث عناصر القائمة
        updateMenuItems();
        
        // تحديث العنوان
        updateTitle();
    } else {
        // عرض رسالة خطأ
        wxMessageBox(_("Failed to save image file."), _("Error"), wxICON_ERROR | wxOK);
    }
}

void MainFrame::onExit(wxCommandEvent& event)
{
    // إغلاق الإطار
    Close();
}

void MainFrame::onClose(wxCloseEvent& event)
{
    // التحقق من وجود تعديلات غير محفوظة
    if (m_isModified) {
        // سؤال المستخدم عن حفظ التغييرات
        wxMessageDialog dialog(this, _("Do you want to save changes?"), _("Save Changes"), wxYES_NO | wxCANCEL | wxICON_QUESTION);
        int result = dialog.ShowModal();
        
        if (result == wxID_YES) {
            // حفظ التغييرات
            wxCommandEvent saveEvent(wxEVT_COMMAND_MENU_SELECTED, wxID_SAVE);
            ProcessEvent(saveEvent);
            
            // التحقق من نجاح الحفظ
            if (m_isModified && !event.CanVeto()) {
                // إلغاء الإغلاق
                event.Veto();
                return;
            }
        } else if (result == wxID_CANCEL && event.CanVeto()) {
            // إلغاء الإغلاق
            event.Veto();
            return;
        }
    }
    
    // تدمير الإطار
    Destroy();
}


void MainFrame::onUndo(wxCommandEvent& event)
{
    // التراجع عن آخر عملية
    m_imageProcessor->undo();
}

void MainFrame::onRedo(wxCommandEvent& event)
{
    // إعادة آخر عملية
    m_imageProcessor->redo();
}

void MainFrame::onAbout(wxCommandEvent& event)
{
    // إنشاء مربع حوار حول
    wxAboutDialogInfo info;
    info.SetName(_("Photo Editor"));
    info.SetVersion(_("1.0"));
    info.SetDescription(_("A simple photo editor application."));
    info.SetCopyright(_("(C) 2023"));
    info.SetWebSite(_("https://www.example.com"));
    info.AddDeveloper(_("Developer Name"));
    
    // عرض مربع حوار حول
    wxAboutBox(info);
}

void MainFrame::onCrop(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تنشيط أداة القص
    m_cropTool->activate();
}

void MainFrame::onRotate90CW(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تدوير الصورة 90 درجة في اتجاه عقارب الساعة
    m_imageProcessor->rotate90CW();
}


void MainFrame::onRotate90CCW(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تدوير الصورة 90 درجة عكس اتجاه عقارب الساعة
    m_imageProcessor->rotate90CCW();
}


void MainFrame::onRotate180(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تدوير الصورة 180 درجة
    m_imageProcessor->rotate180();
}

void MainFrame::onFlipHorizontal(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // قلب الصورة أفقيًا
    m_imageProcessor->flipHorizontal();
}

void MainFrame::onFlipVertical(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // قلب الصورة رأسيًا
    m_imageProcessor->flipVertical();
}

void MainFrame::onGrayscale(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تحويل الصورة إلى تدرج الرمادي
    m_imageProcessor->grayscale();
}

void MainFrame::onSepia(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تطبيق تأثير السيبيا
    m_imageProcessor->sepia();
}

void MainFrame::onNegative(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تطبيق تأثير السلبي
    m_imageProcessor->negative();
}

void MainFrame::onBlur(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Blur"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    
    // إنشاء عناصر مربع الحوار
    wxSlider* slider = new wxSlider(&dialog, wxID_ANY, 5, 1, 20, wxDefaultPosition, wxSize(300, -1));
    wxStaticText* valueText = new wxStaticText(&dialog, wxID_ANY, "5");
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
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Blur radius:")), 0, wxALL, 5);
    mainSizer->Add(slider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(valueText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);
    
    // عرض مربع الحوار
    if (dialog.ShowModal() == wxID_OK) {
        // تطبيق تأثير التمويه
        m_imageProcessor->blur(slider->GetValue());
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}

void MainFrame::onSharpen(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تطبيق تأثير التحديد
    m_imageProcessor->sharpen();
}

void MainFrame::onEdgeDetection(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تطبيق تأثير كشف الحواف
    m_imageProcessor->edgeDetection();
}

void MainFrame::onEmboss(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تطبيق تأثير النقش
    m_imageProcessor->emboss();
}

void MainFrame::onCartoon(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تطبيق تأثير الكرتون
    m_imageProcessor->cartoon();
}

void MainFrame::onWatercolor(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تطبيق تأثير الألوان المائية
    m_imageProcessor->watercolor();
}

void MainFrame::onMosaic(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Mosaic"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    
    // إنشاء عناصر مربع الحوار
    wxSlider* slider = new wxSlider(&dialog, wxID_ANY, 10, 2, 50, wxDefaultPosition, wxSize(300, -1));
    wxStaticText* valueText = new wxStaticText(&dialog, wxID_ANY, "10");
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
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Block size:")), 0, wxALL, 5);
    mainSizer->Add(slider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(valueText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);
    
    // عرض مربع الحوار
    if (dialog.ShowModal() == wxID_OK) {
        // تطبيق تأثير الفسيفساء
        m_imageProcessor->mosaic(slider->GetValue());
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}

void MainFrame::onContrast(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Contrast"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    
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
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Adjust contrast:")), 0, wxALL, 5);
    mainSizer->Add(slider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(valueText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);
    
    // عرض مربع الحوار
    if (dialog.ShowModal() == wxID_OK) {
        // تطبيق التباين
        m_imageProcessor->adjustContrast(slider->GetValue());
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}

void MainFrame::onContrast(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Contrast"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    
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
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Adjust contrast:")), 0, wxALL, 5);
    mainSizer->Add(slider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(valueText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);
    
    // عرض مربع الحوار
    if (dialog.ShowModal() == wxID_OK) {
        // تطبيق التباين
        m_imageProcessor->adjustContrast(slider->GetValue());
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}

void MainFrame::onSaturation(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Saturation"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    
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
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Adjust saturation:")), 0, wxALL, 5);
    mainSizer->Add(slider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(valueText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);
    
    // عرض مربع الحوار
    if (dialog.ShowModal() == wxID_OK) {
        // تطبيق التشبع
        m_imageProcessor->adjustSaturation(slider->GetValue());
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}

void MainFrame::onHue(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Hue"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    
    // إنشاء عناصر مربع الحوار
    wxSlider* slider = new wxSlider(&dialog, wxID_ANY, 0, -180, 180, wxDefaultPosition, wxSize(300, -1));
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
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Adjust hue:")), 0, wxALL, 5);
    mainSizer->Add(slider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(valueText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);
    
    // عرض مربع الحوار
    if (dialog.ShowModal() == wxID_OK) {
        // تطبيق تدرج اللون
        m_imageProcessor->adjustHue(slider->GetValue());
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}

void MainFrame::onGamma(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Gamma"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    
    // إنشاء عناصر مربع الحوار
    wxSlider* slider = new wxSlider(&dialog, wxID_ANY, 10, 1, 50, wxDefaultPosition, wxSize(300, -1));
    wxStaticText* valueText = new wxStaticText(&dialog, wxID_ANY, "1.0");
    wxButton* okButton = new wxButton(&dialog, wxID_OK, _("OK"));
    wxButton* cancelButton = new wxButton(&dialog, wxID_CANCEL, _("Cancel"));
    
    // ربط حدث تغيير المنزلق
    slider->Bind(wxEVT_SLIDER, [slider, valueText](wxCommandEvent&) {
        valueText->SetLabel(wxString::Format("%.1f", slider->GetValue() / 10.0));
    });
    
    // إنشاء السايزر
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Adjust gamma:")), 0, wxALL, 5);
    mainSizer->Add(slider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(valueText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);
    
    // عرض مربع الحوار
    if (dialog.ShowModal() == wxID_OK) {
        // تطبيق جاما
        m_imageProcessor->adjustGamma(slider->GetValue() / 10.0);
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}


void MainFrame::onTemperature(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Temperature"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    
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
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Adjust temperature:")), 0, wxALL, 5);
    mainSizer->Add(slider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(valueText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);
    
    // عرض مربع الحوار
    if (dialog.ShowModal() == wxID_OK) {
        // تطبيق درجة الحرارة
        m_imageProcessor->adjustTemperature(slider->GetValue());
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}

void MainFrame::onShadowsHighlights(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إنشاء مربع حوار
    wxDialog dialog(this, wxID_ANY, _("Shadows/Highlights"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    
    // إنشاء عناصر مربع الحوار
    wxSlider* shadowsSlider = new wxSlider(&dialog, wxID_ANY, 0, -100, 100, wxDefaultPosition, wxSize(300, -1));
    wxStaticText* shadowsText = new wxStaticText(&dialog, wxID_ANY, "0");
    wxSlider* highlightsSlider = new wxSlider(&dialog, wxID_ANY, 0, -100, 100, wxDefaultPosition, wxSize(300, -1));
    wxStaticText* highlightsText = new wxStaticText(&dialog, wxID_ANY, "0");
    wxButton* okButton = new wxButton(&dialog, wxID_OK, _("OK"));
    wxButton* cancelButton = new wxButton(&dialog, wxID_CANCEL, _("Cancel"));
    
    // ربط حدث تغيير المنزلق
    shadowsSlider->Bind(wxEVT_SLIDER, [shadowsSlider, shadowsText](wxCommandEvent&) {
        shadowsText->SetLabel(wxString::Format("%d", shadowsSlider->GetValue()));
    });
    
    highlightsSlider->Bind(wxEVT_SLIDER, [highlightsSlider, highlightsText](wxCommandEvent&) {
        highlightsText->SetLabel(wxString::Format("%d", highlightsSlider->GetValue()));
    });
    
    // إنشاء السايزر
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Adjust shadows:")), 0, wxALL, 5);
    mainSizer->Add(shadowsSlider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(shadowsText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticText(&dialog, wxID_ANY, _("Adjust highlights:")), 0, wxALL, 5);
    mainSizer->Add(highlightsSlider, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(highlightsText, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(new wxStaticLine(&dialog, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    // تعيين السايزر
    dialog.SetSizer(mainSizer);
    mainSizer->Fit(&dialog);
    
    // عرض مربع الحوار
    if (dialog.ShowModal() == wxID_OK) {
        // تطبيق الظلال/الإضاءات
        m_imageProcessor->adjustShadowsHighlights(shadowsSlider->GetValue(), highlightsSlider->GetValue());
    }
    
    // تعيين التركيز على لوحة الرسم
    m_canvasPanel->SetFocus();
}

void MainFrame::onReset(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إعادة تعيين الصورة
    m_imageProcessor->reset();
}

void MainFrame::onZoomIn(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تكبير الصورة
    m_canvasPanel->zoomIn();
}

void MainFrame::onZoomOut(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تصغير الصورة
    m_canvasPanel->zoomOut();
}

void MainFrame::onZoomFit(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // ملاءمة الصورة
    m_canvasPanel->zoomFit();
}

void MainFrame::onZoomReset(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // إعادة تعيين التكبير/التصغير
    m_canvasPanel->zoomReset();
}


void MainFrame::onToggleGrid(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تبديل عرض الشبكة
    m_canvasPanel->toggleGrid();
}

void MainFrame::onToggleInfo(wxCommandEvent& event)
{
    // التحقق من وجود صورة
    if (!m_imageProcessor->hasImage()) {
        return;
    }
    
    // تبديل عرض المعلومات
    m_canvasPanel->toggleInfo();
}
