#ifndef _SOURCE_H
#define _SOURCE_H


class Source {

	
	public:
	Source ();
	

	//method functions
	
virtual Vect getLightPosition(){
	return Vect(0,0,0);
}
virtual Color getColor(){
	return Color(1,1,1,0);
	
};
};

Source::Source (){
}



#endif
