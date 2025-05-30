
def in_contention (high, low):
    
    if high == 1 and low == 1:
        return False
    
    if 2 <= high <= 4 and 2 <= low <= 4:
        return False
    
    if 5 <= high <= 6 and 5 <= low <= 6:
        return False
    
    if high == 7 and low == 7:
        return False
    
    if 18 <= high <= 20 and 18 <= low <= 20:
        return False
    
    if 8 <= high <= 17 and 8 <= low <= 17:
        return False
    
    return True



def intensity_df(last_round_matches, ranking): 
    ranking["points_before"] = ranking.groupby(["team", "run"])["points"].shift(1).fillna(0).astype(int)

    df = last_round_matches.merge(
        ranking.rename(columns={
            'team': 'home_team',
            'points_before': 'home_points_before'
        }),
        on=['run', 'round', 'home_team'],
        how='left'
    )

    # merge points_before for the away team
    df = df.merge(
        ranking.rename(columns={
            'team': 'away_team',
            'points_before': 'away_points_before'
        }),
        on=['run', 'round', 'away_team'],
        how='left'
    )
    df = df[["run", "round", "home_team", "away_team", "home_points_before", "away_points_before"]]
    df["intensity"] = abs(df["home_points_before"] - df["away_points_before"])
    return df 


def uncertainty_df(last_round_matches, ranking): 
    ranking["rating_before"] = ranking.groupby(["team", "run"])["rating"].shift(1).fillna(0).astype(int)

    df = last_round_matches.merge(
        ranking.rename(columns={
            'team': 'home_team',
            'rating_before': 'home_rating_before'
        }),
        on=['run', 'round', 'home_team'],
        how='left'
    )

    # merge rating_before for the away team
    df = df.merge(
        ranking.rename(columns={
            'team': 'away_team',
            'rating_before': 'away_rating_before'
        }),
        on=['run', 'round', 'away_team'],
        how='left'
    )
    df = df[["run", "round", "home_team", "away_team", "home_rating_before", "away_rating_before"]]
    df["uncertainty"] = abs(df["home_rating_before"] - df["away_rating_before"])
    return df 
    






