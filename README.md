# TCL

> Ce language a pour objectif de permettre de dessiner sur une image avec un resultat dynamique du dessins qui apparait.

<!-- ![version](https://img.shields.io/badge/version-0.1.0-blue) -->

---

## Sommaire

1. [Présentation](#presentation)
2. [Exemple programme](#exemple-programme)
3. [Référence du langage](#référence-du-langage)
3. [Fonctions intégrées](#fonctions-intégrées)
3. [Probleme actuellement connue](#rprobleme actuellement connue)

---

## Présentation

TCL est un langage de programmation conçu pour permettre de dessiner des fonctions mathématiques ou des algorithmes avec un résultat dynamique. Il fonctionne avec 2 fonctions obligatoires (init et update). Init permet de définir vos variables global pour le reste de l'exécution et update est la fonction qui vous permettra d'exécuter le code que vous souhaitez a chaque frame. Vous pouvez aussi créer vos propres fonctions.

**Caractéristiques principales :**

- Extension de fichier : `.tcl`

---

## Exemple programme

```TCL
tb12 init c1 c2
c5
	tb11 nbBoucle ma 99

	tb11 x
	tb11 y
	
	t5
c6

tb12 update c1 c2
c5
	DrawPos c1 x y c2

	x ma x mb 1
	P x

	t2 c1 x f2 nbBoucle c2
	c5
		c3
	c6
c6
```

Sortie attendue :

```
Une ligne noir en haut a gauche de la fenetre.
```

---

## Référence du langage

### Mots-clés réservés

| Mot-clé | Sémantique | Exemple |
|---------|------------|---------|
| `tb11`     | Déclare un entier | `tb11 var ma 5` |
| `tb12`   | Retour de fonction vide | `tb12 init c1 c2` |
| `ma`| Assigner une valeur ou variable | `var ma 5` |
| `mb`      | Addition | `var ma var mb 1` |
| `mc`   | Soustraction | `var ma var mc 1` |
| `md`    | Multiplication | `var ma var md 1` |
| `me` | Division | `var ma var me 1` |
| `f1`| Inferieur a | `var f1 5` |
| `f2`| Superieur a | `var f2 5` |
| `t1` | Boucle for | `pas implementer` |
|`t2` | Boucle while | `t2 c1 var f1 5 c2` |
|`t3` | Si vrai | `t3 c1 var f1 5 c2` |
|`t4` | Si t3 faux (else) | `t4` |
|`t5` | retour de fonction | `t5` |
|`t6` | switch | `pas implementer` |
|`t7` | else if | `pas implementer` |
|`c3` | arret complet du programme | `c3` |
|`c1` / `c2` | ( et ) | `c1 c2` |
|`c5` / `c6` | { et } | `c5 c6` |
|`c8` / `c9` | [ et ] | `c8 c9` |

### Fonctions intégrées

| Fonction | Description |
|----------|-------------|
| `DrawColor` | Definit la couleur de dessin |
| `DrawPos` | Dessine a l'endroit choisit avec la couleur definit |

### Probleme actuellement connue

Pas de else if ni de for.
Pas possible d'ecrire apres une boucle ou une condition (source du probleme connue mais pas corriger pour l'instant).
Pas de rendue dynamique pour l'instant.
