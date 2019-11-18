#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <omp.h>
#include <cmath>
#include <ctype.h>
#include <random>
#include <fstream>
#include <iomanip>
#define G (6.674*pow(10, -5))
#define INTERVALO_TIEMPO 0.1
#define DISTANCIA_MIN 2.0
#define WIDTH 200
#define HEIGHT 200
#define MASS 1000
#define SDM 50

using namespace std;

bool isValidInt(char* x);

/* Creacion de las estructuras */
class planeta {
    public:
        double x;
        double y;
        double masa;
        planeta(double cx,double cy, double cmasa):
        x{cx}, y{cy}, masa{cmasa}{}//...

        planeta(){}

        /*Metodos del planeta */
        void printPlaneta(){
            cout << "PLANETA\n";
            cout << "x: " << x << "\n";
            cout << "y: " << y << "\n";
            cout << "masa: " << masa << "\n\n";
        }
};


class asteroide {
    public:
        double x;
        double y;
        double masa;
        double velocidadx;
        double velocidady;
        double aceleracionx;
        double aceleraciony;

        asteroide(double cx,double cy, double cmasa, double cvelocidadx, double cvelocidady, double caceleracionx, double caceleraciony):
        x{cx}, y{cy}, masa{cmasa}, velocidadx{cvelocidadx}, velocidady{cvelocidady}, aceleracionx{caceleracionx}, aceleraciony{caceleraciony}{}//...

        asteroide(){}

        /*Metodos del asteroide */
        void printAsteroide(){
            cout << "ASTEROIDE:\n";
            cout << "x: " << x << "\n";
            cout << "y: " << y << "\n";
            cout << "masa: " << masa << "\n";
            cout << "velocidad en eje x: " << velocidadx << "\n";
            cout << "velocidad en eje y: " << velocidady << "\n";
            cout << "aceleracion en eje x: " << aceleracionx << "\n";
            cout << "aceleracion en eje y: " << aceleraciony << "\n\n";
        }
        void setx(double cx){
            x=cx;
        }
        void sety(double cy){
            x=cy;
        }
};

