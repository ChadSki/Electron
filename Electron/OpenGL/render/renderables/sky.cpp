//
//  render.h
//  
//
//  Created by Samuco on 19/04/2015.
//
//

#include "sky.h"
typedef struct {
    HaloTagDependency model;
} SkyTag;

void SkyManager::read(ObjectManager *objects, ProtonMap *map, ProtonTag *scenario) {
    printf("starting sky manager\n");
    HaloTagReflexive sky = ((HaloScenarioTag*)scenario->Data())->skyBox;
    
    skies.resize(sky.count);
    for (int i=0; i < sky.count; i++) {
        HaloTagDependency skyTag = *(HaloTagDependency*)(scenario->Data() + scenario->PointerToOffset(sky.address) + i * 16);
        ProtonTag *objectTag = map->tags.at(skyTag.tag_id.tag_index).get();
        SkyTag *skyData = (SkyTag *)objectTag->Data();
        Model *skyModel = objects->modelManager->create_model(map, skyData->model);
        skies[i] = skyModel;
    }
}

void SkyManager::render(ShaderType pass) {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
    int i;
    for (i=0; i < skies.size(); i++) {
        skies[i]->render(pass);
        break;
    }
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
}

