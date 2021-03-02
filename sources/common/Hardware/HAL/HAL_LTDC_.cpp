#include "defines.h"
#include "common/Display/Colors_.h"
#include "common/Display/Painter_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "Settings/Settings.h"
#include <stm32f4xx_hal.h>


static LTDC_HandleTypeDef handleLTDC;
void *HAL_LTDC::handle = &handleLTDC;
static uint frontBuffer = 0;
static uint backBuffer = 0;


void HAL_LTDC::Init(uint8 *front, uint8 *back)
{
    handleLTDC.Instance = LTDC; //-V2571
    handleLTDC.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    handleLTDC.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    handleLTDC.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    handleLTDC.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    handleLTDC.Init.HorizontalSync = 0;
    handleLTDC.Init.VerticalSync = 0;
    handleLTDC.Init.AccumulatedHBP = 70;
    handleLTDC.Init.AccumulatedVBP = 13;
    handleLTDC.Init.AccumulatedActiveW = 390;
    handleLTDC.Init.AccumulatedActiveH = 253;
    handleLTDC.Init.TotalWidth = 408;
    handleLTDC.Init.TotalHeigh = 263;
    handleLTDC.Init.Backcolor.Blue = 0;
    handleLTDC.Init.Backcolor.Green = 0;
    handleLTDC.Init.Backcolor.Red = 0;

    if (HAL_LTDC_Init(&handleLTDC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    SetBuffers(front, back);

    LoadPalette();
}


void HAL_LTDC::LoadPalette()
{
    static uint colors[16] =
    {
        Color::Make(0x00, 0x00, 0x00),   // Color::BLACK = 0x00,
        Color::Make(0xFF, 0xFF, 0xFF),   // Color::WHITE = 0x01,
        Color::Make(0x80, 0x80, 0x80),   // Color::GRID = 0x02,
        Color::Make(0x01, 0xCA, 0xFF),   // Color::DATA_A = 0x03,
        Color::Make(0x00, 0xFF, 0x00),   // COLOR_DATA_B = 0x04,
        Color::Make(0xD5, 0xDA, 0xD5),   // Color::MENU_FIELD = 0x05,
        Color::Make(0xFF, 0xB2, 0x00),   // COLOR_MENU_TITLE = 0x06,
        Color::Make(0x7B, 0x59, 0x00),   // Color::MENU_TITLE_DARK = 0x07,
        Color::Make(0xFF, 0xFF, 0x00),   // Color::MENU_TITLE_BRIGHT = 0x08,
        Color::Make(0x69, 0x89, 0x00),   // COLOR_MENU_ITEM = 0x09,
        Color::Make(0x6B, 0x45, 0x00),   // Color::MENU_ITEM_DARK = 0x0a,
        Color::Make(0xFF, 0xCE, 0x00),   // Color::MENU_ITEM_BRIGHT = 0x0b,
        Color::Make(0x00, 0x00, 0x00),   // Color::MENU_SHADOW = 0x0c,
        Color::Make(0x00, 0x00, 0x00),   // COLOR_EMPTY = 0x0d,
        Color::Make(0x08, 0xA2, 0xCE),   // COLOR_EMPTY_A = 0x0e,
        Color::Make(0x00, 0xCE, 0x00)    // COLOR_EMPTY_B = 0x0f,
    };

    if (HAL_LTDC_ConfigCLUT(&handleLTDC, colors, 16, 0) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_LTDC_EnableCLUT(&handleLTDC, 0) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


void HAL_LTDC::SetBuffers(uint8 *front, uint8 *back)
{
    frontBuffer = reinterpret_cast<uint>(front); //-V2571
    backBuffer = reinterpret_cast<uint>(back); //-V2571

    LTDC_LayerCfgTypeDef pLayerCfg;

    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = 320;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = 240;
    pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_L8;
    pLayerCfg.Alpha = 0xFF;
    pLayerCfg.Alpha0 = 0xFF;
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
    pLayerCfg.FBStartAdress = frontBuffer;
    pLayerCfg.ImageWidth = 320;
    pLayerCfg.ImageHeight = 240;
    pLayerCfg.Backcolor.Blue = 0;
    pLayerCfg.Backcolor.Green = 0;
    pLayerCfg.Backcolor.Red = 0;

    if (HAL_LTDC_ConfigLayer(&handleLTDC, &pLayerCfg, 0) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


void HAL_LTDC::ToggleBuffers()
{
    DMA2D_HandleTypeDef hDMA2D;

    hDMA2D.Init.Mode = DMA2D_M2M;
    hDMA2D.Init.ColorMode = LTDC_PIXEL_FORMAT_L8;
    hDMA2D.Init.OutputOffset = 0;

    hDMA2D.XferCpltCallback = nullptr;

    hDMA2D.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hDMA2D.LayerCfg[1].InputAlpha = 0xFF;
    hDMA2D.LayerCfg[1].InputColorMode = LTDC_PIXEL_FORMAT_L8;
    hDMA2D.LayerCfg[1].InputOffset = 0;

    hDMA2D.Instance = DMA2D; //-V2571

    if (HAL_DMA2D_Init(&hDMA2D) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hDMA2D, 1) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hDMA2D, backBuffer, frontBuffer, 160, 240) == HAL_OK)
            {
                if (HAL_DMA2D_PollForTransfer(&hDMA2D, 100) != HAL_OK)
                {
                    uint error_code = hDMA2D.ErrorCode;

                    error_code = error_code;

                    ERROR_HANDLER();
                }
            }
            
            frontBuffer = frontBuffer;
        }
        else
        {
            ERROR_HANDLER();
        }
    }
    else
    {
        ERROR_HANDLER();
    }
}
