#include "flow.h"
#include "ortools/linear_solver/linear_solver.h"

#include <string>

using namespace operations_research;

vector<FlowOnArc> BestFlow(int num_nodes, const vector<BackboneArc> &backbone,
                           const vector<FlowDemand> &demands)
{
    MPSolver solver("Mon solveur", MPSolver::GLOP_LINEAR_PROGRAMMING);
    const int num_arcs = backbone.size();
    const int num_demandes = demands.size();
    const double infinity = solver.infinity();
    MPObjective *objective = solver.MutableObjective();
    objective->SetMaximization();

    vector<vector<MPVariable *>> flows(num_demandes);
    for (int demande = 0; demande < num_demandes; ++demande)
    {
        for (int arc = 0; arc < num_arcs; ++arc)
        {
            MPVariable *flow = solver.MakeNumVar(0, infinity, "{ demande : " + std::to_string(demande) + "," + "arc : " + std::to_string(arc) + "}");
            flows.at(demande).push_back(flow);
        }
    }

    for (int arc = 0; arc < num_arcs; ++arc)
    {
        MPConstraint *ct = solver.MakeRowConstraint(0, backbone[arc].capacity, "capacity of " + std::to_string(arc));
        for (int d = 0; d < num_demandes; ++d)
        {
            ct->SetCoefficient(flows[d][arc], 1);
        }
    }

    vector<vector<int>> out_arc_indices(num_nodes);
    vector<vector<int>> in_arc_indices(num_nodes);
    for (int arc = 0; arc < num_arcs; ++arc)
    {
        out_arc_indices[backbone[arc].from].push_back(arc);
        in_arc_indices[backbone[arc].to].push_back(arc);
    }

    for (int d = 0; d < num_demandes; ++d)
    {
        for (int node = 0; node < num_nodes; ++node)
        {
            MPConstraint *ct = solver.MakeRowConstraint(node == demands[d].dst ? -demands[d].demand : 0.0, node == demands[d].src ? demands[d].demand : 0.0, "Flow(demand:" + std::to_string(d) + ",node:" + std::to_string(node));
            for (int a : out_arc_indices[node])
            {
                ct->SetCoefficient(flows[d][a], 1.0);
            }
            for (int a : in_arc_indices[node])
            {
                ct->SetCoefficient(flows[d][a], -1.0);
            }
        }
    }

    for (int d = 0; d < num_demandes; ++d)
    {
        for (int a : out_arc_indices[demands[d].src])
        {
            objective->SetCoefficient(flows[d][a], demands[d].price);
        }
        for (int a : in_arc_indices[demands[d].src])
        {
            objective->SetCoefficient(flows[d][a], -demands[d].price);
        }
    }

    solver.Solve();
    vector<FlowOnArc> flowArc;
    for (int d = 0; d < num_demandes; ++d)
    {
        for (int a = 0; a < num_arcs; ++a)
        {
            const double flow = flows[d][a]->solution_value();
            flowArc.push_back({a, d, flow});
        }
    }
    return flowArc;
}
