#include "menu.h"
#include "ortools/linear_solver/linear_solver.h"

using namespace operations_research;

/**
 * ajr : [calories => 2000, proteins => 150, lipides => 40]
 * apports par ingredients : [ riz => [400,10,4], poulet => [120,40,2], avocat => [230,4,20]]
 * prix par ingreddients : [100DA,650DA,700DA] 
 * goal minimiser => 100 * riz + 650 * poulet + 700 * avocat 
 * constraints:
 *  calories : riz * 400 + poulet * 120 + avocat * 230 >= 2000 
 *  protein : riz * 10 + poulet * 40 + avocat * 4 >= 150 
 *  lipides : riz * 4 + poulet * 2 + avocat * 20 >= 60 
 */

vector<double> MenuMoinsCher(const vector<double> &ajr,
                             const vector<vector<double>> &apports_par_ingredient,
                             const vector<double> &prix_par_ingredient)
{
  MPSolver solver("Mon solveur", MPSolver::GLOP_LINEAR_PROGRAMMING);
  const int num_ingredients = apports_par_ingredient.size();
  const int num_elements = ajr.size();

  const double infinity = solver.infinity();
  vector<MPVariable *> ingredients;
  vector<MPConstraint *> constraints;
  MPObjective *const goal = solver.MutableObjective();

  // generating variables and setting goal
  for (size_t i = 0; i < num_ingredients; i++)
  {
    ingredients.push_back(solver.MakeNumVar(0.0, infinity, "ingredient_" + std::to_string(i)));
    goal->SetCoefficient(ingredients.at(i), prix_par_ingredient.at(i));
  }

  // generating constraints
  for (size_t ajr_value = 0; ajr_value < num_elements; ajr_value++)
  {
    constraints.push_back(solver.MakeRowConstraint(ajr.at(ajr_value), infinity));
    for (size_t ingredient = 0; ingredient < num_ingredients; ingredient++)
    {
      constraints.at(ajr_value)->SetCoefficient(ingredients.at(ingredient),
                                                apports_par_ingredient.at(ingredient)
                                                    .at(ajr_value));
    }
  }

  goal->SetMinimization();

  solver.Solve();

  vector<double> weights;
  for (size_t ingredient = 0; ingredient < num_ingredients; ingredient++)
  {
    weights.push_back(ingredients.at(ingredient)->solution_value());
  }
  return weights;
}
