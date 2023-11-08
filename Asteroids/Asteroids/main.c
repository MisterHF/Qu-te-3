#include <stdio.h>
#include <stdlib.h>

#include <SFML/Graphics.h>

#include "window.h"

void GameUpdate(sfRenderWindow* window){

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }

        // Game update logic
        if (sfKeyboard_isKeyPressed(sfKeyEscape)) { sfRenderWindow_close(window); } // quit
            //UpdateLogic();

        // Clear screen
        sfRenderWindow_clear(window, sfTransparent);

        // Draw your elements
            //UpdateDraw();

        // Display screen
        sfRenderWindow_display(window);
    }

}


void Destroy(sfRenderWindow* window){

    DestroyWindow(window);


}

int main() {


    sfRenderWindow* window = NULL;

    InitWindow(&window);
    GameUpdate(window);
    Destroy(window);


    return 0;
}
