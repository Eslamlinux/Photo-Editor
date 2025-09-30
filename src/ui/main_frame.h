#ifndef PME_UI_MAIN_FRAME_H
#define PME_UI_MAIN_FRAME_H

#include <wx/frame.h>
#include <wx/string.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>

namespace pme {

namespace core {
class ImageProcessor;
}

namespace ui {

class CanvasPanel;
class CropTool;

// معرفات القوائم
enum {
    ID_CROP = wxID_HIGHEST + 1,
    ID_ROTATE_90_CW,
    ID_ROTATE_90_CCW,
    ID_ROTATE_180,
    ID_FLIP_HORIZONTAL,
    ID_FLIP_VERTICAL,
    ID_GRAYSCALE,
    ID_SEPIA,
    ID_NEGATIVE,
    ID_BLUR,
    ID_SHARPEN,
    ID_EDGE_DETECTION,
    ID_EMBOSS,
    ID_CARTOON,
    ID_WATERCOLOR,
    ID_MOSAIC,
    ID_BRIGHTNESS,
    ID_CONTRAST,
    ID_SATURATION,
    ID_HUE,
    ID_GAMMA,
    ID_TEMPERATURE,
    ID_SHADOWS_HIGHLIGHTS,
    ID_RESET,
    ID_ZOOM_IN,
    ID_ZOOM_OUT,
    ID_ZOOM_FIT,
    ID_ZOOM_RESET,
    ID_TOGGLE_GRID,
    ID_TOGGLE_INFO,
    ID_LANGUAGE_ENGLISH,
    ID_LANGUAGE_ARABIC
};

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);
    virtual ~MainFrame();
    
    bool OpenFile(const wxString& filePath);
    
private:
    void createMenuBar();
    void createToolBar();
    void updateMenuItems();
    void updateTitle();
    
    // معالجات الأحداث
    void onOpen(wxCommandEvent& event);
    void onSave(wxCommandEvent& event);
    void onSaveAs(wxCommandEvent& event);
    void onExit(wxCommandEvent& event);
    void onClose(wxCloseEvent& event);
    void onUndo(wxCommandEvent& event);
    void onRedo(wxCommandEvent& event);
    void onAbout(wxCommandEvent& event);
    void onCrop(wxCommandEvent& event);
    void onRotate90CW(wxCommandEvent& event);
    void onRotate90CCW(wxCommandEvent& event);
    void onRotate180(wxCommandEvent& event);
    void onFlipHorizontal(wxCommandEvent& event);
    void onFlipVertical(wxCommandEvent& event);
    void onGrayscale(wxCommandEvent& event);
    void onSepia(wxCommandEvent& event);
    void onNegative(wxCommandEvent& event);
    void onBlur(wxCommandEvent& event);
    void onSharpen(wxCommandEvent& event);
    void onEdgeDetection(wxCommandEvent& event);
    void onEmboss(wxCommandEvent& event);
    void onCartoon(wxCommandEvent& event);
    void onWatercolor(wxCommandEvent& event);
    void onMosaic(wxCommandEvent& event);
    void onBrightness(wxCommandEvent& event);
    void onContrast(wxCommandEvent& event);
    void onSaturation(wxCommandEvent& event);
    void onHue(wxCommandEvent& event);
    void onGamma(wxCommandEvent& event);
    void onTemperature(wxCommandEvent& event);
    void onShadowsHighlights(wxCommandEvent& event);
    void onReset(wxCommandEvent& event);
    void onZoomIn(wxCommandEvent& event);
    void onZoomOut(wxCommandEvent& event);
    void onZoomFit(wxCommandEvent& event);
    void onZoomReset(wxCommandEvent& event);
    void onToggleGrid(wxCommandEvent& event);
    void onToggleInfo(wxCommandEvent& event);
    void onLanguageEnglish(wxCommandEvent& event);
    void onLanguageArabic(wxCommandEvent& event);
    void onToolApply(wxCommandEvent& event);
    void onToolCancel(wxCommandEvent& event);
    void onResize(wxCommandEvent& event);
    void onOilPainting(wxCommandEvent& event);
    void onPencilSketch(wxCommandEvent& event);
    void onAutoWhiteBalance(wxCommandEvent& event);
    void onAutoContrast(wxCommandEvent& event);
    void onAutoColorEnhance(wxCommandEvent& event);
    void onReduceNoise(wxCommandEvent& event);
    void onAdaptiveSharpen(wxCommandEvent& event);
    void onCropToAspectRatio(wxCommandEvent& event);
    void onAddBorder(wxCommandEvent& event);
    void onAddVignette(wxCommandEvent& event);
    void onAddWatermark(wxCommandEvent& event);
    void onAddText(wxCommandEvent& event);
    
    core::ImageProcessor* m_imageProcessor;
    CanvasPanel* m_canvasPanel;
    CropTool* m_cropTool;
    wxString m_currentFilePath;
    bool m_isModified;
    wxMenuBar* m_menuBar;
    wxToolBar* m_toolBar;
    wxStatusBar* m_statusBar;
    
    DECLARE_EVENT_TABLE()
};

} // namespace ui
} // namespace pme

#endif // PME_UI_MAIN_FRAME_H
