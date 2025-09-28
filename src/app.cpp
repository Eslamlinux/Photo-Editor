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
    
