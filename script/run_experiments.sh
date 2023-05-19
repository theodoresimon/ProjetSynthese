#!/bin/bash

# OBJECTIF :
#
# Réaliser une campagne expérimentale sur une collection des instances.

# PARAMÈTRES DU SCRIPT :
#
# $1 nom du répertoire contenant la collection des instances
# $2 nom du fichier avec les résultats
# $3 nombre minimum des machines à utiliser
# $4 nombre maximum des machines à utiliser
# $5 pas du nombre des machines à utiliser


# On modifie la valeur de la variable d'environnement LC_NUMERIC du shell
# afin d'éviter le problèmes éventuels dans les calculs de nombres flottants.
LC_NUMERIC=en_US.UTF-8


# GESTION DE PARAMÈTRES
#
# Testez si le nombre de paramètres est correct et affichez un message dans
# le cas contraire.
# Assurez vous que le premier paramètre correspond bien à un répertoire.

if [ $# -ne 5 ]; then
    echo "Usage: $0 <directory> <results_file> <min_machines> <max_machines> <step_machines>"
    exit 1
fi

if [ ! -d $1 ]; then
    echo "Error: $1 is not a directory."
    exit 1
fi

# Affectation des paramètres aux variables.

dir=`realpath $1`	# nom du répertoire contenant la collection des instances
fname=$2
m_min=$3
m_max=$4
m_step=$5


# EXÉCUTION DE L'EXPÉRIENCE
#
# Pour toutes les instances de la collection et pour chaque nombre de machines
# de $m_min à $m_max avec un pas de $m_step, exécutez le programme "./expe".
# Pour chaque exécution, vous devez écrire une ligne dans le fichier $fname
# avec 12 colonnes :
# Colonne 1 : nombre des tâches de l'instance/
# Colonne 4 : nombre des machines de l'exécution
# Colonnes 5-12 : résultats obtenus par l'exécution du programme "./expe"
# Les colonnes sont séparées par une espace.

for instance in $dir/*; do
    for m in $(seq $m_min $m_step $m_max); do
        result=$(./expe $instance $m)
        echo "$(basename $instance) $m $result" >> $fname
    done
done