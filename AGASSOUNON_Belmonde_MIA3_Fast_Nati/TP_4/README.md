# 🎓 TP 4 — Système Intégré de Gestion d'un Département Universitaire

Application de gestion administrative et pédagogique développée en **C++** et modélisée avec **PlantUML**, permettant de centraliser la gestion des filières, niveaux, UE, matières, emplois du temps, salles et résultats académiques.

---

## 📋 Fonctionnalités implémentées

* **Organisation pédagogique :** Gestion des départements, filières, niveaux, unités d'enseignement (UE) et matières avec coefficients.
* **Gestion des acteurs et infrastructures :** Enregistrement des étudiants, des enseignants et des salles de cours/TD.
* **Emploi du temps :** Planification des séances avec détection automatique des conflits d'occupation des salles.
* **Évaluation et délibération :** Saisie des notes, calcul des moyennes pondérées par UE, validation des crédits et édition des relevés de notes officiels.
* **Persistance des données :** Exportation automatique des étudiants et des notes au format **CSV** (`etudiants.csv` et `notes.csv`).

---

## 📊 Modélisation UML (PlantUML)

### 1. Diagramme Global de Cas d'Utilisation
![Diagramme de cas d'utilisation](./images/cas_utilisation.png)

### 2. Diagramme de Classes Complet
![Diagramme de classes](./images/classes.png)

### 3. Diagramme de Séquence (Saisie des notes)
![Diagramme de séquence](./images/sequence_notes.png)

### 4. Diagramme d'Activités (Processus de délibération)
![Diagramme d'activités](./images/activite_deliberation.png)

### 5. Diagramme de Composants
![Diagramme de composants](./images/composants.png)

### 6. Diagramme de Déploiement
![Diagramme de déploiement](./images/deploiement.png)

---

## 💻 Exécution du programme (`TP.cpp`)

Le code source complet se trouve dans le fichier [`TP.cpp`](./TP.cpp).

```bash
g++ -std=c++11 -Wall -o gestion_departement TP.cpp
./gestion_departement
```

