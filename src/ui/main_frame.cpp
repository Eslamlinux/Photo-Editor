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
