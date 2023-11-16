#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.h>
#include <time.h>
#include <math.h>


#define bool  _Bool
#define false 0
#define true  1


bool gameover = 0;


int Size = 50;

float delta;

int score = 0;
int speed = 1;
int basteroidsnb = 5;
int bulletnb = 7;

struct Ship {      // Tableau du vaisseau
    float x;
    float y;
    float angle;
    float vit;
    sfVector2f str;
    sfVector2f dir;
};

struct BigAsteroids {      // Tableau des asteroides
    float x;
    float y;
    float angle;
    float vit;
    sfVector2f dir;
    sfSprite* shape;  // Ajout du sfCircleShape pour chaque astéroïde
};

struct Bullet {      // Tableau des balles du vaisseau
    float x;
    float y;
    float angle;
    float vit;
    sfVector2f dir;
    sfSprite* bullet;  // Ajout de plusieurs pour les balles
    _Bool active;
};


struct Ship s;
struct BigAsteroids ba[5];
struct Bullet b [7];


/*void Menu(sfRenderWindow* window) {

    // Création du bouton "Play"
    sfRectangleShape* playButton = sfRectangleShape_create();
    sfVector2f playButtonSize = {200, 50};
    sfRectangleShape_setSize(playButton, playButtonSize);
    sfRectangleShape_setFillColor(playButton, sfGreen);
    sfVector2f playButtonPosition = {400, 300};
    sfRectangleShape_setPosition(playButton, playButtonPosition);

    // Création du bouton "Quitter"
    sfRectangleShape* quitButton = sfRectangleShape_create();
    sfVector2f quitButtonSize = {200, 50};
    sfRectangleShape_setSize(quitButton, quitButtonSize);
    sfRectangleShape_setFillColor(quitButton, sfRed);
    sfVector2f quitButtonPosition = {400, 500};
    sfRectangleShape_setPosition(quitButton, quitButtonPosition);


    // Dessiner les boutons
    sfRenderWindow_drawRectangleShape(window, playButton, NULL);
    sfRenderWindow_drawRectangleShape(window, quitButton, NULL);



    // Libérer les ressources
    sfRectangleShape_destroy(playButton);
    sfRectangleShape_destroy(quitButton);
}*/



void Die(sfRenderWindow* window, /*sfText* ScoreText*/ sfFont* font, sfText* GameOver, sfText* Exit) {

    if (gameover) {
/*        // Texte correspondant au Score
        sfText_setFont(ScoreText, font);
        sfText_setCharacterSize(ScoreText, 30);


        char scoreNb[200000];
        sprintf(scoreNb, "Score: %d", score);

        sfText_setString(ScoreText, scoreNb);
        sfText_setPosition(ScoreText, (sfVector2f) { 20, 20 });
        sfRenderWindow_drawText(window, ScoreText, NULL);
*/

        // Texte GAME OVER !!!
        sfText_setFont(GameOver, font);
        sfText_setCharacterSize(GameOver, 50);


        sfText_setString(GameOver, "Game Over");
        sfText_setPosition(GameOver, (sfVector2f) { 500, 320 });
        sfRenderWindow_drawText(window, GameOver, NULL);

        // Texte Press echap for exit game
        sfText_setFont(Exit, font);
        sfText_setCharacterSize(Exit, 20);


        sfText_setString(Exit, "Press == Echap == for exit game");
        sfText_setPosition(Exit, (sfVector2f) { 480, 400 });
        sfRenderWindow_drawText(window, Exit, NULL);

        if (sfKeyboard_isKeyPressed(sfKeyEscape))
            sfRenderWindow_close(window);
    }

}

void BigAsteroids() {

    int windowWidth = sfVideoMode_getDesktopMode().width / 2;
    int windowHeight = sfVideoMode_getDesktopMode().height / 2;

    // Création asteroids / type / taille / texture
    sfSprite* bigasteroids = sfSprite_create();
    sfSprite_setScale(bigasteroids,(sfVector2f){0.5f, 0.5f});

    sfTexture* astero = sfTexture_createFromFile("caillou.png", NULL);
    sfSprite_setTexture (bigasteroids, astero,1);
    sfSprite_setOrigin(bigasteroids, (sfVector2f) {315 / 2, 250 / 2});

    for (int i = 0; i < basteroidsnb; ++i) {
        ba[i].x = rand() % windowWidth;
        ba[i].y = rand() % windowHeight;
        ba[i].angle = (float)(rand() % 360);
        ba[i].shape = sfSprite_create();
        sfSprite_setScale(ba[i].shape,(sfVector2f){0.5f, 0.5f});
        sfSprite_setTexture(ba[i].shape, astero, 1);
        sfSprite_setOrigin(ba[i].shape, (sfVector2f){315 / 2, 250 / 2});
    }

    for (int i = 0; i < basteroidsnb; ++i) {

        ba[i].vit = 0.3;
        ba[i].dir.x = cosf(ba[i].angle * 3.14 / 180);
        ba[i].dir.y = sinf(ba[i].angle * 3.14 / 180);
    }


}