/* Calcula la distancia entre un asteroide y un planeta */
double distAsteroidePlaneta(asteroide a, planeta b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

/* Calcula la distancia entre dos asteroides */
double distAsteroideAsteroide(asteroide a, asteroide b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

/* Calcula la pendiente entre un asteroide y un planeta */
double pendienteAsteroidePlaneta(asteroide a, planeta p){
	double f = ((a.y - p.y)/(a.x - p.x));
	if(f <= -1){ return -1;}
	if(f >= 1){ return 1;}
	return f;
}

/* Calcula la pendiente entre dos asteroides */
double pendienteAsteroideAsteroide(asteroide a, asteroide p){
	double f = ((a.y - p.y)/(a.x - p.x));
	if(f <= -1){ return -1;}
	if(f >= 1){ return 1;}
	return f;
}

/* Calcula el angulo a partir de la pendiente */
double angulo(double pendiente){
	return atan(pendiente);
}


/*SI LA DIRECCION ES DE a A b EL TERCER PARAMETRO SERÁ 1*/
/*SI LA DIRECCION ES DE b A a EL TERCER PARAMETRO SERÁ -1*/

/*Fuerza de atraccion entre dos asteroides en el eje x */
double fuerzaAtraccionXAsteroideAsteroide(asteroide a, asteroide b) {
    double fx = (G * a.masa * b.masa)/(distAsteroideAsteroide(a, b)) * cos(angulo(pendienteAsteroideAsteroide(a, b)));
    if(fx > 100){
        return 100.0;
    }
    else{
        return fx;
    }
}
double fuerzaAtraccionXAsteroidePlaneta(asteroide a, planeta b) {
    double fx = (G * a.masa * b.masa)/(distAsteroidePlaneta(a, b)) * cos(angulo(pendienteAsteroidePlaneta(a, b)));
    if(fx > 100){
        return 100.0;
    }
    else{
        return fx;
    }
}

/*Fuerza de atraccion entre dos asteroides en el eje y */
double fuerzaAtraccionYAsteroideAsteroide(asteroide a, asteroide b) {
    double fy = (G * a.masa * b.masa)/(distAsteroideAsteroide(a, b)) * sin(angulo(pendienteAsteroideAsteroide(a, b)));
    if(fy > 100){
        return 100.0;
    }
    else{
        return fy;
    }
}
double fuerzaAtraccionYAsteroidePlaneta(asteroide a, planeta b) {
    double fy = (G * a.masa * b.masa)/(distAsteroidePlaneta(a, b)) * sin(angulo(pendienteAsteroidePlaneta(a, b)));
    if(fy > 100){
        return 100.0;
    }
    else{
        return fy;
    }
}

int signoFuerzaXAsteroideAsteroide(asteroide a, asteroide b){
    if(a.x > b.x) {
        return -1;
    }
    if(a.x < b.x) {
        return 1;
    }
    else { //if(a.x == b.x)
        return 0;
    }
}
int signoFuerzaYAsteroideAsteroide(asteroide a, asteroide b){
    if(a.y > b.y) {
        return -1;
    }
    if(a.y < b.y) {
        return 1;
    }
    else { //if(a.y == b.y)
        return 0;
    }
}

int signoFuerzaXAsteroidePlaneta(asteroide a, planeta b){
    if(a.x > b.x) {
        return -1;
    }
    if(a.x < b.x) {
        return 1;
    }
    else { //if(a.x == b.x)
        return 0;
    }
}
int signoFuerzaYAsteroidePlaneta(asteroide a, planeta b){
    if(a.y > b.y) {
        return -1;
    }
    if(a.y < b.y) {
        return 1;
    }
    else { //if(a.y == b.y)
        return 0;
    }
}

asteroide aplicacionDeFuerzasXAsteroideAsteroide(asteroide a, double fuerzas){
	a.aceleracionx=(1/a.masa)*fuerzas;
    return a;
}
asteroide aplicacionDeFuerzasXAsteroidePlaneta(asteroide a, double fuerzas){
	a.aceleracionx=(1/a.masa)*fuerzas;
    return a;
}


asteroide aplicacionDeFuerzasYAsteroideAsteroide(asteroide a, double fuerzas){
	a.aceleraciony=(1/a.masa)*fuerzas;
    return a;
}
asteroide aplicacionDeFuerzasYAsteroidePlaneta(asteroide a, double fuerzas){
	a.aceleraciony=(1/a.masa)*fuerzas;
    return a;
}


asteroide calculoVelocidadX(asteroide a){
	a.velocidadx=a.velocidadx+ a.aceleracionx*INTERVALO_TIEMPO;
    return a;
}

asteroide calculoVelocidadY(asteroide a){
	a.velocidady=a.velocidady+ a.aceleraciony*INTERVALO_TIEMPO;
    return a;
}

asteroide modificarPosicionX(asteroide a){
	a.x=a.x+ a.velocidadx*INTERVALO_TIEMPO;
    return a;
}

asteroide modificarPosicionY(asteroide a){
	a.y=a.y+ a.velocidady*INTERVALO_TIEMPO;
    return a;
}

/*-----------------------------------------------------------*/
/*--------------------EFECTO REBOTE--------------------------*/
/*-----------------------------------------------------------*/


asteroide limiteEspacio(asteroide a){
    if(a.x <= 0){
        cout << "menor o igual que cero en el eje de las x\n\n";
        a.x = 5.0;
        a.velocidadx = -1 * a.velocidadx;
    }
    if(a.y <= 0){
        cout << "menor o igual que cero en el eje de las y\n\n";
        a.y = 5.0;
        a.velocidady = -1 * a.velocidady;
    }
    if(a.x >= WIDTH){
        cout << "menor o igual que width en el eje de las x\n\n";
        a.x = WIDTH - 5.0;
        a.velocidadx = -1 * a.velocidadx;
    }
    if(a.y >= HEIGHT){
        cout << "menor o igual que height en el eje de las y\n\n";
        a.y = HEIGHT - 5.0;
        a.velocidady = -1 * a.velocidady;
    }
    return a;
}

asteroide choqueAsteroide(asteroide a, asteroide b){
    a.velocidadx = b.velocidadx;
    a.velocidady = b.velocidady;
    return a;
}




int main(int argc, char *argv[]) {

    /* Comprobamos que los datos introducidos son correctos */
    if(argc != 5){
       cout << "Tiene que introducir 4 parametros" << "\n";
       return -1;
    }
    for (int i = 1; i < argc; i++){
        if(!isValidInt(argv[i])) {
            cout << "Fallo en dato introducido: " << argv[i] << "\n";
            return -1;
        }
    }
    if(stoi(argv[4]) <= 0){
        cout << "La semilla tiene que ser mayor que cero" << "\n";
        return -1;
    }

    int num_asteroides = stoi (argv[1]);
    int num_planetas = stoi (argv[3]);
    int seed = stoi (argv[4]);

    cout << "Execution setup\n\nNumber of bodies: " << num_asteroides << "\n";
    cout << "Number of iterations: " << argv[2] << "\n";
    cout << "Initial file: init_conf.txt" << "\n";
    cout << "Output file: out.txt" << "\n";
    cout << "Number of planets: " << num_planetas << "\n";
    cout << "Seed: " << seed << "\n\n";
    cout << "Number of bodies: " << num_asteroides << "\n";
    cout << "Gravity : " << G << "\n";
    cout << "Delta time: " << INTERVALO_TIEMPO << "\n";
    cout << "Number of steps: " << argv[2] << "\n";
    cout << "Min. distance: " << DISTANCIA_MIN << "\n";
    cout << "Width: " << WIDTH << "\n";
    cout << "Height: " << HEIGHT << "\n\n";

    planeta planetas[num_planetas];
    asteroide asteroides[num_asteroides];

    default_random_engine re{seed};
    uniform_real_distribution<double> xdist{0.0, std::nextafter(WIDTH, std :: numeric_limits<double>::max())};
    uniform_real_distribution<double> ydist{0.0, std::nextafter(HEIGHT, std :: numeric_limits<double>::max())};
    normal_distribution<double> mdist{MASS, SDM};

    ofstream fout("init_conf.txt");

    fout << argv[1] << " " << argv[2] << " " << argv[3] << " " << argv[4] << "\n";

    for(int j = 0; j < num_asteroides; j++){

        asteroides[j].x = xdist(re);
        asteroides[j].y = ydist(re);
        asteroides[j].masa =  mdist(re);

        /*cout <<"asteroide coordenada X :" << asteroides[j].x << "\n";
        cout << "asteroide coordenada Y :" << asteroides[j].y << "\n";
        cout << "masa asteroide :" << asteroides[j].masa << "\n";*/

        fout << fixed << setprecision(3) << asteroides[j].x <<" " << fixed << setprecision(3) << asteroides[j].y <<" " << fixed << setprecision(3) << asteroides[j].masa << "\n";
    }

    for (int i = 0; i< num_planetas; i++){
        if ( i % 4 == 0){
            planetas[i].x = 0;
            planetas[i].y = ydist(re);
        }
        else if ( i % 4 == 1){
            planetas[i].x = xdist(re);
            planetas[i].y = 0;
        }
        else if ( i % 4 == 2){
            planetas[i].x = WIDTH;
            planetas[i].y = ydist(re);
        }
        else if ( i % 4 == 3){
            planetas[i].x = xdist(re);
            planetas[i].y = HEIGHT;
        }

        planetas[i].masa = mdist(re) * 10;
        /*cout <<"planeta coordenada X :" << planetas[i].x << "\n";
        cout << "planeta coordenada Y :" << planetas[i].y << "\n";
        cout << "masa planeta :" << planetas[i].masa << "\n";*/
        fout << fixed << setprecision(3) << planetas[i].x <<" " << fixed << setprecision(3) << planetas[i].y <<" " << fixed << setprecision(3) << planetas[i].masa << "\n";
    }

    /* BLUCLE DE ITERACIONES */


    /*Empezamos con las iteraciones*/
    for (int i = 0; i < stoi(argv[2]); i++) {
        for (int j = 0; j < stoi(argv[1]); j++) { // Recorremos los asteroides
            double sumFuerzasX = 0;
            double sumFuerzasY = 0;
            for (int k = ceil(stoi(argv[1])/2); k < stoi(argv[1]); k++) { // Recorremos los num_asteroides
                double dist = distAsteroideAsteroide(asteroides[j], asteroides[k]);
                if(dist > 2){
                    if(j != k){
                        /*Calculamos el angulo de influencia*/
                        /*NO HACE FALTA PORQUE YA ESTAN METIDAS EN EL CALCULO DE FUERZAS
                        double pendiente = pendienteAsteroideAsteroide(asteroides[k], asteroides[j]);
                        double alfa = angulo(pendiente);
                        */
                        /*Calculamos la fuerza de atraccion*/
                        double fx = fuerzaAtraccionXAsteroideAsteroide(asteroides[j], asteroides[k]);
                        double fy = fuerzaAtraccionYAsteroideAsteroide(asteroides[j], asteroides[k]);
                        double fxA = signoFuerzaXAsteroideAsteroide(asteroides[j], asteroides[k]) * fx;
                        double fyA = signoFuerzaYAsteroideAsteroide(asteroides[j], asteroides[k]) * fy;
                        sumFuerzasX += fxA;
                        sumFuerzasY += fyA;
                        /*Aplicar fuerza y angulo*/
                    }
                }
                else{ /*Si los asteroides estan a menos de 2, se producira un choque*/
                    asteroide auxJ = choqueAsteroide(asteroides[j], asteroides[k]);
                    asteroide auxK = choqueAsteroide(asteroides[k], asteroides[j]);
                    asteroides[j] = auxJ;
                    asteroides[k] = auxK;



                }
            }
            for (int l = 0; i < stoi(argv[3]); i++) { // Recorremos los planetas
                double dist = distAsteroidePlaneta(asteroides[j], planetas[l]);
                if (dist > 2) {
                    double fx = fuerzaAtraccionXAsteroidePlaneta(asteroides[j], planetas[l]);
                    double fy = fuerzaAtraccionYAsteroidePlaneta(asteroides[j], planetas[l]);
                    double fxA = signoFuerzaXAsteroidePlaneta(asteroides[j], planetas[l]) * fx;
                    double fyA = signoFuerzaYAsteroidePlaneta(asteroides[j], planetas[l]) * fy;
                    sumFuerzasX += fxA;
                    sumFuerzasY += fyA;
                }
            }
            /* Modificamos las aceleraciones */
            asteroides[j] = aplicacionDeFuerzasXAsteroideAsteroide(asteroides[j], sumFuerzasX);
            asteroides[j] = aplicacionDeFuerzasYAsteroideAsteroide(asteroides[j], sumFuerzasY);
            /* Modificamos la velocidad */
            asteroides[j] = calculoVelocidadX(asteroides[j]);
            asteroides[j] = calculoVelocidadY(asteroides[j]);
            /*Modificamos la posicion del asteroide*/
            asteroides[j] = modificarPosicionX(asteroides[j]);
            asteroides[j] = modificarPosicionY(asteroides[j]);
            /*Comprobamos que el asteroide no esté en los bordes del espacio*/
            asteroides[j] = limiteEspacio(asteroides[j]);

        }
    }

    /*Imprimimos el archivo out.txt*/
    ALBERTO SUBNORMAL;
    ofstream outTxt("out.txt");
    for (int i = 0; i < stoi(argv[1]); i++) { // Recorremos los asteroides
        outTxt << asteroides[i].x << " " << asteroides[i].y << " " << asteroides[i].velocidadx << " " << asteroides[i].velocidady << " " << asteroides[i].masa << "\n";
    }

    return 0;
}

/* Metodos para comprobar que los datos introducidos son digitos */
bool isValidInt(char* x) {
	bool Checked = true;
	int i = 0;
	do{
		if (isdigit(x[i])){
			i++;
			Checked = true;
		}
		else{
			i++;
			Checked = false;
			break;
		}
	} while (x[i] != '\0');
	return Checked;
}
