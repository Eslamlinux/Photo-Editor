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
    Bind(wxEVT_MENU, &MainWindow::onSave, this, wxID_SAVE);
    Bind(wxEVT_MENU, &MainWindow::onSaveAs, this, wxID_SAVEAS);
    Bind(wxEVT_MENU, &MainWindow::onImportImage, this, ID_IMPORT_IMAGE);
    Bind(wxEVT_MENU, &MainWindow::onExportImage, this, ID_EXPORT_IMAGE);
    Bind(wxEVT_MENU, &MainWindow::onExit, this, wxID_EXIT);
    
    Bind(wxEVT_MENU, &MainWindow::onUndo, this, wxID_UNDO);
    Bind(wxEVT_MENU, &MainWindow::onRedo, this, wxID_REDO);
    
    Bind(wxEVT_MENU, &MainWindow::onToggleDarkMode, this, ID_TOGGLE_DARK_MODE);
    Bind(wxEVT_MENU, &MainWindow::onRemoveBackground, this, ID_REMOVE_BACKGROUND);
    Bind(wxEVT_MENU, &MainWindow::onAbout, this, wxID_ABOUT);
}

void MainWindow::createToolBar() {
    wxToolBar* toolBar = CreateToolBar(wxTB_HORIZONTAL | wxTB_FLAT | wxTB_TEXT);
    
    toolBar->AddTool(wxID_NEW, _("New"), wxArtProvider::GetBitmap(wxART_NEW), _("New Project"));
    toolBar->AddTool(wxID_OPEN, _("Open"), wxArtProvider::GetBitmap(wxART_FILE_OPEN), _("Open Project"));
    toolBar->AddTool(wxID_SAVE, _("Save"), wxArtProvider::GetBitmap(wxART_FILE_SAVE), _("Save Project"));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_UNDO, _("Undo"), wxArtProvider::GetBitmap(wxART_UNDO), _("Undo"));
    toolBar->AddTool(wxID_REDO, _("Redo"), wxArtProvider::GetBitmap(wxART_REDO), _("Redo"));
    toolBar->AddSeparator();
    toolBar->AddTool(ID_CROP, _("Crop"), wxArtProvider::GetBitmap(wxART_CUT), _("Crop Image"));
    toolBar->AddTool(ID_ROTATE, _("Rotate"), wxArtProvider::GetBitmap(wxART_REDO), _("Rotate Image"));
    toolBar->AddSeparator();
    toolBar->AddTool(ID_REMOVE_BACKGROUND, _("Remove BG"), wxArtProvider::GetBitmap(wxART_DELETE), _("Remove Background"));
    
    toolBar->Realize();
}

void MainWindow::createStatusBar() {
    m_statusBar = new StatusBar(this);
    SetStatusBar(m_statusBar);
}

oid MainWindow::createPanels() {
    // Create canvas panel (central area)
    m_canvasPanel = new CanvasPanel(this, m_imageProcessor.get());
    
    // Create tools panel
    m_toolsPanel = new ToolsPanel(this, m_imageProcessor.get());
    
    // Create project browser panel
    m_projectBrowser = new ProjectBrowser(this, m_projectRepository.get());
    
    // Create layers panel
    m_layersPanel = new LayersPanel(this);
   // Create properties panel
    m_propertiesPanel = new PropertiesPanel(this);
    
    // Add panels to AUI manager
    m_auiManager.AddPane(m_canvasPanel, wxAuiPaneInfo()
        .Name("canvas")
        .CenterPane()
        .Caption(_("Canvas"))
        .CloseButton(false)
        .MaximizeButton(true));
    
    m_auiManager.AddPane(m_toolsPanel, wxAuiPaneInfo()
        .Name("tools")
        .Caption(_("Tools"))
        .Left()
        .BestSize(wxSize(200, 600))
        .CloseButton(true)
        .MaximizeButton(false));

  
    m_auiManager.AddPane(m_projectBrowser, wxAuiPaneInfo()
        .Name("projects")
        .Caption(_("Projects"))
        .Left()
        .BestSize(wxSize(200, 300))
        .CloseButton(true)
        .MaximizeButton(false));
    
    m_auiManager.AddPane(m_layersPanel, wxAuiPaneInfo()
        .Name("layers")
        .Caption(_("Layers"))
        .Right()
        .BestSize(wxSize(200, 300))
        .CloseButton(true)
        .MaximizeButton(false));

  
    m_auiManager.AddPane(m_propertiesPanel, wxAuiPaneInfo()
        .Name("properties")
        .Caption(_("Properties"))
        .Right()
        .BestSize(wxSize(200, 300))
        .CloseButton(true)
        .MaximizeButton(false));
}

