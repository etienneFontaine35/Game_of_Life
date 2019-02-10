#include <vector>
#include <string>
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "environnement.hpp"



//---------------------------------
//      METHODES DE Individu
//---------------------------------

Individu::Individu(bool etat) : mEstEnVie(etat)
{

}


Individu::Individu() : Individu(false)
{

}


bool Individu::estVivant()
{
    return mEstEnVie;
}


void Individu::setEtat(bool etat)
{
    mEstEnVie = etat;
}


void Individu::setEvolution(bool evol)
{
    mEvolution = evol;
}


void Individu::evoluer()
{
    mEstEnVie = mEvolution;
}





//---------------------------------
//      METHODES DE Grille
//---------------------------------

Grille::Grille(unsigned longueur, unsigned hauteur, int hauteurEcran, int red, int green, int blue) : mLongueur(longueur), mHauteur(hauteur), mThickeness(hauteurEcran/mHauteur + 1*(hauteurEcran/mHauteur==0)), mPopulation(Matrix<Individu>(hauteur, longueur))
{
    mSpriteIndividu.setSize(sf::Vector2f(mThickeness, mThickeness));
    mSpriteIndividu.setFillColor(sf::Color(red, green, blue));
}


Grille::Grille(Matrix<Individu> population, int hauteurEcran, int red, int green, int blue) : Grille(population.getNbreColonnes(), population.getNbreLignes(), hauteurEcran, red, green, blue)
{
    for (unsigned i = 0; i < mHauteur; i++)
    {
        for (unsigned j = 0; j < mLongueur; j++)
        {
            mPopulation(i, j) = population(i, j);
        }
    }
}


Individu& Grille::operator()(const unsigned i, const unsigned j)
{
    return mPopulation(i, j);
}


unsigned Grille::compterVoisinsVivants(unsigned i, unsigned j)
{
    unsigned voisinsVivants = 0;
    
    for (int horizontal = -1; horizontal <= 1; horizontal++)
    {
        for (int vertical = -1; vertical <= 1; vertical++)
        {
            if
            (
            !(
                (horizontal == 0 && vertical == 0) ||
                (j + horizontal < 0) ||
                (j + horizontal > mLongueur - 1) ||
                (i + vertical < 0) ||
                (i + vertical > mHauteur - 1)
            )
            )
            {

                if (mPopulation(i + vertical, j + horizontal).estVivant())
                    voisinsVivants++;
            }
        }
    }

    return voisinsVivants;
}


void Grille::predire(unsigned i, unsigned j)
{
    unsigned nbreVoisins(compterVoisinsVivants(i, j));

    if (nbreVoisins == 3)
        mPopulation(i, j).setEvolution(true);
    
    else if (nbreVoisins == 2 && mPopulation(i, j).estVivant())
        mPopulation(i, j).setEvolution(true);
    
    else
        mPopulation(i, j).setEvolution(false);
}


void Grille::evoluer(unsigned i, unsigned j)
{
    mPopulation(i, j).evoluer();
}



void Grille::avancerTimeStep()
{
    for (unsigned i = 0; i < mHauteur; i++)
    {
        for (unsigned j = 0; j < mLongueur; j++)
        {
            predire(i, j);
        }
    }

    for (unsigned i = 0; i < mHauteur; i++)
    {
        for (unsigned j = 0; j < mLongueur; j++)
        {
            evoluer(i, j);
        }
    }

}




void Grille::creerCanon(const unsigned i, const unsigned j)
{
    for (unsigned vertical = 0; (vertical + i < mHauteur) && (vertical < 3); vertical++)
    {
        for (unsigned horizontal = 0; (horizontal + j < mLongueur) && (horizontal < 3); horizontal++)
        {
            if (vertical == 0 && horizontal == 1)
                mPopulation(i + vertical, j + horizontal).setEtat(true);
            
            if (vertical == 1 && horizontal == 2)
                mPopulation(i + vertical, j + horizontal).setEtat(true);
            
            if (vertical == 2)
                mPopulation(i + vertical, j + horizontal).setEtat(true);
        }
    }
}



std::string Grille::convertirTexte()
{
    std::string affichage;
    // affichage += "\n";

    for (unsigned i = 0; i < mHauteur; i++)
    {
        for (unsigned j = 0; j < mLongueur; j++)
        {
            if (mPopulation(i, j).estVivant())
                affichage += "0";
            else
                affichage += ".";
        }
        affichage += "\n";
    }

    // affichage += "\n";
    return affichage;
}


void Grille::afficher()
{
    std::cout << convertirTexte() << std::endl;
}


void Grille::afficher(sf::RenderWindow& fenetre)
{
    for (unsigned i = 0; i < mHauteur; i++)
    {
        for (unsigned j = 0; j < mLongueur; j++)
        {
            if (mPopulation(i, j).estVivant())
            {
                mSpriteIndividu.setPosition(j*mThickeness, i*mThickeness);
                fenetre.draw(mSpriteIndividu);
            }
        }
    }
}






//----------------------------------------
//      METHODES DE PopulationSource
//----------------------------------------

PopulationSource::PopulationSource(std::string fileName) : mNomFicher(fileName)
{

}


unsigned color2gray(sf::Uint8 red, sf::Uint8 green, sf::Uint8 blue)
{
    unsigned gray((red*30 + green*59 + blue*11)/100);
    return gray;
}



void PopulationSource::stockerImage()
{
    sf::Image imageSource;
    if (!imageSource.loadFromFile(mNomFicher))
    {
        std::cout << "Erreur lors de l ouverture de l image" << std::endl;
    }

    sf::Vector2u coord(imageSource.getSize());
    mLongueur = coord.x;
    mHauteur = coord.y;
    mPixelArray.resize(mLongueur * mHauteur);

    const sf::Uint8* vanillaImage(imageSource.getPixelsPtr());

    for (unsigned i = 0; i < mLongueur * mHauteur * 4; i += 4)
    {
        sf::Uint8 redPixel(vanillaImage[i]);
        sf::Uint8 greenPixel(vanillaImage[i + 1]);
        sf::Uint8 bluePixel(vanillaImage[i + 2]);

        mPixelArray[i / 4] = color2gray(redPixel, greenPixel, bluePixel);
    }
}



Matrix<Individu> PopulationSource::transformerEnPopulation()
{
    Matrix<Individu> population(mHauteur, mLongueur);
    for (unsigned i = 0; i < mHauteur; i++)
    {
        for (unsigned j = 0; j < mLongueur; j++)
        {
            if (mPixelArray[i*mLongueur + j] < 126)
            {
                population(i, j).setEtat(false);
            }
            else
            {
                population(i, j).setEtat(true);
            }
        }
    }

    return population;
}




