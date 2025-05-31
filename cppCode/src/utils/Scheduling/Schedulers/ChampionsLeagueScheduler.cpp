#include "utils/Scheduling/Schedulers/ChampionsLeagueScheduler.h"
#include "tournament/Tournament.h"

#include "utils/Metrics/Bounds.h"
#include "config/CplexConfig.h"

void ChampionsLeagueScheduler::setInitialSchedule(Tournament &tournament, std::mt19937 &randomNumberGenerator)  {

    std::vector<std::tuple<int,int>> matchPairings = calculateMatchPairings(tournament, randomNumberGenerator);
    setMatchPairings(matchPairings);

    const IloEnv env;
    const IloModel model(env);

    std::vector<std::vector<std::vector<std::vector<IloNumVar>>>> x = initializeTimeVariables(env, 8, tournament);
    addOneGamePerWeekConstraint(env, model, x, tournament);
    addEachMatchUpOnlyOnceConstraint(env, model, x, tournament, matchPairings);
    addNineMatchesPerWeekConstraint(env, model, x, tournament, matchPairings);
    addOneHomeGameFirstTwoMatches(env, model, x, tournament, matchPairings);
    addOneHomeGameLastTwoMatches(env, model, x, tournament, matchPairings);
    addMaxTwoAwayGamesInARow(env, model, x, tournament, matchPairings);
    addMaxTwoHomeGamesInARow(env, model, x, tournament, matchPairings);
    addMilanMadridConstraint(env, model, x, tournament, matchPairings);

    IloCplex cplex(model);
    cplex.setOut(env.getNullStream());
    cplex.setParam(IloCplex::Param::Threads, CplexConfig::getNumThreads());

    if(cplex.solve()) {
        std::cout<<"solution for the champions league pairing found" << std::endl;
        tournament.setFirstTimeTable(extractTableFromSolutionChampionsLeague(cplex, x, tournament.getNumberRounds(), tournament));

        if(tournament.getNumberDynamicRounds() != 0) {

            TimeTable lastRound = tournament.getFirstTimeTable();

            lastRound.getRounds().erase(lastRound.getRounds().begin(), lastRound.getRounds().begin()+7);

            tournament.setSecondTimeTable(lastRound);
            tournament.getFirstTimeTable().getRounds().pop_back();
            tournament.setCompleteTimeTable();

        }

        else {
            tournament.setCompleteTimeTable();
        }


    }

    else(std::cout<<"no solution found");

}


void ChampionsLeagueScheduler::setSecondSchedule(Tournament &tournament) const {

    // get the edges
    auto matchesWithoutLastRound = getPlayedMatchesExcludingLastRound(tournament);

    std::vector<std::tuple<int,int>> edges = getEdges(&tournament, matchesWithoutLastRound);
    std::vector<Round> allRounds = generateRounds(Round(), std::vector<bool>(tournament.getNumberTeams(), false), 0, &tournament, edges);

    int bestSolution = std::numeric_limits<int>::max(); // best solution so far
    int index = 0; // index of the current best last round solution
    int counter = 0;

    // Go over all the rounds and check where there are the least a mount of irrelevant matches
    for(auto& round: allRounds) {

        tournament.getSecondTimeTable().setRound(0, round);
        tournament.setCompleteTimeTable();

        // calculate the number of irrelevant matches
        int irrelevantMatches = 0;

        // Go over all matches
        for(auto& match: round.getMatches()) {

            const int homeBest = Bounds::solveBestCaseRank(tournament, match.getHomeTeam(), 1);
            const int homeWorst = Bounds::solveWorstCaseRank(tournament, match.getHomeTeam(), 1);
            const int awayBest = Bounds::solveBestCaseRank(tournament, match.getAwayTeam(), 1);
            const int awayWorst = Bounds::solveWorstCaseRank(tournament, match.getAwayTeam(), 1);

            const bool homeDecided = Bounds::teamIsDecidedChampionsLeague(homeBest, homeWorst);
            const bool awayDecided = Bounds::teamIsDecidedChampionsLeague(awayBest, awayWorst);

            if(homeDecided && awayDecided) irrelevantMatches++;
        }

        if(irrelevantMatches < bestSolution) {
            index = counter;
            bestSolution = irrelevantMatches;

            if(bestSolution == 0) {
                tournament.getSecondTimeTable().setRound(0, allRounds[index]);
                return; // return if we have the best solution
            }

        }
        counter++;
    }

    // set the schedule
    tournament.getSecondTimeTable().setRound(0, allRounds[index]);

}


