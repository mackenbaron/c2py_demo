R_gas = 8.314472


def V_vap(n, at, ra, derivs, hes):

    T = ra[at[0]]
    P = ra[at[1]]
    V = R_gas * T / P
    if derivs:
        derivs[0] = R_gas / P
        derivs[1] = -R_gas * T / (P * P)
        if hes:
            hes[0] = 0
            hes[1] = -R_gas / (P * P)
            hes[2] = 2 * R_gas * T / (P * P * P)

    at[0] = 7
    return V
