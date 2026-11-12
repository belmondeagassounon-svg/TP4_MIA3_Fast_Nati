#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Énumération pour typer la catégorie de salle
enum class CategorieSalle {
    COURS,
    TD,
    SOUTENANCE
};

// Fonction utilitaire pour convertir la catégorie en texte lisible
std::string categorieToString(CategorieSalle cat) {
    switch (cat) {
        case CategorieSalle::COURS: return "Cours";
        case CategorieSalle::TD: return "Travaux Diriges";
        case CategorieSalle::SOUTENANCE: return "Soutenance";
        default: return "Inconnue";
    }
}

// ==========================================
// 1. Classe Salle
// ==========================================
class Salle {
private:
    std::string numero;
    int capacite;
    CategorieSalle categorie;

public:
    Salle(std::string num, int cap, CategorieSalle cat)
        : numero(num), capacite(cap), categorie(cat) {}

    std::string getNumero() const { return numero; }
    int getCapacite() const { return capacite; }
    CategorieSalle getCategorie() const { return categorie; }

    void afficher() const {
        std::cout << "Salle " << numero << " [" << categorieToString(categorie) 
                  << "] - Capacite : " << capacite << " places.\n";
    }
};

// ==========================================
// 2. Classe Enseignant
// ==========================================
class Enseignant {
private:
    int id;
    std::string nom;

public:
    Enseignant(int id, std::string nom) : id(id), nom(nom) {}

    int getId() const { return id; }
    std::string getNom() const { return nom; }
};

// ==========================================
// 3. Classe Reservation (Classe d'association)
// ==========================================
class Reservation {
private:
    int idReservation;
    const Salle* salle;
    const Enseignant* enseignant;
    std::string date;
    std::string creneau;

public:
    Reservation(int id, const Salle* s, const Enseignant* e, std::string d, std::string c)
        : idReservation(id), salle(s), enseignant(e), date(d), creneau(c) {}

    int getId() const { return idReservation; }
    const Salle* getSalle() const { return salle; }
    const Enseignant* getEnseignant() const { return enseignant; }
    std::string getDate() const { return date; }
    std::string getCreneau() const { return creneau; }

    void afficher() const {
        std::cout << "Reservation #" << idReservation << " | Salle: " << salle->getNumero()
                  << " | Par: " << enseignant->getNom() << " | Le: " << date 
                  << " (" << creneau << ")\n";
    }
};

// ==========================================
// 4. Classe SystemeReservation (Contrôleur)
// ==========================================
class SystemeReservation {
private:
    std::vector<Salle> salles;
    std::vector<Enseignant> enseignants;
    std::vector<Reservation> reservations;
    int prochainIdReservation = 1;

    const Salle* trouverSalle(const std::string& numeroSalle) const {
        for (const auto& s : salles) {
            if (s.getNumero() == numeroSalle) return &s;
        }
        return nullptr;
    }

    const Enseignant* trouverEnseignant(int idEnseignant) const {
        for (const auto& e : enseignants) {
            if (e.getId() == idEnseignant) return &e;
        }
        return nullptr;
    }

public:
    void ajouterSalle(const Salle& s) {
        salles.push_back(s);
    }

    void ajouterEnseignant(const Enseignant& e) {
        enseignants.push_back(e);
    }

    // Vérifier la disponibilité d'une salle
    bool estDisponible(const std::string& numeroSalle, const std::string& date, const std::string& creneau) const {
        for (const auto& res : reservations) {
            if (res.getSalle()->getNumero() == numeroSalle &&
                res.getDate() == date &&
                res.getCreneau() == creneau) {
                return false; // Conflit détecté
            }
        }
        return true; // Disponible
    }

    // Enregistrer une réservation en bloquant les conflits
    bool reserverSalle(int idEnseignant, const std::string& numeroSalle, const std::string& date, const std::string& creneau) {
        const Salle* salle = trouverSalle(numeroSalle);
        const Enseignant* enseignant = trouverEnseignant(idEnseignant);

        if (!salle || !enseignant) {
            std::cout << "[Erreur] Salle ou enseignant introuvable.\n";
            return false;
        }

        if (!estDisponible(numeroSalle, date, creneau)) {
            std::cout << "[Conflit] La salle " << numeroSalle << " est deja reservee le " 
                      << date << " sur le creneau " << creneau << ".\n";
            return false;
        }

        Reservation nouvelleRes(prochainIdReservation++, salle, enseignant, date, creneau);
        reservations.push_back(nouvelleRes);
        std::cout << "[Succes] Reservation #" << nouvelleRes.getId() << " enregistree pour " 
                  << enseignant->getNom() << " (Salle " << numeroSalle << ").\n";
        return true;
    }

    // Annuler une réservation existante
    bool annulerReservation(int idReservation) {
        auto it = std::remove_if(reservations.begin(), reservations.end(),
            [idReservation](const Reservation& res) {
                return res.getId() == idReservation;
            });

        if (it != reservations.end()) {
            reservations.erase(it, reservations.end());
            std::cout << "[Succes] Reservation #" << idReservation << " annulee.\n";
            return true;
        }

        std::cout << "[Erreur] Aucune reservation trouvee avec l'ID #" << idReservation << ".\n";
        return false;
    }

    // Afficher l'état des réservations
    void afficherReservations() const {
        std::cout << "\n--- Liste des reservations actives ---\n";
        if (reservations.empty()) {
            std::cout << "Aucune reservation en cours.\n";
            return;
        }
        for (const auto& res : reservations) {
            res.afficher();
        }
        std::cout << "--------------------------------------\n\n";
    }
};

// ==========================================
// 5. Fonction principale (Scénario de test)
// ==========================================
int main() {
    SystemeReservation systeme;

    // Création des salles
    systeme.ajouterSalle(Salle("A101", 120, CategorieSalle::COURS));
    systeme.ajouterSalle(Salle("B204", 30, CategorieSalle::TD));
    systeme.ajouterSalle(Salle("S01", 15, CategorieSalle::SOUTENANCE));

    // Création des enseignants
    systeme.ajouterEnseignant(Enseignant(1, "Dr. Diallo"));
    systeme.ajouterEnseignant(Enseignant(2, "Prof. Kouassi"));

    // 1. Réservation valide
    systeme.reserverSalle(1, "A101", "2026-11-15", "08h00-10h00");

    // 2. Tentative de réservation conflictuelle (doit être refusée)
    systeme.reserverSalle(2, "A101", "2026-11-15", "08h00-10h00");

    // 3. Réservation valide sur un autre créneau
    systeme.reserverSalle(2, "A101", "2026-11-15", "10h00-12h00");

    systeme.afficherReservations();

    // 4. Annulation de la réservation #1
    systeme.annulerReservation(1);

    // 5. Nouvelle réservation réussie sur le créneau libéré
    systeme.reserverSalle(2, "A101", "2026-11-15", "08h00-10h00");

    systeme.afficherReservations();

    return 0;
}
