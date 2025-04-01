import pandas as pd  # Library for data manipulation and analysis
import numpy as np  # Library for numerical computations
import matplotlib.pyplot as plt  # Library for creating visualizations
import seaborn as sns  # Library for advanced visualizations
from sklearn.datasets import load_iris  # Function to load the Iris dataset
from sklearn.cluster import KMeans  # Class for K-Means clustering
from sklearn.neighbors import KNeighborsClassifier  # Class for K-Nearest Neighbors classification
from sklearn.metrics import confusion_matrix, classification_report  # Tools for performance evaluation
from sklearn.model_selection import train_test_split  # Function to split data into training and testing sets
from scipy.stats import mode

def charger_donnees():
    """
    Load and return the Iris dataset as a pandas DataFrame.
    The returned DataFrame includes feature columns and the target column.
    """
    iris = load_iris()  # Load the Iris dataset
    data = pd.DataFrame(data=iris.data, columns=iris.feature_names)  # Create a DataFrame for feature data
    data['target'] = iris.target  # Add the target labels as a new column
    return data  # Return the prepared DataFrame

def visualiser_donnees(data):
    """
    Visualize the first two features of the Iris dataset.
    The points are colored based on their true class labels.
    """
    plt.scatter(data.iloc[:, 0], data.iloc[:, 1], c=data['target'], cmap='viridis')  # Scatter plot of two features
    plt.title('Visualisation des données (Iris)')  # Add a title
    plt.xlabel("Longueur des pétales")  # Use Matplotlib for plotting
    plt.ylabel("Largeur des pétales")  # Use Matplotlib for plotting
    plt.colorbar(label='Classe réelle')  # Add a colorbar indicating the class labels
    plt.show()  # Display the plot

def afficher_heatmap(conf_matrix, title, labels):
    """
    Display a heatmap for the given confusion matrix.
    Args:
        conf_matrix: The confusion matrix to visualize.
        title: Title for the heatmap.
        labels: List of class labels for x and y ticks.
    """
    plt.figure(figsize=(6, 6))  # Set the figure size
    sns.heatmap(conf_matrix, annot=True, fmt='d', cmap='Blues', xticklabels=labels, yticklabels=labels)  # Create the heatmap
    plt.title(title)  # Add a title
    plt.xlabel('Prediction')  # Label for the x-axis
    plt.ylabel('Real')  # Label for the y-axis
    plt.show()  # Display the heatmap

def analyse_matrice_de_confusion(conf_matrix):
    """
    Perform a detailed analysis of the confusion matrix.
    Compute metrics like precision, recall, specificity, false positive rate, and F1-score.
    Additionally, include explanations for each metric to help understand the evaluation.
    """
    num_classes = conf_matrix.shape[0]  # Number of classes in the confusion matrix
    precision, recall, specificity, f1_score, false_positive_rate = [], [], [], [], []  # Initialize lists for metrics

    for i in range(num_classes):  # Iterate through each class
        TP = conf_matrix[i, i]  # True Positives
        FN = np.sum(conf_matrix[i, :]) - TP  # False Negatives
        FP = np.sum(conf_matrix[:, i]) - TP  # False Positives
        TN = np.sum(conf_matrix) - (TP + FP + FN)  # True Negatives

        # Compute metrics, handling division by zero
        prec = TP / (TP + FP) if (TP + FP) != 0 else 0  # Precision: TP / (TP + FP)
        rec = TP / (TP + FN) if (TP + FN) != 0 else 0  # Recall: TP / (TP + FN)
        spec = TN / (TN + FP) if (TN + FP) != 0 else 0  # Specificity: TN / (TN + FP)
        fpr = FP / (FP + TN) if (FP + TN) != 0 else 0  # False Positive Rate: FP / (FP + TN)
        f1 = (2 * prec * rec) / (prec + rec) if (prec + rec) != 0 else 0  # F1 Score: Harmonic mean of precision and recall

        # Append metrics to respective lists
        precision.append(prec)
        recall.append(rec)
        specificity.append(spec)
        false_positive_rate.append(fpr)
        f1_score.append(f1)

        # Print metrics for the current class
        print(f"\nClasse {i}:")
        print(f"  True Positives (TP): {TP}")
        print(f"  False Negatives (FN): {FN}")
        print(f"  False Positives (FP): {FP}")
        print(f"  True Negatives (TN): {TN}")
        print(f"  Precision (Précision): {prec:.2f} - Fraction of relevant instances among retrieved ones.")
        print(f"  Recall (Rappel): {rec:.2f} - Fraction of relevant instances successfully retrieved.")
        print(f"  Specificity (Spécificité): {spec:.2f} - Ability to correctly identify negatives.")
        print(f"  False Positive Rate (FPR): {fpr:.2f} - Fraction of negatives incorrectly classified as positives.")
        print(f"  F1 Score: {f1:.2f} - Balance between precision and recall.")

    # Compute and print overall averages
    avg_precision = np.mean(precision)
    avg_recall = np.mean(recall)
    avg_f1_score = np.mean(f1_score)

    print("\nGlobal Averages:")
    print(f"  Average Precision: {avg_precision:.2f}")
    print(f"  Average Recall: {avg_recall:.2f}")
    print(f"  Average F1 Score: {avg_f1_score:.2f}")

