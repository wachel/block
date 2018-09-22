using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

namespace Block
{
    public abstract class ChunkMeshInfoBase
    {
        public List<Vector3> vertices;
        public List<int> triangles;
        public List<Vector3> normals;
        public List<Vector4> tangents;
        public List<Vector2> uv;
        public List<Vector2> uv2;
        public List<Color> colors;

        readonly int[] indices = { 0, 1, 2, 0, 2, 3 };
        public ChunkMeshInfoBase()
        {
            vertices = new List<Vector3>();
            triangles = new List<int>();
            normals = new List<Vector3>();
            tangents = new List<Vector4>();
            uv = new List<Vector2>();
            uv2 = new List<Vector2>();
            colors = new List<Color>();
        }
        public ChunkMeshInfoBase(int surfaceNum)
        {
            vertices = new List<Vector3>(surfaceNum * 4);
            triangles = new List<int>(surfaceNum * 6);
            normals = new List<Vector3>(surfaceNum * 4);
            tangents = new List<Vector4>(surfaceNum * 4);
            uv = new List<Vector2>(surfaceNum * 4);
            uv2 = new List<Vector2>(surfaceNum * 4);
            colors = new List<Color>(surfaceNum * 4);
        }
        public abstract void AddSurface(MeshTool.BlockSurface surface,int normalIndex,BlockTypeFunBase blockFun);
        public void AddCubeSurface(MeshTool.BlockSurface surface, int normalIndex, BlockTypeFunBase blockFun)
        {
            int startVericesNum = vertices.Count;
            //三角形
            for (int t = 0; t < 6; t++) {
                triangles.Add(startVericesNum + indices[t]);
            }
            //顶点
            for (int p = 0; p < 4; p++) {
                vertices.Add(surface.pos + MeshTool.VertexOffset[normalIndex, p]);
                normals.Add(MeshTool.NormalDefine[normalIndex]);
                tangents.Add(new Vector4(surface.raytraceAo[0], surface.raytraceAo[1], surface.raytraceAo[2], surface.raytraceAo[3]));
                colors.Add(blockFun.getFaceColor(surface.type, (Block.BlockFaceIndex)normalIndex));
            }
            //uv
            Rect rect = blockFun.getTextureUV(surface.type, (Block.BlockFaceIndex)normalIndex);
            uv.Add(new Vector2(rect.xMax, rect.yMin));
            uv.Add(new Vector2(rect.xMin, rect.yMin));
            uv.Add(new Vector2(rect.xMin, rect.yMax));
            uv.Add(new Vector2(rect.xMax, rect.yMax));
            //uv2
            uv2.Add(new Vector2(0, 0));
            uv2.Add(new Vector2(0, 1));
            uv2.Add(new Vector2(1, 1));
            uv2.Add(new Vector2(1, 0));            
        }
    }
}