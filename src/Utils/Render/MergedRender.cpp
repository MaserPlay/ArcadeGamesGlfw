//
// Created by super on 16.07.2024.
//

#include "MergedRender.h"
const GLchar* MergedRender::TextureFragmentShader = "#version 330 core\nin vec2 TexCoord;uniform sampler2D ourTexture;void main(){gl_FragColor = texture(ourTexture, TexCoord);}";

#define LogError(shader) {GLchar infoLog[512]; GLint success; \
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success); \
            if (!success) { glGetShaderInfoLog(shader, 512, NULL, infoLog); SPDLOG_WARN("ERROR::SHADER {}", infoLog); \
            }/* else {SPDLOG_DEBUG("No ERROR::SHADER");}*/}
void MergedRender::VerticesChanged() {
    std::vector<GLfloat> vv {};
    indices = {};
    for (int j = 0; j <= 3; ++j) {
        vv.push_back(quard->getVertices()[j].x);
        vv.push_back(quard->getVertices()[j].y);
        //TEXTURE COORDS
        if (quard->texture != NULL) {
            vv.push_back(quard->texture->texturecords[j].x);
            vv.push_back(quard->texture->texturecords[j].y);
        } else {
            vv.push_back(0);
            vv.push_back(0);
        }
        //COLOR
        vv.push_back(quard->color.r);
        vv.push_back(quard->color.g);
        vv.push_back(quard->color.b);
        vv.push_back(quard->color.a);

        indices.push_back((1) * 4 - (4 - j));
    }
    auto speedGL = GL_DYNAMIC_DRAW;
    switch (speed) {
        case SpeedContent::STATIC: {
            speedGL = GL_STATIC_DRAW;
            break;
        }
        case SpeedContent::DYNAMIC: {
            speedGL = GL_DYNAMIC_DRAW;
            break;
        }
        case SpeedContent::STREAM: {
            speedGL = GL_STREAM_DRAW;
            break;
        }
#ifdef _DEBUG
        default:{
            SPDLOG_ERROR("Forgot to add case in speed");
            break;
        }
#endif
    }


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vv.size() * sizeof(vv), vv.data(), speedGL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), indices.data(), speedGL);
}

MergedRender::~MergedRender() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


void MergedRender::SetUniform(float attr, const std::string& name) {
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), attr);
}

void MergedRender::SetUniform(glm::vec2 attr, const std::string& name) {
    glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), attr.x, attr.y);
}

void MergedRender::SetUniform(glm::vec3 attr, const std::string& name) {
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), attr.x, attr.y, attr.z);
}

void MergedRender::SetUniform(glm::vec4 attr, const std::string& name) {
    glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), attr.x, attr.y, attr.z, attr.w);
}

void MergedRender::SetUniform(glm::mat4 attr, const std::string &name) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &attr[0][0]);
}

void MergedRender::use(glm::mat4 matrix) {
    useProgramm();
    SetUniform({getWidth(),getHeight()}, "u_resolution");
    SetUniform(matrix, "projection");
    useClear();
}

void MergedRender::useProgramm() {
    //SHADER
    if (shaderProgram >= 0)
    {
        glUseProgram(shaderProgram);
    }
}

void MergedRender::useClear() {

    //TEXTURE
    if (quard->texture != NULL) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, quard->texture->getInitImage());
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
    }

    //BUFFER
    glBindVertexArray(VAO);
    glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, 0);
    //RESET ALL
//    glBindVertexArray(0);
//    glUseProgram(0);
}

void MergedRender::load() {
    if (shaderProgram <= 0)
    {
        if (quard->texture != NULL) {
            quard->texture->Load();
        }
        //SHADER
        GLint vertexShader, fragmentShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        //LOG
        LogError(vertexShader);
        // Fragment shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // Check for compile time errors
        LogError(fragmentShader);
        // Link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // Check for linking errors
        LogError(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        //BUFFER
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray(VAO);

        VerticesChanged();

        // Атрибут с координатами
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Атрибут с цветом
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(4* sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // Атрибут с тестурными координатами
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

        glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    }
}