std::vector<std::tuple<int, int> > ChampionsLeagueScheduler::getEdges(Tournament *tournament, const std::vector<std::tuple<int,int>>& matches) {

    std::vector<std::tuple<int,int>> edges;
    const std::vector<Match> lastRound = tournament->getSecondTimeTable().getMatchesInRound(0);

    std::map<int, int> lastRoundPot;           // teamIndex -> opponent pot
    std::map<int, bool> lastRoundHomeGame;     // teamIndex -> needs home?
    std::map<int, bool> secured;               // teamIndex -> still relevant?

    // Extract info from scheduled last round matches
    for (const auto& match : lastRound) {

        int homeIndex = tournament->getTeamManager()->getTeamIndex(match.getHomeTeam());
        int awayIndex = tournament->getTeamManager()->getTeamIndex(match.getAwayTeam());

        lastRoundPot[homeIndex] = match.getAwayTeam()->getPotNumber();
        lastRoundPot[awayIndex] = match.getHomeTeam()->getPotNumber();

        lastRoundHomeGame[homeIndex] = true;
        lastRoundHomeGame[awayIndex] = false;
    }

    for(int i = 0; i < tournament->getNumberTeams(); i++) {
        for(int j = i+1; j < tournament->getNumberTeams(); j++) {

            // first condition
            if(lastRoundPot[i] == tournament->getTeamManager()->getTeam(j)->getPotNumber() && lastRoundPot[j] == tournament->getTeamManager()->getTeam(i)->getPotNumber()) {

                // second and third condition
                if(lastRoundHomeGame[i] != lastRoundHomeGame[j] && tournament->getTeamManager()->getTeams()[i]->getAssociation() != tournament->getTeamManager()->getTeams()[j]->getAssociation() && !isPartOfMatches(i,j, matches) && !isPartOfMatches(j,i,matches)) {

                    if(lastRoundHomeGame[i]) {
                        edges.emplace_back(i, j);
                    } else {
                        edges.emplace_back(j, i);
                    }
                }
            }
        }
    }
    return edges;
}


std::vector<Round> ChampionsLeagueScheduler::generateRounds(Round current,std::vector<bool> used,int idx,Tournament *tournament, std::vector<std::tuple<int, int>> edges) {

    std::vector<Round> allRounds;

    if (current.getMatches().size() == tournament->getNumberTeams() / 2) {
        allRounds.push_back(current);
        return allRounds;
    }

    for (int i = idx; i < edges.size(); ++i) {
        auto [home, away] = edges[i];

        if (home >= tournament->getNumberTeams() || away >= tournament->getNumberTeams()) {
            continue;
        }

        if (used[home] || used[away]) continue;

        // Mark teams as used and add the match
        used[home] = used[away] = true;

        // make the match
        Match match = Match(
            tournament->getTeamManager()->getTeam(home),
            tournament->getTeamManager()->getTeam(away)
        );

        match.setPointsHomeTeamBeforeMatch(tournament->getTeamManager()->getTeam(home)->getPoints());
        match.setPointsAwayTeamBeforeMatch(tournament->getTeamManager()->getTeam(away)->getPoints());
        match.setRatingHomeTeamBeforeMatch(tournament->getTeamManager()->getTeam(home)->getRating());
        match.setRatingAwayTeamBeforeMatch(tournament->getTeamManager()->getTeam(away)->getRating());

        current.addMatch(match);


        auto nextRounds = generateRounds(current, used, i + 1, tournament, edges);
        allRounds.insert(allRounds.end(), nextRounds.begin(), nextRounds.end());

        current.dropMatch();
        used[home] = used[away] = false;
    }
    return allRounds;
}




