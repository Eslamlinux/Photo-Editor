#include "main_window.h"
#include "canvas_panel.h"
#include "tools_panel.h"
#include "project_browser.h"
#include "status_bar.h"
#include <wx/aui/aui.h>
#include <wx/artprov.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/aboutdlg.h>

namespace pme {
namespace ui {

MainWindow::MainWindow(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1280, 800)),
      m_auiManager(this),
      m_darkMode(false) {
    
    // Initialize core components
    m_imageProcessor = std::make_unique<core::ImageProcessor>();
    m_projectRepository = std::make_unique<core::db::ProjectRepository>("photomagic.db");
    
    // Create UI components
    createMenuBar();
    createToolBar();
    createStatusBar();
    createPanels();
    
