# MalDev : preuves de concept pédagogiques

Petits PoC écrits en C pour comprendre les mécanismes bas niveau du système :
flux d'exécution sous Linux, parcours de la PEB et des tables d'export PE sous Windows.

## Contenu

| Dossier | Plateforme | Sujet |
|---|---|---|
| [`linux/signal-flux`](linux/signal-flux) | Linux x86-64 | Redirection du flux via `ud2`, `SIGILL` et `ucontext_t` |
| [`windows/peb-walk`](windows/peb-walk) | Windows x64 | Recherche d'un module chargé via la PEB |
| [`windows/api-hash-resolver`](windows/api-hash-resolver) | Windows x64 | Résolution d'une fonction par hash de son nom dans la table d'export |

Chaque dossier contient son propre README avec la compilation, la sortie attendue et une section détection.

## Avertissement

Ces PoC sont destinés à la recherche et à l'apprentissage, à exécuter sur une machine de test que vous maîtrisez.
Ils ne contiennent aucune charge utile et n'interagissent avec aucun autre processus.
Les techniques présentées sont largement documentées et utilisées en sécurité offensive comme défensive.

## Approche

Chaque PoC est accompagné de sa contrepartie défensive : comment repérer le comportement dans un binaire, dans les appels système ou dans les journaux.