// Constraint that makes sure that each team has one home game against every pot
std::vector<std::vector<IloNumVar> > ChampionsLeagueScheduler::initializeMatchVariables(const IloEnv &env, int numberRounds, const Tournament &tournament) {

    // Match variable here is 1 if a team has a home game against another team
    std::vector<std::vector<IloNumVar>> x (tournament.getNumberTeams());

    // loop through the teams
    for(int i = 0; i < tournament.getNumberTeams(); i++) {
        x[i].resize(tournament.getNumberTeams());

        for(int j = 0; j < tournament.getNumberTeams(); j++) {
            x[i][j] = IloNumVar(env, 0, 1, ILOBOOL);
        }
    }
    return x;
}



void ChampionsLeagueScheduler::playOnceHomePot(const IloEnv &env, const IloModel &model, const std::vector<std::vector<IloNumVar>> &x,  const Tournament &tournament) {

    // Go over all teams
    for (const auto& team: tournament.getTeamManager()->getTeams()) {

        // Loop through all pots
        for (int pot = 1; pot <= 4; ++pot) {
            const auto potTeams = tournament.getTeamManager()->getPotTeams(pot);

            IloExpr expr(env);

            for (const auto& otherTeam : potTeams) {
                if (team != otherTeam) {
                    expr += x[tournament.getTeamManager()->getTeamIndex(team)][tournament.getTeamManager()->getTeamIndex(otherTeam)]; // team plays at home against otherTeam
                }
            }

            model.add(expr == 1); // Play exactly one home game against each team in every pot
            expr.end();
        }
    }
}

void ChampionsLeagueScheduler::playOnceAwayPot(const IloEnv &env, const IloModel &model, const std::vector<std::vector<IloNumVar>> &x,  const Tournament &tournament) {


    // Go over all teams
    for (const auto& team: tournament.getTeamManager()->getTeams()) {

        // Loop through all pots
        for (int pot = 1; pot <= 4; ++pot) {

            const auto potTeams = tournament.getTeamManager()->getPotTeams(pot);

            IloExpr expr(env);

            for (const auto& otherTeam : potTeams) {
                if (team != otherTeam) {
                    expr += x[tournament.getTeamManager()->getTeamIndex(otherTeam)][tournament.getTeamManager()->getTeamIndex(team)]; // team plays at home against otherTeam
                }
            }
            model.add(expr == 1); // Play exactly one Away Game against each team in every pot
            expr.end();
        }
    }
}


void ChampionsLeagueScheduler::playFourHomeGames(const IloEnv &env, const IloModel &model, const std::vector<std::vector<IloNumVar> > &x, const Tournament &tournament) {

    for(int i = 0; i < tournament.getNumberTeams(); ++i) {

        IloExpr expr(env);

        for(int j = 0; j < tournament.getNumberTeams(); ++j) {

            if(i!=j) {
                expr += x[i][j]; // team i plays at home against team j
            }
        }
        model.add(expr == 4);
        expr.end();
    }
}

void ChampionsLeagueScheduler::playFourAwayGames(const IloEnv &env, const IloModel &model, const std::vector<std::vector<IloNumVar> > &x, const Tournament &tournament) {

    for(int i = 0; i < tournament.getNumberTeams(); ++i) {

        IloExpr expr(env);

        for(int j = 0; j < tournament.getNumberTeams(); ++j) {

            if(i!=j) {
                expr += x[j][i]; // team i plays at home against team j
            }
        }
        model.add(expr == 4);
        expr.end();
    }
}