void InitBullet(){

    int windowWidth = sfVideoMode_getDesktopMode().width / 2;
    int windowHeight = sfVideoMode_getDesktopMode().height / 2;

     for (int i = 0; i < 7; ++i) {
        b[i].active = false;        // Initialiser le sprite de la balle
    }
    // Création de la balle
    sfSprite* ammo = sfSprite_create();
    sfSprite_setScale(ammo,(sfVector2f){0.1f, 0.1f});

    sfTexture* bullet = sfTexture_createFromFile("bullet.png", NULL);
    sfSprite_setTexture (ammo, bullet,1);
    sfSprite_setOrigin(ammo, (sfVector2f) {s.x, s.y});

    for (int i = 0; i < bulletnb; ++i) {
        b[i].x = s.x;
        b[i].y = s.y;
        b[i].angle = (float)(rand() % 360);
        b[i].bullet = sfSprite_create();
        sfSprite_setScale(b[i].bullet,(sfVector2f){0.05f, 0.05f});
        sfSprite_setTexture(b[i].bullet, bullet, 1);
        sfSprite_setOrigin(b[i].bullet, (sfVector2f){s.x, s.y});
    }

    for (int i = 0; i < bulletnb; ++i) {

        b[i].vit = 0.3;
        b[i].dir.x = cosf(b[i].angle * 3.14 / 180);
        b[i].dir.y = sinf(b[i].angle * 3.14 / 180);
    }
}

void Action() {

    int windowWidth = sfVideoMode_getDesktopMode().width / 2;
    int windowHeight = sfVideoMode_getDesktopMode().height / 2;


    // wraparound vaisseau
    if (s.y < - 10) {s.y = windowHeight + 10;}
    else if (s.y >= windowHeight + 10) {s.y = -10;}

    if (s.x < - 10) {s.x = windowWidth + 10;}
    else if (s.x >= windowWidth + 10) { s.x = -10;}


    // wraparound asteroides
    for (int i = 0; i < basteroidsnb; ++i) {

        if (ba[i].y < -90) {ba[i].y = windowHeight +90;
        }
        else if (ba[i].y >= windowHeight + 90) {ba[i].y = - 90;
        }

        if (ba[i].x < -90) {ba[i].x = windowWidth + 90;
        }
        else if (ba[i].x >= windowWidth + 90) {ba[i].x = -90;
        }

        //deplacement ast
        ba[i].x += ba[i].vit * ba[i].dir.x;
        ba[i].y += ba[i].vit * ba[i].dir.y;
    }

    for (int i = 0; i < basteroidsnb; ++i) {
        float distance_x = ba[i].x - s.x;
        float distance_y = ba[i].y - s.y;
        float distance = sqrt(distance_x * distance_x + distance_y * distance_y);
        if (distance < 40 + 25) {
            s.y = 0;
            s.x = 0;
            printf("mort");
            gameover = 1;
        }

        for (int i = 0; i < bulletnb; ++i) {
            if (b[i].active){
        //deplacement Bullet
        b[i].x += b[i].vit * b[i].dir.x;
        b[i].y += b[i].vit * b[i].dir.y;
            }
        }

    }
}

int Delta(sfClock* deltaclock){


sfTime dtime = sfClock_getElapsedTime(deltaclock);
delta =sfTime_asMilliseconds(dtime);
sfClock_restart(deltaclock);

return 0;;
};





