/***********************************************************************
 *
 * Filename: viewport.cpp
 *
 * Description: Qt3D version of the viewport that displays the scenegraph.
 *
 * Copyright (C) 2015 Richard Layman, rlayman2000@yahoo.com 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "viewport.hpp"
#include "commands.hpp"
#include "state.hpp"
#include "draw.hpp"
#include "plugin.hpp"

// MAIN VIEWPORT

DrawItem::DrawItem(feather::draw::Item* _item, Type _type, Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent),
    m_item(_item),
    m_type(_type)
{
}

DrawItem::~DrawItem()
{
    //Qt3DCore::QNode::cleanup();
}


// PERSPECTIVE CAMERA 
PerspCamera::PerspCamera(Qt3DRender::QCamera* camera, Qt3DRender::QLayer* layer, feather::draw::Item* _item, Qt3DCore::QNode *parent)
    : DrawItem(_item,DrawItem::PerspCamera,parent)
{
    m_pCamera = camera;
}

PerspCamera::~PerspCamera()
{
    //Qt3DCore::QNode::cleanup();
}

void PerspCamera::updateItem()
{
    unsigned int typefid = static_cast<feather::draw::PerspCamera*>(m_item)->typefid;
    unsigned int fovfid = static_cast<feather::draw::PerspCamera*>(m_item)->fovfid;
    unsigned int nearfid = static_cast<feather::draw::PerspCamera*>(m_item)->nearfid;
    unsigned int farfid = static_cast<feather::draw::PerspCamera*>(m_item)->farfid;

    feather::FReal type = static_cast<feather::field::Field<feather::FReal>*>(feather::plugin::get_field_base(m_item->uid,m_item->nid,typefid))->value;
    feather::FReal fov = static_cast<feather::field::Field<feather::FReal>*>(feather::plugin::get_field_base(m_item->uid,m_item->nid,fovfid))->value;
    feather::FReal near = static_cast<feather::field::Field<feather::FReal>*>(feather::plugin::get_field_base(m_item->uid,m_item->nid,nearfid))->value;
    feather::FReal far = static_cast<feather::field::Field<feather::FReal>*>(feather::plugin::get_field_base(m_item->uid,m_item->nid,farfid))->value;

    std::cout << "perspective camera update - " << type << "," << fov << "," << near << "," << far << "\n";
    if(type==0)
        m_pCamera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    m_pCamera->setFieldOfView(fov);
    m_pCamera->setNearPlane(near);
    m_pCamera->setFarPlane(far);
 
    /*
    m_pCamera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    m_pCamera->setFieldOfView(45);
    m_pCamera->setNearPlane(0.1);
    m_pCamera->setFarPlane(100.0);
    m_pCamera->setAspectRatio(1.3);
    m_pCamera->setPosition(QVector3D(0,3,20));
    m_pCamera->setUpVector(QVector3D(0,1,0));
    m_pCamera->setViewCenter(QVector3D(0,0,0));
    */
}

// CAN NOT GET SHADERS TO WORK IN QT3D
// YET BUT I'M LEAVING THIS HERE FOR
// WHEN I CAN
/*
WireEffect::WireEffect(QNode* parent)
    : QEffect(parent),
    m_pPassCriterion(new Qt3D::QAnnotation()),
    m_pRenderPass(new Qt3D::QRenderPass()),
    m_pTechnique(new Qt3D::QTechnique())
{
    addParameter(new Qt3D::QParameter("ambient",QVector3D(0.1,0.1,0.1)));
    addParameter(new Qt3D::QParameter("diffuse",QVector3D(0.7,0.7,0.7)));
    addParameter(new Qt3D::QParameter("specular",QVector3D(0.95,0.95,0.95)));
    addParameter(new Qt3D::QParameter("shininess",150.0));
 

    m_pTechnique->openGLFilter()->setProfile(Qt3D::QOpenGLFilter::Core);
    m_pTechnique->openGLFilter()->setApi(Qt3D::QOpenGLFilter::Desktop);
    m_pTechnique->openGLFilter()->setMajorVersion(3);               
    m_pTechnique->openGLFilter()->setMinorVersion(1);

    m_pTechnique->addParameter(new Qt3D::QParameter("light.position",QVector4D(0,0,0,1)));
    m_pTechnique->addParameter(new Qt3D::QParameter("light.intensity",QVector3D(1,1,1)));
    m_pTechnique->addParameter(new Qt3D::QParameter("line.width",1.0));
    m_pTechnique->addParameter(new Qt3D::QParameter("line.color",QVector4D(1,1,1,1)));

    m_pRenderPass->addBinding(new Qt3D::QParameterMapping("ambient","ka",Qt3D::QParameterMapping::Uniform)); 
    m_pRenderPass->addBinding(new Qt3D::QParameterMapping("diffuse","kd",Qt3D::QParameterMapping::Uniform)); 
    m_pRenderPass->addBinding(new Qt3D::QParameterMapping("specular","ks",Qt3D::QParameterMapping::Uniform)); 
 
    m_pPassCriterion->setName(QStringLiteral("renderingStyle"));
    m_pPassCriterion->setValue(QStringLiteral("forward"));
     
    Qt3D::QShaderProgram *gl3Shader = new Qt3D::QShaderProgram();
    // Vert Shader
    QFile* gl3VertFile = new QFile("/usr/local/feather/shaders/vert/robustwireframe.vert");
    gl3VertFile->open(QIODevice::ReadOnly);
    gl3Shader->setVertexShaderCode(gl3VertFile->readAll());
    gl3VertFile->flush();
    gl3VertFile->close();
    // Frag Shader
    QFile* gl3FragFile = new QFile("/usr/local/feather/shaders/frag/robustwireframe.frag");
    gl3FragFile->open(QIODevice::ReadOnly);
    gl3Shader->setFragmentShaderCode(gl3FragFile->readAll());
    gl3FragFile->flush();
    gl3FragFile->close();
    // Geom Shader
    QFile* gl3GeomFile = new QFile("/usr/local/feather/shaders/geom/robustwireframe.geom");
    gl3GeomFile->open(QIODevice::ReadOnly);
    gl3Shader->setGeometryShaderCode(gl3GeomFile->readAll());
    gl3GeomFile->flush();
    gl3GeomFile->close();
    
    m_pRenderPass->addAnnotation(m_pPassCriterion);
    m_pRenderPass->setShaderProgram(gl3Shader);
    m_pTechnique->addPass(m_pRenderPass);

    addTechnique(m_pTechnique);

}

WireEffect::~WireEffect()
{

}
*/

 
MeshGeometry::MeshGeometry(int _uid, int _nid, int _fid, Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometry(parent),
    m_pVAttribute(new Qt3DRender::QAttribute(this)),
    m_pVertexBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this)),
    m_pVnAttribute(new Qt3DRender::QAttribute(this)),
    m_pNormalBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this))
{
    uid=_uid;
    nid=_nid;
    fid=_fid;

    build();

    // V
    const int vsize = m_aMeshVData.size() * sizeof(feather::FVertex3D);
    QByteArray meshVBytes;
    meshVBytes.resize(vsize);
    memcpy(meshVBytes.data(), m_aMeshVData.data(), vsize);

    //m_pVertexBuffer->setUsage(Qt3DRender::QBuffer::DynamicDraw);
    m_pVertexBuffer->setData(meshVBytes);
    
    m_pVAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    m_pVAttribute->setDataType(Qt3DRender::QAttribute::Float);
    m_pVAttribute->setDataSize(3);
    m_pVAttribute->setCount(m_aMeshVData.size());
    m_pVAttribute->setByteStride(sizeof(feather::FVertex3D));
    m_pVAttribute->setBuffer(m_pVertexBuffer);

    // VN
    const int vnsize = m_aMeshVnData.size() * sizeof(feather::FVertex3D);
    QByteArray meshVnBytes;
    meshVnBytes.resize(vnsize);
    memcpy(meshVnBytes.data(), m_aMeshVnData.data(), vsize);

    //m_pNormalBuffer->setUsage(Qt3DRender::QBuffer::DynamicDraw);
    m_pNormalBuffer->setData(meshVnBytes);

    m_pVnAttribute->setName(Qt3DRender::QAttribute::defaultNormalAttributeName());
    m_pVnAttribute->setDataType(Qt3DRender::QAttribute::Float);
    m_pVnAttribute->setDataSize(3);
    m_pVnAttribute->setCount(m_aMeshVnData.size());
    m_pVnAttribute->setByteStride(sizeof(feather::FVertex3D));
    m_pVnAttribute->setBuffer(m_pNormalBuffer);

    addAttribute(m_pVAttribute);
    addAttribute(m_pVnAttribute);
}

