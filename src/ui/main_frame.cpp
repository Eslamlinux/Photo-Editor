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
    
