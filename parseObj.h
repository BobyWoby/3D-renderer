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

    std::vector<gm::vec3d> getVertices();
    static void normalizeVertices(std::vector<gm::vec3d> &in, int maxX, int maxY, int maxZ);
    static void normalizeVertex(gm::vec3d &in, int maxX, int maxY, int maxZ);
    std::vector<gm::vec3i> getFaces();
    std::vector<gm::vec<2, gm::vec2d>> getLines();
    static gm::vec3d projectPoint(gm::vec3d pointPos, double focalDist);
    static gm::vec2d orthoProject(gm::vec3d pointPos);
};

