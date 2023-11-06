/*
Ci-dessous, ce sont des directives de préprocesseur.
Ces lignes permettent d'ajouter des fichiers au projet,
fichiers que l'on appelle bibliothèques.
Grâce à ces bibliothèques, on disposera de fonctions toutes prêtes pour afficher
par exemple un message à l'écran.
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <SFML/Graphics.h>
#include <time.h>

#define bool  _Bool
#define false 0
#define true  1


bool gameover = 0;

int Size = 20;
const int width = 500;
const int height = 400;   // Taille de la carte (500 de largeur (w) et 400 hauteur (h)
int num = 3;        // Nombre de cases composant le corps du serpent de base
int dir = 1;        // Direction droite au début du serpent
int score = 0;
int speed = 200;

struct Snake {      // Tableau du snake
	int x, y;
};

struct Fruit {      // Tableau du fruit
	int x, y;
};

struct Snake s[1000];   // Maximum de la taille du snake
struct Fruit f;


void Die(sfRenderWindow* app,sfText* ScoreText, sfFont* font, sfText* GameOver, sfText*Exit){

 if (gameover){
     // Texte correspondant au Score
     sfText_setFont(ScoreText, font);
     sfText_setCharacterSize(ScoreText, 30);


     char scoreNb[20];
     sprintf(scoreNb, "Score: %d", score);

     sfText_setString(ScoreText, scoreNb);
     sfText_setPosition(ScoreText, (sfVector2f) { 200, 300 });
     sfRenderWindow_drawText(app, ScoreText, NULL);


     // Texte GAME OVER !!!
     sfText_setFont(GameOver, font);
     sfText_setCharacterSize(GameOver, 50);


     sfText_setString(GameOver, "Game Over");
     sfText_setPosition(GameOver, (sfVector2f) { 110, 150 });
     sfRenderWindow_drawText(app, GameOver, NULL);

     // Texte Press echap for exit game
     sfText_setFont(Exit, font);
     sfText_setCharacterSize(Exit, 20);


     sfText_setString(Exit, "Press == Echap == for exit game");
     sfText_setPosition(Exit, (sfVector2f) { 100, 200 });
     sfRenderWindow_drawText(app, Exit, NULL);

     if (sfKeyboard_isKeyPressed(sfKeyEscape))
         sfWindow_close(app);
 }

}

void Action() {

    // déplace la position de chaque partie du corps du snake vers la position de la partie précédente
	for (int i = num; i > 0; i--) {
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;

	}

	// Valeur de déplacement du snake
	if (dir == 0) s[0].y --;
	if (dir == 1) s[0].x ++;
	if (dir == 2) s[0].y ++;
	if (dir == 3) s[0].x --;

        // toucher le fruit = création d'un nouveau fruit de manière aléatoire sur la map + taille du snake +1
        if (s[0].x == f.x && s[0].y == f.y) {
            num++;
            f.x = rand() % Size;
            f.y = rand() % Size;
            score ++;
            speed -= 5;
        }
       // printf("%d", speed);
    for (int i=0; i<num; i++)
    {
        if (s[1+i].x == f.x && s[1+i].y == f.y){
            f.x = rand() % Size;
            f.y = rand() % Size;
        }
    }

    // Toucher le cadre = game over
    if (s[0].x < 0 || s[0].x >= Size + 5 || s[0].y < 0 || s[0].y >= Size ) {
        printf("Game Over!\n");
        gameover = 1;
    }
        else {
            gameover = 0;
        }

    // si le snake touche son corps = game over
    for (int i = 1; i < num; i++) {
        if (s[0].x == s[i].x && s[0].y == s[i].y) {
            printf("Game Over!\n");
            gameover = 1;
        }
    }

}

int main() {

    // Crée l'écran de jeu
    sfVideoMode mode = { width, height, 32 };
	sfRenderWindow* app = sfRenderWindow_create(mode, "Serpent Game", sfTitlebar, NULL);
	sfRenderWindow_setFramerateLimit(app, 60);

	// Création du snake / type / taille / couleur
	sfRectangleShape* snake = sfRectangleShape_create();
	sfVector2f size = { Size, Size };
	sfRectangleShape_setSize(snake, size);
	sfColor green = sfGreen;
	sfRectangleShape_setFillColor(snake, green);

	// Création du fruit / type / taille / couleur
	sfCircleShape* fruit = sfCircleShape_create();
	sfCircleShape_setRadius(fruit, Size / 2);
	sfColor red = sfRed;
	sfCircleShape_setFillColor(fruit, red);

	// Création du score
    sfFont* font = sfFont_createFromFile("arial.ttf");
    sfText* ScoreText = sfText_create();

    //Création du Game Over
    sfText* GameOver = sfText_create();

    //Création du Press echap for exit
    sfText* Exit = sfText_create();


	sfClock* timer = sfClock_create();

	f.x = rand() % Size;
	f.y = rand() % Size;

	// Permet de fermer la fenêtre du jeu
	while (sfRenderWindow_isOpen(app)) {
		sfEvent event;
		while (sfRenderWindow_pollEvent(app, &event)) {
			if (event.type == sfEvtClosed) {
				sfRenderWindow_close(app);
			}
		}
        sfRenderWindow_clear(app, sfBlack);

        // Vitesse du jeu (valeur proche de zéro = plus rapide)
		if (sfTime_asMilliseconds(sfClock_getElapsedTime(timer)) > speed) {

			if (!gameover){
                Action();

            }

			sfClock_restart(timer);
		}

		// Déplacement mais sans pouvoir faire un retour dans le sens inverse
		if (sfKeyboard_isKeyPressed(sfKeyUp)&& dir != 2) dir = 0;
		if (sfKeyboard_isKeyPressed(sfKeyRight)&& dir != 3) dir = 1;
		if (sfKeyboard_isKeyPressed(sfKeyDown)&& dir != 0) dir = 2;
		if (sfKeyboard_isKeyPressed(sfKeyLeft)&& dir != 1) dir = 3;

		//sfRenderWindow_clear(app, sfBlack);


		Die(app, ScoreText, font, GameOver, Exit);

		// Donner la position + création du snake
		for (int i = 0; i < num; i++) {
			sfRectangleShape_setPosition(snake, (sfVector2f) { s[i].x* Size, s[i].y* Size });
			sfRenderWindow_drawShape(app, snake, NULL);
		}

		// Donner la position + création du fruit
		sfCircleShape_setPosition(fruit, (sfVector2f) { f.x* Size, f.y* Size });
		sfRenderWindow_drawShape(app, fruit, NULL);


		sfRenderWindow_display(app);
	}
    // Détruit tout les éléments ci dessous / fenêtre / snake / fruit / temps
	sfRenderWindow_destroy(app);
	sfRectangleShape_destroy(snake);
	sfCircleShape_destroy(fruit);
    sfText_destroy(ScoreText);
    sfText_destroy(GameOver);
    sfText_destroy(Exit);
	sfClock_destroy(timer);

	return 0;
}
  /*  TEST PRINTF / SCANF


  int resultat = 0, nombre1 = 0, nombre2 = 0;

  // On demande les nombres 1 et 2 à l'utilisateur :

  printf("Entrez le nombre 1 : ");
  scanf("%d", &nombre1);
  printf("Entrez le nombre 2 : ");
  scanf("%d", &nombre2);

  // On fait le calcul :

  resultat = nombre1 + nombre2;

  // Et on affiche l'addition à l'écran :

  printf ("%d + %d = %d\n", nombre1, nombre2, resultat);


  ----------------------------------------------------
   */


  /* TEST IF / ELSE / ELSEIF

  int main()

    int age = 2;

    if (age >= 18 && age < 25) // Si l'âge est supérieur ou égal à 18
{
    printf ("Vous etes majeur !");

    }
        else if ( age > 4 ) // Sinon, si l'âge est au moins supérieur à 4
        {
             printf ("Bon t'es pas trop jeune quand meme...");
        }
            else // Sinon...
            {
                printf ("Ah c'est bete, vous etes mineur !");
            }

  ------------------------------------------------------------------------
*/

