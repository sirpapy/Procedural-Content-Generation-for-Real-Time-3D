/***************************************************************************
                          matrix4d.hpp  -  description
                             -------------------
    copyright            : (C) 2012 by Biri Venceslas
    email                : biri@univ-mlv.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _STP3D_GL_TOOLS_HPP_
#define _STP3D_GL_TOOLS_HPP_

#include "globals.hpp"
#include <string>

#ifndef GL_STP3D_DEFINED

#define GL_STP3D_DEFINED

#define GL_GLEXT_PROTOTYPES 1

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>

namespace STP3D {

// ///////////////////////////////////////////////////////////////////////////
// GL check tool
// ///////////////////////////////////////////////////////////////////////////
inline void checkGL(int line, const char *filename) {
	int err = glGetError();
	if(err != GL_NO_ERROR) {
		std::cerr << "ERROR GL : erreur dans le fichier " << filename << " à la ligne " ;
		std::cerr << line << " : " << gluErrorString(err) << std::endl;
		//exit(0);
	}
}
#define CHECK_GL STP3D::checkGL(__LINE__, __FILE__);

#endif // GL_DEFINED

/** GL Tools.
  * This class is a set of GL tools functions
  * @author Venceslas BIRI (biri@univ-mlv.fr)
  */
class GLTools {
public: 
	static void takeSnapshot(int w,int h,unsigned char *image_data,uint nb_channel = 3);
};

inline void GLTools::takeSnapshot(int w,int h,unsigned char *image_data,uint nb_channel) {
	glPixelStorei(GL_PACK_ALIGNMENT,1);
	if(nb_channel == 1) glReadPixels(0,0,(GLint)w,(GLint)h,GL_RED,GL_UNSIGNED_BYTE,image_data);
	if(nb_channel == 3) glReadPixels(0,0,(GLint)w,(GLint)h,GL_RGB,GL_UNSIGNED_BYTE,image_data);
	if(nb_channel == 4) glReadPixels(0,0,(GLint)w,(GLint)h,GL_RGBA,GL_UNSIGNED_BYTE,image_data);
}

} // End namespace
#endif
