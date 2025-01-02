import numpy as np
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import StandardScaler

def tree_optimize_max_cut_l2(xd, sens_names, class_var, cv_partition, relief_vars, rr_ft):
    treecv_op = {}
    for i_s in range(2):  # Only Back & Neck
        se_na = sens_names[i_s]
        vin = relief_vars[i_s][rr_ft['fsV.Tree'][i_s]]
        dataset = StandardScaler().fit_transform(xd[i_s][:, vin])
        tree_cv_op = DecisionTreeClassifier(random_state=0)
        tree_cv_op.set_params(max_depth=None)
        tree_cv_op = tree_cv_op.fit(dataset, class_var, cv=cv_partition, max_depth=None)
        treecv_op[i_s] = tree_cv_op

    return treecv_op

def tree_optimize_max_cut_l1(xd, sens_names, class_var, relief_vars, cv_partition):
    ns = len(sens_names)
    treecv_op = {}

    for i_s in range(ns):
        se_na = sens_names[i_s]
        dataset = StandardScaler().fit_transform(xd[i_s][:, relief_vars[i_s]])
        tree_cv_op = DecisionTreeClassifier(random_state=0)
        tree_cv_op.set_params(max_depth=None)
        tree_cv_op = tree_cv_op.fit(dataset, class_var, cv=cv_partition, max_depth=None)
        treecv_op[i_s] = tree_cv_op

    return treecv_op

def tree_class_crit_neck(x_train, y_train, x_test, y_test):
    max_splits = 121  # Hard-coded from previous optimization
    tree = DecisionTreeClassifier(max_depth=max_splits, random_state=0)
    tree.fit(x_train, y_train)
    y_pred = tree.predict(x_test)
    criterion = np.sum(y_pred != y_test)
    return criterion

def tree_class_crit_back(x_train, y_train, x_test, y_test):
    max_splits = 124  # Hard-coded from previous optimization
    tree = DecisionTreeClassifier(max_depth=max_splits, random_state=0)
    tree.fit(x_train, y_train)
    y_pred = tree.predict(x_test)
    criterion = np.sum(y_pred != y_test)
    return criterion