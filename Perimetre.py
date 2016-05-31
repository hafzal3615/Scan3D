# Sections planaires de nuages de points et calcul de périmètre via enveloppe convexe
# Rémi Garde - Armand Bouvier - Sylvestre Prakabaran

#%% Modules à importer
from math import *
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

#%% Récupération des points depuis le fichiers
# N'éxécutez cette partie de code qu'une seule fois
# Permet d'obtenir t tableau des points sous la forme [ [x][y][z] ]

# Mettre le chemin du nuage de points. Ne pas avoir d'espaces dans des noms !
filepath = """C:\\Users\\Jay\\Documents\\Scans\\3d.asc"""
file = open(filepath,"r")

def pts_of_list(l):
    "Passage de str en float"
    l2 = [float(x) for x in l]
    return(l2[0],l2[1],l2[2])

def getpts_obj(f):
    """Récupération du nuage de points dans un fichier .OBJ
    Ne prend pas les arêtes ni les surfaces"""
    t = f.readlines()
    t2 = [ x[2:].split() for x in t if x[0:2]=='v ']
    n = len(t2)
    for i in range(n):
        for j in range(3):
            t2[i][j] = float(t2[i][j])
    return(t2)

def getpts_asc(f):
    "Récupération du nuage de points dans un fichier .ASC"
    t = f.readlines()
    t2 = [ (x.split())[1::2] for x in t if x[0:2]=="X "] #Récupération des lignes utiles
    n = len(t2)
    for i in range(n):
        for j in range(3):
            t2[i][j] = float( (t2[i][j]).replace(',','.') ) #Passages en flottants
    return(t2)

def getpts(file, num_pts):
    # Choisissez getpts_asc ou getpts_obj suivant l'extension de votre fichier
    if filepath[-3:] == "obj":
        t = getpts_obj(file)
    elif filepath[-3:] == "asc":
        t = getpts_asc(file)
    else:
        print(filepath[-3:])

    if len(t) > num_pts:
        print(len(t))
        t = t[0::round(len(t) / num_pts)]

    return t

#%% Fonctions algébriques
# Un point est un triplet [x,y,z] dans l'espace ou [x,y] dans le plan
# Un plan pl = (a,b,c,d) représente le plan d'équation cartésienne dans l'espace ax+by+cz+d = 0
# Une droite d = (a,b,c) est la droite du plan d'équation cartésienne ax+by+c=0

def distplan(pt,pl):
    "Distance d'un point à un plan (dans l'espace)"
    xp,yp,zp = pt
    a,b,c,d = pl
    r1 = abs(a*xp + b*yp + c*zp + d)
    r2 = sqrt(a*a +b*b +c*c)
    return(r1/r2)

def section(t,pl,eps):
    "Points du nuage t à une distance inférieure à eps du plan pl"
    return([ x for x in t if distplan(x,pl) < eps])

def prodscal(a,b):
    "Produit scalaire dans le plan ou l'espace"
    x = a[0]*b[0]
    y = a[1]*b[1]
    if len(a)==3:
        z = a[2]*b[2]
    else:
        z = 0
    return(x+y+z)

def norm(u):
    "Norme du vecteur OU"
    return sqrt(prodscal(u,u))

def dist(a,b):
    """Distance entre A et B, dans le plan ou dans l'espace suivant le nombre
    de coordonnées"""
    x= a[0]-b[0]
    y = a[1] - b[1]
    if len(a)==3:
        z = a[2] - b[2]
    else:
        z = 0
    return sqrt(x*x + y*y + z*z)

def proj(t,pl):
    """Projection des points du nuage t dans le plan pl
    (0,u,v) est le repère orthonormé 2D du plan"""
    a,b,c,d = pl
    u = [-b,a,0]
    u = [ i/norm(u) for i in u]
    v = [-a*c,-b*c,a*a+b*b]
    v = [ i/norm(v) for i in v]
    res = [[prodscal(x,u),prodscal(x,v)] for x in t]
    return(res)

## Affichage d'un nuage de points
def aff(s):
    "Affiche le nuage de points 2D s"
    # points
    plt.axis('equal')
    plt.plot([x[0] for x in s],[x[1] for x in s],"o")

def aff_enveloppe(t):
    "Affiche la ligne brisée fermée du plan t"
    plt.axis('equal')

    #Ligne brisée
    x = []
    y = []
    for point in t:
        x += [point[0]]
        y += [point[1]]
    x.append(t[0][0])
    y.append(t[0][1])
    plt.plot(x,y, linewidth=3)


