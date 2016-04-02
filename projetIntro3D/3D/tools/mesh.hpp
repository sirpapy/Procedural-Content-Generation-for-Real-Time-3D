/***************************************************************************
                          mesh.hpp  -  description
                             -------------------
    copyright            : (C) 2012 by Biri
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

#ifndef _STP3D_MESH_HPP_
#define _STP3D_MESH_HPP_

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#define GL4_PROTOTYPES 1


#include <iostream>
#include <string>
#include <vector>
#include "gl_tools.hpp"

namespace STP3D {

	/**
	  * \brief Mesh class allows to store generic informations about a mesh 
	  * that has no indirect order.
	  * Mesh class allows to store several float buffer to use with a GL shaders.
	  * Such buffers are not interleaved and each has a semantic on his own. 
	  * Note that a mesh MUST have at least one buffer of coordinates.
	  * This class allows also the creation of the corresponding VBO.
	  * This class may or may not store the data.
	  */
	class StandardMesh {
	public:
		/// Standard construtor. Creates an empty mesh withouh any information.
		StandardMesh(uint elts = 0,uint new_gl_type = GL_TRIANGLES) : nb_elts(elts),gl_type_mesh(new_gl_type),id_vao(0) {
			/*buffers.clear();
			size_one_elt.clear();
			attr_id.clear();
			attr_semantic.clear();
			vbo_id.clear();*/
		};
		~StandardMesh();
		//  User defined members
		/// All the data in CPU buffers
		std::vector<float*> buffers;
		/// Number of elements in each buffer : must be common !!
		uint nb_elts;
		/// Size of one element in each buffer
		std::vector<uint> size_one_elt;
		/// Attribute id corresponding to each buffer
		std::vector<uint> attr_id;
		/// Attribute semantic corresponding to each buffer
		std::vector<std::string> attr_semantic;
		/// Attribute semantic corresponding to each buffer
		uint gl_type_mesh;

		//  GL defined members
		/// Id of all VBO. Created by the GL API
		std::vector<uint> vbo_id;
		/// Id of the corresponding VAO
		uint id_vao;

		/// Set the number of elements in each buffers
		void setNbElt(uint elts) {nb_elts = elts;};
		void addOneBuffer(uint id_attribute,uint one_elt_size,
		                  float* data,std::string semantic,bool copy=false);
		void releaseCPUMemory();
		/*****************************************************************
		 *                      GL RELATED FUNCTIONS
		 *****************************************************************/
		void changeType(uint new_gl_type) {gl_type_mesh = new_gl_type;};
		bool createVAO();
		void draw() const;
	};

	inline StandardMesh::~StandardMesh() {
		if (buffers.size()>0) {
			for(std::vector<int>::size_type i = 0; i < buffers.size(); ++i) {
				if (buffers[i]) delete[](buffers[i]);
			}
		}
		size_one_elt.clear();
		attr_id.clear();
		attr_semantic.clear();
		glDeleteBuffers(vbo_id.size(),&(vbo_id[0]));
		vbo_id.clear();
		glDeleteVertexArrays(1,&id_vao);
	}

	inline bool StandardMesh::createVAO() {
		// Create and use the VAO
		glGenVertexArrays(1,&id_vao);
		if (id_vao == 0) {
			STP3D::setError("Unable to find a value for a VAO");
			return false;
		}
		glBindVertexArray(id_vao);

		// Check if mesh is defined
		if (buffers.size()==0) {
			STP3D::setError("Impossible to create VBO from empty buffers. This mesh has not been initialized");
			return false;
		}

		// Create all VBO (and check)
		vbo_id.resize(buffers.size());
		//uint* new_id = new uint[buffers.size()];
		glGenBuffers(buffers.size(),&(vbo_id[0]));
		/*
		for(uint i=0;i<buffers.size();++i) {
			std::cerr<<"Creation d'un VBO indice = "<<new_id[i]<<std::endl;
			if (new_id[i]==0) {STP3D::setError("Unable to find an empty VBO");return false;}
			vbo_id[i]=new_id[i];
		}
		delete[](new_id);
		*/

		// Transfer all data for all VBO from CPU to GPU
		for(std::vector<int>::size_type i = 0; i < buffers.size(); ++i) {
			glEnableVertexAttribArray(attr_id[i]);

			glBindBuffer(GL_ARRAY_BUFFER,vbo_id[i]);

			glBufferData(GL_ARRAY_BUFFER,nb_elts*size_one_elt[i]*sizeof(GLfloat),buffers[i],GL_STATIC_DRAW);

			glVertexAttribPointer(attr_id[i], size_one_elt[i], GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER,0);
		}
		
		glBindVertexArray(0);
		return true;
	}

	inline void StandardMesh::addOneBuffer(uint id_attribute,uint one_elt_size,
	                                       float* data,std::string semantic,bool copy) {
		if (copy) {
			float* tab = new float[one_elt_size*nb_elts];
			memcpy(tab,data,one_elt_size*nb_elts*sizeof(float));
			buffers.push_back(tab); 
		}
		else buffers.push_back(data);
		attr_id.push_back(id_attribute);
		size_one_elt.push_back(one_elt_size);
		//std::cerr<<"Adding semantic :"<<semantic<<"!"<<std::endl;
		attr_semantic.push_back(semantic);
	}

	inline void StandardMesh::draw() const {
		glBindVertexArray(id_vao);

		glDrawArrays(gl_type_mesh,0,nb_elts);

		glBindVertexArray(0);
	}


	inline void StandardMesh::releaseCPUMemory() {
		for(std::vector<int>::size_type i = 0; i < buffers.size(); ++i) {
			if (buffers[i]) delete[](buffers[i]);
			buffers[i] = NULL;
		}
	}
};

#endif


