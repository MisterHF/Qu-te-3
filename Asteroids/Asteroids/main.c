#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.h>
#include <time.h>

#define bool  _Bool
#define false 0
#define true  1


bool gameover = 0;


int Size = 50;

int score = 0;
int speed = 1;


struct Ship {      // Tableau du vaisseau
    float x;
    float y;
    float angle;
    float vit;
    sfVector2f dir;
};


struct Ship s;



/*void Die(sfRenderWindow* window, sfText* ScoreText, sfFont* font, sfText* GameOver, sfText* Exit) {

    if (gameover) {
        // Texte correspondant au Score
        sfText_setFont(ScoreText, font);
        sfText_setCharacterSize(ScoreText, 30);


        char scoreNb[20];
        sprintf(scoreNb, "Score: %d", score);

        sfText_setString(ScoreText, scoreNb);
        sfText_setPosition(ScoreText, (sfVector2f) { 200, 300 });
        sfRenderWindow_drawText(window, ScoreText, NULL);


        // Texte GAME OVER !!!
        sfText_setFont(GameOver, font);
        sfText_setCharacterSize(GameOver, 50);


        sfText_setString(GameOver, "Game Over");
        sfText_setPosition(GameOver, (sfVector2f) { 110, 150 });
        sfRenderWindow_drawText(window, GameOver, NULL);

        // Texte Press echap for exit game
        sfText_setFont(Exit, font);
        sfText_setCharacterSize(Exit, 20);


        sfText_setString(Exit, "Press == Echap == for exit game");
        sfText_setPosition(Exit, (sfVector2f) { 100, 200 });
        sfRenderWindow_drawText(window, Exit, NULL);

        if (sfKeyboard_isKeyPressed(sfKeyEscape))
            sfRenderWindow_close(window);
    }

}*/

void Action() {

    int windowWidth = sfVideoMode_getDesktopMode().width / 2;
    int windowHeight = sfVideoMode_getDesktopMode().height / 2;


    // wraparound
    if (s.y < - 100) {s.y = windowHeight - 1;}
    else if (s.y >= windowHeight) {s.y = 100;}

    if (s.x < - 100) {s.x = windowWidth - 1;}
    else if (s.x >= windowWidth) { s.x = -100;}


}

int main() {

    s.vit = 0.5;
    s.angle = -90;  // - 90 car sa permet d'effectuer une poussée vers l'avant
    s.x = 640;
    s.y = 340;

    s.dir = (sfVector2f){0, 0};

    int windowWidth = sfVideoMode_getDesktopMode().width / 2;
    int windowHeight = sfVideoMode_getDesktopMode().height / 2;

    sfRenderWindow* window = NULL;

    InitWindow(&window);


    // Création du vaisseau / type / taille / texture
    sfCircleShape* ship = sfCircleShape_create();
    sfCircleShape_setRadius(ship, Size / 2);


    sfTexture* vaisseau = sfTexture_createFromFile("ship.png", NULL);
    sfCircleShape_setTexture (ship, vaisseau,1);
    sfCircleShape_setOrigin(ship, (sfVector2f) {25,25});

/*
    // Création du score
    sfFont* font = sfFont_createFromFile("arial.ttf");
    sfText* ScoreText = sfText_create();

    //Création du Game Over
    sfText* GameOver = sfText_create();

    //Création du Press echap for exit
    sfText* Exit = sfText_create();
*/
    // Création d'un timer
    sfClock* timer = sfClock_create();



    // Permet de fermer la fenêtre du jeu
    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }
        sfRenderWindow_clear(window, sfBlack);

        // Vitesse du jeu (valeur proche de zéro = plus rapide)
        if (sfTime_asMilliseconds(sfClock_getElapsedTime(timer)) > speed) {

            if (!gameover) {
                Action();

            }

            sfClock_restart(timer);
        }

        // Assignement des touches pour les déplacements du joueur
        if (sfKeyboard_isKeyPressed(sfKeyUp)) {
                s.dir.x = cosf(s.angle * 3.14 / 180); // PI = 3.14 / par 180 pour passer en radians
                s.dir.y = sinf(s.angle * 3.14 / 180);
                s.x +=s.vit* s.dir.x;
                s.y +=s.vit* s.dir.y;

        }
        if (sfKeyboard_isKeyPressed(sfKeyRight)) s.angle += 0.1 ;
        if (sfKeyboard_isKeyPressed(sfKeyLeft)) s.angle-= 0.1 ;

        //sfRenderWindow_clear(app, sfBlack);


    //    Die(window, ScoreText, font, GameOver, Exit);

        // Donner la position + création du vaisseau

        sfCircleShape_setPosition(ship, (sfVector2f) { s.x, s.y});
        sfCircleShape_setRotation(ship, s.angle +90 );

        sfRenderWindow_drawShape(window, ship, NULL);


        sfRenderWindow_display(window);
    }
    // Détruit tout les éléments ci dessous / fenêtre / ship / temps / et les textes
    DestroyWindow(window);
    sfCircleShape_destroy(ship);
    sfTexture_destroy(vaisseau);
   // sfText_destroy(ScoreText);
   // sfText_destroy(GameOver);
  //  sfText_destroy(Exit);
    sfClock_destroy(timer);

    return 0;
}

