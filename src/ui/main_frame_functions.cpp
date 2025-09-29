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

