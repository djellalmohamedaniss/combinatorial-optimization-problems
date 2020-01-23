#ifndef __JOB_H
#define __JOB_H

#include <vector>

using std::vector;

// On veut affecter des tâches (job) à des machines.
// On a un ensemble de machines hétérogènes, et des descriptions de jobs.
// Chaque job utilise une quantité prédéfinie de CPU, RAM et disque de la
// machine sur laquelle il tournera, et chaque machine a une quantité
// prédéfinie des CPU, RAM et disque.
//
// On veut maximiser le nombre de jobs affectés.
//
// Cette fonction doit renvoyer un assignment optimal des jobs à chaque
// machine: l'élement #i sera le numero de machine à laquelle le job #i
// sera affecté, où -1 si on décide de ne pas affecter le job.
/*
job1(2,200,50)  job2(1,300,40)   job3(3,100,60)
m1(2,200,70)  m2(3,100,80)

m.c - j.c > 0  && m.r - j.r > 0 && m.d - j.d > 0  min

*/
struct Resources
{
  double cpu;
  double ram;
  double disk;
};
vector<int> BestJobAssignment(const vector<Resources> &jobs,
                              const vector<Resources> &machines);

#endif // __JOB_H