def distdroite(pt,d):
    "Distance du point pt à la droite d dans le plan"
    a,b,c = d
    r1 = abs(a*pt[0] + b * pt[1] + c)
    r2 = sqrt(a*a +b*b)
    return(r1/r2)

def mediatrice(u,v):
    "Triplet de l'équation cartésienne du plan pour la médiatrice de [UV]"
    milieu = [(u[0]+v[0])/2, (u[1] + v[1])/2]
    # Le vecteur directeur de [UV] est vecteur normal de la méditrice
    a = u[0]-v[0] #Calcul des paramètres cartésiens de la droite
    b = u[1] - v[1]
    c =  -(a*milieu[0] + b*milieu[1]) # le milieu appartient à la médiatrice
    return(a,b,c)

def tangente(u,v):
    "Triplet de l'équation cartésienne du plan de la droite (UV)"
    a = - (u[1] - v[1])
    b = u[0]-v[0]
    c = -(a*u[0] + b*u[1])
    return(a,b,c)

#%% Enveloppe convexe

#!/usr/bin/env python

"""convexhull.py

Calculate the convex hull of a set of n 2D-points in O(n log n) time.
Taken from Berg et al., Computational Geometry, Springer-Verlag, 1997.
Prints output as EPS file.

When run from the command line it generates a random set of points
inside a square of given length and finds the convex hull for those,
printing the result as an EPS file.

Usage:

    convexhull.py <numPoints> <squareLength> <outFile>

Dinu C. Gherman
"""


# Helpers

def _myDet(p, q, r):
    """Calc. determinant of a special matrix with three 2D points.

    The sign, "-" or "+", determines the side, right or left,
    respectivly, on which the point r lies, when measured against
    a directed vector from p to q.
    """

    # We use Sarrus' Rule to calculate the determinant.
    # (could also use the Numeric package...)
    sum1 = q[0]*r[1] + p[0]*q[1] + r[0]*p[1]
    sum2 = q[0]*p[1] + r[0]*q[1] + p[0]*r[1]

    return sum1 - sum2


def _isRightTurn(x):
    "Do the vectors pq:qr form a right turn, or not?"
    [p,q,r] = x
    assert p != q and q != r and p != r

    if _myDet(p, q, r) < 0:
        return 1
    else:
        return 0

# Public interface

def convex(P):
    "Calculate the convex hull of a set of points."

    # Get a local list copy of the points and sort them lexically.
    points = P[:]
    points.sort()
    if len(points)<3:
        return []

    # Build upper half of the hull.
    upper = [points[0], points[1]]
    for p in points[2:]:
        upper.append(p)
        while (len(upper) > 2) and (not _isRightTurn(upper[-3:])) :
            del upper[-2]

    # Build lower half of the hull.
    points.reverse()
    lower = [points[0], points[1]]
    for p in points[2:]:
        lower.append(p)
        while (len(lower) > 2) and ( not _isRightTurn(lower[-3:]) ) :
            del lower[-2]

    # Remove duplicates.

    del lower[0]
    del lower[-1]

    # Concatenate both halfs and return.
    return (upper + lower)

def convex_2(P,dmax):
    """   Reprise de la fonction d'enveloppe convexe dans le but de
     récupérer les grandes parties concaves. Ne marche pas bien
     Get a local list copy of the points and sort them lexically."""
    points = P[:]
    points.sort()
    if len(points)<3:
        return []
    # Build upper half of the hull.
    upper = [points[0], points[1]]
    for p in points[2:]:
        upper.append(p)
        while (len(upper) > 2) and ((not _isRightTurn(upper[-3:]) )or (dist(upper[-3],upper[-1])>dmax)):
            del upper[-2]

    # Build lower half of the hull.
    points.reverse()
    lower = [points[0], points[1]]
    for p in points[2:]:
        lower.append(p)
        while (len(lower) > 2) and(( not _isRightTurn(lower[-3:]) ) or (dist(lower[-3],upper[-1])>dmax)):
            del lower[-2]

    # Remove duplicates.
    del lower[0]
    del lower[-1]

    # Concatenate both halfs and return.
    return (upper + lower)


