#include "Mesh.h"
#include "tol/tiny_obj_loader.h"
#include <glm.hpp>
#define GL_GLEXT_PROTOTYPES
#include <glcorearb.h>

Mesh::Mesh(const char* filename)
{
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    tinyobj::LoadObj(shapes, materials, filename);
    tinyobj::mesh_t mesh = shapes[0].mesh;
    count = mesh.indices.size();
    positions_size = mesh.positions.size();

    glGenBuffers(1, &vbuffer);
    glGenBuffers(1, &ibuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
    int vbufferSize = mesh.positions.size() + mesh.normals.size();

    glBufferData(GL_ARRAY_BUFFER, vbufferSize*sizeof(float), NULL, GL_STATIC_DRAW);

    glBufferSubData
    (GL_ARRAY_BUFFER,
     0,
     mesh.positions.size()*sizeof(float),
     mesh.positions.data());

    glBufferSubData
    (GL_ARRAY_BUFFER,
     mesh.positions.size()*sizeof(float),
     mesh.normals.size()*sizeof(float),
     mesh.normals.data());

    glBufferData
    (GL_ELEMENT_ARRAY_BUFFER,
     mesh.indices.size()*sizeof(unsigned int),
     mesh.indices.data(),
     GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &vbuffer);
    glDeleteBuffers(1, &ibuffer);
}

void Mesh::render
(GLuint program,
 glm::mat4 modelView, glm::mat4 projection,
 glm::vec4 color, GLenum mode) const
{
    glUseProgram(program);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(positions_size*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glUniformMatrix4fv(0, 1, GL_FALSE, &modelView[0][0]);
    glUniformMatrix4fv(1, 1, GL_FALSE, &projection[0][0]);
    glUniform4fv(2, 1, &color[0]);
    glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}
