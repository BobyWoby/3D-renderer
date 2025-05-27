#pragma once
#include <string>
#include <utility>
#include <array>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cassert>
#include "GraphicsMath/vec.h"



class Model {
private:
    std::string filepath;
    std::vector<std::string> split(std::string str, std::string delim);    
public:
    Model(){
       filepath = "/home/bobywoby/dev/c++/tinyrenderer/obj/diablo3_pose/diablo3_pose.obj"; 
    }    
    
    Model(std::string f){
       filepath = f; 
    }    

    std::vector<vec3d> getVertices();
    static void normalizeVertices(std::vector<vec3d> &in, int maxX, int maxY, int maxZ);
    std::vector<vec3i> getFaces();
    std::vector<vec<2, vec2d>> getLines();
    static vec2d projectPoint(vec3d pointPos, vec3d camPos, vec3d pointOrientation, vec3d screenSpacePos);
    static vec2d projectPoint(vec3d pointPos, vec3d camPos, vec3d screenSpacePos);
    static vec2d projectPoint(vec3d pointPos);
    static vec2d orthoProject(vec3d pointPos);
};