void ChampionsLeagueScheduler::playMaxOnce(const IloEnv &env, const IloModel &model, const std::vector<std::vector<IloNumVar>> &x, const Tournament &tournament) {

    const int numberTeams = tournament.getNumberTeams();

    // For every unique pair of teams (i, j), ensure they only play once
    for (int i = 0; i < numberTeams; ++i) {
        for (int j = i + 1; j < numberTeams; ++j) {
            IloExpr expr(env);
            expr += x[i][j] + x[j][i];  // Either team i plays team j or vice versa
            model.add(expr <= 1);   // But not both (and not more than once)
        }
    }
}

bool ChampionsLeagueScheduler::isPartOfMatches(int team1, int team2, std::vector<std::tuple<int, int>> matches) {
    return std::find(matches.begin(), matches.end(), std::make_tuple(team1, team2)) != matches.end();
}


std::vector<std::tuple<int, int>> ChampionsLeagueScheduler::getPlayedMatchesExcludingLastRound(Tournament& tournament) {

    std::vector<std::tuple<int, int>> matches;
    const int lastRoundIndex = tournament.getNumberRounds() - 1;

    for (int round = 0; round < lastRoundIndex; ++round) {
        const auto& roundMatches = tournament.getFirstTimeTable().getMatchesInRound(round);

        for (const Match& match : roundMatches) {
            int homeIndex = tournament.getTeamManager()->getTeamIndex(match.getHomeTeam());
            int awayIndex = tournament.getTeamManager()->getTeamIndex(match.getAwayTeam());
            matches.emplace_back(homeIndex, awayIndex);
        }
    }
    return matches;
}


#include <random>

void ChampionsLeagueScheduler::addObjectiveFunction(const IloEnv &env, const IloModel &model, const std::vector<std::vector<IloNumVar>> &x, const Tournament &tournament, std::mt19937 &randomNumberGenerator) {
    IloExpr objective(env);
    const int numberTeams = tournament.getNumberTeams();

    // Set up a better random number generator
    const double randomNumber = std::uniform_real_distribution<>(0.0, 1.0)(randomNumberGenerator);

    for (int i = 0; i < numberTeams; ++i) {
        for (int j = 0; j < numberTeams; ++j) {
            if (i != j) {
                objective += randomNumber * x[i][j];
            }
        }
    }
    model.add(IloMaximize(env, objective));
    objective.end();
}

void ChampionsLeagueScheduler::associationConstraint(const IloEnv &env, const IloModel &model, const std::vector<std::vector<IloNumVar> > &x, const Tournament &tournament) {

    const int numberOfTeams = tournament.getNumberTeams();

    // Keep track of which pairs have been handled to avoid duplicates
    std::set<std::pair<int, int>> handled;

    for(int i = 0; i < numberOfTeams; i++) {
        const auto team1 = tournament.getTeamManager()->getTeam(i);
        const auto association1 = team1->getAssociation();
        const auto associationTeams = tournament.getTeamManager()->getTeamsFromAssociation(association1);

        for(const auto& team2: associationTeams) {
            if(team1 != team2) {
                const int j = tournament.getTeamManager()->getTeamIndex(team2);

                // Avoid duplicate pairs
                if(i < j && handled.count({i, j}) == 0) {
                    IloExpr expr(env);
                    expr += x[i][j] + x[j][i];
                    model.add(expr == 0); // no match between teams from the same association
                    expr.end();
                    handled.insert({i, j});
                }
            }
        }
    }
}


