#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

// ==========================================
// 1. Classes Pédagogiques (Matiere, UE, Niveau, Filiere)
// ==========================================
class Matiere {
private:
    std::string code;
    std::string nom;
    double coefficient;
    int idEnseignant;

public:
    Matiere(std::string c, std::string n, double coef, int idEns = 0)
        : code(c), nom(n), coefficient(coef), idEnseignant(idEns) {}

    std::string getCode() const { return code; }
    std::string getNom() const { return nom; }
    double getCoefficient() const { return coefficient; }
};

class UE {
private:
    std::string code;
    std::string intitule;
    int credits;
    std::vector<Matiere> matieres;

public:
    UE(std::string c, std::string i, int cred)
        : code(c), intitule(i), credits(cred) {}

    void ajouterMatiere(const Matiere& m) { matieres.push_back(m); }

    std::string getCode() const { return code; }
    std::string getIntitule() const { return intitule; }
    int getCredits() const { return credits; }
    const std::vector<Matiere>& getMatieres() const { return matieres; }
};

class Niveau {
private:
    std::string intitule; // Ex: L1, L2, L3, M1
    std::vector<UE> ues;

public:
    Niveau(std::string i) : intitule(i) {}

    void ajouterUE(const UE& ue) { ues.push_back(ue); }
    std::string getIntitule() const { return intitule; }
    const std::vector<UE>& getUEs() const { return ues; }
};

class Filiere {
private:
    std::string code;
    std::string libelle;
    std::vector<Niveau> niveaux;

public:
    Filiere(std::string c, std::string l) : code(c), libelle(l) {}

    void ajouterNiveau(const Niveau& n) { niveaux.push_back(n); }
    std::string getCode() const { return code; }
    std::string getLibelle() const { return libelle; }
    const std::vector<Niveau>& getNiveaux() const { return niveaux; }
};

// ==========================================
// 2. Acteurs et Évaluations (Etudiant, Enseignant, Note)
// ==========================================
class Etudiant {
private:
    std::string matricule;
    std::string nom;
    std::string prenom;
    std::string filiereCode;
    std::string niveauIntitule;

public:
    Etudiant(std::string mat, std::string n, std::string p, std::string fil, std::string niv)
        : matricule(mat), nom(n), prenom(p), filiereCode(fil), niveauIntitule(niv) {}

    std::string getMatricule() const { return matricule; }
    std::string getNomComplet() const { return nom + " " + prenom; }
    std::string getNom() const { return nom; }
    std::string getPrenom() const { return prenom; }
    std::string getFiliere() const { return filiereCode; }
    std::string getNiveau() const { return niveauIntitule; }
};

class Enseignant {
private:
    int id;
    std::string nom;
    std::string specialite;

public:
    Enseignant(int i, std::string n, std::string spec)
        : id(i), nom(n), specialite(spec) {}

    int getId() const { return id; }
    std::string getNom() const { return nom; }
    std::string getSpecialite() const { return specialite; }
};

class Note {
private:
    std::string matriculeEtudiant;
    std::string codeMatiere;
    double valeur;

public:
    Note(std::string mat, std::string codeMat, double val)
        : matriculeEtudiant(mat), codeMatiere(codeMat), valeur(val) {}

    std::string getMatricule() const { return matriculeEtudiant; }
    std::string getCodeMatiere() const { return codeMatiere; }
    double getValeur() const { return valeur; }
};

// ==========================================
// 3. Logistique (Salle, Seance, EmploiDuTemps)
// ==========================================
class Salle {
private:
    std::string numero;
    int capacite;
    std::string type; // Cours, TD, TP, Soutenance

public:
    Salle(std::string num, int cap, std::string t)
        : numero(num), capacite(cap), type(t) {}

    std::string getNumero() const { return numero; }
    int getCapacite() const { return capacite; }
    std::string getType() const { return type; }
};

class Seance {
private:
    std::string jour;
    std::string creneau;
    std::string codeMatiere;
    std::string numeroSalle;

public:
    Seance(std::string j, std::string c, std::string mat, std::string salle)
        : jour(j), creneau(c), codeMatiere(mat), numeroSalle(salle) {}

    std::string getJour() const { return jour; }
    std::string getCreneau() const { return creneau; }
    std::string getCodeMatiere() const { return codeMatiere; }
    std::string getNumeroSalle() const { return numeroSalle; }
};

class EmploiDuTemps {
private:
    std::vector<Seance> seances;

public:
    bool ajouterSeance(const Seance& nouvelle) {
        for (const auto& s : seances) {
            if (s.getJour() == nouvelle.getJour() &&
                s.getCreneau() == nouvelle.getCreneau() &&
                s.getNumeroSalle() == nouvelle.getNumeroSalle()) {
                std::cout << "[Conflit EDT] La salle " << nouvelle.getNumeroSalle()
                          << " est deja occupee le " << nouvelle.getJour()
                          << " (" << nouvelle.getCreneau() << ").\n";
                return false;
            }
        }
        seances.push_back(nouvelle);
        return true;
    }

