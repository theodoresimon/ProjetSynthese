#!/bin/bash

# OBJECTIF :
#
# Créer un fichier contenant une nouvelle instance du problème
# d'ordonnancement générée aléatoirement.

# PARAMÈTRES DU SCRIPT :
#
# $1 nom du fichier à créer
# $2 nombre de tâches à créer
# $3 paramètre utilisé dans la génération de dates de libération


# On modifie la valeur de la variable d'environnement LC_NUMERIC du shell
# afin d'éviter le problèmes éventuels dans les calculs de nombres flottants.
LC_NUMERIC=en_US.UTF-8


# FONCTION getNormalDistributionSample
#
# Génère de façon aléatoire un entier dans l'intervalle [min,max]
# selon la distribution normale.
#
# PARAMÈTRES : deux entiers correspondants aux limites de l'intervalle.

getNormalDistributionSample () {
	min=$1
	max=$2
	echo $(( $min + ($RANDOM % ($max-$min+1)) ))
}


# FONCTION getParetoDistributionSample
#
# Génère de façon aléatoire un entier selon la distribution de Pareto.

getParetoDistributionSample () {
	alpha=1.1
	lambda=100

	u=$(echo "($RANDOM+1)/32768" | bc -l)

	# x = lambda * (1-u)^(-1/alpha)
	x=$(echo "$lambda * e((-1/$alpha)*l(1-$u))" | bc -l)
	echo $x | awk '{printf("%.0f\n",$1)}'
}


# GESTION DE PARAMÈTRES
#
# Testez si le nombre de paramètres est correct et affichez un message dans
# le cas contraire.
# Affectez les paramètres aux variables.

# A FAIRE


# GÉNÉRATION DE DURÉES OPÉRATOIRES ALÉATOIRES
#
# Dans la suite, cherchez et utilisez la commande "seq".
#
# La valeur de la durée opératoire de chaque tâche est basée sur la
# distribution Pareto. Pour la générer aléatoirement, il faut appeler la
# fonction getParetoDistributionSample.

# A FAIRE


# GÉNÉRATION DE DATES DE LIBÉRATION ALÉATOIRES
#
# La valeur de la date de libération de chaque tâche est basée sur la
# distribution normale. Une fois que les durées opératoires sont générées,
# il faut calculer leur somme. En suite la fonction
# getNormalDistributionSample doit être appelée afin de générer
# aléatoirement une valeur dans l'intervalle [min,max]=[0, t*somme], ou t
# correspond au troisième paramètre du script.

# A FAIRE


# ÉCRITURE DES DONNÉES GÉNÉRÉES AU FICHIER
#
# Format du fichier à créer (chaque ligne) :
# 		job_id processing_time release_date

# A FAIRE