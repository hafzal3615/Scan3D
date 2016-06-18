# Projet Scan3D

Ici réside le logiciel pour le projet Scan3D, projet d’innovation S8 printemps 2016 à l’École Centrale Paris. Le projet est de Hammad Afzal et Jay Taves, encadré par le professeur Pascal Morenton. 

Le but de ce logiciel est de communiquer avec le logiciel David3D pour le calibrer pour faire un scan, faire le scan d’un pied, sauvegarder le nuage de points généré et le traiter pour calculer le périmètre du pied. À cause d’un problème technique décrite plus tard, le logiciel ne calcule pas le périmètre du pied scanné, mais calcule le périmètre d’un scan déjà faite. 

## Processus
À part le calcul du périmètre, le logiciel est assez simple, pour la plus part, tout ce qu’il fait est de communiquer avec David3D et lui donne des commandes. Le logiciel triche un peu et ne calcule pas le périmètre du pied qui est scanné. Ceci est parce que le logiciel a besoin d’un scan complet qui a plusieurs angles sur le pied. N’ayant pas eu le temps de faire la partie du logiciel qui transforme plusieurs scans en un seule, on a utilisé un scan déjà fait et après avoir sauvegardé le scan du pied. Comme on peut sauvegarder les scans et les relire après, la seule chose dont on a besoin et la partie du logiciel qui fait la transformation des points de trois ou quatre nuages pour les mettre ensemble. Ci-dessous est décrite le processus pour calculer le périmètre du pied, d’origine un logiciel de Python écrite par Rémi Garde, Armand Bouvier et Sylvestre Prakabaran. Leur originale est incluse dans le répertoire, sous le nom de Perimetre.py.

1. Le logiciel commence avec un plan donné et le chemin d’un fichier qui contient un nuage de points. Le logiciel ouvre et lit le fichier, pour obtenir un nuage de points. 

2. À partir du nuage de points et du plan, le logiciel prend tous les points qui sont assez proche (défini par un paramètre dans le logiciel) du plan. Après il les projette sur le plan pour avoir un nuage en deux dimensions. Ce nuage représente tous les points qui sont compris dans le périmètre du pied.

3. Avec le nuage en deux dimensions, il commence avec les trois points les plus à gauche. Il calcule le produit vectoriel entre les deux vecteurs qui lient les trois points pour savoir si les trois points forment un virage à droite. Le code continue comme ça, prenant chaque fois un point de plus, si le point forme un virage à droite, il garde le point, sinon il le rejette et continue avec le prochain point. 

4. Cette liste de points comprend l’enveloppe convexe du nuage de point, on peut le comprendre comme le minimum de points nécessaire pour faire le tour de tous les points. 

5. Après avoir traité tous le liste de points, le logiciel prend la liste des points récupéré et va en ordre, sommant la distance entre chaque pair de points. Ceci est le périmètre du pied.

## Prochaines Étapes
Les prochaines étapes pour ce logiciel sont de le rendre plus fiable, et d’ajouter des codes pour travailler avec plusieurs scans pour les coudre ensemble pour faire un scan complet d’un pied. On peut le rendre plus fiable en ajoutant des contrôles sur, par exemple, les données que l’utilisateur met dans le logiciel, et dans le fichier récupéré. Pour coudre ensemble plusieurs scans, il faut mettre une boucle qui attendra pour que l’utilisateur fasse deux, trois ou quatre scans, les sauvegardes, et puis fait une opération de rotation sur chaque nuage de points pour les mettre dans le même répertoire.  
