#include "EngineCore.h"

const char * title = "Example 01";

int main (void)
{
    EngineCore app;
    app.setWindowDimensions(1024, 768);
    app.setWindowTitle(title);
    app.run();
    return 0;
}