MeshGeometry::~MeshGeometry()
{
    //Qt3DCore::QNode::cleanup();
    delete m_pVAttribute;
    m_pVAttribute=0;
    delete m_pVertexBuffer;
    m_pVertexBuffer=0;
    delete m_pVnAttribute;
    m_pVnAttribute=0;
    delete m_pNormalBuffer;
    m_pNormalBuffer=0;
}

void MeshGeometry::build()
{
    m_aMeshVData.clear();
    m_aMeshVnData.clear();

    //feather::FMesh mesh;
    //feather::qml::command::get_field_val(uid,nid,fid,mesh);
    feather::FMesh mesh = static_cast<feather::field::Field<feather::FMesh>*>(feather::plugin::get_field_base(uid,nid,fid))->value;
 
    // build gl mesh from mesh
    feather::FIntArray glei;
    feather::FIntArray gli;
    feather::FColorRGBAArray glc;
    //feather::FVertex3DArray glv;
    uint id=0;
    int fcount=0; // this is a temp value to test selection
    //std::cout << "face count:" << mesh.f.size() << std::endl;
    std::for_each(mesh.f.begin(), mesh.f.end(), [this,&mesh,&id,&fcount,&glei,&gli,&glc](feather::FFace _face){
             
            for_each(_face.begin(),_face.end(),[&mesh,&glei](feather::FFacePoint _fp){ glei.push_back(_fp.v); });
            /*
               std::cout << "build glei\n";
               for_each(glei.begin(),glei.end(),[this](int _v){ std::cout << _v << " "; });
               std::cout << std::endl;
               */

            while(id+2 <= _face.size()) {
            if(fcount==3) {
            glc.push_back(feather::FColorRGBA(0.0,1.0,0.0,1.0));
            glc.push_back(feather::FColorRGBA(0.0,1.0,0.0,1.0));
            glc.push_back(feather::FColorRGBA(0.0,1.0,0.0,1.0));
            } else {
            glc.push_back(feather::FColorRGBA());
            glc.push_back(feather::FColorRGBA());
            glc.push_back(feather::FColorRGBA());
            }

            //std::cout << "v.y id:" << id << "=" << mesh.v.at(_face.at(id).v).y << ",";
            //std::cout << "v.y id:" << id+1 << "=" << mesh.v.at(_face.at(id+1).v).y << ",";
 
            //std::cout << "v" << id << ":" << _face.at(id).v << ",";
            //std::cout << "v.y" << id << ":" << mesh.v.at(_face.at(id).v).y << "," << std::endl;
            m_aMeshVData.push_back(mesh.v.at(_face.at(id).v));
            m_aMeshVnData.push_back(mesh.vn.at(_face.at(id).vn));
            //glvn.push_back(vn.at(_face.at(id).vn));
            gli.push_back(_face.at(id).v);

            //std::cout << "v" << id+1 << ":" << _face.at(id+1).v << "," << std::endl;
            m_aMeshVData.push_back(mesh.v.at(_face.at(id+1).v));
            m_aMeshVnData.push_back(mesh.vn.at(_face.at(id+1).vn));
            //glvn.push_back(vn.at(_face.at(id+1).vn));
            gli.push_back(_face.at(id+1).v);

            if(id+2 < _face.size()) {
                std::cout << "v.y id:" << id+2 << "=" << mesh.v.at(_face.at(id+2).v).y << ",";
                //std::cout << "v" << id+2 << ":" << _face.at(id+2).v << "," << std::endl;
                m_aMeshVData.push_back(mesh.v.at(_face.at(id+2).v));
                m_aMeshVnData.push_back(mesh.vn.at(_face.at(id+2).vn));
                //glvn.push_back(vn.at(_face.at(id+2).vn));
                gli.push_back(_face.at(id+2).v);
            } else {
                std::cout << "v.y id:" << 0 << "=" << mesh.v.at(_face.at(0).v).y << ",";
                //std::cout << "v" << 0 << ":" << _face.at(0).v << "," << std::endl;
                m_aMeshVData.push_back(mesh.v.at(_face.at(0).v));
                m_aMeshVnData.push_back(mesh.vn.at(_face.at(0).vn));
                //glvn.push_back(vn.at(_face.at(0).vn));
                gli.push_back(_face.at(0).v);
            }

            id=id+2;
            }
            std::cout << "\n";
            fcount++;
            id=0;
    });

}

void MeshGeometry::updateBuffers()
{
    build();

    // Position Buffer
    const int vsize = m_aMeshVData.size() * sizeof(feather::FVertex3D);
    QByteArray meshVBytes;
    meshVBytes.resize(vsize);
    memcpy(meshVBytes.data(), m_aMeshVData.data(), vsize);
    m_pVertexBuffer->setData(meshVBytes);
    //emit(m_pVAttribute->buffer()->dataChanged(meshVBytes));
    //m_pVAttribute->setBuffer(m_pVertexBuffer);
 
    std::cout << "V Buffer size=" << m_pVertexBuffer->data().size() << std::endl;
    std::cout << "V Attribute Buffer size=" << m_pVAttribute->buffer()->data().size()
        << ", count size=" << m_pVAttribute->count()
        << ", vertex size=" << m_pVAttribute->vertexSize()
        << std::endl;

    // Normal Buffer
    const int vnsize = m_aMeshVnData.size() * sizeof(feather::FVertex3D);
    QByteArray meshVnBytes;
    meshVnBytes.resize(vnsize);
    memcpy(meshVnBytes.data(), m_aMeshVnData.data(), vsize);
    m_pNormalBuffer->setData(meshVnBytes);
    //m_pVnAttribute->setBuffer(m_pNormalBuffer);
}

// MESHES

Mesh::Mesh(Qt3DRender::QLayer* layer, feather::draw::Item* _item, QNode *parent)
    : DrawItem(_item,DrawItem::Mesh,parent),
    m_pTransform(new Qt3DCore::QTransform()),
    m_pMaterial(new Qt3DExtras::QPhongMaterial()),
    m_pMesh(new Qt3DRender::QGeometryRenderer()),
    m_pLight(new Qt3DRender::QPointLight())
{
    m_pMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
    m_pMesh->setGeometry(new MeshGeometry(_item->uid,_item->nid,static_cast<feather::draw::Mesh*>(item())->fid,this));

    // Shaded Material 
    //m_pMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("meshColor"),QColor(Qt::blue)));  
    m_pMaterial->setDiffuse(QColor(Qt::green));
    m_pMaterial->setAmbient(Qt::black);
    m_pMaterial->setSpecular(Qt::white);
    m_pMaterial->setShininess(100.0f);


    // THIS WAS FROM SHADER TESTING
    /*
    m_pMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("ambient"),QColor(Qt::blue)));
    m_pMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("diffuse"),QColor(Qt::red)));
    m_pMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("specular"),QColor(Qt::white)));
    m_pMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("shininess"),150.0));
    m_pMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("line.width"),0.8));
    m_pMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("line.color"),QColor(Qt::black)));
    */

    // Light testing
    //m_pLight->setColor(Qt::blue);
    //m_pLight->setIntensity(10.5f);
    //m_pLight->setTranslation(QVector3D(4,4,4)); 

    // GONE
    //addComponent(static_cast<Viewport2*>(parent)->frameGraph());

    addComponent(layer);
    addComponent(m_pTransform);
    addComponent(m_pMaterial);
    addComponent(m_pMesh);
    //addComponent(m_pLight);

    //m_pMaterial->setEffect(m_pMaterialEffect);
}

Mesh::~Mesh()
{
    //Qt3DCore::QNode::cleanup();
}


void Mesh::updateItem()
{
    static_cast<MeshGeometry*>(m_pMesh->geometry())->updateBuffers();
    //emit(m_pMesh->geometryChanged(m_pMesh->geometry()));
    /*
    removeComponent(m_pMesh);
    delete m_pMesh;
    m_pMesh=0;
    m_pMesh = new Qt3DRender::QGeometryRenderer(); 
    m_pMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
    m_pMesh->setGeometry(new MeshGeometry(item()->uid,item()->nid,static_cast<feather::draw::Mesh*>(item())->fid,this));
    addComponent(m_pMesh);
    */
    //setParent(Q_NULLPTR);
    //removeAllComponents();
}

void Mesh::test()
{
    //m_pTransform->matrix().translate(1,0,0); 
    //removeComponent(m_pMesh);
    //setParent(Q_NULLPTR);
    //removeAllComponents();
}



