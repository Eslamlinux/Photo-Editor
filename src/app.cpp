#include "ui/main_frame.h"
#include <wx/app.h>
#include <wx/image.h>
#include <wx/cmdline.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/log.h>
#include <wx/translation.h>
#include <wx/splash.h>
#include <wx/sysopt.h>
#include <wx/msgdlg.h>
#include <wx/config.h>
#include <wx/dcmemory.h>  // إضافة هذا الرأس لـ wxMemoryDC
#include <wx/brush.h>     // إضافة هذا الرأس لـ wxBrush

class PhotoEditorApp : public wxApp {
public:
    virtual bool OnInit() override;
    virtual int OnExit() override;
    virtual void OnInitCmdLine(wxCmdLineParser& parser) override;
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser) override;
    virtual bool OnExceptionInMainLoop() override;
    virtual void OnUnhandledException() override;
    virtual void OnFatalException() override;
    
private:
    bool initializeLocale();
    bool initializeConfig();
    void showSplashScreen();
    
    wxString m_filePath;
    wxLocale* m_locale;
    wxConfigBase* m_config;
};

wxIMPLEMENT_APP(PhotoEditorApp);

bool PhotoEditorApp::OnInit() {
    // تعيين اسم التطبيق
    SetAppName("PhotoEditor");
    
    // تهيئة خيارات النظام
    wxSystemOptions::SetOption("msw.remap", 0);
    
    // تهيئة معالجات الصور
    wxInitAllImageHandlers();
    
    // تهيئة التدويل
    if (!initializeLocale()) {
        wxLogWarning("Failed to initialize locale.");
    }
    
    // تهيئة التكوين
    if (!initializeConfig()) {
        wxLogWarning("Failed to initialize configuration.");
    }
    
    // عرض شاشة البداية
    showSplashScreen();
    
    // إنشاء الإطار الرئيسي
    pme::ui::MainFrame* frame = new pme::ui::MainFrame("Photo Editor");
    frame->Show();
    
    // فتح الملف إذا تم تحديده
    if (!m_filePath.empty()) {
        frame->OpenFile(m_filePath);
    }
    
    return true;
}

int PhotoEditorApp::OnExit() {
    // تنظيف الموارد
    if (m_locale) {
        delete m_locale;
        m_locale = nullptr;
    }
    
    if (m_config) {
        delete m_config;
        m_config = nullptr;
    }
    
    return wxApp::OnExit();
}

void PhotoEditorApp::OnInitCmdLine(wxCmdLineParser& parser) {
    wxApp::OnInitCmdLine(parser);
    
    parser.AddParam("image file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
}

bool PhotoEditorApp::OnCmdLineParsed(wxCmdLineParser& parser) {
    wxApp::OnCmdLineParsed(parser);
    
    if (parser.GetParamCount() > 0) {
        m_filePath = parser.GetParam(0);
    }
    
    return true;
}

bool PhotoEditorApp::OnExceptionInMainLoop() {
    try {
        throw;
    } catch (const std::exception& e) {
        wxLogError("Unhandled exception: %s", e.what());
    } catch (...) {
        wxLogError("Unhandled exception of unknown type");
    }
    
    return false;
}

void PhotoEditorApp::OnUnhandledException() {
    wxLogError("Unhandled exception in PhotoEditorApp::OnUnhandledException");
}

void PhotoEditorApp::OnFatalException() {
    wxLogError("Fatal exception in PhotoEditorApp::OnFatalException");
}

bool PhotoEditorApp::initializeLocale() {
    // تهيئة التدويل
    m_locale = new wxLocale(wxLANGUAGE_DEFAULT);
    
    // إضافة مسار الترجمة
    wxString localeDir = wxStandardPaths::Get().GetResourcesDir() + wxFILE_SEP_PATH + "locale";
    if (wxDir::Exists(localeDir)) {
        m_locale->AddCatalogLookupPathPrefix(localeDir);
    }
    
    // إضافة كتالوج الترجمة
    m_locale->AddCatalog("photo_editor");
    
    return true;
}

bool PhotoEditorApp::initializeConfig() {
    // إنشاء تكوين
    m_config = wxConfigBase::Create();
    
    // تعيين التكوين الافتراضي
    wxConfigBase::Set(m_config);
    
    return true;
}

void PhotoEditorApp::showSplashScreen() {
    // تخطي شاشة البداية في الإصدار الحالي لتجنب مشاكل الرسم
    return;
    
    /* تم تعطيل هذا الكود مؤقتًا لتجنب مشاكل الرسم
    // إنشاء صورة شاشة البداية
    wxBitmap bitmap;
    
    // محاولة تحميل صورة شاشة البداية
    wxString splashPath = wxStandardPaths::Get().GetResourcesDir() + wxFILE_SEP_PATH + "splash.png";
    if (wxFileExists(splashPath)) {
        bitmap.LoadFile(splashPath, wxBITMAP_TYPE_PNG);
    } else {
        // إنشاء صورة شاشة بداية افتراضية
        bitmap = wxBitmap(400, 300);
        wxMemoryDC dc(bitmap);
        dc.SetBackground(wxBrush(wxColour(64, 64, 64)));
        dc.Clear();
        
        // رسم النص
        dc.SetTextForeground(wxColour(255, 255, 255));
        dc.SetFont(wxFont(24, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        
        wxString appName = "Photo Editor";
        wxSize textSize = dc.GetTextExtent(appName);
        dc.DrawText(appName, (400 - textSize.GetWidth()) / 2, 100);
        
        dc.SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        wxString version = "Version 1.0";
        textSize = dc.GetTextExtent(version);
        dc.DrawText(version, (400 - textSize.GetWidth()) / 2, 150);
        
        dc.SelectObject(wxNullBitmap);
    }
    
    // عرض شاشة البداية
    wxSplashScreen* splash = new wxSplashScreen(bitmap,
                                              wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
                                              3000, NULL, -1, wxDefaultPosition, wxDefaultSize,
                                              wxBORDER_SIMPLE | wxSTAY_ON_TOP);
    
    // تحديث النافذة
    wxYield();
    */
}
