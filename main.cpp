#include "ui/main_frame.h"
#include <wx/app.h>
#include <wx/intl.h>
#include <wx/translation.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/log.h>

class PhotoEditorApp : public wxApp {
public:
    virtual bool OnInit() {
        // Initialize locale
        m_locale = new wxLocale(wxLANGUAGE_DEFAULT);
        
        // Add catalog lookup paths
        wxString exePath = wxStandardPaths::Get().GetExecutablePath();
        wxString localeDir = wxFileName(exePath).GetPath() + wxFileName::GetPathSeparator() + "locale";
        
        // Add locale directory
        wxLocale::AddCatalogLookupPathPrefix(localeDir);
        
        // Add catalog
        m_locale->AddCatalog("photoeditor");
        
        // Create main frame
        pme::ui::MainFrame* frame = new pme::ui::MainFrame(_("محرر الصور"));
        frame->Show(true);
        
        return true;
    }
    
    virtual int OnExit() {
        // Clean up locale
        delete m_locale;
        
        return wxApp::OnExit();
    }
    
private:
    wxLocale* m_locale;
};

wxIMPLEMENT_APP(PhotoEditorApp);