// LINE
Line::Line(Qt3DRender::QLayer* layer, feather::draw::Item* _item, Qt3DCore::QNode *parent)
    : DrawItem(_item,DrawItem::Line,parent),
    m_pTransform(new Qt3DCore::QTransform()),
    m_pMaterial(new Qt3DExtras::QPhongMaterial()),
    m_pMesh(new Qt3DRender::QGeometryRenderer()),
    //m_pMouseInput(new Qt3D::QMouseInput(this)),
    m_meshAttribute(new Qt3DRender::QAttribute(this)),
    m_vertexBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this))
{
    m_aVertex.push_back(feather::FVertex3D(0,0,0));
    m_aVertex.push_back(feather::FVertex3D(2,2,2));
    //const int nVerts = 2;
    const int size = m_aVertex.size() * sizeof(feather::FVertex3D);
    QByteArray meshBytes;
    meshBytes.resize(size);
    memcpy(meshBytes.data(), m_aVertex.data(), size);

    m_vertexBuffer->setData(meshBytes);

    m_meshAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    m_meshAttribute->setDataType(Qt3DRender::QAttribute::Float);
    m_meshAttribute->setDataSize(3);
    m_meshAttribute->setCount(m_aVertex.size());
    m_meshAttribute->setByteStride(sizeof(feather::FVertex3D));
    m_meshAttribute->setBuffer(m_vertexBuffer);

    //setVerticesPerPatch(4);
    m_pMesh->setGeometry(new Qt3DRender::QGeometry(this));
    m_pMesh->geometry()->addAttribute(m_meshAttribute);

    m_pMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    //m_pMesh->setPrimitiveType(Qt3D::QGeometryRenderer::Triangles);
    //m_pMesh->setGeometry(new QGeometry(this));
    
    m_pMaterial->setDiffuse(QColor(Qt::red));
    m_pMaterial->setAmbient(Qt::red);
    //m_pMaterial->setSpecular(Qt::black);
    //m_pMaterial->setShininess(0.0f);

    addComponent(layer); 
    addComponent(m_pTransform);
    addComponent(m_pMesh);
    addComponent(m_pMaterial);

    //connect(m_pMouseInput,SIGNAL(entered()),this,SLOT(mouseClicked()));
}

Line::~Line()
{
    //Qt3DCore::QNode::cleanup();
    delete m_meshAttribute;
    m_meshAttribute=0;
    delete m_vertexBuffer;
    m_vertexBuffer=0;
}

void Line::updateItem()
{

}

/*
void Line::mouseClicked()
{
    std::cout << "Line Pressed\n";
}
*/


// AXIS GEOMETRY

AxisGeometry::AxisGeometry(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometry(parent),
    m_meshAttribute(new Qt3DRender::QAttribute(this)),
    m_vertexBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this))
{
    build();

    //const int nVerts = 6;
    const int size = m_axis.size() * sizeof(feather::FVertex3D);
    QByteArray meshBytes;
    meshBytes.resize(size);
    memcpy(meshBytes.data(), m_axis.data(), size);

    m_vertexBuffer->setData(meshBytes);

    m_meshAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    m_meshAttribute->setDataType(Qt3DRender::QAttribute::Float);
    m_meshAttribute->setDataSize(3);
    m_meshAttribute->setCount(m_axis.size());
    m_meshAttribute->setByteStride(sizeof(feather::FVertex3D));
    m_meshAttribute->setBuffer(m_vertexBuffer);

    //setVerticesPerPatch(4);
    addAttribute(m_meshAttribute);
}

void AxisGeometry::build()
{
    // X axis 
    m_axis.push_back(feather::FVertex3D(0,0,0));
    m_axis.push_back(feather::FVertex3D(10,0,0));
    m_axis.push_back(feather::FVertex3D(0,10,0));

    /*
    // Y axis 
    m_axis.push_back(feather::FVertex3D(0,0,0));
    m_axis.push_back(feather::FVertex3D(0,10,0));
 
    // Z axis 
    m_axis.push_back(feather::FVertex3D(0,0,0));
    m_axis.push_back(feather::FVertex3D(0,0,10));
    */
}

// AXIS 

