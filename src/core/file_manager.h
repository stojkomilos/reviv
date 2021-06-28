#pragma once

#include<cstdio>
#include<stdio.h>
#include<iostream>
#include<string>
#include<cassert>
#include<fstream>
#include<sstream>

#include"physics/physics_manager.h"

class FileManager {
public:
	int setUpWorld();
	int saveCurrentWorld();
};
