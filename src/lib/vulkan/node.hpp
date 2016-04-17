/***********************************************************************
 *
 * Filename: node.hpp
 *
 * Description: vulkan base node.
 *
 * Copyright (C) 2016 Richard Layman, rlayman2000@yahoo.com 
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

#ifndef NODE_HPP
#define NODE_HPP

#include "deps.hpp"
#include "vulkan_deps.hpp"

namespace feather
{

    namespace vulkan
    {
        struct MeshBufferInfo {
            VkBuffer buf = VK_NULL_HANDLE;
            VkDeviceMemory mem = VK_NULL_HANDLE;
        };
    
        struct MeshBuffer {
            MeshBufferInfo vertices;
            MeshBufferInfo indices;
            MeshBufferInfo faceSelectIndices;
            MeshBufferInfo edges;
            uint32_t indexCount;
            uint32_t faceSelectCount;
            uint32_t edgeCount;
        };

        struct Vertex {
            float pos[3];
            float norm[3];
            float uv[2];
            float col[3];
            uint32_t id[4];
        };

        class Node
        {
            public:
                enum Type { Null, Axis, Grid, Camera, Light, Mesh };

                Node(Type type=Null);
                ~Node();
                virtual void prepareVertices(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties)=0;
                virtual void updateVertices(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties, float step=1.0)=0;
                Type type() { return m_type; }; 
                MeshBuffer* buffer() { return m_pMeshBuffer; };
                void freeBuffer(VkDevice device);
                virtual void build()=0;

            protected:
                // node methods and members
                VkBool32 getMemoryType(VkPhysicalDeviceMemoryProperties deviceMemoryProperties, uint32_t typeBits, VkFlags properties, uint32_t *typeIndex);
                void buildVertex(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties);
                void buildIndex(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties);
                void buildFaceSelect(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties);
                void buildEdge(VkDevice device, VkPhysicalDeviceMemoryProperties deviceMemoryProperties);

                Type m_type;
                MeshBuffer* m_pMeshBuffer;
                std::vector<Vertex> m_vertexBuffer;
                std::vector<uint32_t> m_indexBuffer;
                std::vector<uint32_t> m_faceSelectBuffer;
                std::vector<uint32_t> m_edgeBuffer;

         };

    } // namespace vulkan

} // namespace feather

#endif