    void afficher() const {
        std::cout << "\n=== EMPLOI DU TEMPS DU DEPARTEMENT ===\n";
        for (const auto& s : seances) {
            std::cout << s.getJour() << " | " << s.getCreneau()
                      << " | Matiere : " << s.getCodeMatiere()
                      << " | Salle : " << s.getNumeroSalle() << "\n";
        }
        std::cout << "======================================\n";
    }
};

// ==========================================
// 4. Contrôleur Principal : Departement
// ==========================================
class Departement {
private:
    std::string nom;
    std::vector<Filiere> filieres;
    std::vector<Etudiant> etudiants;
    std::vector<Enseignant> enseignants;
    std::vector<Salle> salles;
    std::vector<Note> notes;
    EmploiDuTemps edt;

    double getNoteEtudiantMatiere(const std::string& matricule, const std::string& codeMatiere) const {
        for (const auto& n : notes) {
            if (n.getMatricule() == matricule && n.getCodeMatiere() == codeMatiere) {
                return n.getValeur();
            }
        }
        return -1.0; // Note non saisie
    }

public:
    Departement(std::string n) : nom(n) {}

    void ajouterFiliere(const Filiere& f) { filieres.push_back(f); }
    void ajouterEtudiant(const Etudiant& e) { etudiants.push_back(e); }
    void ajouterEnseignant(const Enseignant& e) { enseignants.push_back(e); }
    void ajouterSalle(const Salle& s) { salles.push_back(s); }
    void planifierSeance(const Seance& s) { edt.ajouterSeance(s); }
    void afficherEDT() const { edt.afficher(); }

    void saisirNote(const std::string& matricule, const std::string& codeMatiere, double valeur) {
        if (valeur < 0.0 || valeur > 20.0) {
            std::cout << "[Erreur] Note invalide (" << valeur << ").\n";
            return;
        }
        notes.push_back(Note(matricule, codeMatiere, valeur));
    }

    // Persistance CSV
    void sauvegarderCSV() const {
        std::ofstream fEtu("etudiants.csv");
        fEtu << "Matricule;Nom;Prenom;Filiere;Niveau\n";
        for (const auto& e : etudiants) {
            fEtu << e.getMatricule() << ";" << e.getNom() << ";" << e.getPrenom()
                 << ";" << e.getFiliere() << ";" << e.getNiveau() << "\n";
        }
        fEtu.close();

        std::ofstream fNotes("notes.csv");
        fNotes << "Matricule;CodeMatiere;Note\n";
        for (const auto& n : notes) {
            fNotes << n.getMatricule() << ";" << n.getCodeMatiere() << ";" << n.getValeur() << "\n";
        }
        fNotes.close();
        std::cout << "\n[CSV] Fichiers 'etudiants.csv' et 'notes.csv' generes avec succes.\n";
    }

    // Calcul des moyennes, crédits et production du relevé de notes
    void produireReleveNotes(const std::string& matricule) const {
        const Etudiant* etu = nullptr;
        for (const auto& e : etudiants) {
            if (e.getMatricule() == matricule) {
                etu = &e;
                break;
            }
        }
        if (!etu) {
            std::cout << "[Erreur] Etudiant introuvable : " << matricule << "\n";
            return;
        }

        const Niveau* nivTrouve = nullptr;
        for (const auto& f : filieres) {
            if (f.getCode() == etu->getFiliere()) {
                for (const auto& n : f.getNiveaux()) {
                    if (n.getIntitule() == etu->getNiveau()) {
                        nivTrouve = &n;
                        break;
                    }
                }
            }
        }
        if (!nivTrouve) return;

        std::cout << "\n======================================================\n";
        std::cout << "DEPARTEMENT : " << nom << "\n";
        std::cout << "RELEVE DE NOTES OFFICIEL\n";
        std::cout << "Etudiant  : " << etu->getNomComplet() << " (" << etu->getMatricule() << ")\n";
        std::cout << "Parcours  : " << etu->getFiliere() << " - Niveau " << etu->getNiveau() << "\n";
        std::cout << "======================================================\n";

        double sommeMoyennesUE = 0.0;
        int totalCreditsPossibles = 0;
        int totalCreditsValides = 0;

        for (const auto& ue : nivTrouve->getUEs()) {
            std::cout << "\nUE [" << ue.getCode() << "] " << ue.getIntitule()
                      << " (" << ue.getCredits() << " credits)\n";

            double sommePonderee = 0.0;
            double sommeCoefs = 0.0;

            for (const auto& mat : ue.getMatieres()) {
                double note = getNoteEtudiantMatiere(matricule, mat.getCode());
                std::cout << "  - " << std::left << std::setw(25) << mat.getNom()
