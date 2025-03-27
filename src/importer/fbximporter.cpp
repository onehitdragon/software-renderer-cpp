#include "fbximporter.h"

bool property_TypeCode_is_PrimitiveType(const char &typecode){
    return typecode == 'Y' || typecode == 'C' || typecode == 'I' || typecode == 'F' || typecode == 'D' || typecode == 'L';
}

std::variant<int16_t, bool, int32_t, float, double, int64_t> read_PrimitiveData(std::ifstream &file, const char &typecode){
    if(typecode == 'Y'){
        int16_t data;
        file.read(reinterpret_cast<char*>(&data), sizeof(data));
        return data;
    }
    if(typecode == 'C'){
        uint8_t data;
        file.read(reinterpret_cast<char*>(&data), sizeof(data));
        return data & 0x01;
    }
    if(typecode == 'I'){
        int32_t data;
        file.read(reinterpret_cast<char*>(&data), sizeof(data));
        return data;
    }
    if(typecode == 'F'){
        float data;
        file.read(reinterpret_cast<char*>(&data), sizeof(data));
        return data;
    }
    if(typecode == 'D'){
        double data;
        file.read(reinterpret_cast<char*>(&data), sizeof(data));
        return data;
    }
    if(typecode == 'L'){
        int64_t data;
        file.read(reinterpret_cast<char*>(&data), sizeof(data));
        return data;
    }

    throw std::runtime_error("Unsupported primitive typecode");
}

bool property_TypeCode_is_SpecialType(const char &typecode){
    return typecode == 'S' || typecode == 'R';
}

std::string read_SpecialData(std::ifstream &file){
    uint32_t length;
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    char data[length + 1];
    file.read(data, sizeof(uint8_t) * length);
    data[length] = '\0';
    return data;
}

bool property_TypeCode_is_ArrayType(const char &typecode){
    return typecode == 'f' || typecode == 'd' || typecode == 'i' || typecode == 'l' || typecode == 'b';
}

