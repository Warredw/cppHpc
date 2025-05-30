#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <ilcplex/ilocplex.h>
#include <vector>
#include "utils/Scheduling/Strategies/SchedulingStrategy.h"
#include "entities/TimeTable.h"
class Tournament;


class Scheduler {

protected:
    SchedulingStrategy* schedulingStrategy; //This is an IP minimalization/maximization for the dynamic rounds

public:
    explicit Scheduler();
    virtual ~Scheduler();

    // Functions that need to be overridden
    virtual void setInitialSchedule(Tournament& tournament, std::mt19937& randomNumberGenerator)  = 0; // = 0 means that subclasses are forced to override this function
    virtual void setSecondSchedule(Tournament& tournament) const = 0; // override


    // General
    void setSchedulingStrategy(SchedulingStrategy* schedulingStrategy);  // universal (= some heuristic to make the second half schedule based on a weight of matches that we will train with our model)

    // General Constraints
    static std::vector<std::vector<std::vector<IloNumVar>>> initializeDecisionVariables(const IloEnv& env, int numberRounds, const Tournament& tournament); // universal
    static void addPlayMaxOnceConstraint(const IloEnv& env, const IloModel& model , const std::vector<std::vector<std::vector<IloNumVar>>>&x, int numberRounds, const Tournament& tournament); // can be used in different types of tournaments
    static void addOneGamePerRoundConstraint(const IloEnv& env, const IloModel& model , const std::vector<std::vector<std::vector<IloNumVar>>>&x, int numberRounds, const Tournament& tournament); // universal
    virtual void addNoRepeatOpponentsConstraint(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>& x, Tournament& tournament)const; // can be used in different types of tournaments
    virtual void addHomeAwayConstraint(const IloEnv& env, const IloModel& model , const std::vector<std::vector<std::vector<IloNumVar>>>& x, int numberRounds, const Tournament& tournament) const; // really dependent (can be overridden)


    //virtual void addHomeAwayConstraintForSecondSchedule(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<IloNumVar>>>& x, Tournament& tournament) const; // really dependent


    // PremierLeague Constraints
    // Just place definition here and implementation in the sub class


    // Extract TimeTable
    virtual TimeTable extractTableFromSolution(const IloCplex& cplex, const std::vector<std::vector<std::vector<IloNumVar>>>& x, int numberRounds, Tournament& tournament) const; // universal
    // Extract TimeTable ChampionsLeague
    virtual TimeTable extractTableFromSolutionChampionsLeague(const IloCplex& cplex, const std::vector<std::vector<std::vector<std::vector<IloNumVar>>>>& x, int numberRounds, Tournament& tournament) const; // Because here we make a difference in terms of days
};

#endif //SCHEDULER_H
