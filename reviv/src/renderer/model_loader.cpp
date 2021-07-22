#include"model_loader.h"

void ModelLoader::load(const std::string& filePath)
{
    m_FilePath = filePath;

    Assimp::Importer importer;
   // m_Scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_SplitLargeMeshes | aiProcess_OptimizeMeshes); TODO
    m_Scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    // additional flags:    aiProcess_GenNormals
    //                      aiProcess_SplitLargeMeshes
    //                      aiProcess_OptimizeMeshes

    RV_ASSERT(m_Scene && !(m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && m_Scene->mRootNode, "assimp error (wrong file name?)");

    std::string directory = filePath.substr(0, filePath.find_last_of('/'));

    cout << "Number of meshes: " << m_Scene->mNumMeshes << endl;
    meshes.reserve(m_Scene->mNumMeshes);

    processNode(m_Scene->mRootNode);

    isLoaded = true;
}

void ModelLoader::processNode(aiNode* node)
{
    for(unsigned int i=0; i < node->mNumMeshes; i++)
    {
        meshes.emplaceBack();
        //m_Scene->mNumMeshes
        addMesh(m_Scene->mMeshes[node->mMeshes[i]], &meshes[meshes.size() - 1]);
        //loadMaterial(m_Scene->mMeshes[node->mMeshes[i]]); TODO:
    }
    for(unsigned int i=0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i]);
    }
}

void ModelLoader::addMesh(aiMesh* loaderMesh, Mesh* pMesh)
{
    Vertex vertex;

    pMesh->m_Vertices.reserve(loaderMesh->mNumVertices);

    int nIndices = 0;
    for(unsigned int i=0; i < loaderMesh->mNumFaces; i++)
    {
        aiFace face = loaderMesh->mFaces[i];
        nIndices += face.mNumIndices;
    }

    pMesh->m_Indices.reserve(nIndices);

    for(unsigned int i=0; i < loaderMesh->mNumVertices; i++)
    {
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

        pMesh->m_Vertices.pushBack(vertex);
    }

    for(unsigned int i=0; i < loaderMesh->mNumFaces; i++)
    {
        aiFace face = loaderMesh->mFaces[i];
        for(unsigned int j=0; j<face.mNumIndices; j++)
        {
            pMesh->m_Indices.pushBack(face.mIndices[j]);
        }
    }

    pMesh->vao.init();
    pMesh->vao.bind();

    std::vector<BufferElement> tempVboLayout = {
        {ShaderDataType::SdtFloat3, "a_Position", false},
        {ShaderDataType::SdtFloat3, "a_Normal",   false},
        {ShaderDataType::SdtFloat2, "a_TexCoord", false},
    };

    pMesh->vao.addVbo(tempVboLayout);

    pMesh->vao.vertexBuffers.back().bind();
    pMesh->vao.vertexBuffers.back().load(&(pMesh->m_Vertices[0]), sizeof(Vertex) * pMesh->m_Vertices.size());

    pMesh->vao.addEbo();
    pMesh->vao.elementBuffers[0].bind(); //TODO: useless?
    pMesh->vao.elementBuffers[0].load(&(pMesh->m_Indices[0]), pMesh->m_Indices.size() * sizeof(unsigned int));
}


/*
void Mesh::init()
{
    vao.init();
    vao.bind();
    vao.load(m_Vertices.size() * sizeof(Vertex), &m_Vertices[0]);

    vao.unbind(); //TODO: da li ovo treba?

    RV_ASSERT(sizeof(Vertex) == 32, "");
    RV_ASSERT(sizeof(char) == 1 && CHAR_BIT == 8, "something is very wrong");
    RV_ASSERT(GL_STATIC_DRAW < INT_MAX && GL_STATIC_DRAW > INT_MIN, "");
    RV_ASSERT(GL_DYNAMIC_DRAW < INT_MAX && GL_DYNAMIC_DRAW > INT_MIN, "");
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