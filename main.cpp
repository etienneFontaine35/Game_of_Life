#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <ctime>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <SFML/Graphics.hpp>

#include "environnement.hpp"

using namespace std;
using namespace chrono;
using namespace this_thread;


struct sOptions
{
    char const *optFramerate = "--framerate";
    char const *optFramerateBis = "-f";

    char const *optHauteur = "--hauteur-ecran";
    char const *optHauteurBis = "-h";

    char const *optCouleur = "--couleur";
    char const *optCouleurBis = "-c";

    char const *optHelp = "--help";
};



int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        cout << "Il faut au moins indiquer un fichier image !!!" << endl;
        return 1;
    }

    sOptions options; 
    string imageFile;
    int framerate(12); // -f --framerate
    int hauteurEcran(1080); // -h --hauteur-ecran
    int red(200); // -c --couleur
    int green(50); 
    int blue(100); 

    int numArg(1);

    while (numArg < argc)
    {
        if (numArg == 1 && strcmp(argv[numArg], options.optHelp) != 0)
        {
            imageFile = argv[1];
            ifstream imageStream(imageFile.c_str(), ios::in);
            if (!imageStream)
            {
                cout << "Le fichier indiqué n'est pas valide" << endl;
                return 1;
            }
            imageStream.close();
            numArg++;
        }

        else if (strcmp(argv[numArg], options.optFramerate) == 0 || strcmp(argv[numArg], options.optFramerateBis) == 0)
        {
            numArg++;
            framerate = stoi(argv[numArg]);
            numArg++;
        }

        else if (strcmp(argv[numArg], options.optHauteur) == 0 || strcmp(argv[numArg], options.optHauteurBis) == 0)
        {
            numArg++;
            hauteurEcran = stoi(argv[numArg]);
            numArg++;
        }

        else if (strcmp(argv[numArg], options.optCouleur) == 0 || strcmp(argv[numArg], options.optCouleurBis) == 0)
        {
            numArg++;
            red = stoi(argv[numArg]);
            if (red > 255)
                red = 255;
            else if (red < 0)
                red = 0;
            numArg++;

            green = stoi(argv[numArg]);
            if (green > 255)
                green = 255;
            else if (green < 0)
                green = 0;           
            numArg++;

            blue = stoi(argv[numArg]);
            if (blue > 255)
                blue = 255;
            else if (blue < 0)
                blue = 0;
            numArg++;
        }

        else if (strcmp(argv[numArg], options.optHelp) == 0)
        {
            cout << argv[0] << " FICHIER_IMAGE " << "[OPTION]" << endl << endl;
            cout << "           --help                                Afficher l aide" << endl;
            cout << "           --framerate VALEUR, -f VALEUR         Modifie la vitesse d'exécution a VALEUR generations par seconde (defaut 12)" << endl;
            cout << "           --couleur R G B, -c R G B             Modifie la couleur des cellules vivantes en code RGB (defaut 200 50 100)" << endl;
            cout << "           --hauteur-ecran VALEUR, -h VALEUR     Indique la hauteur d ecran utilisee pour que la fenetre soit le mieux adaptee (defaut 1080)" << endl;

            return 0;
        }

        else
        {
            cout << "Option " << to_string(numArg) << " non reconnue" << endl;
            return 1;
        }

    }
    

    cout << "Bienvenue dans le jeu de la vie !" << endl;

    PopulationSource sourcePopulation(imageFile);
    sourcePopulation.stockerImage();

    Grille population(sourcePopulation.transformerEnPopulation(), hauteurEcran, red, green, blue);

    sf::RenderWindow fenetre(sf::VideoMode(population.mLongueur * population.mThickeness, population.mHauteur * population.mThickeness), "Game of Life", sf::Style::Titlebar | sf::Style::Close);
    fenetre.setFramerateLimit(framerate);
    sf::Event event;


    fenetre.clear(sf::Color::Black);
    population.afficher(fenetre);
    fenetre.display();
    sleep_for(milliseconds(2000));

    while (fenetre.isOpen())
    {

        while (fenetre.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                fenetre.close();
        }

        fenetre.clear(sf::Color::Black);
        population.afficher(fenetre);
        fenetre.display();

        population.avancerTimeStep();
    }

    return 0;
}
