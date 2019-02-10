#ifndef ENVIRONNEMENT_HPP
#define ENVIRONNEMENT_HPP

#include <vector>
#include <string>
#include <ctime>
#include <SFML/Graphics.hpp>

class Grille;
class Individu;


template <class C>
class Matrix
{
    public :
        const unsigned mNbreLignes;
        const unsigned mNbreColonnes;

        Matrix(unsigned lignes, unsigned colonnes);

        C& operator()(const unsigned i, const unsigned j)
        {
            return mCoefficients[i][j];
        }


        const unsigned getNbreLignes()
        {
            return mNbreLignes;
        }

        const unsigned getNbreColonnes()
        {
            return mNbreColonnes;
        }

    private :
        
        std::vector<std::vector<C>> mCoefficients;

};


template <class C>
Matrix<C>::Matrix(unsigned lignes, unsigned colonnes) : mNbreLignes(lignes), mNbreColonnes(colonnes)
{
    Matrix<C>::mCoefficients.resize(Matrix<C>::mNbreLignes);
    for (typename std::vector<std::vector<C>>::iterator itLigne = mCoefficients.begin(); itLigne != mCoefficients.end(); itLigne++)
    {
        itLigne->resize(mNbreColonnes);
    }
    
}







class Individu
{
    public :
        Individu(bool etat);
        Individu();
        
        bool estVivant();
        void setEtat(bool etat);
        void setEvolution(bool evol);
        void evoluer();

    private :
        bool mEstEnVie;
        bool mEvolution;

};




class Grille
{
    public :
        const unsigned mLongueur;
        const unsigned mHauteur;
        const unsigned mThickeness;

        

        Grille(unsigned longueur, unsigned hauteur, int hauteurEcran, int red, int green, int blue);
        Grille(Matrix<Individu> population, int hauteurEcran, int red, int green, int blue);
        
        Individu& operator()(const unsigned i, const unsigned j);

        unsigned compterVoisinsVivants(unsigned i, unsigned j);
        void predire(unsigned i, unsigned j);
        void evoluer(unsigned i, unsigned j);
        void avancerTimeStep();

        void creerCanon(const unsigned i, const unsigned j);

        std::string convertirTexte();
        void afficher();
        void afficher(sf::RenderWindow& fenetre);


    private :
        sf::RectangleShape mSpriteIndividu;
        Matrix<Individu> mPopulation;
};




class PopulationSource
{
    public :
        PopulationSource(std::string imageFile);

        void stockerImage(); // passe l'image en noir et blanc en stockant ses pixels dans un tableau 1D
        Matrix<Individu> transformerEnPopulation(); // retourne la Matrix<Individu> correspondant à l'image en noir et blanc

        unsigned mHauteur;
        unsigned mLongueur;
        std::vector<unsigned> mPixelArray;

    private :
        std::string mNomFicher;

};


#endif