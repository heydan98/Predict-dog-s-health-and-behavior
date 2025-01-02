from sklearn.svm import SVC
from sklearn.model_selection import cross_val_score
from sklearn.metrics import confusion_matrix

def SVM_ClassCrit(XTRAIN, ytrain, XTEST, ytest):
  # Define the Gaussian SVM model
  model = SVC(kernel='rbf')  # Radial Basis Function (Gaussian) kernel

  # Train the model
  model.fit(XTRAIN, ytrain)

  # Make predictions on test data
  y_pred = model.predict(XTEST)

  # Calculate classification error (number of misclassified samples)
  criterion = sum(y_pred != ytest)

  # Uncomment the following for optional cross-validation metrics
  # cv_scores = cross_val_score(model, XTRAIN, ytrain, cv=5)  # 5-fold cross-validation
  # print(f"Average Cross-Validation Accuracy: {cv_scores.mean()}")

  # Uncomment the following for confusion matrix (requires actual labels)
  # cm = confusion_matrix(ytest, y_pred)
  # print(cm)

  return criterion
