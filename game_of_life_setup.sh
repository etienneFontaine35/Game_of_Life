#! /bin/bash

action=$1
sfml_file='/usr/lib/x86_64-linux-gnu/libsfml-graphics.so'


echo -e "Bienvenue dans GameOfLife, le sorcier bouclier d'installation va vous guider !"

case $action in

# INSTALLATION
"install")
    echo -e "==> Installation de GameOfLife\n"

    echo -e "Installation de la SFML (librarie graphique)"
    sudo apt install libsfml-dev

    echo -e "Compilation"
    g++ -std=c++11 -Wall environnement.cpp main.cpp -o GameOfLife -lsfml-window -lsfml-graphics -lsfml-system

    echo -e "Ajout du raccourci au dossier de lancements rapides"
    sudo cp -v GameOfLife /usr/local/bin

    echo -e "==> Installation terminee !!!"
    echo -e "Pour désinstaller GameOfLife, executez $0 -u (donc ne supprimez pas cet assistant)"
    echo -e "Pour voir les instructions de commande, essayez 'GameOfLife --help', ou 'GameOfLife FICHIER_IMAGE' pour tester directement !"
    ;;



# DESINSTALLATION
"uninstall")
    echo -e "--> Désinstallation de GameOfLife\n"

    if [ -e '/usr/local/bin/GameOfLife' ]
    then
        echo -e "Suppression des raccourcis de lancement rapide"
        sudo rm -v /usr/local/bin/GameOfLife
    fi

    if [ -e 'GameOfLife' ]
    then
        echo -e "Suppression de l'executable"
        sudo rm -v GameOfLife
    fi

    
    suppr="refaire"
    
    while [ $suppr = "refaire" ]
    do
        echo -e "Voulez-vous supprimer la SFML ? (y/n)"
        read suppr

        case $suppr in
        "y")
            echo -e "Suppression de la SFML"
            sudo apt purge libsfml-*
            ;;
        
        "n")
            echo -e "La SFML est conservee"
            ;;

        *)
            echo -e "Entree non reconnue"
            suppr="refaire"
        esac 
    done

    echo -e "==> Desinstallation terminee !!!"
    ;;



# ERREUR D'ARGUMENT
*)
    echo -e "\nArgument non reconnu"
esac