std::vector<std::tuple<int, int>> ChampionsLeagueScheduler::calculateMatchPairings(const Tournament& tournament, std::mt19937 &randomNumberGenerator) {
    std::vector<std::tuple<int,int>> matches;
    IloEnv env;
    IloModel model(env);
    IloCplex cplex(model);
    cplex.setOut(env.getNullStream());
    cplex.setParam(IloCplex::Param::Threads, CplexConfig::getNumThreads());
    std::vector<std::vector<IloNumVar>> x;

    try {
        // Initialize match variables
        x = initializeMatchVariables(env, 8, tournament);

        // Add constraints
        playOnceHomePot(env, model, x, tournament);
        playOnceAwayPot(env, model, x, tournament);
        playMaxOnce(env, model, x, tournament);
        associationConstraint(env, model, x, tournament);
        addObjectiveFunction(env, model, x, tournament, randomNumberGenerator);

        cplex.setOut(std::cout);

        if (cplex.solve()) {
            std::cout << "solution found" << std::endl;

            for (int i = 0; i < tournament.getNumberTeams(); ++i) {
                for (int j = 0; j < tournament.getNumberTeams(); ++j) {
                    if (i != j && cplex.getValue(x[i][j]) >= 0.95) {
                        matches.emplace_back(i, j);
                    }
                }
            }
        } else {
            std::cout << "no solution found" << std::endl;
        }

    } catch (IloException& e) {
        std::cerr << "Concert exception caught: " << e.getMessage() << std::endl;
    }

    try {
        env.end();
    } catch (IloException& e) {
        std::cerr << "Exception during env.end(): " << e.getMessage() << std::endl;
    }

    return matches;
}


std::vector<std::vector<std::vector<std::vector<IloNumVar>>>> ChampionsLeagueScheduler::initializeTimeVariables(const IloEnv &env, int numberRounds, const Tournament &tournament) {

    std::vector<std::vector<std::vector<std::vector<IloNumVar>>>> x(tournament.getNumberTeams());

    for(int i = 0; i < tournament.getNumberTeams(); i++) {
        x[i].resize(tournament.getNumberTeams());

        for(int j = 0; j < tournament.getNumberTeams(); j++) {

            x[i][j].resize(8);

            for(int k = 0; k < 8; k++) {

                x[i][j][k].resize(2);

                for(int l = 0; l < 2; l++) {
                    x[i][j][k][l] = IloNumVar(env, 0, 1, ILOBOOL);
                }
            }
        }
    }
    return x;
}


void ChampionsLeagueScheduler::addOneGamePerWeekConstraint(const IloEnv &env, const IloModel &model, std::vector<std::vector<std::vector<std::vector<IloNumVar>>>> &x, const Tournament &tournament) {

    const int numberTeams = tournament.getNumberTeams();
    const int numberWeeks = 8;
    const int numberDaysPerWeek = 2;

    for (int t = 0; t < numberTeams; ++t) {
        for (int w = 0; w < numberWeeks; ++w) {

            IloExpr expr(env);

            for (int s = 0; s < numberTeams; ++s) {
                if (s == t) continue;

                for (int d = 0; d < numberDaysPerWeek; ++d) {
                    // Home game: team t hosts s
                    expr += x[t][s][w][d];
                    // Away game: team s hosts t
                    expr += x[s][t][w][d];
                }
            }
            model.add(expr == 1); // Team t must play exactly once in week w
            expr.end();

        }
    }
}


void ChampionsLeagueScheduler::addEachMatchUpOnlyOnceConstraint(const IloEnv &env, const IloModel &model, std::vector<std::vector<std::vector<std::vector<IloNumVar> > > > &x, const Tournament &tournament, std::vector<std::tuple<int,int>> matches) {

    // Go over the matches
    for (const auto& match: matches) {

        // Get the home team
        const int homeTeam = std::get<0>(match);
        const int awayTeam = std::get<1>(match);

        IloExpr expr(env);

        // go over the weeks and days
        for(int i = 0; i  < 8; i++) {
            for(int j = 0; j < 2; j++) {
                expr += x[homeTeam][awayTeam][i][j];
            }
        }
        model.add(expr == 1);
    }
}

