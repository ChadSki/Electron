//
//  senv.cpp
//  
//
//  Created by Samuco on 19/04/2015.
//
//

#include "sgla.h"

// Global instance
void sgla::setup(std::string path) {
    printf("sgla setup\n");
    GLuint vertex_shader, fragment_shader;
    vertex_shader   = make_shader(GL_VERTEX_SHADER,   (path + "/sgla.vert").c_str());
    fragment_shader = make_shader(GL_FRAGMENT_SHADER, (path + "/sgla.frag").c_str());
    program         = make_program(vertex_shader, fragment_shader);
    
    multipurposeMap = glGetUniformLocation(program, "multipurposeMap");
    cubeMap         = glGetUniformLocation(program, "cubeTextureMap");
    maps            = glGetUniformLocation(program, "maps");
    scale           = glGetUniformLocation(program, "scale");
    reflectionScale = glGetUniformLocation(program, "reflectionScale");
    printf("done\n");
}

void sgla::start() {
    glUseProgram(program);
    glUniform1i(multipurposeMap, 0);
    glUniform1i(cubeMap, 1);
}

void sgla::stop() {
    
}

void sgla_object::setBaseUV(float u, float v) {
    uscale = u;
    vscale = v;
}

// Senv object
void sgla_object::setup(ShaderManager *manager, ProtonMap *map, ProtonTag *shaderTag) {
    printf("multi setup\n");
    HaloTagDependency multi = *(HaloTagDependency*)(shaderTag->Data() + 0xC0);
    if (multi.tag_id.tag_index != NULLED_TAG_ID) {
        multipurposeMap = manager->texture_manager()->create_texture(map, multi);
        useMulti = true;
    }
    bumpScale = *(float*)(shaderTag->Data() + 0xBC);
    
    printf("cube setup\n");
    HaloTagDependency cube = *(HaloTagDependency*)(shaderTag->Data() + 0xAC);
    if (cube.tag_id.tag_index != NULLED_TAG_ID) {
        cubeMap = manager->texture_manager()->create_cubemap(map, cube);
        useCube = true;
    }
    reflectionPerpendicular = *(float*)(shaderTag->Data() + 0x8C);
    reflectionParallel = *(float*)(shaderTag->Data() + 0x90);
    
    printf("shader setup\n");
    sgla *shader = (sgla *)(manager->get_shader(shader_SGLA));
    mapsId  = shader->maps;
    scaleId = shader->scale;
    reflectionScaleId = shader->reflectionScale;
};

bool sgla_object::is(ShaderType type) {
    return (type == shader_SGLA);
}
void sgla_object::render() {
    glActiveTexture(GL_TEXTURE0);
    if (useMulti) {
        multipurposeMap->bind();
    }
    
    glActiveTexture(GL_TEXTURE1);
    if (useCube) {
        cubeMap->bind();
    }
    
    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Scales
    glUniform3f(scaleId, uscale, vscale, bumpScale);
    glUniform2f(mapsId , b2f(useMulti), useCube ? 0.5:0.0);
    glUniform2f(reflectionScaleId, reflectionPerpendicular, reflectionParallel);
}
