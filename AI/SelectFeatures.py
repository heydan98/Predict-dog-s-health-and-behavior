import numpy as np
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis, QuadraticDiscriminantAnalysis
from sklearn.svm import SVC
from sklearn.model_selection import cross_val_score
from sklearn.metrics import confusion_matrix

def select_features_da_svm(xd, sens_names, class_var, relief_vars, cv_partition):
    ns = len(sens_names)
    da_type = ['linear', 'quadratic']
    rrf = {}

    for i_s, se_na in enumerate(sens_names):
        dataset = StandardScaler().fit_transform(xd[i_s][:, relief_vars[i_s]])

        # LDA & QDA
        for da_t in da_type:
            if da_t == 'linear':
                da_obj = LinearDiscriminantAnalysis()
            else:
                da_obj = QuadraticDiscriminantAnalysis()

            scores = cross_val_score(da_obj, dataset, class_var, cv=cv_partition, scoring='accuracy')
            corr_rate = np.mean(scores)
            label = da_obj.fit(dataset, class_var).predict(dataset)
            conf_mat = confusion_matrix(class_var, label)

            rrf[f'label.{da_t}'] = [None] * ns
            rrf[f'confMat.{da_t}'] = [None] * ns
            rrf[f'CorrRate.{da_t}'] = [None] * ns
            rrf[f'fsV.{da_t}'] = [None] * ns

            rrf[f'label.{da_t}'][i_s] = label
            rrf[f'confMat.{da_t}'][i_s] = conf_mat
            rrf[f'CorrRate.{da_t}'][i_s] = corr_rate
            rrf[f'fsV.{da_t}'][i_s] = relief_vars[i_s]

        # SVM
        svm_obj = SVC(kernel='rbf')
        scores = cross_val_score(svm_obj, dataset, class_var, cv=cv_partition, scoring='accuracy')
        corr_rate = np.mean(scores)
        label = svm_obj.fit(dataset, class_var).predict(dataset)
        conf_mat = confusion_matrix(class_var, label)

        rrf['label.SVM'] = [None] * ns
        rrf['confMat.SVM'] = [None] * ns
        rrf['CorrRate.SVM'] = [None] * ns
        rrf['fsV.SVM'] = [None] * ns

        rrf['label.SVM'][i_s] = label
        rrf['confMat.SVM'][i_s] = conf_mat
        rrf['CorrRate.SVM'][i_s] = corr_rate
        rrf['fsV.SVM'][i_s] = relief_vars[i_s]

    return rrf

def select_features_tree(xd, sens_names, class_var, relief_vars, cv_partition, tree_cv_op):
    ns = len(sens_names)
    rrf_t = {}

    for i_s, se_na in enumerate(sens_names):
        dataset = StandardScaler().fit_transform(xd[i_s][:, relief_vars[i_s]])

        max_splits = tree_cv_op[i_s].best_params_['max_depth']
        tree = DecisionTreeClassifier(max_depth=max_splits)
        scores = cross_val_score(tree, dataset, class_var, cv=cv_partition, scoring='accuracy')
        corr_rate = np.mean(scores)
        label = tree.fit(dataset, class_var).predict(dataset)
        conf_mat = confusion_matrix(class_var, label)

        rrf_t['label.Tree'] = [None] * ns
        rrf_t['confMat.Tree'] = [None] * ns
        rrf_t['CorrRate.Tree'] = [None] * ns
        rrf_t['fsV.Tree'] = [None] * ns

        rrf_t['label.Tree'][i_s] = label
        rrf_t['confMat.Tree'][i_s] = conf_mat
        rrf_t['CorrRate.Tree'][i_s] = corr_rate
        rrf_t['fsV.Tree'][i_s] = relief_vars[i_s]

    return rrf_t