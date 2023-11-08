#include "window.h"

#include <stdio.h>
#include <stdlib.h>

void InitWindow(sfRenderWindow** window){

    int windowWidth = sfVideoMode_getDesktopMode().width / 2;
    int windowHeight = sfVideoMode_getDesktopMode().height / 2;

    //printf("%d, %d", windowWidth, windowHeight);

    sfVideoMode mode = {windowWidth, windowHeight, 32};
    *window = sfRenderWindow_create(mode, "Asteroids", sfResize | sfClose, NULL);

    if (!*window){

        exit(EXIT_FAILURE);
    }

}

void DestroyWindow(sfRenderWindow* window){

    sfRenderWindow_destroy(window);

}