Axis::Axis(Qt3DRender::QLayer* layer, Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent),
    m_pTransform(new Qt3DCore::QTransform()),
    m_pMaterial(new Qt3DRender::QMaterial()),
    m_pMesh(new Qt3DRender::QGeometryRenderer())
    //m_pMouseInput(new Qt3D::QMouseInput(this))
{
    //m_pMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    m_pMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
    m_pMesh->setGeometry(new AxisGeometry(this));
    
    //m_pMaterial->setDiffuse(QColor(Qt::red));
    //m_pMaterial->setAmbient(Qt::green);
    //m_pMaterial->setSpecular(Qt::black);
    //m_pMaterial->setShininess(0.0f);
  
    Qt3DRender::QEffect* effect = new Qt3DRender::QEffect();
    Qt3DRender::QShaderProgram* shader = new Qt3DRender::QShaderProgram();
    Qt3DRender::QTechnique* technique = new Qt3DRender::QTechnique(effect); 
    Qt3DRender::QRenderPass* pass = new Qt3DRender::QRenderPass();

    technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    //technique->openGLFilter()->setApi(Qt3D::QOpenGLFilter::ES);
    technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);
    technique->graphicsApiFilter()->setMajorVersion(3);
    technique->graphicsApiFilter()->setMinorVersion(3);

    effect->addParameter(new Qt3DRender::QParameter("ka",QVariant(QVector3D(0.5f,0.5f,0.5f))));
    effect->addParameter(new Qt3DRender::QParameter("kd",QVariant(QVector3D(1.0f,0.0f,0.0f))));
    effect->addParameter(new Qt3DRender::QParameter("ks",QVariant(QVector3D(0.0f,0.0f,0.0f))));
    effect->addParameter(new Qt3DRender::QParameter("shininess",QVariant(150)));

    technique->addParameter(new Qt3DRender::QParameter("light.position",QVariant(QVector4D(0.0f,0.0f,0.0f,1.0f))));
    technique->addParameter(new Qt3DRender::QParameter("light.intensity",QVariant(QVector3D(1.0f,1.0f,1.0f))));
    technique->addParameter(new Qt3DRender::QParameter("line.width",QVariant(2.0)));
    technique->addParameter(new Qt3DRender::QParameter("line.color",QVariant(QVector4D(1.0f,0.0f,0.0f,1.0f))));

    m_pMaterial->addParameter(new Qt3DRender::QParameter("ka",QVariant(QVector3D(0.3f,0.3f,0.3f))));
    m_pMaterial->addParameter(new Qt3DRender::QParameter("kd",QVariant(QVector3D(1.0f,0.0f,0.0f))));
    m_pMaterial->addParameter(new Qt3DRender::QParameter("ks",QVariant(QVector3D(0.0f,0.0f,0.0f))));
    m_pMaterial->addParameter(new Qt3DRender::QParameter("shininess",QVariant(150)));
    m_pMaterial->addParameter(new Qt3DRender::QParameter("light.position",QVariant(QVector4D(0.0f,0.0f,0.0f,1.0f))));
    m_pMaterial->addParameter(new Qt3DRender::QParameter("light.intensity",QVariant(QVector3D(1.0f,1.0f,1.0f))));
    m_pMaterial->addParameter(new Qt3DRender::QParameter("line.width",QVariant(2.0)));
    m_pMaterial->addParameter(new Qt3DRender::QParameter("line.color",QVariant(QVector4D(1.0f,0.0f,0.0f,1.0f))));

    Qt3DRender::QFilterKey* filter = new Qt3DRender::QFilterKey();
    filter->setName("renderingStyle");
    filter->setValue(QVariant("forward"));
    technique->addFilterKey(filter);
 
    // GONE
    /*
    pass->addBinding(new Qt3D::QParameterMapping("ambient","ka",Qt3D::QParameterMapping::Uniform));
    pass->addBinding(new Qt3D::QParameterMapping("diffuse","kd",Qt3D::QParameterMapping::Uniform));
    pass->addBinding(new Qt3D::QParameterMapping("specular","ks",Qt3D::QParameterMapping::Uniform));
    */

    /*
    QVariant ka = QVariant(QColor(Qt::red));
    QVariant kd = QVariant(QColor(Qt::red));
    QVariant ks = QVariant(QColor(Qt::red));

    Qt3DRender::QParameter ka_param("ka",ka,m_pMaterial);
    Qt3DRender::QParameter kd_param("kd",kd,m_pMaterial);
    Qt3DRender::QParameter ks_param("ks",ks,m_pMaterial);
    */

    /*
    shader->setVertexShaderCode("#version 130\\
            in vec3 vertexPosition;\\
            in vec3 vertexNormal;\\
            out vec3 position;\\
            out vec3 normal;\\
            uniform mat4 modelView;\\
            uniform mat3 modelViewNormal;\\
            uniform mat4 mvp;\\
            void main()\\
            {\\
            normal = normalize(modelViewNormal * vertexNormal);\\
            position = vec3(modelView * vec4(vertexPosition, 1.0));\\
            gl_Position = mvp * vec4(vertexPosition, 1.0);\\
            }");

            shader->setFragmentShaderCode("#version 130\\
                    in vec3 normal;\\
                    in vec3 position;\\
                    uniform vec3 finalColor;\\
                    out vec4 fragColor;\\
                    void main()\\
                    {\\
                    vec3 n = normalize(normal);\\
                    vec3 s = normalize(vec3(1.0, 0.0, 1.0) - position);\\
                    vec3 v = normalize(-position);\\
                    float diffuse = max(dot(s, n), 0.0);\\
                    //fragColor = vec4(diffuse * finalColor, 1.0);\\
                    gl_FragColor = 1.0;\\
                    }");
    */

    shader->setVertexShaderCode("#version 330 core\n \
            in vec3 vertexPosition;\n\
            in vec3 vertexNormal;\n\
            out EyeSpaceVertex {\n\
            vec3 position;\n\
            vec3 normal;\n\
            } vs_out;\n\
            uniform mat4 modelView;\n\
            uniform mat3 modelViewNormal;\n\
            uniform mat4 mvp;\n\
            void main()\n\
            {\n\
                vs_out.normal = normalize( modelViewNormal * vertexNormal );\n\
                vs_out.position = vec3( modelView * vec4( vertexPosition, 1.0 ) );\n\
                gl_Position = mvp * vec4( vertexPosition, 1.0 );\n\
            }");

    shader->setFragmentShaderCode("#version 330 core\n \
            uniform struct LightInfo {\n\
                vec4 position;\n\
                vec3 intensity;\n\
            } light;\n\
            uniform struct LineInfo {\n\
                float width;\n\
                vec4 color;\n\
            } line;\n\
            uniform vec3 ka;            // Ambient reflectivity\n\
            uniform vec3 kd;            // Diffuse reflectivity\n\
            uniform vec3 ks;            // Specular reflectivity\n\
            uniform float shininess;    // Specular shininess factor\n\
            in WireframeVertex {\n\
                vec3 position;\n\
                vec3 normal;\n\
                noperspective vec4 edgeA;\n\
                noperspective vec4 edgeB;\n\
                flat int configuration;\n\
            } fs_in;\n\
            out vec4 fragColor;\n\
            vec3 adsModel( const in vec3 pos, const in vec3 n )\n\
            {\n\
                // Calculate the vector from the light to the fragment\n\
                vec3 s = normalize( vec3( light.position ) - pos );\n\
                // Calculate the vector from the fragment to the eye position (the\n\
                // origin since this is in \"eye\" or \"camera\" space\n\
                vec3 v = normalize( -pos );\n\
                // Refleft the light beam using the normal at this fragment\n\
                vec3 r = reflect( -s, n );\n\
                // Calculate the diffus component\n\
                vec3 diffuse = vec3( max( dot( s, n ), 0.0 ) );\n\
                // Calculate the specular component\n\
                vec3 specular = vec3( pow( max( dot( r, v ), 0.0 ), shininess ) );\n\
                // Combine the ambient, diffuse and specular contributions\n\
                return light.intensity * ( ka + kd * diffuse + ks * specular );\n\
            }\n\
            vec4 shadeLine( const in vec4 color )\n\
            {\n\
                // Find the smallest distance between the fragment and a triangle edge\n\
                float d;\n\
                if ( fs_in.configuration == 0 )\n\
                {\n\
                    // Common configuration\n\
                    d = min( fs_in.edgeA.x, fs_in.edgeA.y );\n\
                    d = min( d, fs_in.edgeA.z );\n\
                }\n\
                else\n\
                {\n\
                    // Handle configuration where screen space projection breaks down\n\
                    // Compute and compare the squared distances\n\
                    vec2 AF = gl_FragCoord.xy - fs_in.edgeA.xy;\n\
                    float sqAF = dot( AF, AF );\n\
                    float AFcosA = dot( AF, fs_in.edgeA.zw );\n\
                    d = abs( sqAF - AFcosA * AFcosA );\n\
                    vec2 BF = gl_FragCoord.xy - fs_in.edgeB.xy;\n\
                    float sqBF = dot( BF, BF );\n\
                    float BFcosB = dot( BF, fs_in.edgeB.zw );\n\
                    d = min( d, abs( sqBF - BFcosB * BFcosB ) );\n\
                    // Only need to care about the 3rd edge for some configurations.\n\
                    if ( fs_in.configuration == 1 || fs_in.configuration == 2 || fs_in.configuration == 4 )\n\
                    {\n\
                        float AFcosA0 = dot( AF, normalize( fs_in.edgeB.xy - fs_in.edgeA.xy ) );\n\
                        d = min( d, abs( sqAF - AFcosA0 * AFcosA0 ) );\n\
                    }\n\
                    d = sqrt( d );\n\
                }\n\
                // Blend between line color and phong color\n\
                float mixVal;\n\
                if ( d < line.width - 1.0 )\n\
                {\n\
                    mixVal = 1.0;\n\
                }\n\
                else if ( d > line.width + 1.0 )\n\
                {\n\
                    mixVal = 0.0;\n\
                }\n\
                else\n\
                {\n\
                    float x = d - ( line.width - 1.0 );\n\
                    mixVal = exp2( -2.0 * ( x * x ) );\n\
                }\n\
                return mix( color, line.color, mixVal );\n\
            }\n\
            void main()\n\
            {\n\
                // Calculate the color from the phong model\n\
                vec4 color = vec4( adsModel( fs_in.position, normalize( fs_in.normal ) ), 1.0 );\n\
                fragColor = shadeLine( color );\n\
            }");

    shader->setGeometryShaderCode("#version 330 core\n \
        layout( triangles ) in;\n\
        layout( triangle_strip, max_vertices = 3 ) out;\n\
        in EyeSpaceVertex {\n\
            vec3 position;\n\
            vec3 normal;\n\
        } gs_in[];\n\
        out WireframeVertex {\n\
            vec3 position;\n\
            vec3 normal;\n\
            noperspective vec4 edgeA;\n\
            noperspective vec4 edgeB;\n\
            flat int configuration;\n\
        } gs_out;\n\
        uniform mat4 viewportMatrix;\n\
        const int infoA[]  = int[]( 0, 0, 0, 0, 1, 1, 2 );\n\
        const int infoB[]  = int[]( 1, 1, 2, 0, 2, 1, 2 );\n\
        const int infoAd[] = int[]( 2, 2, 1, 1, 0, 0, 0 );\n\
        const int infoBd[] = int[]( 2, 2, 1, 2, 0, 2, 1 );\n\
        vec2 transformToViewport( const in vec4 p )\n\
        {\n\
            return vec2( viewportMatrix * ( p / p.w ) );\n\
        }\n\
        void main()\n\
        {\n\
            gs_out.configuration = int(gl_in[0].gl_Position.z < 0) * int(4)\n\
                + int(gl_in[1].gl_Position.z < 0) * int(2)\n\
                + int(gl_in[2].gl_Position.z < 0);\n\
            // If all vertices are behind us, cull the primitive\n\
            if (gs_out.configuration == 7)\n\
                return;\n\
            // Transform each vertex into viewport space\n\
            vec2 p[3];\n\
            p[0] = transformToViewport( gl_in[0].gl_Position );\n\
            p[1] = transformToViewport( gl_in[1].gl_Position );\n\
            p[2] = transformToViewport( gl_in[2].gl_Position );\n\
            if (gs_out.configuration == 0)\n\
            {\n\
                // Common configuration where all vertices are within the viewport\n\
                gs_out.edgeA = vec4(0.0);\n\
                gs_out.edgeB = vec4(0.0);\n\
                // Calculate lengths of 3 edges of triangle\n\
                float a = length( p[1] - p[2] );\n\
                float b = length( p[2] - p[0] );\n\
                float c = length( p[1] - p[0] );\n\
                // Calculate internal angles using the cosine rule\n\
                float alpha = acos( ( b * b + c * c - a * a ) / ( 2.0 * b * c ) );\n\
                float beta = acos( ( a * a + c * c - b * b ) / ( 2.0 * a * c ) );\n\
                // Calculate the perpendicular distance of each vertex from the opposing edge\n\
                float ha = abs( c * sin( beta ) );\n\
                float hb = abs( c * sin( alpha ) );\n\
                float hc = abs( b * sin( alpha ) );\n\
                // Now add this perpendicular distance as a per-vertex property in addition to\n\
                // the position and normal calculated in the vertex shader.\n\
                // Vertex 0 (a)\n\
                gs_out.edgeA = vec4( ha, 0.0, 0.0, 0.0 );\n\
                gs_out.normal = gs_in[0].normal;\n\
                gs_out.position = gs_in[0].position;\n\
                gl_Position = gl_in[0].gl_Position;\n\
                EmitVertex();\n\
                // Vertex 1 (b)\n\
                gs_out.edgeA = vec4( 0.0, hb, 0.0, 0.0 );\n\
                gs_out.normal = gs_in[1].normal;\n\
                gs_out.position = gs_in[1].position;\n\
                gl_Position = gl_in[1].gl_Position;\n\
                EmitVertex();\n\
                // Vertex 2 (c)\n\
                gs_out.edgeA = vec4( 0.0, 0.0, hc, 0.0 );\n\
                gs_out.normal = gs_in[2].normal;\n\
                gs_out.position = gs_in[2].position;\n\
                gl_Position = gl_in[2].gl_Position;\n\
                EmitVertex();\n\
                // Finish the primitive off\n\
                EndPrimitive();\n\
            }\n\
            else\n\
            {\n\
                // Viewport projection breaks down for one or two vertices.\n\
                // Caclulate what we can here and defer rest to fragment shader.\n\
                // Since this is coherent for the entire primitive the conditional\n\
                // in the fragment shader is still cheap as all concurrent\n\
                // fragment shader invocations will take the same code path.\n\
                // Copy across the viewport-space points for the (up to) two vertices\n\
                // in the viewport\n\
                gs_out.edgeA.xy = p[infoA[gs_out.configuration]];\n\
                gs_out.edgeB.xy = p[infoB[gs_out.configuration]];\n\
                // Copy across the viewport-space edge vectors for the (up to) two vertices\n\
                // in the viewport\n\
                gs_out.edgeA.zw = normalize( gs_out.edgeA.xy - p[ infoAd[gs_out.configuration] ] );\n\
                gs_out.edgeB.zw = normalize( gs_out.edgeB.xy - p[ infoBd[gs_out.configuration] ] );\n\
                // Pass through the other vertex attributes\n\
                gs_out.normal = gs_in[0].normal;\n\
                gs_out.position = gs_in[0].position;\n\
                gl_Position = gl_in[0].gl_Position;\n\
                EmitVertex();\n\
                gs_out.normal = gs_in[1].normal;\n\
                gs_out.position = gs_in[1].position;\n\
                gl_Position = gl_in[1].gl_Position;\n\
                EmitVertex();\n\
                gs_out.normal = gs_in[2].normal;\n\
                gs_out.position = gs_in[2].position;\n\
                gl_Position = gl_in[2].gl_Position;\n\
                EmitVertex();\n\
                // Finish the primitive off\n\
                EndPrimitive();\n\
            }\n\
        }");
  
    pass->setShaderProgram(shader);
    technique->addRenderPass(pass); 
    effect->addTechnique(technique);
    m_pMaterial->setEffect(effect);

    /* 
    QFile vert("ui/shaders/axis_vert.glsl");
    QFile frag("ui/shaders/axis_frag.glsl");
    QFile geom("ui/shaders/axis_geom.glsl");

    if (!vert.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Axis vert shader failed to open.\n";
        return;
    }
    else if (!frag.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Axis frag shader failed to open.\n";
        return;
    }
    else if (!geom.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Axis geom shader failed to open.\n";
        return;
    }
    else {
        std::cout << "Axis shaders loaded OK.\n";
        while (!vert.atEnd()) {
            //std::cout << "loading vert shader\n";
            shader->setVertexShaderCode(vert.readLine());
        }
        while (!frag.atEnd()) {
            //std::cout << "loading frag shader\n";
            shader->setFragmentShaderCode(frag.readLine());
        }
        while (!geom.atEnd()) {
            //std::cout << "loading geom shader\n";
            shader->setGeometryShaderCode(geom.readLine());
        }

        pass->setShaderProgram(shader);
        technique->addRenderPass(pass); 
        effect->addTechnique(technique);
        m_pMaterial->setEffect(effect);
    }
    */

    addComponent(layer);
    addComponent(m_pTransform);
    addComponent(m_pMaterial);
    addComponent(m_pMesh);

    //connect(m_pMouseInput,SIGNAL(entered()),this,SLOT(mouseClicked()));
    /*
    connect(m_pTransform,SIGNAL(entered()),this,SLOT(mouseClicked()));
    connect(m_pMesh,SIGNAL(entered()),this,SLOT(mouseClicked()));
    connect(m_pMaterial,SIGNAL(entered()),this,SLOT(mouseClicked()));
    */
}