#%% Amélioration de l'envelopppe
# Pour chaque segment de l'enveloppe convexe, on essaie de rapprocher son milieu des points du nuage.
# On prend le plus proche au segment des points suffisaments près de la médiatrice du segment (<eps = 0.5-1 mm)
# On le fait tant que les segments sont trop grand (>eta = 1-2 mm) :
# si ils sont suffisamment petits, on considère que l'enveloppe colle bien au nuage
# Si c'est bon, on rajoutera ce point dans l'enveloppe convexe.
# L'algorithme appelle récursivement sur les 2 sous-segments crées.
# Le paramètre mu (= 1-10 mm) correspond à la distance max (si possible) entre deux points de l'enveloppe
# Diminuer mu pour coller au plus près du nuage. Ne pas le mettre trop petit pour éviter
# les "zig-zag" lorsqu'il y a beaucoup de points dans la section
# "diamétralement opposés" sur le nuage, pour éviter une récurrence infinie (diminuer mu si c'est le cas)



def rapproche(u,v,t,eps,eta,mu):
    """u et v sont les 2 extrémités du segment de l'enveloppe convexe.
    t est le nuage de points.
    Renvoie le tableau des points du nuage à rajouter dans l'enveloppe entre u et v"""
    if dist(u,v)<eta:
        return([])
    else :
        eps2 = dist(u,v)/4
        plusproche = []
        med = mediatrice(u,v)
        mil = [(u[0] + v[0])/2,(u[1]+v[1])/2]
        tan = tangente(u,v)
        for x in t: # Tableau des points suffisament proches de la médiatrice
            dmed = distdroite(x,med)
            dtan = dist(x,mil)
            if dmed<min(eps,eps2):
                plusproche.append([dtan,x])
        if plusproche != [] :
            [d,p] = min(plusproche)        # Point le plus proche de la droite
            if d < mu:
                t1 = rapproche(u,p,t,eps,eta,mu)
                t2 = rapproche(p,v,t,eps,eta,mu)
                return(t1 + [p] + t2)
            else:
                return([])
        else:
            return([])

def recolle(env,t,eps,eta,mu):
    "Parcours de l'envellope convexe env pour la rapprocher du nuage de points"
    n = len(env)
    res = env [:]
    for i in range(0,n):
        u = env[i-1]
        v = env[i]
        k = res.index(v)
        res = res [0:k] + rapproche(u,v,t,eps,eta,mu) + res[k:]
    return(res)

#%% Calcul du périmètre

def perimetre(t):
    "Périmètre de la ligne brisée t (compte la dernière arête refermant la ligne"
    p = 0
    n = len(t)
    for i in range(0,n):
        p += dist(t[i-1],t[i])
    return p

#%% Procédure

def main(t, pl, eps1, eps2, eta, mu):
    """Affiche la section du nuage t par le plan pl, l'enveloppe convexe
    et l'enveloppe recollée"""

    t2 = section(t,pl,eps1)
    t3 = proj(t2,pl)
    t4 = convex(t3)
    t5 = recolle(t4,t3,eps2,eta,mu)
    aff(t3)
    aff_enveloppe(t4)
    aff_enveloppe(t5)

    return(t5)

def graph(pts, pl, r):
    fig = plt.figure()
    plt.hold(True)
    ax = fig.add_subplot(111, projection='3d')

    ax.set_xlabel('X axis')
    ax.set_ylabel('Y axis')
    ax.set_zlabel('Z axis')

    plt.show()

    plot_plane(ax, pl, 'red')

    i = 0
    l = len(pts)
    pct = l / 10
    count = 1

    for [x, y, z] in pts:
        ax.scatter(x, y, z)
        i = i + 1
        if i > pct * count:
            print(str(count) + "0% done\r\n")
            count = count + 1

def plot_plane(ax, pl, clr):
    a, b, c, d = pl

    a_range = np.arange(-80, 80, 1)
    b_range = np.arange(-80, 80, 1)

    if not a == 0:
        yy, zz = np.meshgrid(a_range, b_range)

        xx = -1/a * (b * yy + c * zz + d)
        ax.plot_surface(xx, yy, zz, color=clr)
    elif not b == 0:
        zz, xx = np.meshgrid(a_range, b_range)

        yy = -1/b * (c * zz + a * xx + d)
        ax.plot_surface(xx, yy, zz, color=clr)
    elif not c == 0:
        xx, yy = np.meshgrid(a_range, b_range)

        zz = -1/c * (a * xx + b * yy + d)
        ax.plot_surface(xx, yy, zz, color=clr)
    else:
        print("Not all arguments can be zero")

#%% Plan obtenu avec le scanner Breuckmann et low-cost
a = 1
b = 1
c = 0.001
d = 100

pl = (a,b,c,d)

#%% Plusieurs sections parallèles
eps1 = 0.5
eps2 = 2
eta = 2
mu = 8.5

t = getpts(file, 1000)

graph(t, pl, 80)
main(t, pl, eps1, eps2, eta, mu)