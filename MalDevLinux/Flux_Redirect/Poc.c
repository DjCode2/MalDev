/*
   signal_flux.c - Démonstration pédagogique de redirection du flux
   d'exécution via SIGILL et ucontext_t (Linux x86-64 uniquement).

   Usage : gcc -O0 -g -o signal_flux signal_flux.c && ./signal_flux
*/

#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <ucontext.h>

// Table remplie à l'exécution avec les adresses des labels 
static void *jump_table[5];
static int   step = 0;

static void handler(int sig, siginfo_t *info, void *ctx)
{
    (void)sig;
    (void)info;

    ucontext_t *uc = (ucontext_t *)ctx;

    // Adresse de l'instruction fautive (ici le ud2) */
    void *curseur = (void *)uc->uc_mcontext.gregs[REG_RIP];

    fprintf(stderr, "[handler] étape=%d  curseur=%p  ->  dest=%p\n",
            step, curseur, jump_table[step]);

    // Au retour du handler, le noyau restaure ce contexte modifié 
    uc->uc_mcontext.gregs[REG_RIP] = (greg_t)jump_table[step];
    step++;
}

int main(void)
{
    struct sigaction sa = {0};
    sa.sa_flags     = SA_SIGINFO;   // nécessaire pour recevoir ucontext
    sa.sa_sigaction = handler;

    sigemptyset(&sa.sa_mask);
    sigaction(SIGILL, &sa, NULL);

    /*
      &&label : extension GCC, donne l'adresse d'un label (void *).
      Ces adresses ne sont connues qu'au moment où le programme tourne.
    */
    jump_table[0] = &&bloc_5;   // ud2 de bloc_1 -> bloc_5 
    jump_table[1] = &&bloc_3;   // ud2 de bloc_5 -> bloc_3 
    jump_table[2] = &&bloc_2;   // ud2 de bloc_3 -> bloc_2 
    jump_table[3] = &&bloc_4;   // ud2 de bloc_2 -> bloc_4 
    jump_table[4] = &&fin;      // ud2 de bloc_4 -> fin    

bloc_1:
    printf("Bloc N°1\n");
    __asm__ volatile("ud2");    // instruction invalide, déclenche SIGILL 

bloc_2:
    printf("Bloc N°2\n");
    __asm__ volatile("ud2");

bloc_3:
    printf("Bloc N°3\n");
    __asm__ volatile("ud2");

bloc_4:
    printf("Bloc N°4\n");
    __asm__ volatile("ud2");

bloc_5:
    printf("Bloc N°5\n");
    __asm__ volatile("ud2");

fin:
    printf("[fin] sortie propre\n");
    return 0;
}