void MainWindow::applyTheme() {
    wxColour bgColor = m_darkMode ? wxColour(40, 40, 40) : wxColour(240, 240, 240);
    wxColour fgColor = m_darkMode ? wxColour(220, 220, 220) : wxColour(20, 20, 20);
    
    SetBackgroundColour(bgColor);
    SetForegroundColour(fgColor);

 
    // Update menu checkmark
    wxMenuBar* menuBar = GetMenuBar();
    if (menuBar) {
        wxMenu* viewMenu = menuBar->GetMenu(menuBar->FindMenu(_("View")));
        if (viewMenu) {
            viewMenu->Check(ID_TOGGLE_DARK_MODE, m_darkMode);
        }
    }
    
    // Notify all panels about theme change
    wxCommandEvent event(wxEVT_THEME_CHANGED);
    event.SetInt(m_darkMode ? 1 : 0);
    ProcessWindowEvent(event);
    
    Refresh();
}

void MainWindow::onNew(wxCommandEvent& event) {
    // Check for unsaved changes
    if (hasUnsavedChanges() && !confirmDiscardChanges()) {
        return;
    }


 
    // Create new project dialog
    NewProjectDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK) {
        // Get project settings
        int width = dialog.GetWidth();
        int height = dialog.GetHeight();
        wxString name = dialog.GetProjectName();
        
        // Create new project
        core::db::Project project;
        project.name = name.ToStdString();
        project.width = width;
        project.height = height;
        
  // Create new project dialog
    NewProjectDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK) {
        // Get project settings
        int width = dialog.GetWidth();
        int height = dialog.GetHeight();
        wxString name = dialog.GetProjectName();
        
        // Create new project
        core::db::Project project;
        project.name = name.ToStdString();
        project.width = width;
        project.height = height;
        
  int projectId = m_projectRepository->createProject(project);
        if (projectId > 0) {
            // Create blank image
            cv::Mat blankImage(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
            m_imageProcessor->setImage(blankImage);
            
            // Update UI
            m_canvasPanel->updateCanvas();
            m_statusBar->setStatusText(wxString::Format(_("New project created: %s (%dx%d)"), 
                                                      name, width, height));
            
            // Save initial state
            m_currentProjectId = projectId;
            m_currentProjectName = name;
            m_hasUnsavedChanges = false;
            updateTitle();
        }
    }
}

void MainWindow::onOpen(wxCommandEvent& event) {
    // Check for unsaved changes
    if (hasUnsavedChanges() && !confirmDiscardChanges()) {
        return;
    }

 
    // Show project browser in open mode
    m_projectBrowser->showOpenDialog();
}

void MainWindow::onSave(wxCommandEvent& event) {
    if (m_currentProjectId <= 0) {
        // No current project, use Save As instead
        onSaveAs(event);
        return;
    }
    
 
    // Save current project
    saveCurrentProject();
}

void MainWindow::onSaveAs(wxCommandEvent& event) {
    // Show save as dialog
    wxString name = wxGetTextFromUser(
        _("Enter project name:"),
        _("Save Project As"),
        m_currentProjectName,
        this
    );
    
 if (!name.IsEmpty()) {
        // Create new project with current image
        core::db::Project project;
        project.name = name.ToStdString();
        
        cv::Size imageSize = m_imageProcessor->getImageSize();
        project.width = imageSize.width;
        project.height = imageSize.height;
        
 // Create thumbnail
        cv::Mat thumbnail;
        cv::resize(m_imageProcessor->getImage(), thumbnail, cv::Size(200, 200 * imageSize.height / imageSize.width));
        std::vector<uchar> thumbnailData;
        cv::imencode(".jpg", thumbnail, thumbnailData);
        project.thumbnail = thumbnailData;
        
    int projectId = m_projectRepository->createProject(project);
        if (projectId > 0) {
            // Update current project info
            m_currentProjectId = projectId;
            m_currentProjectName = name;
            m_hasUnsavedChanges = false;
            updateTitle();
  
           
            // Save all layers
            saveCurrentProject();
            
            m_statusBar->setStatusText(wxString::Format(_("Project saved as: %s"), name));
        }
    }
}
void MainWindow::onImportImage(wxCommandEvent& event) {
    wxFileDialog openDialog(
        this, 
        _("Import Image"), 
        "", 
        "",
        _("Image files (*.jpg;*.jpeg;*.png;*.webp)|*.jpg;*.jpeg;*.png;*.webp|All files (*.*)|*.*"),
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );

    if (openDialog.ShowModal() == wxID_OK) {
        wxString path = openDialog.GetPath();
        
        // Load image using OpenCV
        cv::Mat image = cv::imread(path.ToStdString());
        if (!image.empty()) {
            // Check for unsaved changes
            if (hasUnsavedChanges() && !confirmDiscardChanges()) {
                return;
            }
            
            // Set image to processor
            m_imageProcessor->setImage(image);
            
            // Update UI
            m_canvasPanel->updateCanvas();
            
            // Update status
            wxFileName fileName(path);
            m_statusBar->setStatusText(wxString::Format(_("Imported image: %s"), fileName.GetFullName()));
            
            // Mark as unsaved changes
            m_hasUnsavedChanges = true;
            updateTitle();

       } else {
            wxMessageBox(_("Failed to load image."), _("Error"), wxICON_ERROR | wxOK, this);
        }
    }
}

