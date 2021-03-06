//
//  shader.h
//  
//
//  Created by Samuco on 19/04/2015.
//
//

#include "defines.h"
#include <map>
#include "bitmaps.h"

#ifndef __texture__
#define __texture__


class texture {
public:
    std::vector<GLuint>textures;
    GLuint tex = 0;
    texture(ProtonMap *map, HaloTagDependency bitm);
    void bind();
    void bind(int index);
};

class texture_cubemap {
public:
    GLuint tex;
    void load_side(GLenum side_target, GLenum i, ProtonTag *bitmapTag);
    void bind();
    texture_cubemap(ProtonMap *map, HaloTagDependency bitm);
};

class TextureManager {
private:
    std::map<uint16_t, texture*> textures;
    std::map<uint16_t, texture_cubemap*> textures_cube;
public:
    texture *create_texture(ProtonMap *map, HaloTagDependency bitm);
    texture_cubemap *create_cubemap(ProtonMap *map, HaloTagDependency bitm);
};


#endif
