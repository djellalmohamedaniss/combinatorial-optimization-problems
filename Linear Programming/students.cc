#include "students.h"
#include <iostream>
#include <string>
#include "ortools/linear_solver/linear_solver.h"

using namespace operations_research;
using namespace std;

double StudentsAssignment(const vector<int> &cours,
                          const vector<vector<double>> valeurs)
{
    MPSolver solver("Mon solveur", MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);
    const int num_course = cours.size();
    const int num_student = valeurs.size();
    MPObjective *objective = solver.MutableObjective();
    const double infinity = solver.infinity();
    objective->SetMaximization();
    // la variable qui maximise la satisfaction minimale
    MPVariable *z = solver.MakeNumVar(0, infinity, "lowest");
    objective->SetCoefficient(z, 1);
    // la matrice des N etudiants dans M classes
    vector<vector<MPVariable *>> var(num_student);
    for (int s = 0; s < num_student; ++s)
    {

        /*
            notre but est de trouver un Z tq Z <= la somme des coff*x pour
            chaque etudiant, avec x qui prend les valeurs 1 s'il est accepté au cours, sinon 0 .
            alors chaque contrainte est de la forme suivante : somme de satisfaction - Z >= 0
        */

        MPConstraint *ct = solver.MakeRowConstraint(0, infinity, "");
        for (int c = 0; c < num_course; ++c)
        {
            MPVariable *v = solver.MakeBoolVar("Student" + std::to_string(s) + " enrolled in  course " +
                                               std::to_string(c));
            var[s].push_back(v);
            ct->SetCoefficient(v, valeurs.at(s).at(c));
        }
        ct->SetCoefficient(z, -1);
    }
    // chaque etudiant a le droit de mettre 10 points au max.
    for (int c = 0; c < num_student; c++)
    {
        MPConstraint *ct = solver.MakeRowConstraint(0, 10, "");
        for (int sc = 0; sc < num_course; sc++)
        {
            ct->SetCoefficient(var.at(c).at(sc), valeurs.at(c).at(sc));
        }
    }
    // le nombre d'etudiants acceptés ne doit pas depasser le nombre de places dans un cours c
    for (int c = 0; c < num_course; c++)
    {
        MPConstraint *ct = solver.MakeRowConstraint(0, cours.at(c), "");
        for (int sc = 0; sc < num_student; sc++)
        {
            ct->SetCoefficient(var.at(sc).at(c), 1);
        }
    }
    solver.Solve();
    // on retourne z qui est la solution optimale
    return objective->Value();
}