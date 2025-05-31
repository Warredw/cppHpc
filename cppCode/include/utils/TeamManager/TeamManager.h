#ifndef TEAMMANAGER_H
#define TEAMMANAGER_H

#include "entities/Team.h"
#include <vector>
#include <memory>

class Tournament;
class TeamManager {

protected:
  std::vector<std::shared_ptr<Team>> teams;

public:
  explicit TeamManager();
  virtual ~TeamManager() = default;

  // Getters
  [[nodiscard]] std::shared_ptr<Team> getTeam(int index) const;
  std::vector<std::shared_ptr<Team>> getTeams();

  [[nodiscard]] int getTeamIndex(const std::shared_ptr<Team>& team) const;
  [[nodiscard]] int getTeamIndex(const std::string& name) const;
  [[nodiscard]] int getNumberOfTeams() const;

  [[nodiscard]] std::vector<std::shared_ptr<Team>> getPotTeams(int potNumber) const; // get the teams from a specific pot
  [[nodiscard]] std::vector<std::shared_ptr<Team>> getTeamsFromAssociation(const std::string& association) const; // get the teams from a specific association


  // Other methods
  [[nodiscard]] bool teamIsWinner(const std::shared_ptr<Team>& team) const;
  void resetTeams() const;

  void changeTeamRatings(const std::string& distribution, int value) const;



};

#endif //TEAMMANAGER_H
