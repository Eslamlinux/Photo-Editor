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
    
    // Set up AUI manager
    m_auiManager.SetFlags(wxAUI_MGR_DEFAULT | wxAUI_MGR_LIVE_RESIZE);
    
    // Set icon
    SetIcon(wxArtProvider::GetIcon(wxART_FRAME_ICON));
    
    // Bind events
    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::onClose, this);
    
    // Apply initial theme
    applyTheme();
    
    // Set up AUI manager
    m_auiManager.Update();
    
    // Center on screen
    Centre();
}

MainWindow::~MainWindow() {
    m_auiManager.UnInit();
}

void MainWindow::createMenuBar() {
    wxMenuBar* menuBar = new wxMenuBar();
    
    // File menu
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_NEW, _("&New Project\tCtrl+N"));
    fileMenu->Append(wxID_OPEN, _("&Open Project\tCtrl+O"));
    fileMenu->Append(wxID_SAVE, _("&Save Project\tCtrl+S"));
    fileMenu->Append(wxID_SAVEAS, _("Save Project &As...\tCtrl+Shift+S"));
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_IMPORT_IMAGE, _("&Import Image...\tCtrl+I"));
    fileMenu->Append(ID_EXPORT_IMAGE, _("&Export Image...\tCtrl+E"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, _("E&xit\tAlt+F4"));
    
    // Edit menu
    wxMenu* editMenu = new wxMenu();
    editMenu->Append(wxID_UNDO, _("&Undo\tCtrl+Z"));
    editMenu->Append(wxID_REDO, _("&Redo\tCtrl+Y"));
    editMenu->AppendSeparator();
    editMenu->Append(wxID_CUT, _("Cu&t\tCtrl+X"));
    editMenu->Append(wxID_COPY, _("&Copy\tCtrl+C"));
    editMenu->Append(wxID_PASTE, _("&Paste\tCtrl+V"));
    editMenu->AppendSeparator();
    editMenu->Append(ID_SELECT_ALL, _("Select &All\tCtrl+A"));
    editMenu->Append(ID_DESELECT, _("&Deselect\tCtrl+D"));
    
    // View menu
    wxMenu* viewMenu = new wxMenu();
    viewMenu->Append(ID_ZOOM_IN, _("Zoom &In\tCtrl++"));
    viewMenu->Append(ID_ZOOM_OUT, _("Zoom &Out\tCtrl+-"));
    viewMenu->Append(ID_ZOOM_ACTUAL, _("Actual &Size\tCtrl+0"));
    viewMenu->Append(ID_ZOOM_FIT, _("&Fit to Window\tCtrl+F"));
    viewMenu->AppendSeparator();
    viewMenu->AppendCheckItem(ID_TOGGLE_DARK_MODE, _("&Dark Mode"));
    viewMenu->AppendSeparator();
    viewMenu->AppendCheckItem(ID_SHOW_TOOLS, _("Show &Tools Panel"));
    viewMenu->AppendCheckItem(ID_SHOW_LAYERS, _("Show &Layers Panel"));
    viewMenu->AppendCheckItem(ID_SHOW_PROPERTIES, _("Show &Properties Panel"));

    // Image menu
    wxMenu* imageMenu = new wxMenu();
    imageMenu->Append(ID_CROP, _("&Crop"));
    imageMenu->Append(ID_ROTATE, _("&Rotate..."));
    imageMenu->Append(ID_RESIZE, _("Re&size..."));
    imageMenu->AppendSeparator();
    imageMenu->Append(ID_ADJUST_BRIGHTNESS, _("Adjust &Brightness..."));
    imageMenu->Append(ID_ADJUST_CONTRAST, _("Adjust &Contrast..."));
    imageMenu->Append(ID_ADJUST_SATURATION, _("Adjust &Saturation..."));
    imageMenu->AppendSeparator();
    imageMenu->Append(ID_REMOVE_BACKGROUND, _("Remove &Background"));

    // Help menu
    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_HELP, _("&Help Contents\tF1"));
    helpMenu->Append(wxID_ABOUT, _("&About PhotoMagicEditor"));

    // Add menus to menu bar
    menuBar->Append(fileMenu, _("&File"));
    menuBar->Append(editMenu, _("&Edit"));
    menuBar->Append(viewMenu, _("&View"));
    menuBar->Append(imageMenu, _("&Image"));
    menuBar->Append(helpMenu, _("&Help"));



  // Set menu bar
    SetMenuBar(menuBar);
    
    // Bind menu events
    Bind(wxEVT_MENU, &MainWindow::onNew, this, wxID_NEW);
    Bind(wxEVT_MENU, &MainWindow::onOpen, this, wxID_OPEN);
      this, wxID_NEW);
    Bind(wxEVT_MENU, &MainWindow::onOpen, this, wxID_OPEN);