Axis::~Axis()
{

}

/*
void Axis::mouseClicked()
{
    std::cout << "Axis Pressed\n";
}
*/

// GRID GEOMETRY

GridGeometry::GridGeometry(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometry(parent),
    m_majorLevel(10),
    m_minorLevel(10),
    m_meshAttribute(new Qt3DRender::QAttribute(this)),
    m_vertexBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this))
{
    build();

    //const int nVerts = 6;
    const int size = m_grid.size() * sizeof(feather::FVertex3D);
    QByteArray meshBytes;
    meshBytes.resize(size);
    memcpy(meshBytes.data(), m_grid.data(), size);

    m_vertexBuffer->setData(meshBytes);

    m_meshAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    m_meshAttribute->setDataType(Qt3DRender::QAttribute::Float);
    m_meshAttribute->setDataSize(3);
    m_meshAttribute->setCount(m_grid.size());
    m_meshAttribute->setByteStride(sizeof(feather::FVertex3D));
    m_meshAttribute->setBuffer(m_vertexBuffer);

    //setVerticesPerPatch(4);
    addAttribute(m_meshAttribute);
}

int GridGeometry::majorSubDividLevel()
{
    return m_majorLevel;
}

int GridGeometry::minorSubDividLevel()
{
    return m_minorLevel;
}

void GridGeometry::setMajorSubDividLevel(const int &level)
{
    m_majorLevel = level;
}

void GridGeometry::setMinorSubDividLevel(const int &level)
{
    m_minorLevel = level;
}

void GridGeometry::build()
{
    float major_spacing = 10.0/m_majorLevel;
    //float minor_spacing = major_spacing/m_minorLevel;

    // Z corners
    m_grid.push_back(feather::FVertex3D(-10,0,10));
    m_grid.push_back(feather::FVertex3D(-10,0,-10));
    m_grid.push_back(feather::FVertex3D(10,0,10));
    m_grid.push_back(feather::FVertex3D(10,0,-10));
 
    // X corners
    m_grid.push_back(feather::FVertex3D(10,0,-10));
    m_grid.push_back(feather::FVertex3D(-10,0,-10));
    m_grid.push_back(feather::FVertex3D(10,0,10));
    m_grid.push_back(feather::FVertex3D(-10,0,10));

    double cpos = 10.0; 
    for(unsigned int i=0; i < 10; i++){
        cpos -= major_spacing;
        // Z corners
        m_grid.push_back(feather::FVertex3D(-cpos,0,10));
        m_grid.push_back(feather::FVertex3D(-cpos,0,-10));
        m_grid.push_back(feather::FVertex3D(cpos,0,10));
        m_grid.push_back(feather::FVertex3D(cpos,0,-10));

        // X corners
        m_grid.push_back(feather::FVertex3D(10,0,-cpos));
        m_grid.push_back(feather::FVertex3D(-10,0,-cpos));
        m_grid.push_back(feather::FVertex3D(10,0,cpos));
        m_grid.push_back(feather::FVertex3D(-10,0,cpos));
    } 
}

// GRID

Grid::Grid(Qt3DRender::QLayer* layer, Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent),
    m_pTransform(new Qt3DCore::QTransform()),
    m_pMaterial(new Qt3DExtras::QPhongMaterial()),
    m_pMesh(new Qt3DRender::QGeometryRenderer())
{
    connect(m_pMaterial, SIGNAL(diffuseChanged(QColor)), this, SIGNAL(diffuseColorChanged(QColor)));
    m_pMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    m_pMesh->setGeometry(new GridGeometry(this));
    
    m_pMaterial->setDiffuse(QColor(Qt::black));
    m_pMaterial->setAmbient(Qt::black);
    m_pMaterial->setSpecular(Qt::black);
    m_pMaterial->setShininess(0.0f);

    addComponent(layer);
    addComponent(m_pTransform);
    addComponent(m_pMesh);
    addComponent(m_pMaterial);
}

