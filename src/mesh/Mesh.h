//
// Created by keega on 6/9/2021.
//

#ifndef FORGE_MESH_H
#define FORGE_MESH_H
#include <glad/glad.h> // holds all OpenGL type declarations

#include <vector>
#include "helpers/Vertex.h"
#include "helpers/Texture.h"
#include "../utils/shader-manager/Shader.h"

class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

    void Draw(Shader &shader) {
        // bind textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        for (unsigned int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit

            std::string number;
            std::string name = textures[i].type;

            if (name == "texture diffuse") {
                number = std::to_string(diffuseNr++);
            } else if(name == "texture_specular") {
                number = std::to_string(specularNr++); // transfer unsigned int to stream
            } else if(name == "texture_normal") {
                number = std::to_string(normalNr++); // transfer unsigned int to stream
            } else if(name == "texture_height") {
                number = std::to_string(heightNr++); // transfer unsigned int to stream
            }

            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);

            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        // set back to default
        glActiveTexture(GL_TEXTURE0);

    }

private:
    unsigned int VBO, EBO;



    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

        // vertex normals
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoords));

        glBindVertexArray(0);

    }

};

#endif //FORGE_MESH_H
