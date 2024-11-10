#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h> /*Pour utiliser les booléens*/


int persoX = 469, persoY = 600; /* Variables pour gérer la position du personnage */
int largeurPerso = 50, hauteurPerso = 70;


/*iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/


enum { SALLE_ENTREE, SALLE_DEUXIEME } salleActuelle = SALLE_ENTREE; /* Déclaration de la salle actuelle */


char* imagePersoActuelle = "images/perso_avant.png"; /* Chemins des images du personnage pour chaque direction */// Image par défaut
char* imagePersoHaut = "images/perso_arriere.png";
char* imagePersoBas = "images/perso_avant.png";
char* imagePersoGauche = "images/perso_gauche.png";
char* imagePersoDroite = "images/perso_droite.png";

/*iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/


void afficherTexte(SDL_Renderer* renderer, TTF_Font* font, const char* texte, int x, int y, SDL_Color couleur) { /* Fonction pour afficher du texte à l'écran */
    SDL_Surface* surfaceTexte = TTF_RenderText_Blended(font, texte, couleur);
    if (surfaceTexte) {
        SDL_Texture* textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
        SDL_Rect r = {x, y, surfaceTexte->w, surfaceTexte->h};
        SDL_RenderCopy(renderer, textureTexte, NULL, &r);
        SDL_FreeSurface(surfaceTexte);
        SDL_DestroyTexture(textureTexte);
    } else {
        fprintf(stderr, "Erreur de rendu du texte : %s\n", TTF_GetError());
    }
}

/*iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/

 void afficherSurface(int x, int y, int largeur, int hauteur, SDL_Renderer* renderer, SDL_Surface* surface) {
    if (surface) {
        SDL_Texture* image = SDL_CreateTextureFromSurface(renderer, surface);
        if (image) {
            SDL_Rect r = {x, y, largeur, hauteur};
            SDL_RenderCopy(renderer, image, NULL, &r);
            SDL_DestroyTexture(image);
        } else {
            fprintf(stderr, "Erreur de création de la texture\n");
        }
    }
}

/*iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/


 void afficherImage(int x, int y, int largeur, int hauteur, SDL_Renderer* renderer, char* nomFichier) {
    SDL_Surface* surface_image = IMG_Load(nomFichier);
    if (surface_image) {
        afficherSurface(x, y, largeur, hauteur, renderer, surface_image);
        SDL_FreeSurface(surface_image);
    } else {
        fprintf(stderr, "Erreur de chargement de l'image %s\n", nomFichier);
    }
}

/*iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/


bool demanderConfirmation(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color couleurBlanche = {255, 255, 255, 255};
    SDL_Color couleurNoire = {0, 0, 0, 255};
    SDL_Event event;
    bool reponse = false;
    bool decisionPrise = false;

    while (!decisionPrise) {
        SDL_RenderClear(renderer);
        afficherTexte(renderer, font, "Souhaitez-vous entrer ? (O: Oui, N: Non)", 250, 200, couleurBlanche);
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_o:  // Oui
                        reponse = true;
                        decisionPrise = true;
                        break;
                    case SDLK_n:  // Non
                        reponse = false;
                        decisionPrise = true;
                        break;
                }
            } else if (event.type == SDL_QUIT) {
                decisionPrise = true;
            }
        }
        SDL_Delay(16);
    }

    return reponse;
}


/*iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/


int main (int argc, char** argv)
 {
  if ( SDL_Init (SDL_INIT_VIDEO) != 0 )
   {
    fprintf (stderr, "Erreur d'initialisation du m�canisme SDL : %s\n", SDL_GetError() );

    return EXIT_FAILURE;
   }

  TTF_Init();

  SDL_Window* fenetre = SDL_CreateWindow ( "Jeu aeroport version 2" ,
                                           SDL_WINDOWPOS_UNDEFINED   ,
                                           SDL_WINDOWPOS_UNDEFINED   ,
                                           1000                      , /*dimension horizontale*/
                                           600                       , /*dimension verticale*/
                                           SDL_WINDOW_SHOWN          );

  if ( fenetre == NULL )
   {
    fprintf (stderr, "Erreur de cr�ation de la fen�tre : %s\n",SDL_GetError());

    return EXIT_FAILURE;
   }

  /*SDL_RendererInfo info;
  int nbDrivers = SDL_GetNumRenderDrivers ();*/


  SDL_Renderer* renderer = SDL_CreateRenderer ( fenetre, -1, SDL_RENDERER_ACCELERATED);

  SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);


  TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 24);
  if (!font) {
    fprintf(stderr, "Erreur de chargement de la police : %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }


    int quitter = 0; /*Boucle d'événements pour garder la fenêtre ouverte jusqu'à la fermeture manuelle*/
    SDL_Event event;
    while (!quitter) {

        while (SDL_PollEvent(&event)) { /* Gestion des événements*/
            if (event.type == SDL_QUIT) { /* Si l'utilisateur clique sur le bouton de fermeture*/
                quitter = 1; /* Met fin à la boucle*/
            }
             if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        persoY -= 10; // Déplacer vers le haut
                        imagePersoActuelle = imagePersoHaut;  // Changer l'image
                        break;
                    case SDLK_DOWN:
                        persoY += 10; // Déplacer vers le bas
                        imagePersoActuelle = imagePersoBas;
                        break;
                    case SDLK_LEFT:
                        persoX -= 10; // Déplacer vers la gauche
                        imagePersoActuelle = imagePersoGauche;
                        break;
                    case SDLK_RIGHT:
                        persoX += 10; // Déplacer vers la droite
                        imagePersoActuelle = imagePersoDroite;
                        break;
                }
        }
    }

        if (persoX < 0) persoX = 0; /* Limite les déplacements du personnage aux dimensions de la fenêtre*/
        if (persoY < 0) persoY = 0;
        if (persoX + largeurPerso > 1000) persoX = 1000 - largeurPerso;
        if (persoY + hauteurPerso > 600) persoY = 600 - hauteurPerso;


        if (salleActuelle == SALLE_ENTREE && persoX >= 450 && persoX <= 550 && persoY >= 250 && persoY <= 350) {
            if (demanderConfirmation(renderer, font)) {
                salleActuelle = SALLE_DEUXIEME;
                persoX = 50;
                persoY = 50;
            } else {
                persoX = 400; // Repositionne si refus
                persoY = 500;
            }
        }
        }


        SDL_RenderClear(renderer); /* Affichage selon la salle actuelle */ /* Effacer l'écran et redessiner */
        if (salleActuelle == SALLE_ENTREE) {
            afficherImage(0, 0, 1000, 600, renderer, "images/entree.png");
        } else if (salleActuelle == SALLE_DEUXIEME) {
            afficherImage(0, 0, 1000, 600, renderer, "images/securite.png");
        }

        afficherImage(persoX, persoY, largeurPerso, hauteurPerso, renderer, imagePersoActuelle);  /* Afficher le personnage */

        SDL_RenderPresent(renderer);

        SDL_Delay(16);  /* Contrôle de la vitesse d'actualisation (environ 60 FPS) */

 }
  SDL_DestroyRenderer (renderer);
  SDL_DestroyWindow   (fenetre);
  TTF_CloseFont(font);
  SDL_Quit();
  TTF_Quit();

  return EXIT_SUCCESS;
 }