def remap_clusters(y_true, y_pred):
    """
    Rimappa i cluster assegnati da K-Means alle etichette reali.
    """
    labels = np.zeros_like(y_pred)
    for i in range(len(np.unique(y_pred))):
        mask = (y_pred == i)
        labels[mask] = mode(y_true[mask])[0]
    return labels

def appliquer_kmeans(data):
    """
    Applica K-Means, rimappa i cluster e visualizza i risultati.
    """
    # K-Means con 3 cluster
    kmeans = KMeans(n_clusters=3, random_state=0)
    clusters = kmeans.fit_predict(data.iloc[:, :-1])  # Predizione dei cluster

    # Aggiunta dei cluster al DataFrame
    data['cluster'] = clusters

    # Rimappatura dei cluster per allinearli alle etichette reali
    data['remapped_cluster'] = remap_clusters(data['target'], data['cluster'])

    # Visualizzazione dei cluster K-Means originali
    plt.scatter(data.iloc[:, 0], data.iloc[:, 1], c=data['cluster'], cmap='viridis')
    plt.title('Résultats K-Means (Clusters)')
    plt.xlabel("Longueur des pétales")
    plt.ylabel("Largeur des pétales")
    plt.colorbar(label='Cluster attribué')
    plt.show()

    # Visualizzazione dei cluster rimappati
    plt.scatter(data.iloc[:, 0], data.iloc[:, 1], c=data['remapped_cluster'], cmap='viridis')
    plt.title('Clusters K-Means (Rimappati)')
    plt.xlabel("Longueur des pétales")
    plt.ylabel("Largeur des pétales")
    plt.colorbar(label='Cluster rimappato')
    plt.show()
    conf_matrix_no_lab = confusion_matrix(data['target'], data['cluster'])
    print("Matrice de confusion avant rimappage :\n", conf_matrix_no_lab)
    afficher_heatmap(conf_matrix_no_lab, "Matrice de confusion (Clusters Originaux)", ["Setosa", "Versicolor", "Virginica"])
    analyse_matrice_de_confusion(conf_matrix_no_lab)
    print("\n")
    # Matrice di confusione tra classi reali e cluster rimappati
    conf_matrix = confusion_matrix(data['target'], data['remapped_cluster'])
    print("Matrice de confusion après rimappage :\n", conf_matrix)
    afficher_heatmap(conf_matrix, "Matrice de confusion (Clusters Rimappés)", ["Setosa", "Versicolor", "Virginica"])
    analyse_matrice_de_confusion(conf_matrix)

def methode_du_coude(data):
    """
    Use the elbow method to determine the optimal number of clusters for K-Means.
    """
    inertias = []  # List to store inertia values for different cluster counts
    k_range = range(1, 11)  # Range of cluster counts to evaluate

    for k in k_range:
        kmeans = KMeans(n_clusters=k, random_state=0)  # Initialize K-Means
        kmeans.fit(data.iloc[:, :-1])  # Fit the model
        inertias.append(kmeans.inertia_)  # Append the inertia value

    # Plot the inertia values to visualize the elbow point
    plt.plot(k_range, inertias, marker='o')
    plt.title('Méthode du coude pour déterminer K')
    plt.xlabel('Nombre de clusters')
    plt.ylabel('Inertie')
    plt.show()

def appliquer_knn(data):
    """
    Apply K-Nearest Neighbors classification to the dataset.
    Evaluate performance using a confusion matrix and detailed metrics.
    Include visual comparison of predicted and actual values.
    """
    # Split the dataset into training and testing sets
    X_train, X_test, y_train, y_test = train_test_split(data.iloc[:, :-2], data['target'], test_size=0.3, random_state=0)

    knn = KNeighborsClassifier(n_neighbors=5)  # Initialize the KNN classifier with 5 neighbors
    knn.fit(X_train, y_train)  # Train the model
    y_pred = knn.predict(X_test)  # Predict the labels for the test set

    plt.scatter(X_test.iloc[:, 0], X_test.iloc[:, 1], c=y_pred, cmap='viridis')  # Scatter plot of predicted values
    plt.title('Prédictions KNN (Iris)')
    plt.xlabel("Longueur des pétales")  # Use Matplotlib for plotting
    plt.ylabel("Largeur des pétales")  # Use Matplotlib for plotting
    plt.colorbar(label='Classe prédite')
    plt.legend(['Setosa', 'Versicolor', 'Virginica'])
    plt.show()


    # Compute and analyze the confusion matrix
    conf_matrix = confusion_matrix(y_test, y_pred)
    print("Matrice de confusion (KNN) :\n", conf_matrix)
    afficher_heatmap(conf_matrix, "Matrice de confusion - KNN", labels=["Setosa", "Versicolor", "Virginica"])
    analyse_matrice_de_confusion(conf_matrix)

def main():
    """
    Main entry point for executing the project steps.
    Walk through loading data, visualizing, clustering, and classification.
    """
    print("Chargement des données...")
    data = charger_donnees()

    print("Visualisation initiale des données...")
    visualiser_donnees(data)

    print("\nApplication de K-Means...")
    appliquer_kmeans(data)

    print("\nMéthode du coude pour K-Means...")
    methode_du_coude(data)

    print("\nApplication de KNN...")
    appliquer_knn(data)

if __name__ == "__main__":
    main()
