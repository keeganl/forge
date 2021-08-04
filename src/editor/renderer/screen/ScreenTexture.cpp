//
// Created by keega on 8/3/2021.
//

#include "ScreenTexture.h"

ScreenTexture::ScreenTexture() {
    // screen quad VAO
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &this->quadVBO);
    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->quadVertices), &this->quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}