Grid::~Grid()
{

}

void Grid::setDiffuseColor(const QColor &diffuseColor)
{
    m_pMaterial->setDiffuse(diffuseColor);
}

QColor Grid::diffuseColor()
{
    return m_pMaterial->diffuse();
}


TessellatedGeometry::TessellatedGeometry(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometry(parent),
    m_positionAttribute(new Qt3DRender::QAttribute(this)),
    m_vertexBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this))
{
    const float positionData[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f
    };

    const int nVerts = 6;
    const int size = nVerts * 3 * sizeof(float);
    QByteArray positionBytes;
    positionBytes.resize(size);
    memcpy(positionBytes.data(), positionData, size);

    m_vertexBuffer->setData(positionBytes);

    m_positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    m_positionAttribute->setDataType(Qt3DRender::QAttribute::Float);
    m_positionAttribute->setDataSize(3);
    m_positionAttribute->setCount(nVerts);
    m_positionAttribute->setByteStride(3 * sizeof(float));
    m_positionAttribute->setBuffer(m_vertexBuffer);

    //setVerticesPerPatch(4);
    addAttribute(m_positionAttribute);

}


Object::Object(Qt3DRender::QLayer* layer, Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent),
    m_translate(QVector3D()),
    m_transform(new Qt3DCore::QTransform()),
    //m_translate(new Qt3D::QTranslateTransform()),
    m_mesh(new Qt3DRender::QGeometryRenderer()),
    m_material(new Qt3DExtras::QPhongMaterial())
{
    m_transform->setTranslation(m_translate);

    connect(m_material, SIGNAL(diffuseChanged(QColor)), this, SIGNAL(diffuseColorChanged(QColor)));
    m_material->setAmbient(Qt::red);
    m_material->setSpecular(Qt::white);
    m_material->setShininess(0.0f);

    //m_pMouseController = new Qt3D::QMouseController(this);
    //m_pMouseInput = new Qt3D::QMouseInput(this);
    //m_pMouseInput->setController(m_pMouseController);
    //connect(m_pMouseInput,SIGNAL(clicked(Qt3D::Q3DMouseEvent*)),this,SLOT(onClicked(Qt3D::Q3DMouseEvent*)));
    //connect(m_pMouseInput,SIGNAL(event(Qt3D::Q3DMouseEvent*)),this,SLOT(onEvent(Qt3D::Q3DMouseEvent*)));

    /*
    Points
    Lines
    LineLoop
    LineStrip
    Triangles
    TriangleStrip
    TriangleFan
    LinesAdjacency
    TrianglesAdjacency
    LineStripAdjacency
    TriangleStripAdjacency
    Patches
    */
    // Faces
    m_mesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
    // Edges
    //m_mesh->setPrimitiveType(Qt3D::QGeometryRenderer::LineLoop);
    // Points
    //m_mesh->setPrimitiveType(Qt3D::QGeometryRenderer::Points);
    m_mesh->setGeometry(new TessellatedGeometry(this));

    addComponent(layer);
    addComponent(m_transform);
    addComponent(m_mesh);
    addComponent(m_material);
    //addComponent(m_pMouseInput);
}

void Object::setAmbientColor(const QColor &ambientColor)
{
    m_material->setAmbient(ambientColor);
}


void Object::setSpecularColor(const QColor &specularColor)
{
    m_material->setSpecular(specularColor);
}

void Object::setDiffuseColor(const QColor &diffuseColor)
{
    m_material->setDiffuse(diffuseColor);
}

QColor Object::ambientColor()
{
    return m_material->ambient();
}

QColor Object::specularColor()
{
    return m_material->specular();
}

QColor Object::diffuseColor()
{
    return m_material->diffuse();
}

/*
void Object::onClicked(Qt3D::Q3DMouseEvent* event)
{
    std::cout << "Object Clicked\n";
}

void Object::onEvent(Qt3D::Q3DMouseEvent* event)
{
    std::cout << "Object Event\n";
}
*/


// FRAMEGRAPH
FrameGraph::FrameGraph(Qt3DCore::QNode* parent)
    : QRenderSettings(parent),
    m_pViewport(new Qt3DRender::QViewport()),
    m_pClearBuffer(new Qt3DRender::QClearBuffers()),
    m_pCameraSelector(new Qt3DRender::QCameraSelector()),
    m_pLayerFilter(new Qt3DRender::QLayerFilter()),
    m_pRenderSurfaceSelector(new Qt3DRender::QRenderSurfaceSelector())
{
    m_pViewport->setNormalizedRect(QRect(0,0,1,1));
    // GONE
    m_pClearBuffer->setClearColor(QColor("grey"));
    //setActiveFrameGraph(m_pRenderSurfaceSelector);
    m_pLayerFilter->setEnabled(true);
    m_pLayerFilter->addLayer(new Qt3DRender::QLayer());
    m_pClearBuffer->setBuffers(Qt3DRender::QClearBuffers::AllBuffers);
    //m_pCameraSelector->setParent(m_pClearBuffer);
    m_pCameraSelector->setParent(m_pLayerFilter);
 
    m_pClearBuffer->setParent(m_pCameraSelector);
    m_pLayerFilter->setParent(m_pRenderSurfaceSelector);
    m_pRenderSurfaceSelector->setParent(m_pViewport);
    //setExternalRenderTargetSize(QSize(800,600));
    // GONE
    setActiveFrameGraph(m_pViewport);
}

FrameGraph::~FrameGraph()
{
    //QNode::cleanup();
}

void FrameGraph::setCamera(Qt3DRender::QCamera* camera)
{
    m_pCameraSelector->setCamera(camera); 
}