int main() {

    s.vit = 0.009   ;
    s.angle = -90;  // - 90 car sa permet d'effectuer une poussée vers l'avant
    s.x = 640;
    s.y = 340;

    s.dir = (sfVector2f){0, 0};

    _Bool CheckSpace = 0;

    sfClock* deltaclock = sfClock_create();

    int windowWidth = sfVideoMode_getDesktopMode().width / 2;
    int windowHeight = sfVideoMode_getDesktopMode().height / 2;

    sfRenderWindow* window = NULL;


    InitWindow(&window);

    // Création du vaisseau / type / taille / texture
    sfCircleShape* ship = sfCircleShape_create();
    sfCircleShape_setRadius(ship, Size / 2);


    sfTexture* vaisseau = sfTexture_createFromFile("vaiss.png", NULL);
    sfCircleShape_setTexture (ship, vaisseau,1);
    sfCircleShape_setOrigin(ship, (sfVector2f) {25,25});

    // Création du score
    sfFont* font = sfFont_createFromFile("arial.ttf");
   // sfText* ScoreText = sfText_create();

    //Création du Game Over
    sfText* GameOver = sfText_create();

    //Création du Press echap for exit
    sfText* Exit = sfText_create();



    BigAsteroids();
    InitBullet();
    Delta(deltaclock);

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
            Die(window, /*ScoreText*/ font, GameOver, Exit);

            }

            sfClock_restart(timer);
        }

        // Assignement des touches pour les déplacements du joueur
        if (sfKeyboard_isKeyPressed(sfKeyUp)) {
                s.dir.x = cosf(s.angle * 3.14 / 180); // PI = 3.14 / par 180 pour passer en radians
                s.dir.y = sinf(s.angle * 3.14 / 180);

                s.vit = 1.0f;

        }
        else {
                s.vit *= 0.999f;

        }

        s.x +=  delta / 1000 * s.vit / 10 * s.dir.x;
        s.y += delta / 1000 * s.vit / 10 * s.dir.y;


    if (sfKeyboard_isKeyPressed(sfKeyRight)) s.angle += 0.1 ;
    if (sfKeyboard_isKeyPressed(sfKeyLeft)) s.angle-= 0.1 ;


    if (sfKeyboard_isKeyPressed(sfKeySpace)) {
        if (!CheckSpace){
            for (int i = 0; i < 7; ++i) {
            if (!b[i].active) {
                b[i].x = s.x;
                b[i].y = s.y;
                b[i].angle = s.angle;
                b[i].dir.x = cosf(b[i].angle * 3.14 / 180);
                    b[i].dir.y = sinf(b[i].angle * 3.14 / 180);
                    b[i].vit = 0.5;
                    b[i].active = true;
                break;
                }
            }
        }
        CheckSpace = 1;
    }
    else {

        CheckSpace = 0;
    }

        //sfRenderWindow_clear(app, sfBlack);


        Die(window, /*ScoreText*/ font, GameOver, Exit);

        // Donner la position + création du vaisseau

        sfCircleShape_setPosition(ship, (sfVector2f) { s.x, s.y});
        sfCircleShape_setRotation(ship, s.angle +90 );

        for (int i = 0; i < basteroidsnb; ++i) {
    sfSprite_setPosition(ba[i].shape, (sfVector2f){ba[i].x, ba[i].y});
    sfSprite_setRotation(ba[i].shape, ba[i].angle + 90);
    sfRenderWindow_drawShape(window, ba[i].shape, NULL);
        }

        for (int i = 0; i < bulletnb; ++i) {
    sfSprite_setPosition(b[i].bullet, (sfVector2f){b[i].x, b[i].y});
    sfSprite_setRotation(b[i].bullet, b[i].angle + 90);
    sfRenderWindow_drawShape(window, b[i].bullet, NULL);
        }



        sfRenderWindow_drawShape(window, ship, NULL);

        sfRenderWindow_display(window);
    }
    // Détruit tout les éléments ci dessous / fenêtre / ship / temps / et les textes

    for (int i = 0; i < basteroidsnb; ++i) {
    sfSprite_destroy(ba[i].shape);
    sfTexture_destroy(sfSprite_getTexture(ba[i].shape));
    }

    for (int i = 0; i < bulletnb; ++i) {
    sfSprite_destroy(b[i].bullet);
    sfTexture_destroy(sfSprite_getTexture(b[i].bullet));
    }

    DestroyWindow(window);
    sfCircleShape_destroy(ship);
    sfTexture_destroy(vaisseau);
   // sfText_destroy(ScoreText);
    sfText_destroy(GameOver);
    sfText_destroy(Exit);
    sfClock_destroy(timer);
    sfClock_destroy(deltaclock);

    return 0;

}

