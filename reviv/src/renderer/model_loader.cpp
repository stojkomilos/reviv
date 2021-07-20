#include"model_loader.h"

void* gData1;
void* gData2;

void ModelLoader::load(const std::string& filePath)
{
    m_FilePath = filePath;

    Assimp::Importer importer;
   // m_Scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_SplitLargeMeshes | aiProcess_OptimizeMeshes); TODO
    m_Scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    // additional flags:    aiProcess_GenNormals
    //                      aiProcess_SplitLargeMeshes
    //                      aiProcess_OptimizeMeshes

    RV_ASSERT(m_Scene and !(m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) and m_Scene->mRootNode, "assimp error");

    std::string directory = filePath.substr(0, filePath.find_last_of('/'));
    processNode(m_Scene->mRootNode);

    cout << "Number of meshes: " << m_Scene->mNumMeshes << endl;
    meshes.reserve(m_Scene->mNumMeshes);
    isLoaded = true;
}

void ModelLoader::processNode(aiNode* node)
{
    for(int i=0; i < node->mNumMeshes; i++)
    {
        meshes.emplace_back();
        //m_Scene->mNumMeshes
        addMesh(m_Scene->mMeshes[node->mMeshes[i]], &meshes[meshes.size() - 1]);
        //loadMaterial(m_Scene->mMeshes[node->mMeshes[i]]); TODO:
    }
    for(int i=0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i]);
    }
}

void ModelLoader::addMesh(aiMesh* loaderMesh, Mesh* pMesh)
{
    Vertex vertex;

    int as=0;
    pMesh->m_Vertices.reserve(loaderMesh->mNumVertices);


    int nIndices = 0;
    for(int i=0; i < loaderMesh->mNumFaces; i++)
    {
        aiFace face = loaderMesh->mFaces[i];
        nIndices += face.mNumIndices;
    }

    pMesh->m_Indices.reserve(nIndices);

    for(int i=0; i < loaderMesh->mNumVertices; i++)
    {
        as++;
        vertex.position.x = loaderMesh->mVertices[i].x;
        vertex.position.y = loaderMesh->mVertices[i].y;
        vertex.position.z = loaderMesh->mVertices[i].z;

        vertex.normal.x = loaderMesh->mNormals[i].x;
        vertex.normal.y = loaderMesh->mNormals[i].y;
        vertex.normal.z = loaderMesh->mNormals[i].z;

        if(loaderMesh->mTextureCoords[0])
        {
            vertex.texCoords.x = loaderMesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = loaderMesh->mTextureCoords[0][i].y;
        }
        else 
        {
            RV_ASSERT(false, "");
        }

        pMesh->m_Vertices.push_back(vertex);
    }
    cout << "AS: " << as << " m_Vertices.size(): " << pMesh->m_Vertices.size() << endl;

    as = 0;
    for(int i=0; i < loaderMesh->mNumFaces; i++)
    {
        aiFace face = loaderMesh->mFaces[i];
        for(int j=0; j<face.mNumIndices; j++)
        {
            pMesh->m_Indices.push_back(face.mIndices[j]);
            as++;
        }
    }
    cout << "AS: " << as << " m_Indices.size(): " << pMesh->m_Indices.size() << endl;

    pMesh->vao.init();
    pMesh->vao.bind();

    Vbo vbo;
    vbo.init();
    std::vector<BufferElement> tempVboLayout = {
        {ShaderDataType::SdtFloat3, "a_Position", false},
        {ShaderDataType::SdtFloat3, "a_Normal",   false},
        {ShaderDataType::SdtFloat2, "a_TexCoord", false},
    };
    vbo.layout.elements = tempVboLayout;
    vbo.layout.init();
    pMesh->vao.addVbo(vbo);

    //vbo.bind(); //TODO: useless

    gData1 = &pMesh->m_Vertices[0];

    pMesh->vao.vertexBuffers[0].load(&(pMesh->m_Vertices[0]), sizeof(Vertex) * pMesh->m_Vertices.size());

    //cout << "stride: " << vbo.layout.stride << endl;

    Ebo ebo;
    ebo.init();
    pMesh->vao.addEbo(ebo);
    pMesh->vao.elementBuffers[0].bind(); //TODO: useless?
    pMesh->vao.elementBuffers[0].load(&(pMesh->m_Indices[0]), pMesh->m_Indices.size() * sizeof(unsigned int));

    gData2 = &pMesh->m_Indices[0];

}

    //vboLayout1.elements.push_back({ShaderDataType::SdtFloat3, "a_Position", false});
    //vboLayout1.elements.push_back({ShaderDataType::SdtFloat3, "a_Normal", false});
    //vboLayout1.elements.push_back({ShaderDataType::SdtFloat3, "a_TexCoord", false});
    //vboLayout1.init();

/*
void Mesh::init()
{
    vao.init();
    vao.bind();
    vao.load(m_Vertices.size() * sizeof(Vertex), &m_Vertices[0]);

    vao.unbind(); //TODO: da li ovo treba?

    RV_ASSERT(sizeof(Vertex) == 32, "");
    RV_ASSERT(sizeof(char) == 1 and CHAR_BIT == 8, "something is very wrong");
    RV_ASSERT(GL_STATIC_DRAW < INT_MAX and GL_STATIC_DRAW > INT_MIN, "");
    RV_ASSERT(GL_DYNAMIC_DRAW < INT_MAX and GL_DYNAMIC_DRAW > INT_MIN, "");
    RV_ASSERT(sizeof(float) * 3 == sizeof(Vec3f), "");

}



void Mesh::load(aiMesh* mesh, const aiScene* scene)
{
   

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuesMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        texutres.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        /////
    }


}

*/