/* TEST SWITCH //// Apprendre le switch
    int main()
    int choixMenu;
{
    printf (" === Menu ===\n ");
    printf("1. Royal Cheese\n");
    printf("2. Mc Deluxe\n");
    printf("3. Mc Bacon\n");
    printf("4. Big Mac\n");
    printf ("\nVotre choix ? ");

    scanf("%d", &choixMenu);

    switch (choixMenu)
    {
        case 1:
            printf("Vous avez choisi le Royal Cheese\n");
            break;
        case 2:
            printf("Vous avez choisi le Mc Deluxe\n");
            break;
        case 3:
            printf("Vous avez choisi le Mc Bacon\n");
            break;
         case 4:
            printf("Vous avez choisi le Big Mac\n");
            break;
         default:
            printf("Votre demande est erronée\n");
            break;
    }
    printf(&choixMenu);
    printf("\n");
}
    -------------------------------------------------------------
    */


    /* TEST BOUCLE WHILE / DO...WHILE / FOR

    */

/* TEST 1

  int main(int argc, char *argv[])
{
    int nombreEntre = 0;
    int compteur = 0;

    while (nombreEntre != 47)
{
    printf("Tapez le nombre 47 ! ");
    scanf("%d", &nombreEntre);
}
  return 0;
} */