void MainWindow::onExportImage(wxCommandEvent& event) {
    if (m_imageProcessor->getImage().empty()) {
        wxMessageBox(_("No image to export."), _("Error"), wxICON_ERROR | wxOK, this);
        return;
    }
    
    wxFileDialog saveDialog(
        this, 
        _("Export Image"), 
        "", 
        "",
        _("JPEG files (*.jpg)|*.jpg|PNG files (*.png)|*.png|WebP files (*.webp)|*.webp"),
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

  
    if (saveDialog.ShowModal() == wxID_OK) {
        wxString path = saveDialog.GetPath();
        
        // Determine format from extension
        wxFileName fileName(path);
        wxString ext = fileName.GetExt().Lower();
        
        std::string format;
        if (ext == "jpg" || ext == "jpeg") {
            format = "jpg";
        } else if (ext == "png") {
            format = "png";
        } else if (ext == "webp") {
            format = "webp";
        } else {
            // Default to jpg
            format = "jpg";
            path += ".jpg";
        }
      // Save image using OpenCV
        if (m_imageProcessor->saveImage(path.ToStdString(), format)) {
            m_statusBar->setStatusText(wxString::Format(_("Image exported to: %s"), fileName.GetFullName()));
        } else {
            wxMessageBox(_("Failed to export image."), _("Error"), wxICON_ERROR | wxOK, this);
        }
    }
}

void MainWindow::onExit(wxCommandEvent& event) {
    Close();
}

void MainWindow::onClose(wxCloseEvent& event) {
    // Check for unsaved changes
    if (hasUnsavedChanges() && !confirmDiscardChanges()) {
        if (event.CanVeto()) {
            event.Veto();
            return;
        }
    }
    
    // Destroy the frame
    Destroy();
}

void MainWindow::onUndo(wxCommandEvent& event) {
    if (m_imageProcessor->undoLastOperation()) {
        m_canvasPanel->updateCanvas();
        m_statusBar->setStatusText(_("Undo"));
        m_hasUnsavedChanges = true;
        updateTitle();
    }
}

void MainWindow::onRedo(wxCommandEvent& event) {
    if (m_imageProcessor->redoOperation()) {
        m_canvasPanel->updateCanvas();
        m_statusBar->setStatusText(_("Redo"));
        m_hasUnsavedChanges = true;
        updateTitle();
    }
}


void MainWindow::onToggleDarkMode(wxCommandEvent& event) {
    m_darkMode = !m_darkMode;
    applyTheme();
}
void MainWindow::onRemoveBackground(wxCommandEvent& event) {
    if (m_imageProcessor->getImage().empty()) {
        wxMessageBox(_("No image loaded."), _("Error"), wxICON_ERROR | wxOK, this);
        return;
    }
   // Show progress dialog
    wxProgressDialog progressDialog(
        _("Removing Background"),
        _("Processing image with AI model..."),
        100,
        this,
        wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_SMOOTH
    );
    
    progressDialog.Update(10);
    
    // Process in a separate thread to avoid UI freezing
    std::thread bgThread([this, &progressDialog]() {
        try {
            // Remove background
            bool success = m_imageProcessor->removeBackground();
            
            // Update UI in main thread
            wxTheApp->CallAfter([this, success, &progressDialog]() {
                progressDialog.Update(100);
                
  
                
                if (success) {
                    m_canvasPanel->updateCanvas();
                    m_statusBar->setStatusText(_("Background removed"));
                    m_hasUnsavedChanges = true;
                    updateTitle();
                } else {
                    wxMessageBox(_("Failed to remove background."), _("Error"), wxICON_ERROR | wxOK, this);
                }
            });
        } catch (const std::exception& e) {
            wxTheApp->CallAfter([this, e, &progressDialog]() {
                progressDialog.Update(100);
                wxMessageBox(
                    wxString::Format(_("Error: %s"), e.what()),
                    _("Background Removal Failed"),
                    wxICON_ERROR | wxOK,
                    this
                );
            });
        }
    });
   
    // Detach thread to let it run independently
    bgThread.detach();
}

void MainWindow::onAbout(wxCommandEvent& event) {
    wxAboutDialogInfo info;
    info.SetName(_("PhotoMagicEditor"));
    info.SetVersion(_("1.0"));
    info.SetDescription(_("A professional photo editing software with AI capabilities"));
    info.SetCopyright(_("(C) 2023"));
    info.SetWebSite(_("https://photomagiceditor.example.com"));
    info.AddDeveloper(_("Your Name"));
    
    wxAboutBox(info, this);
}


bool MainWindow::hasUnsavedChanges() const {
    return m_hasUnsavedChanges;
}

bool MainWindow::confirmDiscardChanges() {
    wxMessageDialog dialog(
        this,
        _("You have unsaved changes. Do you want to save them?"),
        _("Unsaved Changes"),
        wxYES_NO | wxCANCEL | wxICON_QUESTION
    );
    
    int result = dialog.ShowModal();
    
    if (result == wxID_YES) {
        // Save changes
        wxCommandEvent dummyEvent;
        onSave(dummyEvent);
        return !m_hasUnsavedChanges; // Return true if save was successful
    } else if (result == wxID_NO) {
        // Discard changes
        return true;
    } else {
        // Cancel
        return false;
    }
}