uint32_t read_ArrayData_get_uncompressedLength(
    const char &typecode,
    const uint32_t &arrayLength,
    const uint32_t &encoding,
    const uint32_t &compressedLength
){
    if(encoding == 0){
        return compressedLength;
    }
    if(encoding == 1){
        if(typecode == 'f'){
            return sizeof(float) * arrayLength;
        }
        if(typecode == 'd'){
            return sizeof(double) * arrayLength;
        }
        if(typecode == 'i'){
            return sizeof(int32_t) * arrayLength;
        }
        if(typecode == 'l'){
            return sizeof(int64_t) * arrayLength;
        }
        if(typecode == 'b'){
            return sizeof(uint8_t) * arrayLength;
        }
        throw std::runtime_error("Unsupported array typecode");
    }
    throw std::runtime_error("Unsupported array encoding");
}
void read_ArrayData_uncompress(
    const char &typecode,
    const uint32_t &arrayLength,
    const uint32_t &encoding,
    const uint32_t &compressedLength,
    std::ifstream &file, 
    uint8_t *uncompressed_data,
    uint32_t uncompressedLength
){
    if(encoding == 0){
        file.read(reinterpret_cast<char*>(uncompressed_data), sizeof(uint8_t) * uncompressedLength);
    }
    else if(encoding == 1){
        auto compressed_data = std::make_unique<uint8_t[]>(compressedLength);
        file.read(reinterpret_cast<char*>(compressed_data.get()), sizeof(uint8_t) * compressedLength);
        z_stream zs = {};
        zs.next_in = compressed_data.get();
        zs.avail_in = compressedLength;
        zs.next_out = uncompressed_data;
        zs.avail_out = uncompressedLength;
        if(inflateInit(&zs) != Z_OK) {
            throw std::runtime_error("fail inflateInit");
        }
        if(inflate(&zs, Z_NO_FLUSH) != Z_STREAM_END){
            throw std::runtime_error("fail inflate");
        }
    }
    else{
        throw std::runtime_error("Unsupported array encoding");
    }
}
std::vector<std::variant<float, double, int64_t, int32_t, bool>> read_ArrayData(
    std::ifstream &file, std::ofstream &outfile, const char &typecode, std::string tab_s
){
    uint32_t arrayLength;
    file.read(reinterpret_cast<char*>(&arrayLength), sizeof(arrayLength));
    uint32_t encoding;
    file.read(reinterpret_cast<char*>(&encoding), sizeof(encoding));
    uint32_t compressedLength;
    file.read(reinterpret_cast<char*>(&compressedLength), sizeof(compressedLength));
    outfile << "arrayLength: " << arrayLength << " " << "encoding: " << encoding << " " << "compressedLength: " << compressedLength << std::endl;

    uint32_t uncompressedLength = read_ArrayData_get_uncompressedLength(typecode, arrayLength, encoding, compressedLength);
    auto uncompressed_data = std::make_unique<uint8_t[]>(uncompressedLength);
    read_ArrayData_uncompress(typecode, arrayLength, encoding, compressedLength, file, uncompressed_data.get(), uncompressedLength);

    if(typecode == 'f'){
        std::vector<std::variant<float, double, int64_t, int32_t, bool>> data;
        data.reserve(arrayLength);
        outfile << tab_s << "  " << "Data: ";
        for(int i = 0; i < arrayLength; i++){
            float value = *(reinterpret_cast<float*>(uncompressed_data.get() + i * sizeof(float)));
            data.push_back(value);
            outfile << value << " ";
        }
        return data;
    }

    if(typecode == 'd'){
        std::vector<std::variant<float, double, int64_t, int32_t, bool>> data;
        data.reserve(arrayLength);
        outfile << tab_s << "  " << "Data: ";
        for(int i = 0; i < arrayLength; i++){
            double value = *(reinterpret_cast<double*>(uncompressed_data.get() + i * sizeof(double)));
            data.push_back(value);
            outfile << value << " ";
        }
        return data;
    }

    if(typecode == 'i'){
        std::vector<std::variant<float, double, int64_t, int32_t, bool>> data;
        data.reserve(arrayLength);
        outfile << tab_s << "  " << "Data: ";
        for(int i = 0; i < arrayLength; i++){
            int32_t value = *(reinterpret_cast<int32_t*>(uncompressed_data.get() + i * sizeof(int32_t)));
            data.push_back(value);
            outfile << value << " ";
        }
        return data;
    }

    if(typecode == 'l'){
        std::vector<std::variant<float, double, int64_t, int32_t, bool>> data;
        data.reserve(arrayLength);
        outfile << tab_s << "  " << "Data: ";
        for(int i = 0; i < arrayLength; i++){
            int64_t value = *(reinterpret_cast<int64_t*>(uncompressed_data.get() + i * sizeof(int64_t)));
            data.push_back(value);
            outfile << value << " ";
        }
        return data;
    }

    if(typecode == 'b'){
        std::vector<std::variant<float, double, int64_t, int32_t, bool>> data;
        data.reserve(arrayLength);
        outfile << tab_s << "  " << "Data: ";
        for(int i = 0; i < arrayLength; i++){
            bool value = *(reinterpret_cast<bool*>(uncompressed_data.get() + i * sizeof(bool)));
            data.push_back(value);
            outfile << value << " ";
        }
        return data;
    }

    throw std::runtime_error("Unsupported array typecode");
}