/*  TEST 2 WHILE
 int main()
{
    int compteur = 0;

    while (compteur != 10)    // != différent ou égal
    {
        printf("%d\n", compteur);
        compteur++;
    }
    printf("%d\n", compteur); // Permet de print aussi la valeur 10
  return 0;
}
*/


/* TEST BOUCLE DO ... WHILE
    int main() {

    int compteur = 0;

do
{
    printf("Bienvenue sur OpenClassrooms !\n");
    compteur++;
} while (compteur < 10);

    return 0;
    }

    La boucle while peut très bien ne jamais être exécutée si la condition
    est fausse dès le départ.
    Par exemple, si on avait initialisé le compteur à 50, la condition
    aurait été fausse dès le début, et on ne serait jamais rentré dans la boucle.

    Pour la boucle do… while, c'est différent :
    cette boucle s'exécutera toujours au moins une fois.
    En effet, le test se fait à la fin, comme vous pouvez le voir.
    Si on initialise compteur à 50, la boucle s'exécutera une fois.

    */

    /*   TEST BOUCLE FOR
    int main() {

    for (int compteur = 0; compteur <= 10; compteur ++)
    {
        printf("%d\n", compteur);
    }
    return 0;
    }   */


    /*
    -----------------------------------------------------------

    TEST SUR LES FONCTIONS

    int menu()
{
    int choix = 0;

    while (choix < 1 || choix > 4)
    {
        printf("Menu :\n");
        printf("1 : Poulet de dinde aux escargots rotis a la sauce bearnaise\n");
        printf("2 : Concombres sucres a la sauce de myrtilles enrobee de chocolat\n");
        printf("3 : Escalope de kangourou saignante et sa gelee aux fraises poivree\n");
        printf("4 : La surprise du Chef (j'en salive d'avance...)\n");
        printf("Votre choix ? ");
        scanf("%d", &choix);
    }

    return choix;
}

int main(int argc, char *argv[])
{
    switch (menu())
    {
        case 1:
            printf("Vous avez pris le poulet\n");
            break;
        case 2:
            printf("Vous avez pris les concombres\n");
            break;
        case 3:
            printf("Vous avez pris l'escalope\n");
            break;
        case 4:
            printf("Vous avez pris la surprise du Chef. Vous etes un sacre aventurier dites donc !\n");
            break;
    }

    return 0;
}*/

/*
--------------------------------

TEST des prototype pour les functions


// La ligne suivante est le prototype de la fonction aireRectangle :

double aireRectangle(double largeur, double hauteur);

int main(int argc, char *argv[])
{
    printf("Rectangle de largeur 5 et hauteur 10. Aire = %f\n", aireRectangle(5, 10));
    printf("Rectangle de largeur 2.5 et hauteur 3.5. Aire = %f\n", aireRectangle(2.5, 3.5));
    printf("Rectangle de largeur 4.2 et hauteur 9.7. Aire = %f\n", aireRectangle(4.2, 9.7));

    return 0;
}

// Notre fonction aireRectangle peut maintenant être mise n'importe où dans le code source :
double aireRectangle(double largeur, double hauteur)
{
    return largeur * hauteur;
} */

/*
----------------------------------------------------

EXERCICE DE NOMBRE MYSTERE


int main ( int argc, char** argv )

{

    int nombreMystere = 0, nombreEntre = 0;

    const int MAX = 100, MIN = 1;


    // Génération du nombre aléatoire


    srand(time(NULL));

    nombreMystere = (rand() % (MAX - MIN + 1)) + MIN;


    La boucle du programme. Elle se répète tant que l'utilisateur n'a pas trouvé le nombre mystère


    do

    {

        // On demande le nombre

        printf("Quel est le nombre ? ");

        scanf("%d", &nombreEntre);


        // On compare le nombre entré avec le nombre mystère


        if (nombreMystere > nombreEntre)

            printf("C'est plus !\n\n");

        else if (nombreMystere < nombreEntre)

            printf("C'est moins !\n\n");

        else

            printf ("Bravo, vous avez trouve le nombre mystere !!!\n\n");

    } while (nombreEntre != nombreMystere);



    return 0;

}

*/