void ChampionsLeagueScheduler::addNineMatchesPerWeekConstraint(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<std::vector<IloNumVar>>>>& x, const Tournament& tournament, const std::vector<std::tuple<int,int>>& matches) {

    const int numWeeks = 8;
    const int numDays = 2;
    const int numTeams = tournament.getNumberTeams();

    for(int w = 0; w < numWeeks; ++w) {
        for(int d = 0; d < numDays; ++d) {

            IloExpr expr(env);

            for(int t = 0; t < numTeams; ++t) {
                for(int s = 0; s < numTeams; ++s) {

                    if(t != s && isPartOfMatches(t, s, matches)) {
                        expr += x[t][s][w][d];
                    }
                }
            }
            model.add(expr == 9);
            expr.end(); // Always free the expression to avoid memory leaks
        }
    }
}

void ChampionsLeagueScheduler::addOneHomeGameFirstTwoMatches(const IloEnv &env, const IloModel &model, std::vector<std::vector<std::vector<std::vector<IloNumVar> > > > &x, const Tournament &tournament, std::vector<std::tuple<int, int> > matches) {

    for(int i = 0; i < tournament.getNumberTeams(); i++) {

        IloExpr expr(env);

        for(int j = 0; j < tournament.getNumberTeams(); j++) {

            for(int day = 0; day < 2; day++) {
                if(i != j && isPartOfMatches(i,j,matches)) {
                    expr += x[i][j][0][day] + x[i][j][1][day];
                }
            }
        }
        model.add(expr == 1);
        expr.end();
    }
}


void ChampionsLeagueScheduler::addOneHomeGameLastTwoMatches(const IloEnv &env, const IloModel &model, std::vector<std::vector<std::vector<std::vector<IloNumVar> > > > &x, const Tournament &tournament, std::vector<std::tuple<int, int> > matches) {

    for(int i = 0; i < tournament.getNumberTeams(); i++) {

        IloExpr expr(env);

        for(int j = 0; j < tournament.getNumberTeams(); j++) {
            for(int day = 0; day < 2; day++) {
                if(i != j && isPartOfMatches(i,j,matches)) {
                    expr += x[i][j][6][day] + x[i][j][7][day];
                }
            }
        }
        model.add(expr == 1);
        expr.end();
    }
}

void ChampionsLeagueScheduler::addMaxTwoHomeGamesInARow(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<std::vector<IloNumVar>>>>& x, const Tournament& tournament, const std::vector<std::tuple<int,int>>& matches) {

    const int numberTeams = tournament.getNumberTeams();
    const int totalWeeks = 8; // Assuming there are 8 weeks in total

    for (int i = 0; i < numberTeams; i++) {

        for (int week = 0; week <= totalWeeks - 3; week++) {
            IloExpr expr(env);

            for (int offset = 0; offset < 3; offset++) {

                const int currentWeek = week + offset;

                for (int j = 0; j < numberTeams; j++) {

                    if (i != j && isPartOfMatches(i, j, matches)) {

                        for (int day = 0; day < 2; day++) {
                            expr += x[i][j][currentWeek][day];
                        }
                    }
                }
            }

            model.add(expr <= 2);
            expr.end();
        }
    }
}


void ChampionsLeagueScheduler::addMaxTwoAwayGamesInARow(const IloEnv& env, const IloModel& model, const std::vector<std::vector<std::vector<std::vector<IloNumVar>>>>& x, const Tournament& tournament, const std::vector<std::tuple<int,int>>& matches) {

    const int numberTeams = tournament.getNumberTeams();
    const int totalWeeks = 8; // Assuming there are 8 weeks in total

    for (int i = 0; i < numberTeams; i++) {

        for (int week = 0; week <= totalWeeks - 3; week++) {
            IloExpr expr(env);

            for (int offset = 0; offset < 3; offset++) {

                const int currentWeek = week + offset;

                for (int j = 0; j < numberTeams; j++) {

                    if (i != j && isPartOfMatches(i, j, matches)) {

                        for (int day = 0; day < 2; day++) {
                            expr += x[j][i][currentWeek][day];
                        }
                    }
                }
            }

            model.add(expr <= 2);
            expr.end();
        }
    }
}



