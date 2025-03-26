#ifndef FBXIMPORTER_H
#define FBXIMPORTER_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <variant>
#include <vector>
#include <zlib.h>
#include <memory>
#include "../global.h"

void importFBX(std::string path, Model *model);

#endif