import numpy as np  # Import necessary libraries
import matplotlib.pyplot as plt  # Import necessary libraries
from sklearn.datasets import make_blobs  # Execute statement
import projet_matrice_confusion as mc  # Import necessary libraries

def euclidean_distance(x1, x2):  # Define function 'euclidean_distance'
    return np.sqrt(np.sum((x1 - x2) ** 2))  # Return the result of the function


class KMeans:  # Define class 'KMeans:'
    def __init__(self, K=5, max_iters=100, plot_steps=False):  # Define function '__init__'
        self.K = K  # Assign value to a variable
        self.max_iters = max_iters  # Assign value to a variable
        self.plot_steps = plot_steps  # Assign value to a variable

        self.clusters = [[] for _ in range(self.K)]  # Start a loop

        self.centroids = []  # Assign value to a variable

    def predict(self, X):  # Define function 'predict'
        self.X = X  # Assign value to a variable
        self.n_samples, self.n_features = X.shape  # Assign value to a variable

        # Difference avec kmeans.py (utilisation de kmeans++)  # Execute statement
        self.centroids = self._initialize_centroids()  # Assign value to a variable

        for _ in range(self.max_iters):  # Start a loop
            self.clusters = self._create_clusters(self.centroids)  # Assign value to a variable

            if self.plot_steps:  # Start a conditional check
                self.plot(str(_))  # Execute statement

            centroids_old = self.centroids  # Assign value to a variable
            self.centroids = self._get_centroids(self.clusters)  # Assign value to a variable

            if self._is_converged(centroids_old, self.centroids):  # Start a conditional check
                break  # Execute statement

            if self.plot_steps:  # Start a conditional check
                self.plot(str(_)+"_NewCenter")  # Execute statement

        return self._get_cluster_labels(self.clusters)  # Return the result of the function

    def _initialize_centroids(self):  # Define function '_initialize_centroids'
        centroids = []  # Assign value to a variable
        # le premier centroïd est choisi de manière aléatoire  # Execute statement
        centroids.append(self.X[np.random.choice(self.n_samples)])  # Perform a NumPy operation

        for _ in range(1, self.K):  # Start a loop
            # Calcule la distance du centroïd le plus proche  # Execute statement
            distances = np.array([min(euclidean_distance(x, centroid) ** 2 for centroid in centroids) for x in self.X])  # Start a loop

            # Selectionne le prochain centroïd grace aux probabilité  # Execute statement
            probabilities = distances / np.sum(distances)  # Perform a NumPy operation
            cumulative_probabilities = np.cumsum(probabilities)  # Perform a NumPy operation
            r = np.random.rand()  # Perform a NumPy operation

            for i, cp in enumerate(cumulative_probabilities):  # Start a loop
                if r < cp:  # Start a conditional check
                    centroids.append(self.X[i])  # Execute statement
                    break  # Execute statement

        return centroids  # Return the result of the function

    def _get_cluster_labels(self, clusters):  # Define function '_get_cluster_labels'
        labels = np.empty(self.n_samples)  # Perform a NumPy operation
        for cluster_idx, cluster in enumerate(clusters):  # Start a loop
            for sample_idx in cluster:  # Start a loop
                labels[sample_idx] = cluster_idx  # Assign value to a variable
        return labels  # Return the result of the function

    def _create_clusters(self, centroids):  # Define function '_create_clusters'
        clusters = [[] for _ in range(self.K)]  # Start a loop
        for idx, sample in enumerate(self.X):  # Start a loop
            centroid_idx = self._closest_centroid(sample, centroids)  # Assign value to a variable
            clusters[centroid_idx].append(idx)  # Execute statement
        return clusters  # Return the result of the function

    def _closest_centroid(self, sample, centroids):  # Define function '_closest_centroid'
        distances = [euclidean_distance(sample, point) for point in centroids]  # Start a loop
        return np.argmin(distances)  # Return the result of the function

    def _get_centroids(self, clusters):  # Define function '_get_centroids'
        centroids = np.zeros((self.K, self.n_features))  # Perform a NumPy operation
        for cluster_idx, cluster in enumerate(clusters):  # Start a loop
            cluster_mean = np.mean(self.X[cluster], axis=0)  # Perform a NumPy operation
            centroids[cluster_idx] = cluster_mean  # Assign value to a variable
        return centroids  # Return the result of the function

    def _is_converged(self, centroids_old, centroids):  # Define function '_is_converged'
        distances = [  # Assign value to a variable
            euclidean_distance(centroids_old[i], centroids[i]) for i in range(self.K)  # Start a loop
        ]  # Execute statement
        return sum(distances) == 0  # Return the result of the function

    def plot(self, name):  # Define function 'plot'
        fig, ax = plt.subplots(figsize=(12, 8))  # Use Matplotlib for plotting

        for i, index in enumerate(self.clusters):  # Start a loop
            point = self.X[index].T  # Assign value to a variable
            ax.scatter(*point)  # Execute statement

        for point in self.centroids:  # Start a loop
            ax.scatter(*point, marker="x", color="black", linewidth=2)  # Assign value to a variable

        path = 'plotPlus/' + name + '.png'  # Assign value to a variable
        plt.savefig(path)  # Use Matplotlib for plotting
        # plt.show()  # Use Matplotlib for plotting

    def calculate_inertia(self):  # Define function 'calculate_inertia'
        """  # Execute statement
        Calcule l'inertie (somme des distances intra-clusters).  # Execute statement
        """  # Execute statement
        inertia = 0  # Assign value to a variable
        for cluster_idx, cluster in enumerate(self.clusters):  # Start a loop
            centroid = self.centroids[cluster_idx]  # Assign value to a variable
            for sample_idx in cluster:  # Start a loop
                sample = self.X[sample_idx]  # Assign value to a variable
                inertia += np.linalg.norm(sample - centroid) ** 2  # Perform a NumPy operation
        return inertia  # Return the result of the function

    def calculate_dunn_index(self):  # Define function 'calculate_dunn_index'
        """  # Execute statement
        Calcule l'indice de Dunn.  # Execute statement
        """  # Execute statement
        inter_cluster_distances = []  # Assign value to a variable
        for i in range(len(self.centroids)):  # Start a loop
            for j in range(i + 1, len(self.centroids)):  # Start a loop
                distance = np.linalg.norm(self.centroids[i] - self.centroids[j])  # Perform a NumPy operation
                inter_cluster_distances.append(distance)  # Execute statement
        min_inter_cluster_distance = min(inter_cluster_distances)  # Assign value to a variable

        intra_cluster_diameters = []  # Assign value to a variable
        for cluster in self.clusters:  # Start a loop
            if len(cluster) > 1:  # Start a conditional check
                distances = [  # Assign value to a variable
                    np.linalg.norm(self.X[i] - self.X[j])  # Perform a NumPy operation
                    for i in cluster  # Start a loop
                    for j in cluster  # Start a loop
                    if i != j  # Start a conditional check
                ]  # Execute statement
                intra_cluster_diameters.append(max(distances))  # Execute statement
            else:  # Execute statement
                intra_cluster_diameters.append(0)  # Execute statement
        max_intra_cluster_diameter = max(intra_cluster_diameters)  # Assign value to a variable

        if max_intra_cluster_diameter == 0:  # Start a conditional check
            return float('inf')  # Return the result of the function
        dunn_index = min_inter_cluster_distance / max_intra_cluster_diameter  # Assign value to a variable
        return dunn_index  # Return the result of the function


if __name__ == "__main__":  # Start a conditional check
    np.random.seed(42)  # Perform a NumPy operation

    X, y = make_blobs(  # Assign value to a variable
        centers=3, n_samples=500, n_features=2, shuffle=True, random_state=40  # Assign value to a variable
    )  # Execute statement
    print(X.shape)  # Execute statement

    clusters = len(np.unique(y))  # Perform a NumPy operation
    print(clusters)  # Execute statement

    kmeans = KMeans(K=clusters, max_iters=150, plot_steps=False)  # Assign value to a variable
    y_pred = kmeans.predict(X)  # Assign value to a variable

    confusionMatrix=mc.confusion_matrix(y, y_pred)  # Assign value to a variable

    mc.analyse_matrice_de_confusion(confusionMatrix)  # Execute statement
