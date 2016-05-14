#ifndef _COLOR_H
#define _COLOR_H


struct RGBType {
    double r;
    double g;
    double b;
};

class Color {
    double red, green, blue, special;

public:
    Color();
    Color(double);

    Color(double, double, double, double);
    //method functions

    double getColorRed() {
        return red;
    }



    double getColorGreen() {
        return green;
    }

    double getColorBlue() {
        return blue;
    }

    double getColorSpecial() {
        return special;
    }

    double setColorRed(double redValue) {
        red = redValue;
    };

    double setColorGreen(double greenValue) {
        green = greenValue;
    };

    double setColorBlue(double blueValue) {
        blue = blueValue;
    };

    RGBType returnForPixelColor() {
        RGBType *rgbType = new RGBType[0];
        rgbType[0].r = red;
        rgbType[0].g = green;
        rgbType[0].b = blue;

        return *rgbType;
    }

    double setColorSpecial(double specialValue) {
        special = specialValue;
    };




/*Amelioration*/




    double brightness(){
        return (red+green+blue)/3;
    }
    Color colorScalar(double scalar){
        return Color(red*scalar, green*scalar, blue*scalar, special);
    }
    Color colorAdd(Color color){
        return Color(red+ color.getColorRed(), green + color.getColorGreen(), blue+color.getColorBlue(), special+color.getColorSpecial());
    }
    Color colorMult(Color color){
        return Color(red*color.getColorRed(), green*color.getColorGreen(), blue*color.getColorBlue(), special*color.getColorSpecial());
    }

    Color colorAverage(Color color){
        return Color((red+color.getColorRed())/2, (green+color.getColorGreen())/2, (red+color.getColorRed())/2, (special+color.getColorSpecial())/2);
    }




    /**
 * @method: rogner
 * @description: Permet de normaliser les couleurs. Cest à dire Si la valeur d'une couleur dépasse 1
               * on le remet à 1 et si inferieur à 0, on le remet à 0
 * @return retourne la couleur normalisé
**/
    Color rogner(){
        double sommeCouleurs = red+blue+green;
        double testeurDeLimite = sommeCouleurs - 3;
        if(testeurDeLimite>0){
                red = red + testeurDeLimite*(red/sommeCouleurs);
                green = green + testeurDeLimite*(green/sommeCouleurs);
                blue = blue + testeurDeLimite*(blue/sommeCouleurs);
        }
        if(red > 1){
            red = 1;
        }
        if(green > 1){
            green = 1;
        }
        if(blue > 1){
            blue = 1;
        }
        if(red < 0){
            red = 0;
        }
        if(green< 0){
            green = 0;
        }
        if(blue < 0){
            blue = 0;
        }
        return Color(red, green, blue, special);
    }




/*Fin Amelioration*/



};

Color::Color(double s) {
    red = 0.5;
    green = 0.5;
    blue = 0.5;
    special = s;
}

Color::Color() {
    red = 0.5;
    green = 0.5;
    blue = 0.5;
}

Color::Color(double r, double g, double b, double s) {
    red = r;
    green = g;
    blue = b;
    special = s;

}

#endif
