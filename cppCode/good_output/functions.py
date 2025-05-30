
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








