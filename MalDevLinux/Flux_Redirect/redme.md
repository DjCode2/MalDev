# signal-flux

Démonstration pédagogique de redirection du flux d'exécution en C via
l'exception `#UD` (instruction `ud2`), le signal `SIGILL` et la modification
de `RIP` dans `ucontext_t`. Peut tout aussi bien se faire avec d'autres executions invalides. 

## Ce que fait le programme

1. Chaque bloc exécute une instruction `ud2`, invalide par définition.
2. Le CPU lève `#UD`, le noyau envoie `SIGILL` au processus.
3. Le gestionnaire `handler` lit une table (`jump_table`) et écrit la
   destination suivante dans `RIP` du contexte sauvegardé.
4. Au retour du handler, l'exécution reprend à la nouvelle adresse.

## Prérequis

- Linux, architecture x86-64
- GCC

## Compilation et exécution

```bash
gcc -O0 -g -o signal_flux signal_flux.c
./signal_flux
```

## Sortie attendue

```
Bloc N°1
[handler] étape=0 ...
Bloc N°5
[handler] étape=1 ...
Bloc N°3
[handler] étape=2 ...
Bloc N°2
[handler] étape=3 ...
Bloc N°4
[handler] étape=4 ...
[fin] sortie propre
```

## Avertissement

Exemple pédagogique destiné à être exécuté sur une machine de test.
Il ne contient aucune charge utile et n'interagit avec aucun autre processus.

## Détection

Ce comportement peut se repérer par :
- des appels à `sigaction` sur `SIGILL` ou `SIGSEGV` dans un binaire
  qui n'en a pas besoin.
- des écritures dans `uc_mcontext.gregs[REG_RIP]` depuis un handler.
- une succession de fautes `SIGILL` suivies de reprises rapprochées.
- des instructions `ud2` répétées, séparées de peu de code utile.