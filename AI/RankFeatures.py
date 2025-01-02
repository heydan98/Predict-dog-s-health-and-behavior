from sklearn.preprocessing import StandardScaler
import ReliefF as rf 
def RankFeatures(Xd, SensNames, Class, knums):
  NS = len(SensNames)
  ranks = {name: [] for name in SensNames}  # Dictionary to store ranks per feature
  weights = {name: [] for name in SensNames}  # Dictionary to store weights per feature

  for k in knums:
    for iS, SeNa in enumerate(SensNames):
      dataset = StandardScaler().fit_transform(Xd[iS].reshape(-1, 1))  # Standardize data
      scores, weights_k = rf(dataset, Class, k)
      ranks[SeNa].append(scores)
      weights[SeNa].append(weights_k)

  # Extract top features
  kRanks = {name: ranks[name][-1] for name in SensNames}
  kweights = {name: weights[name][-1] for name in SensNames}

  return ranks, weights, kRanks, kweights
