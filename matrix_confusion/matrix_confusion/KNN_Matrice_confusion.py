import numpy as np  # Import necessary libraries
from sklearn import datasets  # Execute statement
from sklearn.model_selection import train_test_split  # Execute statement
import matplotlib.pyplot as plt  # Import necessary libraries
from matplotlib.colors import ListedColormap  # Execute statement
from KNN import KNN  # Execute statement
import projet_matrice_confusion as mc  # Import necessary libraries

cmap = ListedColormap(['#FF0000', '#00FF00', '#0000FF'])  # Assign value to a variable
iris = datasets.load_iris()  # Assign value to a variable
X, y = iris.data, iris.target  # Assign value to a variable

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=1234)  # Assign value to a variable

knn_model = KNN(k=5)  # Assign value to a variable
knn_model.train(X_train, y_train)  # Execute statement
predictions = knn_model.predict(X_test)  # Assign value to a variable
print("Prédictions :", predictions)  # Execute statement
accuracy = np.mean(predictions == y_test)  # Assign value to a variable
print("Précision du modèle :", accuracy)  # Execute statement

plt.figure()  # Use Matplotlib for plotting
plt.scatter(X[:, 2], X[:, 3], c=y, cmap=cmap, edgecolor='k', s=20)  # Use Matplotlib for plotting
plt.xlabel("Longueur des pétales")  # Use Matplotlib for plotting
plt.ylabel("Largeur des pétales")  # Use Matplotlib for plotting
plt.title("Données Iris")  # Use Matplotlib for plotting
plt.show()  # Use Matplotlib for plotting

confusionMatrix = mc.confusion_matrix(y_test, predictions)  # Assign value to a variable

mc.analyse_matrice_de_confusion(confusionMatrix)  # Execute statement


