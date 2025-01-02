import numpy as np
import pandas as pd
from scipy.stats import trim_mean

def calc_features_acc_gyro(acc_data, window_len=200, overlap=50, n_ecdf=7):
    fs = 100  # Assuming 100Hz sampling rate
    w = np.ones(window_len) / window_len
    if overlap > 0:
        win_index = int(window_len * (overlap / fs))
    else:
        win_index = window_len
    f_limit = 0.2
    min_rlen = 5
    win_sec = window_len / 100

    var_names = ['VarBack', 'VarNeck']
    sens_names = ['Back', 'Neck']
    n_sensor = len(sens_names)
    feat = acc_data[['DogID', 'TestNum']]

    # Subtract "normal position" == "robust mean of standing"
    acc_names = ['ABack', 'ANeck']
    for i_dog in range(len(acc_data)):
        ind = (acc_data.loc[i_dog, 'sensorData']['Task'] == 'Task stand') & \
              (acc_data.loc[i_dog, 'sensorData']['Behavior'].apply(lambda x: 'Standing' in x))
        for i_m in range(n_sensor):
            sens = acc_names[i_m]
            acc_data.loc[i_dog, 'sensorData'][sens] = acc_data.loc[i_dog, 'sensorData'][sens] - \
                                                      trim_mean(acc_data.loc[i_dog, 'sensorData'][sens][ind], 0.1)

    # Features in window_len windows
    ag = ['A', 'G']  # Accelerometer, Gyro
    for i_sens in range(n_sensor):
        s_name = sens_names[i_sens]
        feat[s_name] = [None] * len(feat)
        for i_dog in range(len(acc_data)):
            n_max = int(len(acc_data.loc[i_dog, 'sensorData']) / win_index)
            n_max_f = n_max - 1

            # Arrange Tasks and Behaviors
            re_task = np.array(acc_data.loc[i_dog, 'sensorData']['Task'][:win_index * n_max]).reshape(win_index, n_max)
            task_inds = ~np.isnan(re_task)
            task = pd.Categorical([np.nan] * n_max, categories=acc_data.loc[i_dog, 'sensorData']['Task'].cat.categories)
            task[task_inds] = re_task[0, task_inds]
            task = task[:n_max_f]

            # Behaviors
            nb = acc_data.loc[i_dog, 'sensorData']['Behavior'].shape[1]
            beh_cats = acc_data.loc[i_dog, 'sensorData']['Behavior'].dtypes.categories
            b_pros = np.zeros((n_max_f, len(beh_cats)))
            for i_b in range(nb):
                tre_b = np.array(acc_data.loc[i_dog, 'sensorData']['Behavior'][:win_index * n_max, i_b]).reshape(win_index, n_max)
                re_b = np.vstack((tre_b[:, :-1], tre_b[:, 1:]))
                for it_step in range(n_max_f):
                    b_tab = pd.Series(re_b[:, it_step]).value_counts()
                    if not b_tab.index.isin(beh_cats).all():
                        raise ValueError('Categories...')
                    b_pros[it_step, :] += b_tab.reindex(beh_cats, fill_value=0).values

            b_pros_matrix = (b_pros / window_len) * 100
            valid_cats = [cat.replace(' ', '_').replace('<', 'LT') for cat in beh_cats]
            behavior = pd.DataFrame(b_pros_matrix, columns=valid_cats)

            t = pd.DataFrame({'Task': task})

            for i_ag in range(2):
                acc_name = ag[i_ag] + s_name

                # Mean and offset from zero
                f_acc = np.convolve(acc_data.loc[i_dog, 'sensorData'][acc_name], w, mode='valid')
                mean_v = f_acc[::win_index]
                off = np.sqrt(np.sum(mean_v ** 2, axis=1))

                # Total activity: Moving std
                mov_var = np.apply_along_axis(lambda x: np.convolve(x ** 2, np.ones(window_len) / window_len, mode='valid'), axis=0,
                                               arr=acc_data.loc[i_dog, 'sensorData'][acc_name] ** 2)
                f_std = np.sum(np.sqrt(mov_var), axis=1)
                std_sum = f_std[::win_index][:n_max_f]

                # ECDF features
                ecdf_xyz = np.zeros((n_max_f, 3 * n_ecdf))
                for it_step in range(n_max_f):
                    ind1 = it_step * win_index
                    ind2 = ind1 + window_len
                    ll = np.zeros((n_ecdf, 3))
                    for xyz in range(3):
                        f, x = np.array(acc_data.loc[i_dog, 'sensorData'][acc_name][ind1:ind2, xyz]).ecdf()
                        ll[:, xyz] = np.interp(np.linspace(0, 1, n_ecdf), f, x, left=np.nan, right=np.nan)
                    ecdf_xyz[it_step, :] = ll.ravel()

                # "Frequency" features
                peak_num = np.zeros((n_max_f, 3))
                for it_step in range(n_max_f):
                    ind1 = it_step * win_index
                    ind2 = ind1 + window_len
                    for xyz in range(3):
                        x_win = acc_data.loc[i_dog, 'sensorData'][acc_name][ind1:ind2, xyz]
                        rl_g, r_num = run_length(x_win > mean_v[it_step, xyz] + f_limit)
                        peak_num[it_step, xyz] = np.sum(rl_g >= min_rlen) / win_sec

                t[f'{ag[i_ag]}MeanV'] = mean_v[:n_max_f]
                t[f'{ag[i_ag]}Off'] = off[:n_max_f]
                t[f'{ag[i_ag]}stdSum'] = std_sum
                t[f'{ag[i_ag]}ecdfXYZ'] = list(ecdf_xyz)
                t[f'{ag[i_ag]}PeakNum'] = list(peak_num)

            feat.loc[i_dog, s_name] = pd.concat([t, behavior], axis=1)

    return feat

def run_length(x):
    """
    Helper function to compute run lengths and run numbers for a boolean vector.
    """
    rl = np.r_[0, x[1:] != x[:-1].astype(int)]
    rnum = np.cumsum(rl)
    rl_g = np.diff(np.flatnonzero(rl))
    return rl_g, rnum[:-1]