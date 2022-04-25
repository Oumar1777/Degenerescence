// g++ -Wall projet.cpp -o projet
// time ./projet

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
using namespace std;
// Variables Globales
// Graphe
// nombre de sommets et d'aretes
int nbSom = 0, nbAre = 0;
// Matrice des voisins
vector<vector<int>> voisinage;
// Tableau de clés associatives
map<int,int> code;
// Tableau marqued
vector<bool> marqued;
// Tableau des numéro de centres
vector<int> numCentre;

// Matula & Beck
// Liste de sortie L
vector<int> L;
// Tableau des sommets
vector<vector<int>> D;


void initTabs(){
    numCentre.assign(nbSom, 0);
    marqued.assign(nbSom, false);
}

/**
 * @brief Fonction construction
 * @details Construire une representation de graphe à partir d'un fichier au format SNAP
 * @param file : le nom du fichier
 * @return void
 */
void construction(string file){
    string ligne;
    // Ouverture de file en lecture
    ifstream fichier(file, ios::in);

    if(!fichier){
        cerr << "Impossible d'ouvrir le fichier " << file <<" !!" << endl;
        exit (EXIT_FAILURE);
    }
    else{
        cout << "Fichier " << file << " ouvert !!" << endl;

        // Lecture du fichier ligne par ligne
        int pos, val = 0, s1, s2;
        while (getline(fichier, ligne))
        {
            if(ligne.find('#') == string::npos && ligne.find('%') == string::npos && ligne != ""){
                //cout << ligne << endl;
                pos = ligne.find("\t");
                if(ligne.find("\t") == string::npos)
                    pos = ligne.find(" ");
                val = stoi(ligne.substr(0,pos));

                auto it = code.find(val);
                if(it == code.end()){
                    code.insert({val, nbSom++});
                    vector<int> v;
                    voisinage.push_back(v);
                }
                   
                s1 = code.at(val);

                val = stoi(ligne.substr(pos, ligne.size()-pos));

                it = code.find(val);
                if(it == code.end()){
                    code.insert({val, nbSom++});
                    vector<int> v;
                    voisinage.push_back(v);
                }

                s2 = code.at(val);

                voisinage[s1].push_back(s2);
                voisinage[s2].push_back(s1);
            
                nbAre++;
            }
        } 
        // Initialisation des tableaux
        initTabs();
        // Fermeture du fichier
        fichier.close();
    }
    
}

/**
 * @brief Fonction d'affichage showCode
 * @details Montre le contenu de la map code (valeur et code associé)
 */
void showCode(){
    for(auto& it : code)
        cout << "val : " << it.first << " code : " << it.second << endl;
}

/**
 * @brief Retourne le code associé à une valeur
 * @param val: la valeur dont le code est à trouver
 * @return int: le code associé à la valeur
 */
int codeOf(int val){
    return code.at(val);
}

/**
 * @brief Retourne la valeur associée à un code
 * @param lecode 
 * @return int 
 */
int valOf(int lecode){
    for(auto& it : code)
        if(it.second == lecode) return it.first;
    return -1;
}


/**
 * @brief Fonction d'affichage showVoisinage
 * @details Montre la matrice des voisins de chaque sommet 
 */
void showVoisinage(){
    for(int i = 0; i < (int)voisinage.size(); i++){
        cout << "|";
        for(int j = 0; j < (int)voisinage[i].size(); j++){
            cout << "" << voisinage[i][j] << "|";
        }
        cout << "\n";
    }
    cout << "Nombre de sommets : " << nbSom << endl;
    cout << "Nombre d'arêtes : " << nbAre << endl;
}


/**
 * @brief Retourne le dégré du sommet avec le code ind
 * @param ind 
 * @return int 
 */
int degree(int ind){
    int deg = 0;
    for(int i = 0; i < (int)voisinage[ind].size(); i++)
        if(!marqued[voisinage[ind][i]])
            deg++;
    return deg;
}

/**
 * @brief Retourne le dégré max du graphe
 * 
 * @return int 
 */
int degreeMax(){
    int deg = 0, maxi = 0;
    for(int i = 0; i < (int)voisinage.size(); i++){
        deg = degree(i);
        if(deg > maxi)
            maxi = deg;
    }   
    return maxi;
}

/**
 * @brief Calcule la dégénerescence du graphe et remplit le tableau de centre
 * 
 */
int degenerescence(){
    int k = 0, nbNotMarqued = nbSom;
    bool modified = false;
    while(nbNotMarqued != 0){
        //cout << "Pour k = " << k << endl;
        for(int i = 0; i < nbSom; i++){
            if((!marqued[i]) && (degree(i) <= k)){
                marqued[i] = true;
                nbNotMarqued--;
                modified = true;
                numCentre[i] = k;
                
                //cout << "Sommet codé "<< i <<" supprimé !" <<endl;
            }
        }

        if(!modified){
            //cout << "k incrémenté !" << endl;
            k++;
        }
        modified = false;
    }
    return k+1;
}

/**
 * @brief 
 * 
 */
void showVect(vector<int> vect){
    for(int i = 0; i < (int)vect.size(); i++)
        cout << "|" << vect[i];
    cout << "|" << endl;
}

void showD(){
    for(int i = 0;  i < (int)D.size(); i++){
        cout << "D[" << i << "] = [";
        for(int j = 0; j < (int)D[i].size(); j++)
            cout << D[i][j] << ", ";
        cout << "]" << endl;
    }       
}

bool isEmpty(vector<int> vec){
    for(int i = 0; i < (int)vec.size(); i++)
        if(!marqued[vec[i]]) return false;
    return true;
}

int firstNotMarqued(vector<int> vec){
    for(int i = 0; i < (int)vec.size(); i++)
        if(!marqued[vec[i]]) return vec[i];
    return -1;
}

/**
 * @brief 
 * 
 * @return int 
 */
int degenerescenceByMatulaAndBeck(){
    // Liste de sortie L initialisé

    // Initialisation de D telque D[i] <==> liste des sommets de degré i
    vector <int> vec;
    D.assign(degreeMax()+1, vec);
    for(int i = 0; i < (int)voisinage.size(); i++)
        if(!marqued[i])
            D[degree(i)].push_back(i);

    //showD();
    //showVect(L);

    // Initialisation de k à 0
    int k = 0, i = 0, s, deg;

    for(int j = 0; j < nbSom; j++){
        // Parcours jusqu'à trouver un D[i] non vide
        if(i != 0) i--;
        //cout << "i = " << i << endl;
        while (D[i].empty() || ((s = firstNotMarqued(D[i])) == -1))
            i++;
        // K max entre i et lui-même
        k = max(k,i);
    
        L.insert(L.begin(),s);
        marqued[s] =  true;
    
    // Pour tous voisins non marqués de s
       for(auto v : voisinage[s]){
           if(!marqued[v]){
                deg = degree(v);
                D[deg].insert(D[deg].begin(), v);
                //cout << "voisin non marqué de " << s << " : " << v << endl;
           }
       }
  
        numCentre[s] = k;

        //cout << "Parcours " << j << " :" << endl;
        //showD();
        //showVect(L);
    }
    return k+1;
}



int main()
{
    construction("Files/facebook.txt");
    int d;
    //showCode();
    //showVoisinage();
    d = degenerescence();
    cout << "Degenerence : " << d << endl;
    //showVect(numCentre);
    
    initTabs();

    d = degenerescenceByMatulaAndBeck();
    cout << "Degenerence by Matula & Beck : " << d << endl;
    //showVect(numCentre);
    //showVect(L);

    return 0;   
}