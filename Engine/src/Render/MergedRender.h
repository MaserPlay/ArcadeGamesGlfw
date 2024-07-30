//
// Created by super on 16.07.2024.
//

#ifndef ARCADEGAMES_MERGEDRENDER_H
#define ARCADEGAMES_MERGEDRENDER_H

#include <glm/ext/matrix_float4x4.hpp>
#include <utility>
#include "Context.h"
#include "Texture.h"
#include "Other.h"

class MergedRender final {
    //SHADER
public:
    MergedRender() = default;
    MergedRender(const MergedRender &p) = delete;   // удаляем конструктор
    void load();
    void use(glm::mat4 matrix);
    void useClear();
    void useProgramm();

    void setVertexShader(const GLchar *shaderSource) {
        MergedRender::vertexShaderSource = shaderSource;
    }

    void setFragmentShader(const GLchar *shaderSource) {
        MergedRender::fragmentShaderSource = shaderSource;
    }
    void SetUniform(float attr, const std::string& name);
    void SetUniform(glm::vec2 attr, const std::string& name);
    void SetUniform(glm::mat4 attr, const std::string& name);
    void SetUniform(glm::vec3 attr, const std::string& name);
    void SetUniform(glm::vec4 attr, const std::string& name);

    [[nodiscard]] GLuint getShaderProgram() const {
        return shaderProgram;
    }

    static const GLchar* TextureFragmentShader;

private:
    const GLchar* vertexShaderSource = "#version 330 core\nlayout (location = 0) in vec3 position;layout (location = 1) in vec4 color;layout (location = 2) in vec2 texCoord;uniform mat4 projection;out vec2 TexCoord;out vec4 Color;void main(){gl_Position = projection * vec4(position, 1);TexCoord = texCoord;Color = color;}";
    const GLchar* fragmentShaderSource = "#version 330 core\nin vec4 Color;void main()\n{\ngl_FragColor = Color;\n}\n\0";
    GLuint shaderProgram = 0;
    //BUFFER
public:
    enum class SpeedContent{
        STATIC,
        DYNAMIC,
        STREAM
    };
    struct Quard{
        Quard(ExtendedCoords ld, float width, float height, std::shared_ptr<Texture> texture = NULL) : texture(std::move(texture)) { setVertices(ld, width, height);}
        Quard() : Quard(std::array<ExtendedCoords, 4>{}, NULL) {}

        inline
        void setVertices(ExtendedCoords ld, float width, float height){
            vertices = {ExtendedCoords{ld.x + width, ld.y}, {ld.x + width, ld.y + height}, {ld.x, ld.y + height}, ld};
        }
        std::shared_ptr<Texture> texture {};
        Color color {1.,1.,1.,1.};

        [[nodiscard]] const std::array<ExtendedCoords, 4> &getVertices() const {
            return vertices;
        }

    private:
        /**
         * @deprecated
         */
        explicit Quard(const std::array<ExtendedCoords, 4> &vertices, std::shared_ptr<Texture> texture = nullptr) : vertices(vertices),
                                                                                        texture(std::move(texture)) {}
        std::array<ExtendedCoords, 4> vertices {};
    };
    std::unique_ptr<Quard> quard {new Quard()};
    void VerticesChanged();

    virtual ~MergedRender();
    void setSpeed(SpeedContent speed) {
        MergedRender::speed = speed;
    }

private:
    SpeedContent speed = SpeedContent::DYNAMIC;

    GLuint VBO{}, VAO{}, EBO{};
    std::vector<GLuint> indices;

};


#endif //ARCADEGAMES_MERGEDRENDER_H
