#include "MiniFb.h"

void MiniFb::SetBufferColor(int r = 255, int g = 255, int b = 255)
{
    for (int i = 0; i < height * width - 1; i++)
    {
        buffer[i] = MFB_RGB(r, g, b);
    }
}

void MiniFb::Init(int _height, int _width)
{
    height = _height;
    width = _width;
    buffer = (uint32_t*)malloc(_height * _width * 4);

    SetBufferColor();
}

void MiniFb::Run()
{
    struct mfb_window* window = mfb_open_ex("my display", height, width, MFB_WF_RESIZABLE);
    if (window == NULL)
        return;

    mfb_update_state state;
    do {

        state = mfb_update_ex(window, buffer, height, width);

        if (state != MFB_STATE_OK)
            break;

    } while (mfb_wait_sync(window));

    free(buffer);
    buffer = NULL;
    window = NULL;

    return;
}
