#include <SDL2/SDL.h>
#include <unistd.h>
#include "function.h"

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 920
#define FPS 60
#define RAYON_BALL 10
#define LONGUEUR_PADDLE 150
#define HAUTEUR_PADDLE 30
#define NB_BRIQUE 7
#define NB_LIGNE 6


#define PREMIERS_PIX 300
#define ECART_HORIZ_BRIQUE 10
#define ECART_VERTI_BRIQUE 10

/* Variables de la balle */
int x_ball = 350;
int y_ball = 800;
double speed_ball_x = 2;
double speed_ball_y = 2;
int red_ball = 13;
int green_ball = 98;
int blue_ball = 23;

/* Variables du paddle */
int x_paddle = 275;
int y_paddle = 810;
int m_paddle = LONGUEUR_PADDLE / 2; 
int red_paddle = 170;
int green_paddle = 170;
int blue_paddle = 170;

/* Variables des briques */
int briques[NB_LIGNE][NB_BRIQUE];
int hauteur_brique = ((PREMIERS_PIX / NB_LIGNE)- ECART_VERTI_BRIQUE);
int longueur_brique = (((WINDOW_WIDTH - ECART_HORIZ_BRIQUE)/ NB_BRIQUE) - ECART_HORIZ_BRIQUE);
int red1_bricks = 127;
int green1_bricks = 9;
int blue1_bricks = 9;
int red2_bricks = 255;
int green2_bricks = 197;
int blue2_bricks = 0;

/** Appel des fonctions **/
int initBricks();
int drawBall();
int drawPaddle();
int drawBricks();
int bounceBricks();
int bouncePaddle();
int bounceWall();
int moveBall();

void init_game()
{
	initBricks();
}

void drawGame()
{
    clear();
    drawBall();
    drawBricks();
    drawPaddle();
    actualize();
    moveBall();
    usleep(1000000 / FPS); // 60 images par seconde | 1000000 = 1 seconde
}

void KeyPressed(SDL_Keycode touche)
{
    /** @brief event.key.keysym.sym renvoi la touche appuyé
     *
     */
    switch (touche) {
        case SDLK_q:
            printf("Q\n");
            if (x_paddle >= 0) {
            x_paddle = x_paddle - 10;
		}
            break;
        case SDLK_d:
            printf("D\n");
            if (x_paddle <= WINDOW_WIDTH - LONGUEUR_PADDLE) {
            x_paddle = x_paddle + 10;
		}
            break;
        case SDLK_ESCAPE:
            freeAndTerminate();
            break;
        default:
            break;
    }
}

void joyButtonPressed(){
}

void gameLoop() {
    int programLaunched = 1;
    while (programLaunched == 1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                /** event.type renvoi le type d'evenement qui se passe
                 * (si on appuie sur une touche, clique de souris...)
                 * en fonction du type d'évènement on à alors
                 * différentes information accessibles
                 * voir doc pour plus d'event https://wiki.libsdl.org/SDL_EventType
                 */
                case SDL_QUIT:
                    programLaunched = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    printf("position de la souris x : %d , y : %d\n", event.motion.x, event.motion.y);
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);
                    break;
                case SDL_JOYBUTTONDOWN:
                    break;
                default:
                    break;
            }
        }
        drawGame();
    }
}

int main()
{	
    init(WINDOW_WIDTH, WINDOW_HEIGHT);
    init_game();
    gameLoop();
    printf("Fin du programme\n");
    freeAndTerminate();
}

/* Fonctions de la balle */

int drawBall() 
{
	changeColor(red_ball, green_ball, blue_ball);
	drawCircle(x_ball, y_ball, RAYON_BALL);
}

int bounceWall()
{
	if ((x_ball <= (0 + RAYON_BALL)) || (x_ball >= (- RAYON_BALL + WINDOW_WIDTH))) {
		speed_ball_x = - speed_ball_x;
	}
	
	if ((y_ball <= (0 + RAYON_BALL)) || (y_ball >= (- RAYON_BALL + WINDOW_HEIGHT))) {
		speed_ball_y = - speed_ball_y;
	}
}