void copyVerticesToModel(
    const std::vector<std::variant<float, double, int64_t, int32_t, bool>> &data,
    Model *model
){
    model->vertices.reserve(data.size() / 3);
    Vec3 vertex;
    auto visitorX = [&vertex](const auto &val){
        vertex.x = val;
    };
    auto visitorY = [&vertex](const auto &val){
        vertex.y = val;
    };
    auto visitorZ = [&vertex](const auto &val){
        vertex.z = val;
    };
    for(int i = 0; i < data.size(); i += 3){
        std::visit(visitorX, data[i]);
        std::visit(visitorY, data[i + 1]);
        std::visit(visitorZ, data[i + 2]);
        Vec3 vertex_yUp_zForward;
        vertex_yUp_zForward.x = vertex.x;
        vertex_yUp_zForward.y = vertex.z;
        vertex_yUp_zForward.z = vertex.y;
        model->vertices.push_back(vertex_yUp_zForward);
    }
}
void copyTrianglesToModel(
    const std::vector<std::variant<float, double, int64_t, int32_t, bool>> &data,
    Model *model
){
    model->triangles.reserve(data.size() / 3);
    Triangle triangle;
    auto visitorX = [&triangle](const auto &val){
        triangle.x = val;
    };
    auto visitorY = [&triangle](const auto &val){
        triangle.y = val;
    };
    auto visitorZ = [&triangle](const auto &val){
        triangle.z = (val * -1) - 1;
    };
    for(int i = 0; i < data.size(); i += 3){
        std::visit(visitorX, data[i]);
        std::visit(visitorY, data[i + 1]);
        std::visit(visitorZ, data[i + 2]);
        model->triangles.push_back(triangle);
    }
}
void copyUVToModel(
    const std::vector<std::variant<float, double, int64_t, int32_t, bool>> &data,
    Model *model
){
    model->uv.reserve(data.size() / 2);
    Vec2 uv;
    auto visitorX = [&uv](const auto &val){
        uv.x = val;
    };
    auto visitorY = [&uv](const auto &val){
        uv.y = val;
    };
    for(int i = 0; i < data.size(); i += 2){
        std::visit(visitorX, data[i]);
        std::visit(visitorY, data[i + 1]);
        model->uv.push_back(uv);
    }
}
void copyUVIndexToModel(
    const std::vector<std::variant<float, double, int64_t, int32_t, bool>> &data,
    Model *model
){
    model->textureCoors.reserve(data.size() / 3);
    TextureCoor textureCoor;
    auto visitorX = [&textureCoor, &model](const auto &val){
        textureCoor.uv1 = model->uv[val];
    };
    auto visitorY = [&textureCoor, &model](const auto &val){
        textureCoor.uv2 = model->uv[val];
    };
    auto visitorZ = [&textureCoor, &model](const auto &val){
        textureCoor.uv3 = model->uv[val];
    };
    for(int i = 0; i < data.size(); i += 3){
        std::visit(visitorX, data[i]);
        std::visit(visitorY, data[i + 1]);
        std::visit(visitorZ, data[i + 2]);
        model->textureCoors.push_back(textureCoor);
    }
}