void ChampionsLeagueScheduler::addMilanMadridConstraint(const IloEnv& env,const IloModel& model,const std::vector<std::vector<std::vector<std::vector<IloNumVar>>>>& x,const Tournament& tournament,const std::vector<std::tuple<int, int>>& matches){

    const int realMadridIndex = tournament.getTeamManager()->getTeamIndex("Real Madrid");
    const int atleticoMadridIndex = tournament.getTeamManager()->getTeamIndex("Atletico Madrid");
    const int milanIndex = tournament.getTeamManager()->getTeamIndex("AC Milan");
    const int interIndex = tournament.getTeamManager()->getTeamIndex("Inter");

    const int numWeeks = 8;
    const int numDays = 2;
    const int lastWeek = numWeeks - 1;

    // ---------- T8: Milan and Inter cannot both host on same gameday ----------
    for (int week = 0; week < numWeeks; ++week) {

        for (int day = 0; day < numDays; ++day) {
            IloExpr expr(env);

            for (int opponent = 0; opponent < tournament.getNumberTeams(); ++opponent) {
                if (opponent != milanIndex && isPartOfMatches(milanIndex, opponent, matches)) {
                    expr += x[milanIndex][opponent][week][day];
                }
                if (opponent != interIndex && isPartOfMatches(interIndex, opponent, matches)) {
                    expr += x[interIndex][opponent][week][day];
                }
            }

            model.add(expr <= 1);
            expr.end();
        }
    }

    // ---------- T9: Real and Atletico cannot both host on same gameday ----------
    for (int week = 0; week < numWeeks; ++week) {
        for (int day = 0; day < numDays; ++day) {
            IloExpr expr(env);

            for (int opponent = 0; opponent < tournament.getNumberTeams(); ++opponent) {
                if (opponent != realMadridIndex && isPartOfMatches(realMadridIndex, opponent, matches)) {
                    expr += x[realMadridIndex][opponent][week][day];
                }
                if (opponent != atleticoMadridIndex && isPartOfMatches(atleticoMadridIndex, opponent, matches)) {
                    expr += x[atleticoMadridIndex][opponent][week][day];
                }
            }

            model.add(expr <= 1);
            expr.end();
        }
    }

    // ---------- T10: Milan and Inter cannot both host in last match week ----------
    {
        IloExpr expr(env);

        for (int opponent = 0; opponent < tournament.getNumberTeams(); ++opponent) {
            for (int day = 0; day < numDays; ++day) {
                if (opponent != milanIndex && isPartOfMatches(milanIndex, opponent, matches)) {
                    expr += x[milanIndex][opponent][lastWeek][day];
                }
                if (opponent != interIndex && isPartOfMatches(interIndex, opponent, matches)) {
                    expr += x[interIndex][opponent][lastWeek][day];
                }
            }
        }

        model.add(expr <= 1);
        expr.end();
    }

    // ---------- T11: Real and Atletico cannot both host in last match week ----------
    {
        IloExpr expr(env);

        for (int opponent = 0; opponent < tournament.getNumberTeams(); ++opponent) {
            for (int day = 0; day < numDays; ++day) {
                if (opponent != realMadridIndex && isPartOfMatches(realMadridIndex, opponent, matches)) {
                    expr += x[realMadridIndex][opponent][lastWeek][day];
                }
                if (opponent != atleticoMadridIndex && isPartOfMatches(atleticoMadridIndex, opponent, matches)) {
                    expr += x[atleticoMadridIndex][opponent][lastWeek][day];
                }
            }
        }
        model.add(expr <= 1);
        expr.end();
    }
}