int bouncePaddle() // Rajouter un if pour faire rebondir suivant la partie où tape la balle (m_paddle)
{	
	if ((y_ball >= y_paddle - RAYON_BALL) && (y_paddle <= y_paddle - RAYON_BALL + HAUTEUR_PADDLE) 
	&& (x_ball >= x_paddle + RAYON_BALL) && (x_ball <= x_paddle + LONGUEUR_PADDLE + RAYON_BALL)) {
		if ((x_ball >= x_paddle + RAYON_BALL) && (x_ball <= x_paddle + RAYON_BALL + m_paddle)) {
			if (speed_ball_x > 0) {
				speed_ball_x = - speed_ball_x;
			}
		} 
		if ((x_ball >= x_paddle + RAYON_BALL + m_paddle) && (x_ball <= x_paddle + RAYON_BALL + LONGUEUR_PADDLE)) {
			if  (speed_ball_x < 0) {
				speed_ball_x = - speed_ball_x;
			}
		}
		speed_ball_y = - speed_ball_y; 
	}
}

int bounceBricks()
{
	int i, j;
	int x_bricks = ECART_HORIZ_BRIQUE;
	int y_bricks = ECART_VERTI_BRIQUE;
	
	for (i = 0; i < NB_LIGNE; i++) {
		for (j = 0; j < NB_BRIQUE; j++) {
		if (briques[i][j] == 1) {
			if ((x_ball >= x_bricks + RAYON_BALL) && (x_ball <= x_bricks + RAYON_BALL + longueur_brique) && (y_ball >= y_bricks + RAYON_BALL) && (y_ball <= y_bricks + RAYON_BALL + hauteur_brique)
			&& (y_ball <= y_bricks + RAYON_BALL + hauteur_brique + longueur_brique)) {
				speed_ball_x = - speed_ball_x;
				printf("ma couille \n");
			} 
			/*if ((y_ball >= y_bricks + RAYON_BALL) && (y_ball <= y_bricks + RAYON_BALL + hauteur_brique) && (x_ball >= x_bricks + RAYON_BALL) && (x_ball <= x_bricks + RAYON_BALL + longueur_brique)
			&& (x_ball <= x_bricks + RAYON_BALL + longueur_brique + hauteur_brique)) {
				speed_ball_y = - speed_ball_y;
			}*/
			if (( y_ball >= y_bricks + RAYON_BALL) && (y_ball <= y_bricks + RAYON_BALL + hauteur_brique)
			&& (y_ball <= y_bricks + RAYON_BALL + hauteur_brique + longueur_brique) && (x_ball >= x_bricks + RAYON_BALL) && (x_ball <= x_bricks + RAYON_BALL + longueur_brique)
			&& (x_ball <= x_bricks + RAYON_BALL + longueur_brique + hauteur_brique)) {
				briques[i][j] = 0;
			}
		}
		x_bricks = x_bricks + ECART_HORIZ_BRIQUE + longueur_brique;
		}
	x_bricks = ECART_HORIZ_BRIQUE;
	y_bricks = y_bricks + ECART_VERTI_BRIQUE + hauteur_brique;
	}

}

int moveBall() 
{
	bounceWall();
	bouncePaddle();
	bounceBricks();
	
	x_ball = speed_ball_x + x_ball;
	y_ball = speed_ball_y + y_ball;
}

/* Fonctions du paddle */

int drawPaddle() 
{
	changeColor(red_paddle, green_paddle, blue_paddle);
	drawRect(x_paddle, y_paddle, LONGUEUR_PADDLE, HAUTEUR_PADDLE);
}

/* Fonctions des briques */

int initBricks()
{
	int i, j;
	
	for (i = 0; i < NB_LIGNE; i++) {
		for (j = 0; j < NB_BRIQUE; j++) {
			briques[i][j] = 1;
		}
	}
}

int drawBricks()
{
	int i, j;
	int x_bricks = ECART_HORIZ_BRIQUE;
	int y_bricks = ECART_VERTI_BRIQUE;
	
	changeColor(red2_bricks, green2_bricks, blue2_bricks);
	
	for (i = 0; i < NB_LIGNE; i++) {
		for (j = 0; j < NB_BRIQUE; j++) {
			if (briques[i][j] == 1) {
				/* Formule pour changer la couleur une case sur deux */
				if ((i+j)%2 == 0) {											
					changeColor(red1_bricks, green1_bricks, blue1_bricks);
				} else {
						changeColor(red2_bricks, green2_bricks, blue2_bricks);
					}
				drawRect(x_bricks, y_bricks, longueur_brique, hauteur_brique);
			}
			x_bricks = x_bricks + ECART_HORIZ_BRIQUE + longueur_brique;
		}
		x_bricks = ECART_HORIZ_BRIQUE;
		y_bricks = y_bricks + ECART_VERTI_BRIQUE + hauteur_brique;
	}
}
