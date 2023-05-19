#!/bin/bash

# OBJECTIF :
#
# Générer une collection d'instances pour un problème d'ordonnancement.

# PARAMÈTRES DU SCRIPT (optionnels) :
#
# -d Donner le nom du répertoire sur lequel les nouvelles instances vont
#	 être enregistrées.
# -j Les différentes instances générées avec ce script contiennent un nombre
#	 de tâches différent. Ce paramètre précise le nombre de tâches à
#	 utiliser dans un format MIN:MAX:PAS.
#	 Exemple : 20:40:10 va produire des instances avec 20, 30 et 40 tâches.
# -t Il s'agit d'un paramètre lié à la génération aléatoire des dates de
#	 libération de tâches. La valeur de ce paramètre varie pour les
#	 différentes instances et est donnée dans un format MIN:MAX:PAS.
#	 Exemple : 0.1:0.2:0.1 va générer des instances avec t=0.1 et 0.2.
# -i Donner le nombre de différentes instances qui vont être générées
#	 pour chaque combinaison de paramètres j et t.


# On modifie la valeur de la variable d'environnement LC_NUMERIC du shell
# afin d'éviter le problèmes éventuels dans les calculs de nombres flottants.
LC_NUMERIC=en_US.UTF-8


# FONCTION USAGE
#
# Affiche une aide sur l'utilisation du script et termine son exécution.

usage () {
	echo ""
	echo "NAME"
	echo -e "\t generate_collection\n"
	echo "DESCRIPTION"
	echo -e "\t Generates a collection of instances for a scheduling problem.\n"
	echo "USAGE"
	echo -e "\t bash generate_collection.sh [OPTIONS]\n"
	echo "OPTIONS"
	echo -e "\t -d DIR_NAME"
	echo -e "\t\t set the name of the created directory which will contain the collection;"
	echo -e "\t\t default value: collection\n"
	echo -e "\t -j MIN:MAX:STEP"
	echo -e "\t\t set the limites and the step concerning the number of jobs that will be"
	echo -e "\t\t created in the generated instances;"
	echo -e "\t\t default values: 10:100:10\n"
	echo -e "\t -t MIN:MAX:STEP"
	echo -e "\t\t set the limites and the step concerning the parameter t that will be"
	echo -e "\t\t used in the generation of the release dates;"
	echo -e "\t\t default values: 0.1:0.5:0.1\n"
	echo -e "\t -i NUM"
	echo -e "\t\t create NUM instances for each combination of the parameters j and t;"
	echo -e "\t\t default value: 10\n"
	echo -e "\t -h"
	echo -e "\t\t display this help and exit\n"
	echo "EXAMPLE"
	echo -e "\t bash generate_collection.sh -j 50:200:50 -d boo\n"
	echo -e "\t Creates in the directory boo 10 instances (default value) for each"
	echo -e "\t combination of parameters t={0.1,0.2,0.3,0.4,0.5} (default value)"
	echo -e "\t and j={50,100,150,200}\n"
	exit 1
}


# DÉFINITION DE PARAMÈTRES PAR DÉFAUT
#
# On définit d'abord les valeurs par défaut des paramètres qui vont être
# utilisées dans le cas où l'utilisateur ne donne pas la valeur de certains
# ou de tous les paramètres.

dir="collection"
nbjobsmin=10
nbjobsmax=100
nbjobsstep=10
tmin=0.1
tmax=0.5
tstep=0.1
nbinstances=10


# GESTION DE PARAMÈTRES DONNÉS PAR L'UTILISATEUR
#
# Cherchez et utilisez la commande "getopts" (et pas "getopt").
# L'objectif est de mettre éventuellement à jour les paramètres par défaut,
# si l'utilisateur a choisi d'utiliser des valeurs différentes.
#
# Supprimez le # dans les deux lignes suivantes :
#		usage
#		exit 0
#
# enregistrez et exécutez le script avec la commande :
# 	bash generate_collection.sh
#
# afin d'avoir une idée des différents cas à traiter.
# N'oubliez pas à remettre les # dans les deux lignes !

while getopts "d:n:m:s:t:x:y:z:h" opt; do
  case $opt in
    d)
      dir=$OPTARG
      ;;
    n)
      nbjobsmin=$OPTARG
      ;;
    m)
      nbjobsmax=$OPTARG
      ;;
    s)
      nbjobsstep=$OPTARG
      ;;
    t)
      tmin=$OPTARG
      ;;
    x)
      tmax=$OPTARG
      ;;
    y)
      tstep=$OPTARG
      ;;
    z)
      nbinstances=$OPTARG
      ;;
    h)
      usage
      exit 0
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      usage
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      usage
      exit 1
      ;;
  esac
done

# Fonction d'affichage de l'utilisation
usage() {
  echo "Usage: $0 [-d dir] [-n nbjobsmin] [-m nbjobsmax] [-s nbjobsstep] [-t tmin] [-x tmax] [-y tstep] [-z nbinstances] [-h]" >&2
  echo "Options:" >&2
  echo "  -d dir           Directory to store the generated instances (default: collection)" >&2
  echo "  -n nbjobsmin     Minimum number of jobs (default: 10)" >&2
  echo "  -m nbjobsmax     Maximum number of jobs (default: 100)" >&2
  echo "  -s nbjobsstep    Step between minimum and maximum number of jobs (default: 10)" >&2
  echo "  -t tmin          Minimum processing time (default: 0.1)" >&2
  echo "  -x tmax          Maximum processing time (default: 0.5)" >&2
  echo "  -y tstep         Step between minimum and maximum processing time (default: 0.1)" >&2
  echo "  -z nbinstances   Number of instances to generate (default: 10)" >&2
  echo "  -h               Display this help message" >&2
}


# GÉNÉRATION DES INSTANCES ALÉATOIRES
#
# Créez dans le répertoire dir des instances aléatoires avec les paramètres
# déjà définis : pour chaque couple de valeurs ($nbjobs, $t), il faut créer
# $nbinstances instances en appelant le script generate_instance.sh.
#
# ATTENTION : NOM DU FICHIER DE CHAQUE INSTANCE
#
# Il faut obligatoirement respecter le format suivant :
#		nbjobs_t_instance
#
# Exemple : l'instance numéro 4 avec 20 tâches et t=0.3 sera enregistrer
# au fichier (sans suffixe, des espaces ou d'autres caractères) :
#		20_0.3_4

mkdir -p $dir

count=1

for nbjobs in $(seq $nbjobsmin $nbjobsstep $nbjobmax); do
  if (( nbjobs > nbjobsmax )); then
    break
  fi
  for t in $(seq $tmin $tmax); do
    for (( i=1 ; i<=$nbinstances; i++ )); do
      instance_filename="${nbjobs}_${t}_${count}"
      ((count++))
      ./generate_instance.sh -n $nbjobs -t $t > "$dir/$instance_filename"
    done
  done 
done 