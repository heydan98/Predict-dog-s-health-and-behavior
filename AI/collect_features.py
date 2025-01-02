import numpy as np
import pandas as pd

def grpsum(x, gv):
    unig = np.unique(gv)
    n = len(unig)
    gsums = np.zeros(n)
    gnums = np.zeros(n)
    for ii in range(n):
        gi = (gv == unig[ii])
        gnums[ii] = np.sum(gi)
        gsums[ii] = np.sum(x[gi])
    return gsums, gnums

def collect_features(gfeat, sens_names):
    last_var = 12
    first_beh = 13
    ns = len(sens_names)
    xd = [None] * ns
    fnames = []
    class_data = []
    dog_id = []
    test_num = []

    for i_s in range(ns):
        se_na = sens_names[i_s]
        vnames = gfeat[se_na][0].columns.tolist()
        vn1 = vnames[:last_var]
        ab = vnames[first_beh:]
        for i_dog in range(1, len(gfeat)):
            ab = list(set(ab) & set(gfeat[se_na][i_dog].columns[first_beh:]))
        var_list = vn1 + ab

        td = []
        for i_dog in range(len(gfeat)):
            sti = gfeat[se_na][i_dog].dropna(subset=['Task']).index[0]
            dog = [gfeat.loc[i_dog, 'DogID']] * (len(gfeat[se_na][i_dog]) - sti)
            test_num_i = [gfeat.loc[i_dog, 'TestNum']] * (len(gfeat[se_na][i_dog]) - sti)
            td_i = gfeat[se_na][i_dog].iloc[sti:, var_list].reset_index(drop=True)
            td_i = pd.concat([pd.DataFrame({'Dog': dog, 'TestNum': test_num_i}), td_i], axis=1)
            if 'Galloping' in gfeat[se_na][i_dog].columns:
                td_i['Galloping'] = gfeat[se_na][i_dog].iloc[sti:, gfeat[se_na][i_dog].columns.get_loc('Galloping')]
            else:
                td_i['Galloping'] = 0
            td.append(td_i)
        fd = pd.concat(td, ignore_index=True)
        fd[se_na] = fd

    d = fd['Back']  # Same in all sensors
    b2class = ['Walking', 'Standing', 'Lying_chest', 'Trotting', 'Sitting', 'Galloping', 'Sniffing']
    all_bnam = [b.replace('_', ' ') for b in b2class]
    class_data = d[b2class]

    bind = class_data >= 75
    num_m = bind.sum(axis=1)
    max_p = class_data.max(axis=1)
    n_b = class_data.idxmax(axis=1)
    fd['Labels'] = [''] * len(d)
    fd['NoneInd'] = (num_m == 0)
    fd.loc[fd['NoneInd'], 'Labels'] = 'None'
    fd['OneInd'] = (num_m == 1)
    fd.loc[fd['OneInd'], 'Labels'] = [all_bnam[i] for i in n_b[fd['OneInd']]]
    fd['MultiInd'] = (num_m == 2)
    fd.loc[fd['MultiInd'], 'Labels'] = 'Many'

    pl_ind = fd['OneInd']
    class_var = pd.Categorical(fd.loc[pl_ind, 'Labels'], categories=all_bnam + ['Many', 'None'])
    dog_id = fd.loc[pl_ind, 'Dog']
    test_num = fd.loc[pl_ind, 'TestNum']

    for i_s in range(ns):
        se_na = sens_names[i_s]
        tmp = np.column_stack((
            fd[se_na].loc[pl_ind, 'AstdSum'],
            fd[se_na].loc[pl_ind, 'AMeanV'].values,
            fd[se_na].loc[pl_ind, 'AOff'],
            np.mean(fd[se_na].loc[pl_ind, 'APeakNum'].values, axis=1),
            fd[se_na].loc[pl_ind, 'AecdfXYZ'].values,
            fd[se_na].loc[pl_ind, 'GstdSum'],
            fd[se_na].loc[pl_ind, 'GMeanV'].values,
            fd[se_na].loc[pl_ind, 'GOff'],
            np.mean(fd[se_na].loc[pl_ind, 'GPeakNum'].values, axis=1),
            fd[se_na].loc[pl_ind, 'GecdfXYZ'].values
        ))
        xd[i_s] = tmp

    fn_a = ['ATotAct', 'AMeanX', 'AMeanY', 'AMeanZ', 'AOffset', 'ANMeanCros']
    fn_g = ['GTotAct', 'GMeanX', 'GMeanY', 'GMeanZ', 'GOffset', 'GNMeanCros']
    xyz = 'XYZ'
    ec_a = [[] for _ in range(ns)]
    ec_g = [[] for _ in range(ns)]
    for i_dir in range(ns):
        for ii in range(1, 8):
            ec_a[i_dir].append(f'A{xyz[i_dir]}{ii}')
            ec_g[i_dir].append(f'G{xyz[i_dir]}{ii}')
    fnames = fn_a + sum(ec_a, []) + fn_g + sum(ec_g, [])

    return xd, class_var, fnames, dog_id, test_num