void readNode(std::ifstream &file, std::ofstream &outfile, Model *model, const int &tab = 0){
    uint32_t node_EndOffset;
    file.read(reinterpret_cast<char*>(&node_EndOffset), sizeof(node_EndOffset));
    uint32_t node_NumProperties;
    file.read(reinterpret_cast<char*>(&node_NumProperties), sizeof(node_NumProperties));
    uint32_t node_PropertyListLen;
    file.read(reinterpret_cast<char*>(&node_PropertyListLen), sizeof(node_PropertyListLen));
    uint8_t node_NameLen;
    file.read(reinterpret_cast<char*>(&node_NameLen), sizeof(node_NameLen));
    char node_Name[node_NameLen + 1];
    file.read(node_Name, sizeof(uint8_t) * node_NameLen);
    node_Name[node_NameLen] = '\0';

    std::string tab_s = "";
    std::string arrow_s = " -";
    for(int i = 0; i < tab; i++){
        tab_s.append("    ");
        arrow_s.append("-");
    }
    arrow_s.append(">");

    outfile << tab_s << "EndOffset: " << node_EndOffset << arrow_s << std::endl;
    outfile << tab_s << "NumProperties: " << node_NumProperties << std::endl;
    outfile << tab_s << "PropertyListLen: " << node_PropertyListLen << std::endl;
    outfile << tab_s << "NameLen: " << (int)node_NameLen << std::endl;
    outfile << tab_s << "Name: " << node_Name << std::endl;
    for(int i = 0; i < node_NumProperties; i++){
        char property_TypeCode;
        file.read(&property_TypeCode, sizeof(property_TypeCode));
        if(property_TypeCode_is_PrimitiveType(property_TypeCode)){
            outfile << tab_s << "  " << "TypeCode: " << property_TypeCode << "(PrimitiveType)" << std::endl;
            outfile << tab_s << "  " << "Data: ";
            auto visitor = [&outfile](const auto &val){
                outfile << val;
            };
            std::visit(visitor, read_PrimitiveData(file, property_TypeCode));
            outfile << std::endl;
        }
        else if(property_TypeCode_is_SpecialType(property_TypeCode)){
            outfile << tab_s << "  " << "TypeCode: " << property_TypeCode << "(SpecialType)" << std::endl;
            outfile << tab_s << "  " << "Data: " << read_SpecialData(file) << std::endl;
        }
        else if(property_TypeCode_is_ArrayType(property_TypeCode)){
            outfile << tab_s << "  " << "TypeCode: " << property_TypeCode << "(ArrayType) ";
            auto array = read_ArrayData(file, outfile, property_TypeCode, tab_s);
            outfile << std::endl;

            if(std::string(node_Name) == "Vertices"){
                std::cout << "Vertices: " << array.size() / 3 << std::endl;
                copyVerticesToModel(array, model);
            }
            if(std::string(node_Name) == "PolygonVertexIndex"){
                std::cout << "PolygonVertexIndex: " << array.size() / 3 << std::endl;
                copyTrianglesToModel(array, model);
            }
            if(std::string(node_Name) == "UV"){
                std::cout << "UV: " << array.size() / 2 << std::endl;
                copyUVToModel(array, model);
            }
            if(std::string(node_Name) == "UVIndex"){
                std::cout << "UVIndex: " << array.size() / 3 << std::endl;
                copyUVIndexToModel(array, model);
            }
        }
        else{
            throw std::runtime_error("Unsupported typecode");
        }
    }
    outfile << tab_s << "file.tellg: " << file.tellg() << std::endl;

    while(file.tellg() < node_EndOffset){
        readNode(file, outfile, model, tab + 1);
    }
}

void importFBX(std::string path, Model *model){
    std::ifstream file(path, std::ios::binary);
    if(!file.is_open()){
        std::cout << "cant open to read: " << path << std::endl;
        return;
    }
    std::ofstream outfile(path.append(".out"));
    if(!outfile.is_open()){
        std::cout << "cant open to write: " << path << std::endl;
        return;
    }

    outfile << "================FBX Start============="<< std::endl;

    char h_name[21];
    uint32_t h_version;
    file.read(h_name, sizeof(uint8_t) * 21);
    outfile << "FBX type: " << h_name << std::endl;
    file.seekg(2, std::ios::cur);
    file.read(reinterpret_cast<char*>(&h_version), sizeof(h_version));
    outfile << "FBX version: " << h_version << std::endl;

    //node record
    outfile << std::endl;
    readNode(file, outfile, model);
    outfile << std::endl;
    readNode(file, outfile, model);
    outfile << std::endl;
    readNode(file, outfile, model);
    outfile << std::endl;
    readNode(file, outfile, model);
    outfile << std::endl;
    readNode(file, outfile, model);
    outfile << std::endl;
    readNode(file, outfile, model);
    outfile << std::endl;
    readNode(file, outfile, model);
    outfile << std::endl;
    readNode(file, outfile, model);
    outfile << std::endl;
    readNode(file, outfile, model);

    outfile << "=================FBX End=============="<< std::endl;

    file.close();
    outfile.close();
}