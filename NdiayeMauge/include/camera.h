
#ifndef _CAMERA_H
#define _CAMERA_H


class Camera {
	Vect camposition, camdirection, camright, camdown;
	
	public:
	Camera ();
	
	Camera(Vect,Vect,Vect,Vect);
	
	//method functions
	
Vect getCameraPostion(){
	return camposition;
}


Vect getCameraDirection(){
	return camdirection;
}

Vect getCameraRight(){
	return camright;
}
Vect getCameraDown(){
	return camdown;
}	
};

Camera::Camera (){
	camposition = Vect(0,0,0);
	camdirection =  Vect(0,0,1);
	camright = Vect(0,0,0);
	camdown = Vect(0,0,0);
}

Camera::Camera (Vect position, Vect direction, Vect right, Vect down){
	camposition = position;
	camdirection = direction;
	camright = right;
	camdown = down;
}

#endif
