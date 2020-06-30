/* CODEVSI Base Roulante - Groupe 4
* 
* Programme Arduino permettant de piloter les deux moteurs d'une base roulante afin de réaliser une trjectoire programmée via l'application associée
* 
* VERSION SANS LE PID
* 
* Pour le faire fonctionner: 
*   - Lancer la simulation
*   - Copiez-collez dans le moniteur la liste de vitesses présente dans le fichier texte issu de l'application
*   - Appuyez sur "Envoyer"
*   - Les deux moteurs tournent aux vitesses désirées afin de réaliser la trajectoire demandé
*
*/


// DESIGNATION DES PINS


int IN_G = 11; //broches hacheur Enable et IN
int IN_D = 6;
int EN_G = 8;
int EN_D = 7;

int TRIG = 13; //broches capteur à Ultrasons
int ECHO = 4;

int GAZ = A0;  //broche analog du détecteur de gaz


// DECLARATION DES VARIABLES 


char chiffre ;       // va valloir chaque caractère du moniteur ex: 2 puis 5 puis 5 puis ,
String nombre = "";  // va valloir chaque valeur de vitesse du moniteur ex: 255 puis 245

int nombrevirgules ; //variable permettant de savoir quel moteur piloter à chaque valeur

int compteur_test_distance; //compteur pour savoir quand mesurer la distance


// FONCTION CAPTEUR DE CALCUL DE LA DISTANCE


int lecture_distance() //Fonction sans entrée qui renvoie la distance en cm calculée par le capteur.
	{       
    int cm, lecture_echo; 
  
    digitalWrite(TRIG, HIGH); //Envoi du ping
    delayMicroseconds(0.1);  
    digitalWrite(TRIG, LOW);
  
    lecture_echo = pulseIn(ECHO,HIGH);
    cm = lecture_echo /58;    //Calcul distance en cm
    return(cm);
	}


void setup() 
	{  

// DECLARATION DES PINS DIGITAL

    pinMode(ECHO, INPUT);    //pour les capteurs
    pinMode(TRIG, OUTPUT);
    pinMode(GAZ, INPUT);
  
    Serial.begin(9600);
  
    digitalWrite(EN_D,HIGH); //on enclenche les moteurs
    digitalWrite(EN_G,HIGH);  
}




void loop()
	{  

// TEST CAPTEUR DE DISTANCE

    if (compteur_test_distance == 20)
    	{
  		if  (lecture_distance() < 20) //si le mur est à moins de 20cm
  			{
    		digitalWrite(EN_D,LOW);   //on coupe les deux moteurs
    		digitalWrite(EN_G,LOW);
    		Serial.println("DISTANCE TROP COURTE");
          	delay(3000);
  			}
  		compteur_test_distance = 0;   //on réinitialise le compteur
   	 	}
  	compteur_test_distance = compteur_test_distance + 1;  //on incrémente le compteur
  
// TEST DETECTEUR DE GAZ - COMMUNICATION INTERFACE - PILOTAGE MOTEUR
  
  	if (compteur_test_distance != 20)
    	{
        if (analogRead(GAZ) > 2)     //si le capteur de gaz détecte plus de gaz que le seuil fixée
            {
            Serial.println("TROP DE GAZ");
            }  
        if (Serial.available())      // si des caractères sont présents dans le moniteur
            {
            chiffre = Serial.read(); //on lit le caractère suivant
            if (chiffre == ',')      // dans ce cas, nombre est "complet" et on peut exploiter sa valeur 
                {
                nombrevirgules = nombrevirgules + 1;
               	if (nombrevirgules % 2 == 0)  //si le nombre de virgules passées est pair on pilote le moteur gauche
                	{                                 
                    analogWrite(IN_G,nombre.toInt());  // on donne la consigne au moteur gauche                    
                	}
              	else   //si le nombre de virgules est impair on pilote le moteur gauche
                	{   
                  	Serial.println(nombre);            
                    analogWrite(IN_D,nombre.toInt());  // on donne la consigne au moteur                                           	  
                    }
              	nombre = ""; // on réinitialise la variable nombre
            	} 
          	else   //nombre n'est pas complet et on lui ajoute chiffre
            	{
                if (chiffre != ' ' and chiffre != '[' and chiffre != ']') //on ne complète pas nombre par chiffre dans ces cas
                	{  
                    nombre = nombre + chiffre;
                    }
                }
            }
      	}    
	}
