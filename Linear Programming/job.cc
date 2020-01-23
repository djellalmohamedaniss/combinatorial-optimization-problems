#include "job.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "ortools/linear_solver/linear_solver.h"
using namespace operations_research;
using namespace std;

vector<int> BestJobAssignment(const vector<Resources> &jobs,
                              const vector<Resources> &machines)
{
    MPSolver solver("Mon solveur", MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);

    //attributs
    const int num_jobs = jobs.size();         //nombre de job
    const int num_machines = machines.size(); //nombre machines
    vector<int> result(num_jobs, -1);
    MPObjective *const goal = solver.MutableObjective();
    goal->SetMaximization();

    //les variables
    vector<vector<MPVariable *>> tab_var(num_jobs);
    for (int i = 0; i < num_jobs; i++)
    {
        for (int j = 0; j < num_machines; j++)
        {
            MPVariable *var = solver.MakeBoolVar("job_" + to_string(i) + "machine_" + to_string(j));
            tab_var[i].push_back(var);
            goal->SetCoefficient(var, 1);
        }
    }

    //le job dans un seul machine
    for (int i = 0; i < num_jobs; i++)
    {
        MPConstraint *ct = solver.MakeRowConstraint(0, 1, "ct_" + to_string(i));
        for (int j = 0; j < num_machines; j++)
        {
            ct->SetCoefficient(tab_var[i][j], 1);
        }
    }

    //verifier les ressource
    for (int j = 0; j < num_machines; j++)
    {
        //cpu
        MPConstraint *ct1 = solver.MakeRowConstraint(0, machines[j].cpu, "ct1_" + to_string(j));
        for (int i = 0; i < num_jobs; i++)
        {
            ct1->SetCoefficient(tab_var[i][j], jobs[i].cpu);
        }
        //ram
        MPConstraint *ct2 = solver.MakeRowConstraint(0, machines[j].ram, "ct2_" + to_string(j));
        for (int i = 0; i < num_jobs; i++)
        {
            ct2->SetCoefficient(tab_var[i][j], jobs[i].ram);
        }
        //disk
        MPConstraint *ct3 = solver.MakeRowConstraint(0, machines[j].disk, "ct3_" + to_string(j));
        for (int i = 0; i < num_jobs; i++)
        {
            ct3->SetCoefficient(tab_var[i][j], jobs[i].disk);
        }
    }
    solver.Solve();
    for (int i = 0; i < num_jobs; i++)
    {
        for (int j = 0; j < num_machines; j++)
        {
            if (tab_var[i][j]->solution_value() != 0)
            {
                result[i] = j;
            }
        }
    }
    return result;
}
