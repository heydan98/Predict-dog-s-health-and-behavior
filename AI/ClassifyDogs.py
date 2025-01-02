import numpy as np
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis, QuadraticDiscriminantAnalysis
from sklearn.svm import SVC
from sklearn.multiclass import OneVsRestClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import cross_val_predict
from sklearn.metrics import confusion_matrix



def classify_dogs_svm(xd, sens_names, class_var, relief_vars, cv_partition, rrf, dog_id):
    ns = len(sens_names)
    res = {'label': {}, 'confMat': {}, 'CorrRate': {}, 'DogRate': {}}

    for i_s in range(ns):
        se_na = sens_names[i_s]
        dataset = StandardScaler().fit_transform(xd[i_s][:, relief_vars[i_s][rrf.fsV.SVM[i_s]]])
        svm_model = OneVsRestClassifier(SVC(kernel='rbf')).fit(dataset, class_var)
        label = cross_val_predict(svm_model, dataset, class_var, cv=cv_partition)
        conf_mat = confusion_matrix(class_var, label)
        corr_rate = np.trace(conf_mat) / np.sum(conf_mat)
        correct = (class_var == label)
        sums, nums = np.bincount(dog_id[correct], weights=correct)
        dog_rate = sums / nums

        res['label']['SVM'] = {i_s: label}
        res['confMat']['SVM'] = {i_s: conf_mat}
        res['CorrRate']['SVM'] = {i_s: corr_rate}
        res['DogRate']['SVM'] = {i_s: dog_rate}

    return res

def classify_dogs_da(xd, sens_names, class_var, relief_vars, cv_partition, rrf, dog_id):
    ns = len(sens_names)
    da_type = ['linear', 'quadratic']
    res = {'label': {}, 'confMat': {}, 'CorrRate': {}, 'DogRate': {}}

    for i_s in range(ns):
        se_na = sens_names[i_s]
        for i_da in range(2):
            da_t = da_type[i_da]
            dataset = StandardScaler().fit_transform(xd[i_s][:, relief_vars[i_s][getattr(rrf.fsV, da_t)[i_s]]])
            if da_t == 'linear':
                da_model = LinearDiscriminantAnalysis().fit(dataset, class_var)
            else:
                da_model = QuadraticDiscriminantAnalysis().fit(dataset, class_var)
            label = cross_val_predict(da_model, dataset, class_var, cv=cv_partition)
            conf_mat = confusion_matrix(class_var, label)
            corr_rate = np.trace(conf_mat) / np.sum(conf_mat)
            correct = (class_var == label)
            sums, nums = np.bincount(dog_id[correct], weights=correct)
            dog_rate = sums / nums

            res['label'][da_t] = {i_s: label}
            res['confMat'][da_t] = {i_s: conf_mat}
            res['CorrRate'][da_t] = {i_s: corr_rate}
            res['DogRate'][da_t] = {i_s: dog_rate}

    return res

def classify_dogs_tree(xd, sens_names, class_var, relief_vars, cv_partition, rrf_t, dog_id, tree_cv_op):
    ns = len(sens_names)
    max_splits = [tree_cv_op[0].best_params_['max_depth'],
                  tree_cv_op[1].best_params_['max_depth']]
    res = {'label': {}, 'confMat': {}, 'CorrRate': {}, 'DogRate': {}}

    for i_s in range(ns):
        se_na = sens_names[i_s]
        dataset = StandardScaler().fit_transform(xd[i_s][:, relief_vars[i_s][rrf_t.fsV.Tree[i_s]]])
        tree_model = DecisionTreeClassifier(max_depth=max_splits[i_s]).fit(dataset, class_var)
        label = cross_val_predict(tree_model, dataset, class_var, cv=cv_partition)
        conf_mat = confusion_matrix(class_var, label)
        corr_rate = np.trace(conf_mat) / np.sum(conf_mat)
        correct = (class_var == label)
        sums, nums = np.bincount(dog_id[correct], weights=correct)
        dog_rate = sums / nums

        res['label']['Tree'] = {i_s: label}
        res['confMat']['Tree'] = {i_s: conf_mat}
        res['CorrRate']['Tree'] = {i_s: corr_rate}
        res['DogRate']['Tree'] = {i_s: dog_rate}

    return res