// VIEWPORT
Viewport::Viewport(Qt3DCore::QNode* parent)
    : Qt3DCore::QEntity(parent),
    m_showGrid(true),
    m_showAxis(true),
    m_width(800),
    m_height(600),
    m_ignoreFirstLeftMousePress(true),
    m_ignoreFirstRightMousePress(true),
    m_ignoreFirstMiddleMousePress(true),
    m_translateSpeed(7.0),
    m_panSpeed(360.0),
    m_orbitSpeed(600.0),
    m_wheelSpeed(-0.005),
    m_inputEnabled(true),
    m_cameraUp(QVector3D(0.0,1.0,0.0)),
    m_adjustCameraAtMouseRelease(false),
    //m_pMouseController(new Qt3D::QMouseController()),
    m_pMouseDevice(new Qt3DInput::QMouseDevice(this)),
    m_pMouseHandler(new Qt3DInput::QMouseHandler(this)),
    m_pKeyboardDevice(new Qt3DInput::QKeyboardDevice(this)),
    m_pKeyboardHandler(new Qt3DInput::QKeyboardHandler(this)),
    m_pInputSettings(new Qt3DInput::QInputSettings(this)),
    m_pLeftMouseButtonAction(new Qt3DInput::QAction()),
    m_pLeftMouseButtonInput(new Qt3DInput::QActionInput()),
    m_pRightMouseButtonAction(new Qt3DInput::QAction()),
    m_pRightMouseButtonInput(new Qt3DInput::QActionInput()),
    m_pMiddleMouseButtonAction(new Qt3DInput::QAction()),
    m_pMiddleMouseButtonInput(new Qt3DInput::QActionInput()),
    m_pRx(new Qt3DInput::QAxis()),
    m_pMouseRxInput(new Qt3DInput::QAnalogAxisInput()),
    m_pRy(new Qt3DInput::QAxis()),
    m_pMouseRyInput(new Qt3DInput::QAnalogAxisInput()),
    m_pPickingSettings(new Qt3DRender::QPickingSettings()),
    m_pLogicalDevice(new Qt3DInput::QLogicalDevice()),
    m_pFrameAction(new Qt3DLogic::QFrameAction()),
 
    //m_pLight(new Qt3DRender::QPointLight(this)),
    m_pCamera(new Qt3DRender::QCamera()),
    //m_pRenderSettings(new Qt3DRender::QRenderSettings()),
    //m_pRenderSurfaceSelector(new Qt3DRender::QRenderSurfaceSelector()),
    //m_pConfiguration(new Qt3D::Quick::Quick3DConfiguration(this)),
    m_pFrameGraph(new FrameGraph())
{
 
    m_pCamera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    m_pCamera->setFieldOfView(45);
    m_pCamera->setNearPlane(0.1);
    m_pCamera->setFarPlane(100.0);
    m_pCamera->setAspectRatio(1.3);
    m_pCamera->setPosition(QVector3D(0,3,20));
    m_pCamera->setUpVector(QVector3D(0,1,0));
    m_pCamera->setViewCenter(QVector3D(0,0,0));

    // GONE
    //m_pConfiguration->setControlledCamera(m_pCamera);
    
    m_pFrameGraph->setCamera(m_pCamera);

    //m_pRenderSettings->setActiveFrameGraph(m_pFrameGraph);

    m_clearBuffer.setBuffers(Qt3DRender::QClearBuffers::AllBuffers);
    // GONE
    //m_pMouseInput->setController(m_pMouseController);
 
    m_pGrid = new Grid(m_pFrameGraph->layer(),this);
    //m_pAxis = new Axis(m_pFrameGraph->layer(),this);
    //m_pTorus = new Qt3DExtras::QTorusMesh();

    m_pPickingSettings->setPickMethod(Qt3DRender::QPickingSettings::TrianglePicking);
    m_pPickingSettings->setPickResultMode(Qt3DRender::QPickingSettings::AllPicks);

    m_pInputSettings->setEventSource(m_pMouseHandler);
    m_pMouseHandler->setSourceDevice(m_pMouseDevice);
    m_pKeyboardHandler->setSourceDevice(m_pKeyboardDevice);
    m_pKeyboardHandler->setFocus(true); // keyboard won't be seen if this is not here

    m_pLeftMouseButtonInput->setButtons(QVector<int>() << Qt3DInput::QMouseEvent::LeftButton); 
    m_pLeftMouseButtonInput->setSourceDevice(m_pMouseDevice);
    m_pLeftMouseButtonAction->addInput(m_pLeftMouseButtonInput);

    m_pRightMouseButtonInput->setButtons(QVector<int>() << Qt3DInput::QMouseEvent::RightButton); 
    m_pRightMouseButtonInput->setSourceDevice(m_pMouseDevice);
    m_pRightMouseButtonAction->addInput(m_pRightMouseButtonInput);

    m_pMiddleMouseButtonInput->setButtons(QVector<int>() << Qt3DInput::QMouseEvent::MiddleButton); 
    m_pMiddleMouseButtonInput->setSourceDevice(m_pMouseDevice);
    m_pMiddleMouseButtonAction->addInput(m_pMiddleMouseButtonInput);

    m_pMouseRxInput->setAxis(Qt3DInput::QMouseDevice::X);
    m_pMouseRxInput->setSourceDevice(m_pMouseDevice);
    m_pRx->addInput(m_pMouseRxInput);

    m_pMouseRyInput->setAxis(Qt3DInput::QMouseDevice::Y);
    m_pMouseRyInput->setSourceDevice(m_pMouseDevice);
    m_pRy->addInput(m_pMouseRyInput);

    m_pLogicalDevice->addAction(m_pLeftMouseButtonAction);
    m_pLogicalDevice->addAction(m_pRightMouseButtonAction);
    m_pLogicalDevice->addAction(m_pMiddleMouseButtonAction);
    m_pLogicalDevice->addAxis(m_pRx);
    m_pLogicalDevice->addAxis(m_pRy);

    connect(m_pFrameAction,&Qt3DLogic::QFrameAction::triggered,this,&Viewport::handleTriggered);

    // get all the cameras
    //feather::draw::DrawItems items;
    //feather::qml::command::get_node_draw_items(2,items);
    addItems(2);
    //std::cout << "There are " << items.size() << " draw items\n";

    //buildScene(items);
    updateScene();

    //connect(m_pMouseHandler,SIGNAL(entered()),this,SLOT(onEntered()));
    //connect(m_pMouseHandler,SIGNAL(clicked(Qt3DInput::QMouseEvent*)),this,SLOT(onClicked(Qt3DInput::QMouseEvent*)));
    connect(m_pMouseHandler,SIGNAL(wheel(Qt3DInput::QWheelEvent*)),this,SLOT(wheelEvent(Qt3DInput::QWheelEvent*)));
    connect(m_pKeyboardHandler,SIGNAL(spacePressed(Qt3DInput::QKeyEvent*)),this,SLOT(doSpacePressed(Qt3DInput::QKeyEvent*)));


    // Light testing
    //m_pLight->setColor(Qt::blue);
    //m_pLight->setIntensity(1.5f);
    
    //m_pLight->setPosition(QVector3D(0,4,0)); 
    //Qt3D::QClearBuffer clearBuffer(this);
    addComponent(m_pFrameGraph);
    addComponent(m_pMouseHandler);
    addComponent(m_pKeyboardHandler);
    addComponent(m_pInputSettings);
    addComponent(m_pLogicalDevice);
    addComponent(m_pFrameAction);

    if(!m_pMouseHandler->containsMouse())
        std::cout << "THERE IS NO MOUSE DETECTED FOR THE VIEWPORT\n";
}

Viewport::~Viewport()
{
    delete m_pGrid;
    m_pGrid=0;
    delete m_pAxis;
    m_pAxis=0;
    qDeleteAll(m_apDrawItems);
}

void Viewport::handleTriggered(float dt)
{
    if(m_pCamera){
        if(m_pLeftMouseButtonAction->isActive()) {
            //std::cout << "left mouse button active\n";
            if (m_ignoreFirstLeftMousePress) {                     
                m_ignoreFirstLeftMousePress = false;                  
            } else {
                // Pan and tilt the camera with left mouse button         
                m_pCamera->pan(m_pRx->value() * m_panSpeed * dt, m_cameraUp);                           
                m_pCamera->tilt(m_pRy->value() * m_panSpeed * dt);           
                m_adjustCameraAtMouseRelease = true;                                  
            }
        }
        else if(m_pRightMouseButtonAction->isActive()) {
            //std::cout << "right mouse button active\n";
            // Ignore first press so you don't get the initial jolt,
            //             // as the mouse delta is usually maximum into some direction.
            if (m_ignoreFirstRightMousePress) {
                m_ignoreFirstRightMousePress = false;
            } else {
                // Move camera around with right mouse button
                QMatrix4x4 viewMatrix = m_pCamera->viewMatrix().inverted();
                float translateMultipler = m_translateSpeed * dt * m_pCamera->viewVector().length();
                QVector3D translateVector(m_pRx->value() * translateMultipler,
                        m_pRy->value() * translateMultipler, 0.0f);
                translateVector = viewMatrix.mapVector(translateVector);
                adjustCamera(translateVector);
            }
        }
        else if(m_pMiddleMouseButtonAction->isActive()) {
            //std::cout << "middle mouse button active\n";
            if (m_ignoreFirstMiddleMousePress) {
                m_ignoreFirstMiddleMousePress = false;
            } else {
                // Orbit camera around viewCenter with middle mouse button
                m_pCamera->panAboutViewCenter(-m_pRx->value() * m_orbitSpeed * dt, m_cameraUp);
                m_pCamera->tiltAboutViewCenter(-m_pRy->value() * m_orbitSpeed * dt);
                m_adjustCameraAtMouseRelease = true;
            }
        }
        else {
        }
    }
}

void Viewport::wheelEvent(Qt3DInput::QWheelEvent *event)
{
    if (m_inputEnabled)
        handleWheel(event);
    else
        event->setAccepted(false);
}

void Viewport::handleWheel(Qt3DInput::QWheelEvent *event)
{
    if (m_pCamera) {
        // Find intersection of cursor and camera far plane.
        QVector3D planeNormal = cameraNormal(m_pCamera);
        QVector3D planeOrigin = m_pCamera->position() + m_pCamera->viewVector();
        float cosAngle = QVector3D::dotProduct(planeOrigin.normalized(), planeNormal);
        float planeOffset = planeOrigin.length() * cosAngle;
        QPoint pos = QPoint(event->x(),event->y());
        QVector3D ray = unprojectRay(m_pCamera->viewMatrix(), m_pCamera->projectionMatrix(), m_width, m_height, event->angleDelta());
        float t = 0.0f;
        QVector3D intersection = findIntersection(m_pCamera->position(), ray, planeOffset, planeNormal, t);

        // We want to keep the same world position under cursor, so we use this formula to find
        // correct translation:
        // x = camera viewVector
        // y = vector from camera viewCenter to intersection
        // x_delta = Camera translation in x vector direction
        // y_delta = Camera translation in y vector direction
        // To solve y_delta, we can use similar triangles:
        // x.len()/y.len() = x_delta/y_delta
        // --> y_delta = (y.len() * x_delta) / x.len()
        QMatrix4x4 viewMatrix = m_pCamera->viewMatrix().inverted();
        QVector3D translateVec(0.0f, 0.0f, event->angleDelta().y() * m_wheelSpeed);
        translateVec = viewMatrix.mapVector(translateVec); // just x_delta vector for now

        float x_delta = translateVec.length();
        if (event->angleDelta().y() < 0)
            x_delta = -x_delta;

        // Add y_delta vector
        translateVec += ((intersection - m_pCamera->viewCenter()) * x_delta) / m_pCamera->viewVector().length();
        adjustCamera(translateVec);
    }
}

