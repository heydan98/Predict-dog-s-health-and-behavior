import numpy as np
import pandas as pd
from sklearn.model_selection import LeaveOneGroupOut
from sklearn.preprocessing import StandardScaler
from CalcFeatures_Acc_Gyro import calc_features_acc_gyro
from collect_features import collect_features
from RankFeatures import RankFeatures
from SelectFeatures import select_features_da_svm, select_features_tree
from Tree_ClassCrit_optimizeMaxCutL import tree_optimize_max_cut_l1, tree_optimize_max_cut_l2
from ClassifyDogs import classify_dogs_da, classify_dogs_svm, classify_dogs_tree

# Load data
dog_move_data = pd.read_csv('/home/heydan/AI/Book2.csv')

# Calculate features
w_len = 200  # Window 2 secmạch cách li là gì
n_ecdf = 7   # Interpolation points for ecdf
overlap = 50 # Overlap 50%
features = calc_features_acc_gyro(dog_move_data, w_len, overlap, n_ecdf)

# Collect and arrange features into matrices
sens_names = ['Back', 'Neck']
xd, class_var, fnames, dog_id, test_num = collect_features(features, sens_names)

# Rank features with relieff algorithm and KNN
k_values = [3, 5, 9, 13, 17, 21]
ranks, weights, k, k_ranks, k_weights = RankFeatures(xd, sens_names, class_var, k_values)

min_vars = 20  # Using max 20 variables
relief_vars = []
for i_s, se_na in enumerate(sens_names):
    relief_vars.append(np.unique(k_ranks[se_na][:, :min_vars]))

# Feature selection for LDA, QDA, and SVM
cv_partition = LeaveOneGroupOut().split(np.zeros(len(class_var)), dog_id)
rrf = select_features_da_svm(xd, sens_names, class_var, relief_vars, cv_partition)

# Feature selection for Decision Tree
tree_cv_op_l1 = tree_optimize_max_cut_l1(xd, sens_names, class_var, relief_vars, cv_partition)
rrf_t = select_features_tree(xd, sens_names, class_var, relief_vars, cv_partition, tree_cv_op_l1)
tree_cv_op_l2 = tree_optimize_max_cut_l2(xd, sens_names, class_var, cv_partition, relief_vars, rrf_t)

# Classification
results_da = classify_dogs_da(xd, sens_names, class_var, relief_vars, cv_partition, rrf, dog_id)
results_svm = classify_dogs_svm(xd, sens_names, class_var, relief_vars, cv_partition, rrf, dog_id)
results_tree = classify_dogs_tree(xd, sens_names, class_var, relief_vars, cv_partition, rrf_t, dog_id, tree_cv_op_l2)