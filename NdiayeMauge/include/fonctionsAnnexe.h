//
// Created by sirpapy on 14/05/16.
//

#ifndef MAINFOLDER_FONCTIONSANNEXE_H
#define MAINFOLDER_FONCTIONSANNEXE_H
#define FLT_MAX 3.40282347E+38F
#define X 0
#define Y 1
#define Z 2




Color black(0.0, 0.0, 0.0, 0);
Color white_light(1.0, 1.0, 1.0, 0);
Color maroon(0.6, 0.2, 0.0, 0);
Color gold(1.0, 0.843137, 0.0, 0);
Color gray(0.5, 0.5, 0.5, 0);
Color darkturquoise(0.0, 0.807843, 0.819608, 0);
Color firebrick(0.698039, 0.133333, 0.133333, 0);
Color crimson(0.862745, 0.0784314, 0.235294, 0);
Color seagreen(0.180392, 0.545098, 0.341176, 0.3);

/**
 * @method:
 * @description:
 * @param   ray   (Ray)  -- tracer object that defines its point and direction in 3-space.
 * @return.
**/

double fRand(double fMin, double fMax) {
    double f = (double) rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}


/**
 * @method: pickAColor
 * @description: Permet de choisir une couleur et ainsi definir sa valeur spéciale
 * @param  un entier correspondant à la couleur voulu et sa valeur speciale
 * @return une couleur
**/
Color pickAColor(int color, double special) {
    switch (color) {
        case 0: {
            Color white_light(1.0, 1.0, 1.0, special);
            return white_light;
        }
        case 1: {
            Color maroon(0.6, 0.2, 0.0, special);
            return maroon;
        }
        case 2: {
            Color gold(1.0, 0.843137, 0.0, special);
            return gold;
        }
        case 3: {
            Color gray(0.5, 0.5, 0.5, special);
            return gray;
        }
        case 4: {
            Color darkturquoise(0.0, 0.807843, 0.819608, special);
            return darkturquoise;
        }
        case 5: {
            Color crimson(0.862745, 0.0784314, 0.235294, special);
            return crimson;
        }
        case 6: {
            Color firebrick(0.698039, 0.133333, 0.133333, special);
            return firebrick;
        }
        case 7: {
            Color seagreen(0.180392, 0.545098, 0.341176, special);
            return seagreen;
        }
        default: {
            Color white_light(1.0, 1.0, 1.0, special);
            return white_light;
        }
    }
}


/**
 * @method: RandomColor
 * @description: Permet de choisir de manière "aleatoire" une couleur
 * @return Une couleur
**/
Color RandomColor() {
    switch ((int) fRand(0, 7)) {
        case 0:
            return white_light;
        case 1:
            return maroon;
        case 2:
            return gold;
        case 3:
            return gray;
        case 4:
            return darkturquoise;
        case 5:
            return firebrick;
        case 6:
            return crimson;
        case 7:
            return seagreen;
        default:
            return white_light;
    }
}


/**
 * @method: split
 * @description: Permet de splitter une chaine de caractére grace à un délimiteur en plusieurs sous chaines
 * @param   str String, delimiter char
 * @return liste de sous chaines.
**/

vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}


/**
 * @method: findMaxValue
 * @description: permet de trouver la valeur maximale d'une liste
 * @param   list vector<double>
 * @return le max
**/
double findMaxValue(vector<double> list) {
    double max = 0;
    for (int i = 0; i < list.size(); i++) {
        if (max < list.at(i)) {
            max = list.at(i);
        }
    }
    return max;
}


/**
 * @method: closestObject
 * @description: Quand il ya plusieurs intersection on doit trouver la plus petite face toucher par le rayon et être affiché.
 * @param   object_intersections vector<double> liste des intersection
 * @return l'index à afficher
**/

int closestObject(vector<double> object_intersections) {

    int minIndex;


    if (object_intersections.size() == 0) {
        //Il n'y a pas eu d'intersection
        return -1;
    }
    else if (object_intersections.size() == 1) {
        if (object_intersections.at(0) == 0) {
            // le seul point d'intersection
            return 0;
        } else {
            //Le rayon n'a pas fait d'intersection
            return -1;

        }
    } else {

        //otherwise there is more than on intersection
        //first find the maximum value

        double max = findMaxValue(object_intersections);

        //then starting from the maximum value and find the minimum positive value
        if (max > 0) {

            // we only want positive intersection
            for (int index = 0; index < object_intersections.size(); index++) {
                if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
                    max = object_intersections.at(index);
                    minIndex = index;

                }
            }

            return minIndex;

        } else {
            //all the intersections were negative
            return -1;
        }
    }
}












#endif //MAINFOLDER_FONCTIONSANNEXE_H