QVector3D Viewport::unprojectRay(const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix, int viewPortWidth, int viewPortHeight, const QPoint &pos)
{       
    float x = ((2.0f * pos.x()) / viewPortWidth) - 1.0f;
    float y = 1.0f - ((2.0f * pos.y()) / viewPortHeight);

    // Figure out the ray to the screen position
    QVector4D ray = projectionMatrix.inverted() * QVector4D(x, y, -1.0f, 1.0f);
    ray.setZ(-1.0f);
    ray.setW(0.0f);
    ray = viewMatrix.inverted() * ray;
    return ray.toVector3D().normalized();
}

void Viewport::adjustCamera(QVector3D &translateVec)
{
    // Adjust viewcenter so that it is on a plane intersecting origin, if possible.
    // That way we get nice orbiting with middle button.
    // Otherwise just adjust it same as position.
    float t = 0.0f;
    QVector3D newPosition = m_pCamera->position() + translateVec;
    QVector3D intersection = findIntersection(newPosition, m_pCamera->viewVector().normalized(), 0, cameraNormal(m_pCamera), t);
    if (t > 0.0f)
        m_pCamera->setViewCenter(intersection);
    else
        m_pCamera->setViewCenter(m_pCamera->viewCenter() + translateVec);

    m_pCamera->setPosition(newPosition);
}

// Returns the intersection point of a plane and a ray.
// Parameter t returns the distance in ray lengths. If t is negative, intersection
// is behind rayOrigin.
// If there is no intersection, i.e. plane and the ray are paraller, t is set to -1 and
// rayOrigin is returned.
QVector3D Viewport::findIntersection(const QVector3D &rayOrigin, const QVector3D &ray, float planeOffset, const QVector3D &planeNormal, float &t)
{
    float divisor = QVector3D::dotProduct(ray, planeNormal);
    if (qFuzzyCompare(1.0f, 1.0f + divisor)) {
        t = -1.0f;
        return rayOrigin;
    }

    t = -(QVector3D::dotProduct(rayOrigin, planeNormal) - planeOffset) / divisor;

    return rayOrigin + ray * t;

}

QVector3D Viewport::cameraNormal(Qt3DRender::QCamera *camera)
{
    QVector3D planeNormal;
    if (camera) {
        planeNormal = camera->position() - camera->viewCenter();
        planeNormal.normalize();
    }
    return planeNormal;
}

void Viewport::doSpacePressed(Qt3DInput::QKeyEvent* event)
{
    std::cout << "SPACE PRESSED\n";
}

void Viewport::updateScene()
{
    std::cout << "updating vp scene\n";
    /* 
    feather::draw::DrawItems items;
    feather::qml::command::get_node_draw_items(1,items);
    */
    std::cout << "There are " << m_apDrawItems.size() << " draw items\n";

    /* 
    if(m_apDrawItems.isEmpty()) {
        buildScene(items);
        //buildScene();
    } else {
    
    */
    /* 
        Q_FOREACH (DrawItem* item, m_apDrawItems) {
            item->setParent(this);
        }
    */
    //}

    for(auto item : m_apDrawItems) {
        switch(item->item()->type){
            case feather::draw::Item::Mesh:
                std::cout << "updating Mesh draw item\n";
                static_cast<Mesh*>(item)->updateItem();
                // GONE
                //removeAllComponents();
                //delete item;
                break;
            case feather::draw::Item::Line:
                std::cout << "updating Line draw item\n";
                static_cast<Line*>(item)->updateItem();
                break;
            case feather::draw::Item::PerspCamera:
                std::cout << "updating Perspective Camear draw item\n";
                static_cast<PerspCamera*>(item)->updateItem();
                break;
            default:
                std::cout << "nothing built\n";
        }
    }

    std::cout << "THERE ARE " << components().count() << std::endl;

    /*
    Q_FOREACH(DrawItem* item, m_apDrawItems){
        //item->updateItem();
        
        delete item;
    }
    */
}

bool Viewport::buildItems(feather::draw::DrawItems& items)
{
    return false;
}

int Viewport::majorSubDividLevel()
{
    return m_pGrid->grid()->majorSubDividLevel();
}

int Viewport::minorSubDividLevel()
{
    return m_pGrid->grid()->minorSubDividLevel();
}

bool Viewport::showGrid()
{
    return m_showGrid;
}

bool Viewport::showAxis()
{
    return m_showAxis;
}

int Viewport::width()
{
    return m_width;
}

int Viewport::height()
{
    return m_height;
}

void Viewport::setMajorSubDividLevel(const int &level)
{
    m_pGrid->grid()->setMajorSubDividLevel(level);
    majorSubDividLevelChanged();
}

void Viewport::setMinorSubDividLevel(const int &level)
{
    m_pGrid->grid()->setMinorSubDividLevel(level);
    minorSubDividLevelChanged();
}

void Viewport::setShowGrid(const bool &show)
{
    m_showGrid = show;
    showGridChanged();
}

void Viewport::setShowAxis(const bool &show)
{
    m_showAxis = show;
    showAxisChanged();
}

void Viewport::setWidth(const int &val)
{
    m_width = val;
    widthChanged();
}

void Viewport::setHeight(const int &val)
{
    m_height = val;
    heightChanged();
}

void Viewport::buildScene(feather::draw::DrawItems& items)
{
    for(auto item : items) {
        switch(item->type){
            case feather::draw::Item::Mesh:
                std::cout << "build Mesh\n";
                m_apDrawItems.append(new Mesh(m_pFrameGraph->layer(),item,this));
                break;
            case feather::draw::Item::Line:
                std::cout << "build Line\n";
                m_apDrawItems.append(new Line(m_pFrameGraph->layer(),item,this));
                break;
            case feather::draw::Item::PerspCamera:
                std::cout << "build PerspCamera\n";
                m_apDrawItems.append(new PerspCamera(m_pCamera,m_pFrameGraph->layer(),item,this));
                break;
            default:
                std::cout << "nothing built\n";
        }
    }
}

void Viewport::onEntered()
{
    std::cout << "VP Entered\n";
}

void Viewport::onClicked(Qt3DInput::QMouseEvent* event)
{
    std::cout << "VP Clicked\n";
}

void Viewport::addItems(unsigned int uid)
{
    feather::status pass;
    unsigned int nid = feather::qml::command::get_node_id(uid,pass);
    feather::draw::DrawItems items;
    feather::qml::command::get_node_draw_items(nid,items);
    std::cout << "add draw item " << uid << std::endl;

    m_apDrawItems.clear();

    for(auto item : items) {
        item->uid=uid;
        item->nid=nid;
        std::cout << "add drawitem uid:" << uid << " nid:" << nid << " type:" << item->type << std::endl;
        switch(item->type){
            case feather::draw::Item::Mesh:
                std::cout << "add Mesh\n";
                m_apDrawItems.append(new Mesh(m_pFrameGraph->layer(),item,this));
                break;
            case feather::draw::Item::Line:
                std::cout << "add Line\n";
                m_apDrawItems.append(new Line(m_pFrameGraph->layer(),item,this));
                break;
            case feather::draw::Item::PerspCamera:
                std::cout << "updating Perspective Camear draw item\n";
                m_apDrawItems.append(new PerspCamera(m_pCamera,m_pFrameGraph->layer(),item,this));
                break;
            default:
                std::cout << "nothing built\n";
        }
        m_apDrawItems.at(m_apDrawItems.size()-1)->updateItem();
    }
}

void Viewport::updateItems(unsigned int uid)
{
    for(auto item : m_apDrawItems) {
        if(item->item()->uid == uid){
            switch(item->item()->type){
                case feather::draw::Item::Mesh:
                    std::cout << "updating Mesh draw item\n";
                    static_cast<Mesh*>(item)->updateItem();
                    break;
                case feather::draw::Item::Line:
                    std::cout << "updating Line draw item\n";
                    static_cast<Line*>(item)->updateItem();
                    break;
                case feather::draw::Item::PerspCamera:
                    std::cout << "updating Perspective Camear draw item\n";
                    static_cast<PerspCamera*>(item)->updateItem();
                    break;
                 default:
                    std::cout << "nothing built\n";
            }
        }
    }
}
