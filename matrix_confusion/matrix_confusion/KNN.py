import numpy as np  # Import necessary libraries
from collections import Counter  # Execute statement

"""  # Execute statement
Calcule la distance entre 2 points  # Execute statement
"""  # Execute statement
def distance(point1, point2):  # Define function 'distance'
    return np.sqrt(np.sum((point1 - point2) ** 2))  # Return the result of the function

class KNN:  # Define class 'KNN:'

    """  # Execute statement
    Constructeur de KNN avec comme valeur par defaut de 3 pour K  # Execute statement
    """  # Execute statement
    def __init__(self, k=3):  # Define function '__init__'
        """  # Execute statement
        Initialisation du modèle KNN avec un paramètre k (nombre de voisins)  # Execute statement
        """  # Execute statement
        self.k = k  # Assign value to a variable

    """  # Execute statement
    Stocke les données d'entrainement et leurs labels  # Execute statement
    """  # Execute statement
    def train(self, training_data, training_labels):  # Define function 'train'
        self.training_data = training_data  # Assign value to a variable
        self.training_labels = training_labels  # Assign value to a variable

    """  # Execute statement
    Predit les labels pour tout les points de test_data  # Execute statement
    """  # Execute statement
    def predict(self, test_data):  # Define function 'predict'
        predictions = [self._predict_single(test_point) for test_point in test_data]  # Start a loop
        return predictions  # Return the result of the function

    """  # Execute statement
    Prédit le label d'un seul point de test_data  # Execute statement
    """  # Execute statement
    def _predict_single(self, test_point):  # Define function '_predict_single'
        distances = [distance(test_point, train_point) for train_point in self.training_data]  # Start a loop
        k_nearest_indices = np.argsort(distances)[:self.k]  # Assign value to a variable
        k_nearest_labels = [self.training_labels[i] for i in k_nearest_indices]  # Start a loop
        most_common_label = Counter(k_nearest_labels).most_common(1)[0][0]  # Assign value to a variable
        return most_common_label  # Return the result of the function



